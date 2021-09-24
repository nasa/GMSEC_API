/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file JNI.cpp
 *  @brief This file contains utilities for interfacing with JNI.
 *
 * This support is intended to be generally available (through the gmsec_jni
 * library) but it is not ready for v3.4.
 */

#include <gmsec4/internal/JNI.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/Log.h>

#include <jni.h>

#include <iostream>
#include <string>


namespace gmsec {

namespace api {

namespace jni {


using namespace gmsec::api::util;


static const int DEBUG_CLASSLOADER = 1;


JNILookup::JNILookup(JNIEnv *e, bool global)
	:
	jenv(e),
	isGlobal(global)
{
}


JNILookup::~JNILookup()
{
}


jclass JNILookup::getClass(const char *name)
{
	jclass ref = 0;

	ref = jenv->FindClass(name);
	if (!ref)
		throw JNIException(std::string("getClass: FindClass(") + name + ") failed");

	if (isGlobal)
	{
		jobject globalRef = jenv->NewGlobalRef(ref);
		if (!globalRef)
			throw JNIException(std::string("getClass: unable to create global reference for ") + name);

		// done with the local reference
		jenv->DeleteLocalRef(ref);

		ref = reinterpret_cast<jclass>(globalRef);
	}

	GMSEC_DEBUG << "JNILookup.getClass(" << name << ") => " << ref;

	return ref;
}


jmethodID JNILookup::getMethod(jclass klazz, const char *name, const char *signature)
{
	jmethodID mid = jenv->GetMethodID(klazz, name, signature);
	if (!mid)
		throw JNIException("JNILookup.getMethod: invalid method " + std::string(name) + " / " + signature);
	return mid;
}


jmethodID JNILookup::getStaticMethod(jclass klazz, const char *name, const char *signature)
{
	jmethodID mid = jenv->GetStaticMethodID(klazz, name, signature);
	if (!mid)
		throw JNIException(std::string("getStaticMethod: invalid method ") + name + " / " + signature);
	return mid;
}


JavaVM * getJavaVM()
{
	const int MAX_JVMS = 4;
	JavaVM * jvm = NULL;
	JavaVM * jvms[MAX_JVMS] = { NULL };
	jsize count = 0;

	int code = JNI_GetCreatedJavaVMs(jvms, MAX_JVMS, &count);
	GMSEC_INFO << "getJavaVM: JNI_GetCreatedJavaVMs=" << code << " count=" << count;

	if (count == 1)
		jvm = jvms[0];
	else if (count > 1)
		GMSEC_WARNING << "error: getJavaVM: multiple JVMs!";
	else
		GMSEC_WARNING << "error: getJavaVM: zero JVMs!";

	return jvm;
}


JNIEnv * getJNIEnv(JavaVM * jvm)
{
	JNIEnv * jenv = NULL;

	if (!jvm)
		jvm = getJavaVM();

	if (jvm)
		jenv = attachMe(jvm);

	return jenv;
}



bool checkJVM (JNIEnv * jenv, const std::string &s)
{
	return checkJVM(jenv, s.c_str());
}


bool checkJVM (JNIEnv * jenv, const char *tag)
{
	bool ok = false;

	if (!tag)
		tag = "none";

	if (!jenv)
	{
		GMSEC_ERROR << "gmsec.jni.checkJVM[" << tag << "] NULL jenv";
		throw gmsec::api::Exception(
		                         MIDDLEWARE_ERROR,
		                         OTHER_ERROR_CODE,
		                         (std::string("checkJVM NULL jenv:  ") +
		                         tag).c_str());
	}
	else if (!jenv->ExceptionCheck())
		ok = true;
	else
	{
		jthrowable  e = jenv->ExceptionOccurred();
		std::string message;

		if (e != NULL)
		{
			jenv->ExceptionClear();

			jmethodID   toString = jenv->GetMethodID(jenv->FindClass("java/lang/Object"),
			                                                     "toString",
			                                                     "()Ljava/lang/String;");
			jstring     estring  = (jstring) jenv->CallObjectMethod(e, toString);

			jboolean    isCopy;
			const char* utf = jenv->GetStringUTFChars(estring, &isCopy);

			if (utf != NULL)
			{
				message = utf;

				jenv->ReleaseStringUTFChars(estring, utf);
			}
		}
		GMSEC_ERROR << "gmsec.jni.checkJVM[" << tag << "] JVM has an exception:"
		            << message.c_str();

		throw gmsec::api::Exception(MIDDLEWARE_ERROR,
		                            OTHER_ERROR_CODE,
		                            (std::string("Problem detected in checkJVM: ") + tag).c_str());
	}

	return ok;
}


JNIEnv* attachMe(JavaVM * jvm)
{
	JNIEnv *jenv = NULL;
	void * tmp = jenv;
	int code = jvm->AttachCurrentThread(&tmp, NULL);
	if (code < 0)
		GMSEC_WARNING << "gmsec.jni.attachMe: AttachCurrentThread failed: " << code;
	else
		jenv = reinterpret_cast<JNIEnv*>(tmp);
	return jenv;
}


void detachMe(JavaVM *jvm)
{
	if (jvm)
	{
		int code = jvm->DetachCurrentThread();
		GMSEC_DEBUG << "gmsec.jni.detachMe: jvm=" << jvm << " code=" << code;
	}
	else
		GMSEC_WARNING << "gmsec.jni.detachMe: null JVM";
}


jobject makeGlobalRef(JNIEnv *env, jobject localRef)
{
	if (!localRef)
	{
		GMSEC_WARNING << "gmsec.JNI: makeGlobalRef: null input";
	}

	jobject globalRef = env->NewGlobalRef(localRef);
	if (globalRef != localRef)
		env->DeleteLocalRef(localRef);
	else
		GMSEC_WARNING << "gmsec.JNI: makeGlobalRef: no change";

	return globalRef;
}


void updateClassLoader(JNIEnv *env)
{
	JNILookup lookup(env, false);

	jclass class_ClassLoader = lookup.getClass("java/lang/ClassLoader");
if (DEBUG_CLASSLOADER)
{
	GMSEC_VERBOSE << "Classloader class: " << class_ClassLoader;
}

	jmethodID method_getSystemClassLoader = lookup.getStaticMethod(class_ClassLoader,
			"getSystemClassLoader", "()Ljava/lang/ClassLoader;");
if (DEBUG_CLASSLOADER)
{
	GMSEC_VERBOSE << "method_getSystemClassLoader: " << method_getSystemClassLoader;
}

	jobject cobj = env->CallStaticObjectMethod(class_ClassLoader,
			method_getSystemClassLoader);
if (DEBUG_CLASSLOADER)
{
	GMSEC_VERBOSE << "updateClassLoader: getSystemClassLoader=" << cobj;
}

	jclass class_Thread = lookup.getClass("java/lang/Thread");
if (DEBUG_CLASSLOADER)
{
	GMSEC_VERBOSE << "class_Thread: " << class_Thread;
}

	jmethodID method_currentThread = lookup.getStaticMethod(class_Thread,
			"currentThread", "()Ljava/lang/Thread;");
if (DEBUG_CLASSLOADER)
{
	GMSEC_VERBOSE << "method_currentThread: " << method_currentThread;
}

	jobject tobj = env->CallStaticObjectMethod(class_Thread,
		method_currentThread);
if (DEBUG_CLASSLOADER)
{
	GMSEC_VERBOSE << "currentThread=" << tobj;
}

	jmethodID method_setContextClassLoader = lookup.getMethod(class_Thread,
			"setContextClassLoader", "(Ljava/lang/ClassLoader;)V");
if (DEBUG_CLASSLOADER)
{
	GMSEC_VERBOSE << "method_setContextClassLoader: " << method_setContextClassLoader;
}
	env->CallVoidMethod(tobj, method_setContextClassLoader, cobj);

	checkJVM(env, "setContextClassLoader");
}


void
GJNI_ThrowException(JNIEnv *jenv, GJNI_JavaExceptionCodes code, const char *msg)
{
	jclass excep;
	static const GJNI_JavaExceptions_t java_exceptions[] =
	{
		{ GJNI_JavaOutOfMemoryError, "java/lang/OutOfMemoryError" },
		{ GJNI_JavaIOException, "java/io/IOException" },
		{ GJNI_JavaRuntimeException, "java/lang/RuntimeException" },
		{ GJNI_JavaIndexOutOfBoundsException, "java/lang/IndexOutOfBoundsException" },
		{ GJNI_JavaArithmeticException, "java/lang/ArithmeticException" },
		{ GJNI_JavaIllegalArgumentException, "java/lang/IllegalArgumentException" },
		{ GJNI_JavaNullPointerException, "java/lang/NullPointerException" },
		{ GJNI_JavaUnknownError,  "java/lang/UnknownError" },
		{(GJNI_JavaExceptionCodes)0,  "java/lang/UnknownError" }
	};
	const GJNI_JavaExceptions_t *except_ptr = java_exceptions;

	while (except_ptr->code != code && except_ptr->code)
		except_ptr++;

	jenv->ExceptionClear();
	excep = jenv->FindClass(except_ptr->java_exception);

	if (excep)
		jenv->ThrowNew(excep, msg);
}


#if 0

JNIObject::JNIObject()
	:
	object(0)
{
}


JNIObject::~JNIObject()
{
}


void JNIObject::setObject(jobject o)
{
	object = o;
}


jobject JNIObject::getObject() const
{
	return object;
}

#endif



} // namespace jni
} // namespace api
} // namespace gmsec

