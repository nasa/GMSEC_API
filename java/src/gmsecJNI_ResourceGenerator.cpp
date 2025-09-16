/*
 * Copyright 2007-2025 United States Government as represented by the
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
	Config* cfg = JNI_JLONG_TO_CONFIG(jCfgPtr);

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

	ResourceGenerator* created = 0;

	try
	{
		created = new ResourceGenerator(*cfg, static_cast<GMSEC_U16>(jPubRate),
		                                      static_cast<GMSEC_U16>(jSampleInterval),
		                                      static_cast<GMSEC_U16>(jAverageInterval),
		                                      fields);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1Destroy
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	ResourceGenerator* rsrcGen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

	delete rsrcGen;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1Start
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

	jboolean result = JNI_FALSE;

	try
	{
		result = (rsrcgen->start() ? JNI_TRUE : JNI_FALSE);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1Stop
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

	jboolean result = (rsrcgen->stop() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1IsRunning
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);

	jboolean result = (rsrcgen->isRunning() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1SetField
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jlong jFieldPtr, jobject jField)
{
	ResourceGenerator* rsrcgen = JNI_JLONG_TO_RESOURCE_GENERATOR(jGenPtr);
	Field*             field   = JNI_JLONG_TO_FIELD(jFieldPtr);

	jboolean result = JNI_FALSE;

	try
	{
		result = (rsrcgen->setField(*field) ? JNI_TRUE : JNI_FALSE);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ResourceGenerator_1CreateResourceMessage
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactory, jobject msgFactory, jint jSampleInterval, jint jAverageInterval)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactory);

	jlong rsrcMsg = 0;

	try
	{
		Message tmp = ResourceGenerator::createResourceMessage( *factory, static_cast<GMSEC_U16>(jSampleInterval), static_cast<GMSEC_U16>(jAverageInterval) );

		rsrcMsg = JNI_POINTER_TO_JLONG( new Message(tmp) );
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return rsrcMsg;
}


#ifdef __cplusplus
}
#endif
