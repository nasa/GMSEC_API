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
#include <gmsec4/Fields.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <iostream>
#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Message__Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jKind)
{
	Message* created = 0;

	try
	{
		JStringManager subject(jenv, jSubject);

		if (jvmOk(jenv, "Message.Message(subject, kind)"))
		{
			Message::MessageKind kind = messageKindToNative(jenv, jKind);

			created = new Message(subject.c_str(), kind);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Message__Ljava_lang_String_2IJLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jKind, jlong jCfgPtr, jobject jCfg)
{
	Message* created = 0;

	try
	{
		JStringManager subject(jenv, jSubject);

		if (jvmOk(jenv, "Message.Message(subject, kind, config)"))
		{
			Message::MessageKind kind = messageKindToNative(jenv, jKind);
			Config* config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			created = new Message(subject.c_str(), kind, *config);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Message__Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	Message* created = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "Message.Message(data)"))
		{
			created = new Message(data.c_str());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Message_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* created = 0;

	try
	{
		Message* other = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (other == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			created = new Message(*other);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Message
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddConfig
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
			msg->addConfig(*cfg);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetConfig
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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetSubject
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1SetSubject
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


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetKind
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
			kind = (jint) messageKindToJava(jenv, msg->getKind());
		}
	}
	JNI_CATCH

	return kind;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddField
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldByteArray
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

				result = (msg->addField(fieldName.c_str(), (GMSEC_BIN) fieldValue, (size_t) fieldValueLen) ? JNI_TRUE : JNI_FALSE);

				jenv->ReleaseByteArrayElements(jFieldValue, fieldValue, JNI_ABORT);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldBoolean
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldChar
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldByte
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldShort
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldInt
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldLong
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldFloat
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldDouble
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1AddFieldString
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1ClearFields
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1ClearField
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetIntegerValue
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

			if (jvmOk(jenv, "Message.getIntegerValue"))
			{
				result = msg->getIntegerValue(fieldName.c_str());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetUnsignedIntegerValue
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

			if (jvmOk(jenv, "Message.getUnsignedIntegerValue"))
			{
				GMSEC_U64 value = msg->getUnsignedIntegerValue(fieldName.c_str());

				std::ostringstream oss;
				oss << value;

				jstring valueAsString = makeJavaString(jenv, oss.str().c_str());

				result = jenv->NewObject(Cache::getCache().classU64, Cache::getCache().methodU64Init, valueAsString);

				if (!gmsec::api::jni::jvmOk(jenv, "Message::getUnsignedIntegerValue: new U64") || !result)
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


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetDoubleValue
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

			if (jvmOk(jenv, "Message.getDoubleValue"))
			{
				result = msg->getDoubleValue(fieldName.c_str());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetStringValue
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetField
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


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetFieldType
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
				type = (jint) fieldTypeToJava(jenv, msg->getFieldType(name.c_str()));
			}
		}
	}
	JNI_CATCH

	return type;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetBinaryField
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
				try
				{
					const BinaryField& field = msg->getBinaryField(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetBooleanField
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
				try
				{
					const BooleanField& field = msg->getBooleanField(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetCharField
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
				try
				{
					const CharField& field = msg->getCharField(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetF32Field
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
				try
				{
					const F32Field& field = msg->getF32Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetF64Field
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
				try
				{
					const F64Field& field = msg->getF64Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetI8Field
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
				try
				{
					const I8Field& field = msg->getI8Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetI16Field
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
				try
				{
					const I16Field& field = msg->getI16Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetI32Field
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
				try
				{
					const I32Field& field = msg->getI32Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetI64Field
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
				try
				{
					const I64Field& field = msg->getI64Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetU8Field
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
				try
				{
					const U8Field& field = msg->getU8Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetU16Field
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
				try
				{
					const U16Field& field = msg->getU16Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetU32Field
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
				try
				{
					const U32Field& field = msg->getU32Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetU64Field
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
				try
				{
					const U64Field& field = msg->getU64Field(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetStringField
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
				try
				{
					const StringField& field = msg->getStringField(name.c_str());

					result = JNI_POINTER_TO_JLONG(&field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetFieldCount
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1CopyFields
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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1ToXML
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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1ToJSON
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetSize
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Message_1GetFieldIterator
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
