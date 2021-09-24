/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#include "gmsecJNI_Jenv.h"
#include "gmsecJNI_Cache.h"

#include <gmsec/util/Log.h>

#include <iostream>


JNIException::JNIException(const std::string& s)
	: message(s)
{
}


JNIException::~JNIException() throw()
{
}

const char* JNIException::what() const throw()
{
	return message.c_str();
}


void SWIG_JavaThrowException(JNIEnv* jenv, SWIG_JavaExceptionCodes code, const char* msg)
{
	jclass excep;
	static const SWIG_JavaExceptions_t java_exceptions[] =
	{
		{ SWIG_JavaOutOfMemoryError, "java/lang/OutOfMemoryError" },
		{ SWIG_JavaIOException, "java/io/IOException" },
		{ SWIG_JavaRuntimeException, "java/lang/RuntimeException" },
		{ SWIG_JavaIndexOutOfBoundsException, "java/lang/IndexOutOfBoundsException" },
		{ SWIG_JavaArithmeticException, "java/lang/ArithmeticException" },
		{ SWIG_JavaIllegalArgumentException, "java/lang/IllegalArgumentException" },
		{ SWIG_JavaNullPointerException, "java/lang/NullPointerException" },
		{ SWIG_JavaUnknownError,  "java/lang/UnknownError" },
		{(SWIG_JavaExceptionCodes)0,  "java/lang/UnknownError" }
	};
	const SWIG_JavaExceptions_t *except_ptr = java_exceptions;

	while (except_ptr->code != code && except_ptr->code)
		except_ptr++;

	jenv->ExceptionClear();
	excep = jenv->FindClass(except_ptr->java_exception);

	if (excep)
		jenv->ThrowNew(excep, msg);
}


/** @fn jvmOk
 * @brief Checks for pending exceptions from the JVM.
 */
bool jvmOk(JNIEnv* jenv, const char* context)
{
    bool ok = false;
    if (!jenv->ExceptionCheck())
        ok = true;
    else
    {
        if (context)
            FPRINTF(stderr, "JNI: [%s] jvm is not ok!\n", context);
        else
            FPRINTF(stderr, "JNI: jvm is not ok!\n");
        jenv->ExceptionDescribe();
    }
    return ok;
}


/** @fn makeJavaString
 * @brief Helper function to convert a const char* to a Java GMSEC_String
 * while properly handling String encoding issues.
 */
jstring makeJavaString(JNIEnv *jenv, const char* cStr)
{
	if (cStr == NULL)
	{
		throw std::invalid_argument("cannot construct a Java String with a NULL C-style string");
	}

	// Convert the C-style string into a Java binary array
	std::string str(cStr);
	int byteCount = str.length();
	const jbyte* pNativeMessage = reinterpret_cast<const jbyte*>(str.c_str());
	jbyteArray bytes = jenv->NewByteArray(byteCount);
	jenv->SetByteArrayRegion(bytes, 0, byteCount, pNativeMessage);
	jclass stringClass = jenv->FindClass("java/lang/String");
	jmethodID ctor = jenv->GetMethodID(stringClass, "<init>", "([B)V");

	// Convert the Java binary array into the platform default Charset
	return reinterpret_cast<jstring>(jenv->NewObject(stringClass, ctor, bytes));
}



/**
   @brief JStringManager assists in conversions from jstring to char *.
    The destructor releases the char * in the event of an exception.
    This implementation assumes the native character set is ASCII.
    If this is not the case, google JNU_GetStringNativeChars.
*/
JStringManager::JStringManager(JNIEnv* e, jstring s, bool checkJVM)
	: jenv(e), jtext(s), p(0)
{
	if (jtext)
	{
		if (!checkJVM || jvmOk(jenv, "+JStringManager"))
		{
			p = jenv->GetStringUTFChars(jtext, 0);
		}
	}
}


JStringManager::~JStringManager()
{
	if (p)
	{
		jenv->ReleaseStringUTFChars(jtext, p);
	}
}


const char* JStringManager::c_str() const
{
	return p;
}


/*
    @brief JByteArrayManager assists in conversions from jbyteArray to jbyte *.
    The destructor releases the jbyte * in the event of an exception.
*/
JByteArrayManager::JByteArrayManager(JNIEnv* e, jbyteArray a)
	: jenv(e), array(a), p(0)
{
	if (array)
	{
		p = jenv->GetByteArrayElements(array, 0);
	}
}


JByteArrayManager::~JByteArrayManager()
{
	if (p)
	{
		jenv->ReleaseByteArrayElements(array, p, 0);
	}
}


const jbyte* JByteArrayManager::jbytes() const
{
	return p;
}


AutoJEnv::AutoJEnv()
	: jenv(0), jvm(0), doDetach(false)
{
	jsize count = 0;
	int x = JNI_GetCreatedJavaVMs(&jvm, 1, &count);
	if (x)
	{
		std::cerr << "AutoJEnv: JNI_GetJavaVMs failed: " << x << '\n';
	}
	else 
	{
		x = jvm->GetEnv(reinterpret_cast<void**>(&jenv), JNI_VERSION_1_4);
		if (x == JNI_OK)
		{
			// already attached
		}
		else if (x == JNI_EDETACHED)
		{
			x = jvm->AttachCurrentThread(reinterpret_cast<void**>(&jenv), NULL);
			if (x == JNI_OK)
			{
				doDetach = true;
			}
			else
			{
				std::cerr << "AutoJEnv: AttachCurrentThread failed: " << x << '\n';
			}
		}
		else
		{
			// already attached
			std::cerr << "AutoJEnv: GetEnv failed: " << x << '\n';
		}
	}
}


AutoJEnv::~AutoJEnv()
{
	if (doDetach && jvm && Cache::getCache().enableDetach)
	{
		int x = jvm->DetachCurrentThread();
		if (x)
		{
			std::cerr << "AutoJEnv: unable to DetachCurrentThread: " << x << '\n';
		}
	}
	jenv = 0;
	jvm = 0;
}


JNIEnv* AutoJEnv::getEnv()
{
	return jenv;
}


JavaVM* AutoJEnv::getVM()
{
	return jvm;
}


jobject createJavaMessage(JNIEnv* jenv, gmsec::Message* message)
{
	jobject javaMessage = 0;

	jobject jniMessage = jenv->NewObject(Cache::getCache().classJNIMessage, Cache::getCache().methodMessageInitJZ, JNI_POINTER_TO_JLONG(message), JNI_FALSE);
	if (!jvmOk(jenv, "createJavaMessage: new JNIMessage") || !jniMessage)
	{
		LOG_DEBUG << "Unable to create JNIMessage";
		return javaMessage;
	}

	javaMessage = jenv->NewObject(Cache::getCache().classMessage, Cache::getCache().methodMessageInit);
	if (!jvmOk(jenv, "createJavaMessage: new Message") || !javaMessage)
	{
		LOG_WARNING << "Unable to create Java Message";
		return 0;
	}

	jenv->CallVoidMethod(javaMessage, Cache::getCache().methodMessageSetImpl, jniMessage);
	if (!jvmOk(jenv, "createJavaMessage: SetImpl"))
	{
		LOG_WARNING << "Unable to Message.SetImpl";
		return 0;
	}

	return javaMessage;
}


/*
   @brief Find the class for a given name throwing an exception if a problem occurs.
    Create a global reference to the jclass so it remains valid.
*/
jclass getClass(JNIEnv* jenv, const std::string& name)
{
	jclass localRef = jenv->FindClass(name.c_str());
	if (!localRef)
		throw JNIException("gmsecJNI: FindClass(" + name + ") failed");

	jobject globalRef = jenv->NewGlobalRef(localRef);
	if (!globalRef)
		throw JNIException("gmsecJNI: unable to create global reference for " + name);

	// done with the local reference
	jenv->DeleteLocalRef(localRef);

	Cache::getCache().addGlobalReference(globalRef);
	jvmOk(jenv, "getClass");

	return (jclass) globalRef;
}


/** @brief Look up a field for a class given its name and type.
*/
jfieldID getFieldID(JNIEnv* jenv, jclass klazz, const std::string& fname, const std::string& ftype)
{
	jfieldID fid = jenv->GetFieldID(klazz, fname.c_str(), ftype.c_str());
	if (!fid)
		throw JNIException("gmsecJNI: invalid field name " + fname + " / type " + ftype);
	jvmOk(jenv, "getFieldID");
	return fid;
}


/** @brief Look up a method for a class given its signature.
*/
jmethodID getMethod(JNIEnv* jenv, jclass klazz, const std::string& name, const std::string& signature)
{
	jmethodID mid = jenv->GetMethodID(klazz, name.c_str(), signature.c_str());
	if (!mid)
		throw JNIException("gmsecJNI: invalid method " + name + signature);
	jvmOk(jenv, "getMethod");
	return mid;
}
