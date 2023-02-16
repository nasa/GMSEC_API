/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Message class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Config.h>
#include <gmsec5/Fields.h>
#include <gmsec5/Message.h>

#include <iostream>
#include <sstream>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Message
  (JNIEnv *jenv, jclass jcls)
{
	jlong jMessage = 0;

	try
	{
		jMessage = JNI_POINTER_TO_JLONG( new Message() );
	}
	JNI_CATCH

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Message_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jlong jMessage = 0;

	try
	{
		Message* other = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (other == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			jMessage = JNI_POINTER_TO_JLONG( new Message(*other) );
		}
	}
	JNI_CATCH

	return jMessage;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1Message
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			delete msg;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1Acknowledge
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			msg->acknowledge();
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1Destroy
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			Message::destroy(msg);
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSchemaID
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jstring jSchemaID = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			const char* schemaID = msg->getSchemaID();

			jSchemaID = makeJavaString(jenv, schemaID);

			jvmOk(jenv, "Message.getSchemaID");
		}
	}
	JNI_CATCH

	return jSchemaID;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetVersion
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jint jVersion = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			jVersion = (jint) msg->getVersion();
		}
	}
	JNI_CATCH

	return jVersion;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSchemaLevel
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jint jLevel = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			jLevel = static_cast<jint>(msg->getSchemaLevel());
		}
	}
	JNI_CATCH

	return jLevel;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1IsCompliant
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jobject jStatus = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			Status status = msg->isCompliant();

			jStatus = createJavaStatus(jenv, status);
		}
	}
	JNI_CATCH

	return jStatus;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1RegisterMessageValidator
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jValPtr)
{
	try
	{
		Message*          msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);
		MessageValidator* val = JNI_JLONG_TO_MESSAGE_VALIDATOR(jValPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else if (val == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageValidator reference is null");
		}
		else
		{
			msg->registerMessageValidator(val);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueString
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jstring jFieldValue)
{
	jboolean replaced = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);
			JStringManager fieldValue(jenv, jFieldValue, true);

			if (jvmOk(jenv, "Message.setFieldValueString"))
			{
				replaced = (msg->setFieldValue(fieldName.c_str(), fieldValue.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return replaced;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueInt
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jint jFieldValue)
{
	jboolean replaced = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.setFieldValueI32"))
			{
				replaced = (msg->setFieldValue(fieldName.c_str(), static_cast<GMSEC_I32>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return replaced;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueLong
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jlong jFieldValue)
{
	jboolean replaced = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.setFieldValueU32"))
			{
				replaced = (msg->setFieldValue(fieldName.c_str(), static_cast<GMSEC_I64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return replaced;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueDouble
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jdouble jFieldValue)
{
	jboolean replaced = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.setFieldValueF64"))
			{
				replaced = (msg->setFieldValue(fieldName.c_str(), static_cast<GMSEC_F64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return replaced;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1SetConfig
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jCfgPtr, jobject jCfg)
{
	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);
		Config*  cfg = JNI_JLONG_TO_CONFIG(jCfgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			msg->setConfig(*cfg);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetConfig
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jlong jConfig = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			const Config& cfg = msg->getConfig();

			jConfig = JNI_POINTER_TO_JLONG(&cfg);
		}
	}
	JNI_CATCH

	return jConfig;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1SetSubject
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jSubject)
{
	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "Message.setSubject"))
			{
				msg->setSubject(subject.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSubject
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jstring s = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			const char* tmp = msg->getSubject();
			s = makeJavaString(jenv, tmp);
			jvmOk(jenv, "Message.getSubject");
		}
	}
	JNI_CATCH

	return s;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1SetKind
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jint jKind)
{
	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			msg->setKind(static_cast<Message::Kind>(jKind));
		}
	}
	JNI_CATCH
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetKind
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jint kind = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			kind = static_cast<jint>( msg->getKind() );
		}
	}
	JNI_CATCH

	return kind;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jFieldPtr, jobject jField)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg   = JNI_JLONG_TO_MESSAGE(jMsgPtr);
		Field*   field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else if (field == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = (msg->addField(*field) ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldByteArray
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jbyteArray jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, blob)"))
			{
				jbyte* fieldValue    = jenv->GetByteArrayElements(jFieldValue, NULL);
				jsize  fieldValueLen = jenv->GetArrayLength(jFieldValue);

				result = (msg->addField(fieldName.c_str(), reinterpret_cast<GMSEC_U8*>(fieldValue), (size_t) fieldValueLen) ? JNI_TRUE : JNI_FALSE);

				jenv->ReleaseByteArrayElements(jFieldValue, fieldValue, JNI_ABORT);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldBoolean
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jboolean jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, bool)"))
			{
				result = (msg->addField(fieldName.c_str(), (jFieldValue == JNI_TRUE)) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldChar
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jchar jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, char)"))
			{
				result = (msg->addField(fieldName.c_str(), (GMSEC_CHAR) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldByte
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jbyte jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, byte)"))
			{
				result = (msg->addField(fieldName.c_str(), (GMSEC_I8) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldShort
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jshort jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, short)"))
			{
				result = (msg->addField(fieldName.c_str(), (GMSEC_I16) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldInt
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jint jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, int)"))
			{
				result = (msg->addField(fieldName.c_str(), (GMSEC_I32) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldLong
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jlong jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, long)"))
			{
				result = (msg->addField(fieldName.c_str(), (GMSEC_I64) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldFloat
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jfloat jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, float)"))
			{
				result = (msg->addField(fieldName.c_str(), (GMSEC_F32) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldDouble
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jdouble jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.addField(string, double)"))
			{
				result = (msg->addField(fieldName.c_str(), (GMSEC_F64) jFieldValue) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldString
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jstring jFieldValue)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);
			JStringManager fieldValue(jenv, jFieldValue, true);

            if (jvmOk(jenv, "Message.addField(string, string)") && fieldName.c_str() && fieldValue.c_str())
			{
				result = (msg->addField(fieldName.c_str(), fieldValue.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ClearFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			msg->clearFields();
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ClearField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.clearField"))
			{
				result = (msg->clearField(name.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetStringValue
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jstring result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getStringValue"))
			{
				result = makeJavaString(jenv, msg->getStringValue(fieldName.c_str()));
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetBooleanValue
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getBooleanValue"))
			{
				result = (msg->getBooleanValue(fieldName.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI16Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jshort result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getI16Value"))
			{
				result = msg->getI16Value(fieldName.c_str());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI32Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jint result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getI32Value"))
			{
				result = msg->getI32Value(fieldName.c_str());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI64Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getI64Value"))
			{
				result = msg->getI64Value(fieldName.c_str());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU16Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jobject result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getU16Value"))
			{
				GMSEC_U16 value = msg->getU16Value(fieldName.c_str());

				result = jenv->NewObject(Cache::getCache().classU16, Cache::getCache().methodU16Init, static_cast<jint>(value));

				if (!jvmOk(jenv, "Message::getU16Value: new U32") || !result)
				{
					GMSEC_WARNING << "Unable to create U16 object";
					result = 0;
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU32Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jobject result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getU32Value"))
			{
				GMSEC_U32 value = msg->getU32Value(fieldName.c_str());

				result = jenv->NewObject(Cache::getCache().classU32, Cache::getCache().methodU32Init, static_cast<jlong>(value));

				if (!jvmOk(jenv, "Message::getU32Value: new U32") || !result)
				{
					GMSEC_WARNING << "Unable to create U32 object";
					result = 0;
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU64Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jobject result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getU64Value"))
			{
				GMSEC_U64 value = msg->getU64Value(fieldName.c_str());

				std::ostringstream oss;
				oss << value;

				jstring valueAsString = makeJavaString(jenv, oss.str().c_str());

				result = jenv->NewObject(Cache::getCache().classU64, Cache::getCache().methodU64Init, valueAsString);

				if (!jvmOk(jenv, "Message::getU64Value: new U64") || !result)
				{
					GMSEC_WARNING << "Unable to create U64 object";
					result = 0;
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetF64Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	jdouble result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Message.getF64Value"))
			{
				result = msg->getF64Value(fieldName.c_str());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1HasField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jboolean result = JNI_FALSE;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jName);

			if (jvmOk(jenv, "Message.hasField"))
			{
				result = (msg->hasField(fieldName.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getField"))
			{
				const Field* field = msg->getField(name.c_str());

				result = JNI_POINTER_TO_JLONG(field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetFieldType
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jint type = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getFieldType"))
			{
				type = static_cast<jint>( msg->getFieldType(name.c_str()) );
			}
		}
	}
	JNI_CATCH

	return type;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetBinaryField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getBinaryField"))
			{
				const BinaryField& field = msg->getBinaryField(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetBooleanField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getBooleanField"))
			{
				const BooleanField& field = msg->getBooleanField(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetCharField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getCharField"))
			{
				const CharField& field = msg->getCharField(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetF32Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getF32Field"))
			{
				const F32Field& field = msg->getF32Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetF64Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getF64Field"))
			{
				const F64Field& field = msg->getF64Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI8Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getI8Field"))
			{
				const I8Field& field = msg->getI8Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI16Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getI16Field"))
			{
				const I16Field& field = msg->getI16Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI32Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getI32Field"))
			{
				const I32Field& field = msg->getI32Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI64Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getI64Field"))
			{
				const I64Field& field = msg->getI64Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU8Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getU8Field"))
			{
				const U8Field& field = msg->getU8Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU16Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getU16Field"))
			{
				const U16Field& field = msg->getU16Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU32Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getU32Field"))
			{
				const U32Field& field = msg->getU32Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU64Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getU64Field"))
			{
				const U64Field& field = msg->getU64Field(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetStringField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	jlong result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getStringField"))
			{
				const StringField& field = msg->getStringField(name.c_str());

				result = JNI_POINTER_TO_JLONG(&field);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetFieldCount
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jint result = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			result = (jint) msg->getFieldCount();
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1CopyFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jToMsgPtr, jobject jToMsg)
{
	try
	{
		Message* msg   = JNI_JLONG_TO_MESSAGE(jMsgPtr);
		Message* toMsg = JNI_JLONG_TO_MESSAGE(jToMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else if (toMsg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "To-Message reference is null");
		}
		else
		{
			msg->copyFields(*toMsg);
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jstring s = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			const char* xml = msg->toXML();
			s = makeJavaString(jenv, xml);
			jvmOk(jenv, "Message.toXML");
		}
	}
	JNI_CATCH

	return s;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ToJSON
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jstring s = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			const char* json = msg->toJSON();
			s = makeJavaString(jenv, json);
			jvmOk(jenv, "Message.toJSON");
		}
	}
	JNI_CATCH

	return s;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSize
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jlong size = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			size = (jlong) msg->getSize();
		}
	}
	JNI_CATCH

	return size;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetFieldIterator
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jint jSelector)
{
	jlong jIterator = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			MessageFieldIterator::Selector selector = static_cast<MessageFieldIterator::Selector>((int) jSelector);

			MessageFieldIterator& iter = msg->getFieldIterator(selector);

			jIterator = JNI_POINTER_TO_JLONG(&iter);
		}
	}
	JNI_CATCH

	return jIterator;
}


#ifdef __cplusplus
}
#endif
