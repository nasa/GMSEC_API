/*
 * Copyright 2007-2024 United States Government as represented by the
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
	jlong jMessage = JNI_POINTER_TO_JLONG( new Message() );

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Message_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* other = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong jMessage = JNI_POINTER_TO_JLONG( new Message(*other) );

	return jMessage;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1Message
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	delete msg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1Acknowledge
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	msg->acknowledge();
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1Destroy
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	Message::destroy(msg);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSchemaID
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	const char* schemaID = msg->getSchemaID();

	jstring jSchemaID = makeJavaString(jenv, schemaID);

	jvmOk(jenv, "Message.getSchemaID");

	return jSchemaID;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetVersion
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jint jVersion = static_cast<jint>(msg->getVersion());

	return jVersion;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSchemaLevel
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jint jLevel = static_cast<jint>(msg->getSchemaLevel());

	return jLevel;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1IsCompliant
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	Status status = msg->isCompliant();

	jobject jStatus = createJavaStatus(jenv, status);

	return jStatus;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1RegisterMessageValidator
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jValPtr)
{
	Message*          msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);
	MessageValidator* val = JNI_JLONG_TO_MESSAGE_VALIDATOR(jValPtr);

	msg->registerMessageValidator(val);
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueString
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jstring jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean replaced = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);
	JStringManager fieldValue(jenv, jFieldValue, true);

	if (jvmOk(jenv, "Message.setFieldValueString"))
	{
		try
		{
			replaced = (msg->setFieldValue(fieldName.c_str(), fieldValue.c_str()) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return replaced;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueInt
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jint jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean replaced = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.setFieldValueI32"))
	{
		try
		{
			replaced = (msg->setFieldValue(fieldName.c_str(), static_cast<GMSEC_I32>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return replaced;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueLong
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jlong jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean replaced = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.setFieldValueU32"))
	{
		try
		{
			replaced = (msg->setFieldValue(fieldName.c_str(), static_cast<GMSEC_I64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return replaced;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageSetFieldValueDouble
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jdouble jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean replaced = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.setFieldValueF64"))
	{
		try
		{
			replaced = (msg->setFieldValue(fieldName.c_str(), static_cast<GMSEC_F64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return replaced;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1SetConfig
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jCfgPtr, jobject jCfg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);
	Config*  cfg = JNI_JLONG_TO_CONFIG(jCfgPtr);

	msg->setConfig(*cfg);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetConfig
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	const Config& cfg = msg->getConfig();

	jlong jConfig = JNI_POINTER_TO_JLONG(&cfg);

	return jConfig;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1SetSubject
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jSubject)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	JStringManager subject(jenv, jSubject);

	if (jvmOk(jenv, "Message.setSubject"))
	{
		try
		{
			msg->setSubject(subject.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1SetSubjectElement
(JNIEnv* jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName, jstring jValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	JStringManager name(jenv, jName);
	JStringManager value(jenv, jValue);

	if (jvmOk(jenv, "Message.setSubjectElement"))
	{
		try
		{
			msg->setSubjectElement(name.c_str(), value.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSubject
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	const char* tmp = msg->getSubject();
	jstring s = makeJavaString(jenv, tmp);
	jvmOk(jenv, "Message.getSubject");

	return s;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1SetKind
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jint jKind)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	msg->setKind(static_cast<Message::Kind>(jKind));
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetKind
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jint kind = static_cast<jint>( msg->getKind() );

	return kind;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jFieldPtr, jobject jField)
{
	Message* msg   = JNI_JLONG_TO_MESSAGE(jMsgPtr);
	Field*   field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jboolean result = (msg->addField(*field) ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldByteArray
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jbyteArray jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, blob)"))
	{
		jbyte* fieldValue    = jenv->GetByteArrayElements(jFieldValue, NULL);
		jsize  fieldValueLen = jenv->GetArrayLength(jFieldValue);

		result = (msg->addField(fieldName.c_str(), reinterpret_cast<GMSEC_U8*>(fieldValue), static_cast<size_t>(fieldValueLen)) ? JNI_TRUE : JNI_FALSE);

		jenv->ReleaseByteArrayElements(jFieldValue, fieldValue, JNI_ABORT);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldBoolean
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jboolean jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, bool)"))
	{
		result = (msg->addField(fieldName.c_str(), (jFieldValue == JNI_TRUE)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldChar
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jchar jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, char)"))
	{
		result = (msg->addField(fieldName.c_str(), static_cast<GMSEC_CHAR>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldByte
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jbyte jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, byte)"))
	{
		result = (msg->addField(fieldName.c_str(), static_cast<GMSEC_I8>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldShort
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jshort jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, short)"))
	{
		result = (msg->addField(fieldName.c_str(), static_cast<GMSEC_I16>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldInt
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jint jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, int)"))
	{
		result = (msg->addField(fieldName.c_str(), static_cast<GMSEC_I32>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldLong
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jlong jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, long)"))
	{
		result = (msg->addField(fieldName.c_str(), static_cast<GMSEC_I64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldFloat
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jfloat jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, float)"))
	{
		result = (msg->addField(fieldName.c_str(), static_cast<GMSEC_F32>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldDouble
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jdouble jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.addField(string, double)"))
	{
		result = (msg->addField(fieldName.c_str(), static_cast<GMSEC_F64>(jFieldValue)) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1AddFieldString
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName, jstring jFieldValue)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);
	JStringManager fieldValue(jenv, jFieldValue, true);

	if (jvmOk(jenv, "Message.addField(string, string)") && fieldName.c_str() && fieldValue.c_str())
	{
		result = (msg->addField(fieldName.c_str(), fieldValue.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ClearFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	msg->clearFields();
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ClearField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.clearField"))
	{
		result = (msg->clearField(name.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetStringValue
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jstring result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getStringValue"))
	{
		try
		{
			result = makeJavaString(jenv, msg->getStringValue(fieldName.c_str()));
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetBooleanValue
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getBooleanValue"))
	{
		try
		{
			result = (msg->getBooleanValue(fieldName.c_str()) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI16Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jshort result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getI16Value"))
	{
		try
		{
			result = msg->getI16Value(fieldName.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI32Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jint result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getI32Value"))
	{
		try
		{
			result = msg->getI32Value(fieldName.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI64Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getI64Value"))
	{
		try
		{
			result = msg->getI64Value(fieldName.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU16Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jobject result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getU16Value"))
	{
		try
		{
			GMSEC_U16 value = msg->getU16Value(fieldName.c_str());

			result = jenv->NewObject(Cache::getCache().classU16, Cache::getCache().methodU16Init, static_cast<jint>(value));

			if (!jvmOk(jenv, "Message::getU16Value: new U32") || !result)
			{
				GMSEC_WARNING << "Unable to create U16 object";
				result = 0;
			}
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU32Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jobject result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getU32Value"))
	{
		try
		{
			GMSEC_U32 value = msg->getU32Value(fieldName.c_str());

			result = jenv->NewObject(Cache::getCache().classU32, Cache::getCache().methodU32Init, static_cast<jlong>(value));

			if (!jvmOk(jenv, "Message::getU32Value: new U32") || !result)
			{
				GMSEC_WARNING << "Unable to create U32 object";
				result = 0;
			}
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU64Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jobject result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getU64Value"))
	{
		try
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
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetF64Value
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jFieldName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jdouble result = 0;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Message.getF64Value"))
	{
		try
		{
			result = msg->getF64Value(fieldName.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1HasField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jName);

	if (jvmOk(jenv, "Message.hasField"))
	{
		result = (msg->hasField(fieldName.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getField"))
	{
		const Field* field = msg->getField(name.c_str());

		result = JNI_POINTER_TO_JLONG(field);
	}

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetFieldType
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jint type = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getFieldType"))
	{
		try
		{
			type = static_cast<jint>( msg->getFieldType(name.c_str()) );
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return type;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetBinaryField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getBinaryField"))
	{
		try
		{
			const BinaryField& field = msg->getBinaryField(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetBooleanField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getBooleanField"))
	{
		try
		{
			const BooleanField& field = msg->getBooleanField(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetCharField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getCharField"))
	{
		try
		{
			const CharField& field = msg->getCharField(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetF32Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getF32Field"))
	{
		try
		{
			const F32Field& field = msg->getF32Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetF64Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getF64Field"))
	{
		try
		{
			const F64Field& field = msg->getF64Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI8Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getI8Field"))
	{
		try
		{
			const I8Field& field = msg->getI8Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI16Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getI16Field"))
	{
		try
		{
			const I16Field& field = msg->getI16Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI32Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getI32Field"))
	{
		try
		{
			const I32Field& field = msg->getI32Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetI64Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getI64Field"))
	{
		try
		{
			const I64Field& field = msg->getI64Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU8Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getU8Field"))
	{
		try
		{
			const U8Field& field = msg->getU8Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU16Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getU16Field"))
	{
		try
		{
			const U16Field& field = msg->getU16Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU32Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getU32Field"))
	{
		try
		{
			const U32Field& field = msg->getU32Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetU64Field
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getU64Field"))
	{
		try
		{
			const U64Field& field = msg->getU64Field(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetStringField
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jstring jName)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong result = 0;

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Message.getStringField"))
	{
		try
		{
			const StringField& field = msg->getStringField(name.c_str());

			result = JNI_POINTER_TO_JLONG(&field);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetFieldCount
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jint result = static_cast<jint>(msg->getFieldCount());

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1CopyFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jlong jToMsgPtr, jobject jToMsg)
{
	Message* msg   = JNI_JLONG_TO_MESSAGE(jMsgPtr);
	Message* toMsg = JNI_JLONG_TO_MESSAGE(jToMsgPtr);

	msg->copyFields(*toMsg);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	const char* xml = msg->toXML();
	jstring s = makeJavaString(jenv, xml);
	jvmOk(jenv, "Message.toXML");

	return s;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1ToJSON
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	const char* json = msg->toJSON();
	jstring s = makeJavaString(jenv, json);
	jvmOk(jenv, "Message.toJSON");

	return s;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetSize
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	jlong size = static_cast<jlong>(msg->getSize());

	return size;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Message_1GetFieldIterator
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg, jint jSelector)
{
	Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	MessageFieldIterator::Selector selector = static_cast<MessageFieldIterator::Selector>(static_cast<int>(jSelector));

	MessageFieldIterator& iter = msg->getFieldIterator(selector);

	jlong jIterator = JNI_POINTER_TO_JLONG(&iter);

	return jIterator;
}


#ifdef __cplusplus
}
#endif
