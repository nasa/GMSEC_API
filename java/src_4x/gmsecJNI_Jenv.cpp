/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include "gmsecJNI_Jenv.h"
#include "gmsecJNI_Cache.h"

#include <gmsec4/internal/util/InternalLog.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <iostream>
#include <sstream>


using namespace gmsec::api::jni;
using namespace gmsec::api::util;
using namespace gmsec::api::internal;


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


void gmsec::api::jni::ThrowGmsecException(JNIEnv* jenv, const char* errorMsg)
{
	const char* className = "gov/nasa/gsfc/gmsec/api/GMSEC_Exception";

	jenv->ExceptionClear();
	jclass exClass = jenv->FindClass(className);

	if (exClass == NULL)
	{
		std::string errmsg = std::string("Cannot find class ") + className;

		SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, errmsg.c_str());
	}
	else
	{
		jenv->ThrowNew(exClass, errorMsg);
	}
}


void gmsec::api::jni::SWIG_JavaThrowException(JNIEnv* jenv, SWIG_JavaExceptionCodes code, const char* msg)
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
bool gmsec::api::jni::jvmOk(JNIEnv* jenv, const char* context)
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
 * @brief Helper function to convert a const char* to a Java String
 * while properly handling String encoding issues.
 */
jstring gmsec::api::jni::makeJavaString(JNIEnv *jenv, const char* cStr)
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
		if (!checkJVM || gmsec::api::jni::jvmOk(jenv, "+JStringManager"))
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
		std::stringstream ss;
		ss << "AutoJEnv: JNI_GetJavaVMs failed: " << x;

		LogEntry warning = { __FILE__, __LINE__, logWARNING, TimeUtil::getCurrentTime(), ss.str().c_str() };

		std::cerr << InternalLog::prepareLogMessage(warning) << std::endl;
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
				std::stringstream ss;
				ss << "AutoJEnv: AttachCurrentThread failed: " << x;

				LogEntry warning = { __FILE__, __LINE__, logWARNING, TimeUtil::getCurrentTime(), ss.str().c_str() };

				std::cerr << InternalLog::prepareLogMessage(warning) << std::endl;
			}
		}
		else
		{
			std::stringstream ss;
			ss << "AutoJEnv: GetEnv failed: " << x;

			LogEntry warning = { __FILE__, __LINE__, logWARNING, TimeUtil::getCurrentTime(), ss.str().c_str() };

			std::cerr << InternalLog::prepareLogMessage(warning) << std::endl;
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
			std::stringstream ss;
			ss << "AutoJEnv: Unable to DetachCurrentThread: " << x;

			LogEntry warning = { __FILE__, __LINE__, logWARNING, TimeUtil::getCurrentTime(), ss.str().c_str() };

			std::cerr << InternalLog::prepareLogMessage(warning) << std::endl;
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


jobject gmsec::api::jni::createJavaMessage(JNIEnv* jenv, const gmsec::api::Message& message)
{
	jobject jniMessage = jenv->NewObject(Cache::getCache().classJNIMessage, Cache::getCache().methodMessageInitJZ, JNI_POINTER_TO_JLONG((&message)), JNI_FALSE);
	jobject jMessage = jenv->NewObject(Cache::getCache().classMessage, Cache::getCache().methodMessageInit, jniMessage);

	if (!gmsec::api::jni::jvmOk(jenv, "createJavaMessage: new Message") || !jMessage)
	{
		GMSEC_DEBUG << "Unable to create Java Message";
		return 0;
	}

	return jMessage;
}


jobject gmsec::api::jni::createJavaStatus(JNIEnv* jenv, const gmsec::api::Status& status)
{
    jstring statusString = makeJavaString(jenv, status.getReason());

    if (!jvmOk(jenv, "makeJavaString(jenv, status)") || !statusString)
    {
		GMSEC_WARNING << "Unable to create Java Status";
        return 0;
    }

    jobject jStatus = jenv->NewObject(Cache::getCache().classJNIStatus,
            Cache::getCache().methodStatusInitIJString,
            (jint) status.getClass(), (jint) status.getCode(), statusString, status.getCustomCode());

    if (!jvmOk(jenv, "createJavaStatus") || jStatus == NULL)
    {
		GMSEC_WARNING << "Unable to create Java Status object";
        return 0;
    }

	jenv->DeleteLocalRef(statusString);

	return jStatus;
}


int gmsec::api::jni::messageKindToJava(JNIEnv* jenv, gmsec::api::Message::MessageKind msgKind)
{
    switch (msgKind)
    {
	case gmsec::api::Message::PUBLISH: return 0;
	case gmsec::api::Message::REQUEST: return 1;
	case gmsec::api::Message::REPLY:   return 2;
	}

	ThrowGmsecException(jenv, "Unknown message kind");

	// We will never reach here, but the compiler will be happy nonetheless.
	return 0;
}


gmsec::api::Message::MessageKind gmsec::api::jni::messageKindToNative(JNIEnv* jenv, jint msgKind)
{
	switch ((int) msgKind)
	{
	case 0: return gmsec::api::Message::PUBLISH;
	case 1: return gmsec::api::Message::REQUEST;
	case 2: return gmsec::api::Message::REPLY;
	}

	ThrowGmsecException(jenv, "Unknown message kind");

	// We will never reach here, but the compiler will be happy nonetheless.
	return gmsec::api::Message::PUBLISH;
}


int gmsec::api::jni::fieldTypeToJava(JNIEnv* jenv, gmsec::api::Field::FieldType fieldType)
{
    switch (fieldType)
    {
	case gmsec::api::Field::BIN_TYPE   : return 0;
	case gmsec::api::Field::BOOL_TYPE  : return 1;
	case gmsec::api::Field::CHAR_TYPE  : return 2;
	case gmsec::api::Field::I8_TYPE    : return 3;
	case gmsec::api::Field::I16_TYPE   : return 4;
	case gmsec::api::Field::I32_TYPE   : return 5;
	case gmsec::api::Field::I64_TYPE   : return 6;
	case gmsec::api::Field::F32_TYPE   : return 7;
	case gmsec::api::Field::F64_TYPE   : return 8;
	case gmsec::api::Field::STRING_TYPE: return 9;
	case gmsec::api::Field::U8_TYPE    : return 10;
	case gmsec::api::Field::U16_TYPE   : return 11;
	case gmsec::api::Field::U32_TYPE   : return 12;
	case gmsec::api::Field::U64_TYPE   : return 13;
	}

	ThrowGmsecException(jenv, "Unknown field type");

	// We will never reach here, but the compiler will be happy nonetheless.
	return 0;
}


jobject gmsec::api::jni::lookupEvent(gmsec::api::Connection::ConnectionEvent event)
{
	return Cache::getCache().connectionEvents[ event ];
}


/*
   @brief Find the class for a given name throwing an exception if a problem occurs.
    Create a global reference to the jclass so it remains valid.
*/
jclass gmsec::api::jni::getClass(JNIEnv* jenv, const std::string& name)
{
	jclass localRef = jenv->FindClass(name.c_str());
	if (!localRef)
		throw JNIException("gmsecJNI: FindClass(" + name + ") failed");

	jobject globalRef = jenv->NewGlobalRef(localRef);
	if (!globalRef)
		throw JNIException("gmsecJNI: unable to create global reference for " + name);

	// done with the local reference
	jenv->DeleteLocalRef(localRef);

	Cache::getCache().addGlobalReference(jenv, globalRef);
	gmsec::api::jni::jvmOk(jenv, "getClass");

	return (jclass) globalRef;
}


/** @brief Look up a field for a class given its name and type.
*/
jfieldID gmsec::api::jni::getFieldID(JNIEnv* jenv, jclass klazz, const std::string& fname, const std::string& ftype)
{
	jfieldID fid = jenv->GetFieldID(klazz, fname.c_str(), ftype.c_str());
	if (!fid)
		throw JNIException("gmsecJNI: invalid field name " + fname + " / type " + ftype);
	gmsec::api::jni::jvmOk(jenv, "getFieldID");
	return fid;
}


/** @brief Look up a method for a class given its signature.
*/
jmethodID gmsec::api::jni::getMethod(JNIEnv* jenv, jclass klazz, const std::string& name, const std::string& signature)
{
	jmethodID mid = jenv->GetMethodID(klazz, name.c_str(), signature.c_str());
	if (!mid)
		throw JNIException("gmsecJNI: invalid method " + name + signature);
	gmsec::api::jni::jvmOk(jenv, "getMethod");
	return mid;
}
