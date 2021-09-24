/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/Deprecated.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::jni;
using namespace gmsec::api::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MistMessage
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jstring jSchemaID, jlong jSpecPtr, jobject jSpec)
{
	MistMessage* created = 0;

	try
	{
		JStringManager subject(jenv, jSubject);
		JStringManager schemaID(jenv, jSchemaID);
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}

		if (jvmOk(jenv, "MistMessage(subject, schemaID, specification)"))
		{
			created = new MistMessage(subject.c_str(), schemaID.c_str(), *spec);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MistMessageWithConfig
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jstring jSchemaID, jlong jConfigPtr, jobject jConfig, jlong jSpecPtr, jobject jSpec)
{
	MistMessage* created = 0;

	try
	{
		JStringManager subject(jenv, jSubject);
		JStringManager schemaID(jenv, jSchemaID);
		Config*        config = JNI_JLONG_TO_CONFIG(jConfigPtr);
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}

		if (jvmOk(jenv, "MistMessage(subject, schemaID, config, specification)"))
		{
			created = new MistMessage(subject.c_str(), schemaID.c_str(), *config, *spec);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MistMessageWithData
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	MistMessage* created = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "MistMessage(data)"))
		{
			created = new MistMessage(data.c_str());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MistMessageWithSpec
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jstring jData)
{
	MistMessage* created = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}

		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "MistMessage(data, spec)"))
		{
			created = new MistMessage(*spec, data.c_str());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MistMessageCopy
  (JNIEnv *jenv, jclass jcls, jlong jOtherPtr, jobject jOther)
{
	MistMessage* created = 0;

	try
	{
		MistMessage* other = JNI_JLONG_TO_MISTMESSAGE(jOtherPtr);

		if (!other)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MistMessage reference is null");
		}

		if (jvmOk(jenv, "MistMessage(other)"))
		{
			created = new MistMessage(*other);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MistMessageFromMessage
  (JNIEnv *jenv, jclass jclass, jlong jMsgPtr, jobject jMsg, jlong jConfigPtr, jobject jConfig)
{
	MistMessage* created = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);
		Config*  cfg = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		if (!cfg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}

		if (jvmOk(jenv, "MistMessage(Message, Config)"))
		{
			created = new MistMessage(*msg, *cfg);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1MistMessage
  (JNIEnv *jenv, jclass jcls, jlong jMistMsgPtr, jobject jMistMsg)
{
	try
	{
		MistMessage* msg = JNI_JLONG_TO_MISTMESSAGE(jMistMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MistMessage reference is null");
		}
		else
		{
			delete msg;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MistMessageSetStandardFields
  (JNIEnv *jenv, jclass jcls, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	try
	{
		size_t numFields = (size_t) jNumFields;

		if (numFields > 0)
		{
			DataList<Field*> fields;

			jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

			for (size_t i = 0; i < numFields; ++i)
			{
				fields.push_back(JNI_JLONG_TO_FIELD(fldptrs[i]));
			}

			MistMessage::setStandardFields(fields);

			jenv->ReleaseLongArrayElements(jFieldPtrs, fldptrs, 0);
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MistMessageClearStandardFields
  (JNIEnv *jenv, jclass jcls)
{
	MistMessage::clearStandardFields();
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MistMessageGetSchemaID
  (JNIEnv *jenv, jclass jcls, jlong jMistMsgPtr, jobject jMistMsg)
{
	jstring jSchemaID = 0;

	try
	{
		MistMessage* msg = JNI_JLONG_TO_MISTMESSAGE(jMistMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MistMessage reference is null");
		}
		else
		{
			jSchemaID = makeJavaString(jenv, msg->getSchemaID());

			jvmOk(jenv, "MistMessage.getSchemaID");
		}
	}
	JNI_CATCH

	return jSchemaID;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MistMessageSetValueString
  (JNIEnv *jenv, jclass jcls, jlong jMistMsgPtr, jobject jMistMsg, jstring jFieldName, jstring jValue)
{
	try
	{
		MistMessage* msg = JNI_JLONG_TO_MISTMESSAGE(jMistMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MistMessage reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);
			JStringManager value(jenv, jValue);

			if (jvmOk(jenv, "MistMessage.setValue(fieldName, value)"))
			{
				msg->setValue(fieldName.c_str(), value.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MistMessageSetValueLong
  (JNIEnv *jenv, jclass jcls, jlong jMistMsgPtr, jobject jMistMsg, jstring jFieldName, jlong jValue)
{
	try
	{
		MistMessage* msg = JNI_JLONG_TO_MISTMESSAGE(jMistMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MistMessage reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "MistMessage.setValue(fieldName, value)"))
			{
				msg->setValue(fieldName.c_str(), (GMSEC_I64) jValue);
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MistMessageSetValueDouble
  (JNIEnv *jenv, jclass jcls, jlong jMistMsgPtr, jobject jMistMsg, jstring jFieldName, jdouble jValue)
{
	try
	{
		MistMessage* msg = JNI_JLONG_TO_MISTMESSAGE(jMistMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MistMessage reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "MistMessage.setValue(fieldName, value)"))
			{
				msg->setValue(fieldName.c_str(), (GMSEC_F64) jValue);
			}
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
