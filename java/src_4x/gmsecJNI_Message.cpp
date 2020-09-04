/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <iostream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


static int messageKindToJava(JNIEnv* jenv, Message::MessageKind msgKind)
{
    switch (msgKind)
    {
	case Message::PUBLISH: return 0;
	case Message::REQUEST: return 1;
	case Message::REPLY:   return 2;
	}

	ThrowGmsecException(jenv, "Unknown message kind");

	// We will never reach here, but the compiler will be happy nonetheless.
	return -1;
}


static Message::MessageKind messageKindToNative(JNIEnv* jenv, jint msgKind)
{
	switch ((int) msgKind)
	{
	case 0: return Message::PUBLISH;
	case 1: return Message::REQUEST;
	case 2: return Message::REPLY;
	}

	ThrowGmsecException(jenv, "Unknown message kind");

	// We will never reach here, but the compiler will be happy nonetheless.
	return Message::PUBLISH;
}



JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Message__Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jstring jName, jint jKind)
{
	Message* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "Message.Message(name, kind)"))
		{
			Message::MessageKind kind = messageKindToNative(jenv, jKind);

			created = new Message(name.c_str(), kind);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Message__Ljava_lang_String_2IJLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jstring jName, jint jKind, jlong jCfgPtr, jobject jCfg)
{
	Message* created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "Message.Message(name, kind, config)"))
		{
			Message::MessageKind kind = messageKindToNative(jenv, jKind);
			Config* config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			created = new Message(name.c_str(), kind, *config);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Message__Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jXML)
{
	Message* created = 0;

	try
	{
		JStringManager xml(jenv, jXML);

		if (jvmOk(jenv, "Message.Message(xml)"))
		{
			created = new Message(xml.c_str());
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
			s = jenv->NewStringUTF(tmp);
			jvmOk(jenv, "Message.getSubject");
		}
	}
	JNI_CATCH

	return s;
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
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Message.getFieldType"))
			{
				result = (jint) msg->getFieldType(name.c_str());
			}
		}
	}
	JNI_CATCH

	return result;
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

					result = JNI_POINTER_TO_JLONG(new BinaryField(field));
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

					result = JNI_POINTER_TO_JLONG(new BooleanField(field));
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

					result = JNI_POINTER_TO_JLONG(new CharField(field));
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

					result = JNI_POINTER_TO_JLONG(new F32Field(field));
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

					result = JNI_POINTER_TO_JLONG(new F64Field(field));
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

					result = JNI_POINTER_TO_JLONG(new I8Field(field));
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

					result = JNI_POINTER_TO_JLONG(new I16Field(field));
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

					result = JNI_POINTER_TO_JLONG(new I32Field(field));
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

					result = JNI_POINTER_TO_JLONG(new I64Field(field));
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

					result = JNI_POINTER_TO_JLONG(new U8Field(field));
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

					result = JNI_POINTER_TO_JLONG(new U16Field(field));
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

					result = JNI_POINTER_TO_JLONG(new U32Field(field));
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

					result = JNI_POINTER_TO_JLONG(new U64Field(field));
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

					result = JNI_POINTER_TO_JLONG(new StringField(field));
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
			s = jenv->NewStringUTF(xml);
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
			s = jenv->NewStringUTF(json);
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
