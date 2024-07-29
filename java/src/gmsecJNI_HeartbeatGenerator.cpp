/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// HeartbeatGenerator class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Config.h>
#include <gmsec5/HeartbeatGenerator.h>

#include <gmsec5/util/List.h>

using namespace gmsec::api5;
using namespace gmsec::api5::jni;
using namespace gmsec::api5::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1Create
  (JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jCfg, jint jPubRate, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	Config* cfg = JNI_JLONG_TO_CONFIG(jCfgPtr);

	HeartbeatGenerator* created = 0;

	try
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

		created = new HeartbeatGenerator(*cfg, static_cast<GMSEC_U16>(jPubRate), fields);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1Destroy
  (JNIEnv *jenv, jclass jcls, jlong jHbGenPtr, jobject jHbGen)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jHbGenPtr);

	delete hbgen;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1Start
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

	jboolean result = JNI_FALSE;

	try
	{
		result = (hbgen->start() ? JNI_TRUE : JNI_FALSE);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1Stop
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

	jboolean result = (hbgen->stop() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1IsRunning
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

	jboolean result = (hbgen->isRunning() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1ChangePublishRate
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jint jPubRate)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

	try
	{
		hbgen->changePublishRate(static_cast<GMSEC_U16>(jPubRate));
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api5_jni_JNIHeartbeatGenerator_2JLgov_nasa_gsfc_gmsec_api5_jni_field_JNIField_2
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jlong jFieldPtr, jobject jField)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);
	Field*              field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jboolean result = JNI_FALSE;

	try
	{
		result = (hbgen->setField(*field) ? JNI_TRUE : JNI_FALSE);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api5_jni_JNIHeartbeatGenerator_2Ljava_lang_String_2J
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jstring jFieldName, jlong jFieldValue)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "HeartbeatGenerator::setField(fieldName, fieldValueLong"))
	{
		try
		{
			result = (hbgen->setField(fieldName.c_str(), static_cast<GMSEC_I64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api5_jni_JNIHeartbeatGenerator_2Ljava_lang_String_2D
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jstring jFieldName, jdouble jFieldValue)
{
	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "HeartbeatGenerator::setField(fieldName, fieldValueDouble"))
	{
		try
		{
			result = (hbgen->setField(fieldName.c_str(), static_cast<GMSEC_F64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api5_jni_JNIHeartbeatGenerator_2Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jstring jFieldName, jstring jFieldValue)
{
	jboolean result = JNI_FALSE;

	HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

	JStringManager fieldName(jenv, jFieldName);
	JStringManager fieldValue(jenv, jFieldValue);

	if (jvmOk(jenv, "HeartbeatGenerator::setField(fieldName, fieldValueString"))
	{
		try
		{
			result = (hbgen->setField(fieldName.c_str(), fieldValue.c_str()) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


#ifdef __cplusplus
}
#endif
