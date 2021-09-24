/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// HeartbeatGenerator class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/HeartbeatGenerator.h>

#include <gmsec4/util/DataList.h>

using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1HeartbeatGenerator__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jCfg, jstring jSubject, jint jPubRate)
{
	HeartbeatGenerator* created = 0;

	try
	{
		Config* cfg = JNI_JLONG_TO_CONFIG(jCfgPtr);

		if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "HeartbeatGenerator(config, subject, pubRate)"))
			{
				created = new HeartbeatGenerator(*cfg, subject.c_str(), (GMSEC_U16) jPubRate);
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1HeartbeatGenerator__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2I_3J_3Lgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2I
  (JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jCfg, jstring jSubject, jint jPubRate, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	HeartbeatGenerator* created = 0;

	try
	{
		Config* cfg = JNI_JLONG_TO_CONFIG(jCfgPtr);

		if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			DataList<Field*> fields;
			size_t           numFields = (size_t) jNumFields;

			if (numFields > 0)
			{
				jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

				for (size_t i = 0; i < numFields; ++i)
				{
					fields.push_back(JNI_JLONG_TO_FIELD(fldptrs[i]));
				}
			}

			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "HeartbeatGenerator(config, subject, pubRate, standardFields)"))
			{
				created = new HeartbeatGenerator(*cfg, subject.c_str(), (GMSEC_U16) jPubRate, fields);
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1HeartbeatGenerator
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			delete hbgen;
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1Start
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	jboolean result = JNI_FALSE;

	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			result = (hbgen->start() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1Stop
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	jboolean result = JNI_FALSE;

	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			result = (hbgen->stop() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1IsRunning
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen)
{
	jboolean result = JNI_FALSE;

	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			result = (hbgen->isRunning() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1ChangePublishRate
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jint jPubRate)
{
	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			hbgen->changePublishRate((GMSEC_U16) jPubRate);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api_jni_JNIHeartbeatGenerator_2JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jlong jFieldPtr, jobject jField)
{
	jboolean result = JNI_FALSE;

	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);
		Field*              field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else if (field == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = (hbgen->setField(*field) ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api_jni_JNIHeartbeatGenerator_2Ljava_lang_String_2J
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jstring jFieldName, jlong jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "HeartbeatGenerator::setField(fieldName, fieldValueLong"))
			{
				result = (hbgen->setField(fieldName.c_str(), (GMSEC_I64) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api_jni_JNIHeartbeatGenerator_2Ljava_lang_String_2D
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jstring jFieldName, jdouble jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "HeartbeatGenerator::setField(fieldName, fieldValueDouble"))
			{
				result = (hbgen->setField(fieldName.c_str(), (GMSEC_F64) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_HeartbeatGenerator_1SetField__JLgov_nasa_gsfc_gmsec_api_jni_JNIHeartbeatGenerator_2Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jGenPtr, jobject jGen, jstring jFieldName, jstring jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		HeartbeatGenerator* hbgen = JNI_JLONG_TO_HEARTBEAT_GENERATOR(jGenPtr);

		if (hbgen == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "HeartbeatGenerator reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);
			JStringManager fieldValue(jenv, jFieldValue);

			if (jvmOk(jenv, "HeartbeatGenerator::setField(fieldName, fieldValueString"))
			{
				result = (hbgen->setField(fieldName.c_str(), fieldValue.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


#ifdef __cplusplus
}
#endif
