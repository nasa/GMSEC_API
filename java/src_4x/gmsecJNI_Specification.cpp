/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// SchemaIDIterator class functions
//

#include "gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/mist/Specification.h>

#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Specification
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jlong spec = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (config == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			spec = JNI_POINTER_TO_JLONG(new Specification(*config));
		}
	}
	JNI_CATCH

	return spec;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Specification_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jlong spec = 0;

	try
	{
		Specification* other = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (other == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			spec = JNI_POINTER_TO_JLONG(new Specification(*other));
		}
	}
	JNI_CATCH

	return spec;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Specification
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			delete spec;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1ValidateMessage
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);
		Message*       msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			spec->validateMessage(*msg);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetSchemaIDIterator
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jlong jIter = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			SchemaIDIterator& iter = spec->getSchemaIDIterator();

			jIter = JNI_POINTER_TO_JLONG(&iter);
		}
	}
	JNI_CATCH

	return jIter;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetVersion
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jint version = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			version = (jint) spec->getVersion();
		}
	}
	JNI_CATCH

	return version;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetTemplateXML
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jstring jSubject, jstring jSchemaID)
{
	jstring xml = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			JStringManager schemaID(jenv, jSchemaID);

			const char* tmp = spec->getTemplateXML(subject.c_str(), schemaID.c_str());

			xml = jenv->NewStringUTF(tmp);

			jvmOk(jenv, "Specification.getTemplateXML");
		}
	}
	JNI_CATCH

	return xml;
}


#ifdef __cplusplus
}
#endif
