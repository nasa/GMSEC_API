/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file JNI.h
 *  @brief This file contains utilities for interfacing with JNI.
 *
 * The goal is to share these functions across GMSEC modules using JNI
 * by building them into the gmsec_jni library.  However, at this time,
 * they are being built directly into the generic JMS wrapper.
 */

#ifndef GMSEC_API_INTERNAL_JNI_H
#define GMSEC_API_INTERNAL_JNI_H


#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>

#include <jni.h>

#include <string>


namespace gmsec {

namespace api {

namespace jni {


/* check for JVM exception and log if present */
bool checkJVM(JNIEnv *jenv, const char *tag = 0);
bool checkJVM(JNIEnv *jenv, const std::string &tag);


/* attach current thread to JVM acquiring environment */
JNIEnv* attachMe(JavaVM *jvm);


/* detach current thread from JVM */
void detachMe(JavaVM *jvm);


/**
* @fn makeGlobalRef(JNIEnv *, jobject localRef)
* @brief Converts a local JNI reference to a global one.
* The local reference is no longer valid after the call.
*/
jobject makeGlobalRef(JNIEnv *jenv, jobject localRef);


class JNILookup
{
public:

	JNILookup(JNIEnv *env, bool global);
	~JNILookup();

	// these throw JNIException on failure...
	jclass getClass(const char *name);
	jmethodID getMethod(jclass klazz, const char *name, const char *signature);
	jmethodID getStaticMethod(jclass klazz, const char *name, const char *signature);

	jobject makeGlobalRef(jobject localRef);

private:

	// Declared, but not implemented.
	JNILookup(const JNILookup &);
	JNILookup &operator=(const JNILookup &);

	JNIEnv *jenv;
	bool isGlobal;
};


/* jstring helper */
class JStringManager
{
public:
	JStringManager(JNIEnv *e, jstring s, bool doCheck = false)
		: jenv(e), jtext(s), p(0)
	{
		if (jtext)
			if (!doCheck || checkJVM(jenv))
				p = jenv->GetStringUTFChars(jtext, 0);
	}

	~JStringManager()
	{
		if (p)
			jenv->ReleaseStringUTFChars(jtext, p);
	}

	const char *c_str()
	{
		return p;
	}

private:
	// Declared, but not implemented.
	JStringManager(const JStringManager &);
	JStringManager &operator=(const JStringManager &);

	JNIEnv*     jenv;
	jstring     jtext;
	const char* p;
};


class JNIException : public std::exception
{
public:
	JNIException (const std::string &s)
		: text(s) { }

	virtual ~JNIException () throw () { }

	virtual const char * what () const throw ()
	{
		return text.c_str();
	}

private:
	std::string text;
};


typedef enum
{
	GJNI_JavaOutOfMemoryError = 1,
	GJNI_JavaIOException,
	GJNI_JavaRuntimeException,
	GJNI_JavaIndexOutOfBoundsException,
	GJNI_JavaArithmeticException,
	GJNI_JavaIllegalArgumentException,
	GJNI_JavaNullPointerException,
	GJNI_JavaUnknownError
} GJNI_JavaExceptionCodes;


typedef struct
{
	GJNI_JavaExceptionCodes code;
	const char *java_exception;
} GJNI_JavaExceptions_t;


void GJNI_ThrowException(JNIEnv *jenv, GJNI_JavaExceptionCodes code, const char *msg);

void updateClassLoader(JNIEnv *jenv);


/* The JNI_CATCH macro prevents any exceptions from propagating
through a JNI call.  The JVM knows nothing about C++ exceptions.
*/
#define JNI_CATCH(env) \
	catch (std::bad_alloc& e) { \
		GMSEC_WARNING << "JNI_CATCH(std::bad_alloc): " << e.what(); \
		if (jni::checkJVM(env) && gmsec::api::util::StringUtil::stringLength(e.what()) < 200) { \
			char buffer[256]; \
			gmsec::api::util::StringUtil::stringFormat(buffer, sizeof(buffer), "GMSEC JNI encountered bad_alloc %s", e.what()); \
			GJNI_ThrowException(env, GJNI_JavaOutOfMemoryError, buffer); \
		} \
	} \
	catch (std::exception& e) { \
		GMSEC_WARNING << "JNI_CATCH(std::exception): " << e.what(); \
		if (jni::checkJVM(env) && gmsec::api::util::StringUtil::stringLength(e.what()) < 200) { \
			char buffer[256]; \
			gmsec::api::util::StringUtil::stringFormat(buffer, sizeof(buffer), "GMSEC JNI encountered %s", e.what()); \
			GJNI_ThrowException(env, GJNI_JavaUnknownError, buffer); \
		} \
	} \
	catch (...) { \
		GMSEC_WARNING << "JNI_CATCH(...)"; \
		if (jni::checkJVM(env)) { \
			GJNI_ThrowException(env, GJNI_JavaUnknownError, "GMSEC JNI encountered unknown exception"); \
		} \
	}


} // namespace jni
} // namespace api
} // namespace gmsec


#endif // GMSEC_API_INTERNAL_JNI_H
