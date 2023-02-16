/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// ResourceGenerator class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Config.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/ResourceGenerator.h>

#include <gmsec5/util/List.h>

using namespace gmsec::api5;
using namespace gmsec::api5::jni;
using namespace gmsec::api5::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1Create
  (JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jCfg, jint jPubRate, jint jSampleInterval, jint jAverageInterval, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	ResourceGenerator* created = 0;

	try
	{
		Config* cfg = JNI_JLONG_TO_CONFIG(jCfgPtr);

		if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			List<Field*> fields;
			size_t numFields = static_cast<size_t>(jNumFields);

			if (numFields > 0)
			{
				jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

				for (size_t i = 0; i < numFields; ++i)
				{
					fields.push_back(JNI_JLONG_TO_FIELD(fldptrs[i]));
				}
			}

			created = new ResourceGenerator(*cfg, static_cast<GMSEC_U16>(jPubRate),
			                                      static_cast<GMSEC_U16>(jSampleInterval),
			                                      static_cast<GMSEC_U16>(jAverageInterval),
			                                      fields);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1Destroy
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	try
	{
		ResourceGenerator* rsrcGen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

		if (rsrcGen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ResourceGenerator reference is null");
		}

		delete rsrcGen;
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1Start
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	jboolean result = JNI_FALSE;

	try
	{
		ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

		if (rsrcgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ResourceGenerator reference is null");
		}
		else
		{
			result = (rsrcgen->start() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1Stop
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	jboolean result = JNI_FALSE;

	try
	{
		ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

		if (rsrcgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ResourceGenerator reference is null");
		}
		else
		{
			result = (rsrcgen->stop() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1IsRunning
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	jboolean result = JNI_FALSE;

	try
	{
		ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

		if (rsrcgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ResourceGenerator reference is null");
		}
		else
		{
			result = (rsrcgen->isRunning() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1SetField
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jlong jFieldPtr, jobject jField)
{
	jboolean result = JNI_FALSE;

	try
	{
		ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);
		Field*             field   = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (rsrcgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ResourceGenerator reference is null");
		}
		else if (field == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = (rsrcgen->setField(*field) ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1CreateResourceMessage
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactory, jobject msgFactory, jint jSampleInterval, jint jAverageInterval)
{
	jlong rsrcMsg = 0;

	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactory);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			Message tmp = ResourceGenerator::createResourceMessage( *factory, static_cast<GMSEC_U16>(jSampleInterval), static_cast<GMSEC_U16>(jAverageInterval) );

			rsrcMsg = JNI_POINTER_TO_JLONG( new Message(tmp) );
		}
	}
	JNI_CATCH

	return rsrcMsg;
}


#ifdef __cplusplus
}
#endif
