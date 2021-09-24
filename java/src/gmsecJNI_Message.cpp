/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Message class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec/Message.h>
#include <gmsec/internal/MessageBuddy.h>

using gmsec::Config;
using gmsec::Field;
using gmsec::Message;
using gmsec::Status;


#ifdef __cplusplus
extern "C"
{
#endif

	/** @brief Deletes a message. */
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_delete_1Message(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		try
		{
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				gmsec::internal::MessageBuddy::destroyMessage(message);
			}
		}
		JNI_CATCH
	}


	/**
	 * @brief This function will check if the message is Valid.
	 * @note depricated, use IsValid instead.
	 * @return true if the message is valid
	 *
	 */
	JNIEXPORT jboolean JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1isValid(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		jboolean valid =  0;
		try
		{
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				valid = (jboolean) message->isValid();
			}
		}
		JNI_CATCH

		return valid;
	}

	/**
	 * @brief This function will check if the message is Valid.
	 *
	 * @return true if the message is valid
	 *
	 */
	JNIEXPORT jboolean JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1IsValid(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		jboolean valid =  0;
		try
		{
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				valid = (jboolean) message->IsValid();
			}
		}
		JNI_CATCH

		return valid;
	}


	/**
	 * @brief This function set the 'kind' of this message.
	 *
	 * @param jKind - valid message kind
	 * @return status - result of the operation
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetKind(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jint jKind)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				status = message->SetKind((unsigned short) jKind);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/**
	 * @brief This function get the 'kind' of this message.
	 *
	 * @param [out] jGkind - current message kind
	 * @return status - result of the operation
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetKind(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jobject jGkind)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				unsigned short kind = 0;
				status = message->GetKind(kind);

				jenv->SetIntField(jGkind, Cache::getCache().fieldU16_value, (jint) kind);
				jvmOk(jenv, "Message.GetKind");
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/**
	 * @brief This function set the subject to publish/request this message upon.
	 *
	 * @param jSubject - valid message subject name
	 * @return status - result of the operation
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetSubject(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jstring jSubject)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				if (jvmOk(jenv, "Message.SetSubject"))
				{
					status = message->SetSubject(subject.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/**
	 * @brief This function get the subject this message will be/was published upon.
	 *
	 * @param [out] jGsubject - subject of the message
	 * @return status - result of the operation
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetSubject(JNIEnv *jenv, jclass jcls, jlong jMessagePtr, jobject jMessage, jobject jGsubject)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMessagePtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				const char *subject = NULL;
				status = message->GetSubject(subject);

				jstring s = 0;
				if (!status.isError() && subject)
					s = makeJavaString(jenv, subject);

				if (jvmOk(jenv, "Message.GetSubject"))
				{
					jenv->SetObjectField(jGsubject, Cache::getCache().fieldString_value, s);
					jvmOk(jenv, "Message.GetSubject");
				}

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/**
	 * @brief This function will add a new field to this message. A new field will be added to the message
	 * that matches the field passed in. Therefore the field is owned by the client program and can be reused.
	 * The client program must also clean up that field when no longer needed.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1AddField(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jlong jFieldPtr, jobject jField)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				Field *field = JNI_JLONG_TO_FIELD(jFieldPtr);
				if (!field)
				{
					SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
				}
				else
				{
					status = message->AddField(*field);

					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);

	}


	/**
	 * @brief This function will copy a field within a message into a field created by the client program, by name.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetField(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jstring jName, jlong jFldPtr, jobject jField)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else if (!jFldPtr)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field object is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				jvmOk(jenv, "Message.GetField");

				Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

				status = message->GetField(name.c_str(), *field);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function gets the number of fields contained in this message. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetFieldCount(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jobject jGcount)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				GMSEC_I32 count = 0;
				status = message->GetFieldCount(count);

				jenv->SetIntField(jGcount, Cache::getCache().fieldI32_value, (jint) count);
				jvmOk(jenv, "Message.GetFieldCount");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Gets the first field in the specified Message. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetFirstField(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jlong jFldPtr, jobject jField)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (message == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else if (jFldPtr == (jlong) 0)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field object is null");
			}
			else
			{
				Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

				status = message->GetFirstField(*field);

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function and GetFirstField() are used to iterate through the fields of a message. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetNextField(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jlong jFldPtr, jobject jField)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else if (!jFldPtr)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field object is null");
			}
			else
			{
				Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

				status = message->GetNextField(*field);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function sets the Configuration of this message.  This can be used to
	 * pass middleware specific items to the Message object.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetConfig(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jlong jCfgPtr, jobject jConfig)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (message == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);
				status = message->SetConfig(config);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Converts a message to XML. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1ToXML(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jobject jGxml)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				const char *xml = NULL;
				status = message->ToXML(xml);

				if (!status.isError())
				{
					if (jvmOk(jenv, "Message.ToXML"))
					{
						jstring s = makeJavaString(jenv, xml);

						// Convert the Java String into the Java GMSEC_String object
						jenv->SetObjectField(jGxml, Cache::getCache().fieldString_value, s);

						jvmOk(jenv, "Message.ToXML");
						created = new Status(status);
					}
				}

			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Creates a new message from XML.
	 * @param [out] jMessage the new message.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1FromXML(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jstring jxml)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				JStringManager xml(jenv, jxml);
				if (jvmOk(jenv, "Message.FromXML"))
				{
					status = message->FromXML(xml.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}

	/** @brief Converts a message to JSON. */
	JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1ToJSON
	(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
	{
		jstring s = 0;

		try
		{
			Message* message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				const char* json = message->ToJSON();
				s = makeJavaString(jenv, json);
				jvmOk(jenv, "Message.ToJSON");
			}
		}
		JNI_CATCH

		return s;
	}

	/** @brief Creates a new message from JSON.
	 * @param [out] jMessage the new message.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1FromJSON(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jstring jjson)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				JStringManager json(jenv, jjson);
				if (jvmOk(jenv, "Message.FromJSON"))
				{
					status = message->FromJSON(json.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function will return the current GMT time/date in GMSEC standard format */
	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTime(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		jstring s = 0;

		try
		{
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			const char *tmp = "";

			if (message == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				tmp = message->GetTime();
				s = makeJavaString(jenv, tmp);
				jvmOk(jenv, "Message.GetTime");
			}
		}
		JNI_CATCH

		return s;
	}


	/** @brief This function will return the physical storage size of the message in bytes. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetMSGSize(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jobject jGsize)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				GMSEC_U32 size = 0;
				status = message->GetMSGSize(size);

				jenv->SetIntField(jGsize, Cache::getCache().fieldU32_value, (jint) size);
				jvmOk(jenv, "Message.GetMSGSize");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Clears all the fields of the specified Message. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1ClearFields(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				status = message->ClearFields();
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Clears the field with the specified name from the specified Message. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1ClearField(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jstring jName)
	{
		Status *created = 0;

		try
		{
			Status status;
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				status = message->ClearField(name.c_str());
				created = new Status(status);
			}

		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}

	/**
	 * The Set/GetTracking functions are left here in case someday they are supported within the C++ binding of the GMSEC API.
	 *
	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTracking(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTracking(flag);
		#endif
	}

	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTrackingNode(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTrackingNode(flag);
		#endif
	}

	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTrackingProcessId(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTrackingProcessId(flag);
		#endif
	}

	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTrackingConnectionId(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTrackingConnectionId(flag);
		#endif
	}

	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTrackingUserName(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTrackingUserName(flag);
		#endif
	}

	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTrackingPublishTime(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTrackingPublishTime(flag);
		#endif
	}

	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTrackingUniqueId(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTrackingUniqueId(flag);
		#endif
	}

	JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1SetTrackingMwInfo(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage, jshort jFlag)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}

			// Do not permit unknown values.
			int flag = MESSAGE_TRACKINGFIELDS_UNSET;
			if (jFlag == MESSAGE_TRACKINGFIELDS_UNSET || jFlag == MESSAGE_TRACKINGFIELDS_ON || jFlag == MESSAGE_TRACKINGFIELDS_OFF)
			{
				flag = jFlag;
			}

			message->SetTrackingMwInfo(flag);
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTracking(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTracking();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTrackingNode(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTrackingNode();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTrackingProcessId(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTrackingProcessId();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTrackingConnectionId(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTrackingConnectionId();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTrackingUserName(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTrackingUserName();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTrackingPublishTime(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTrackingPublishTime();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTrackingUniqueId(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTrackingUniqueId();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}

	JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Message_1GetTrackingMwInfo(JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMessage)
	{
		#ifdef GMSEC_EXPOSE_TRACKING
			Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			if (!message)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
			}
			return message->GetTrackingMwInfo();
		#else
			return MESSAGE_TRACKINGFIELDS_UNSET;
		#endif
	}
	 */

#ifdef __cplusplus
}
#endif
