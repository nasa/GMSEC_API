/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Connection class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Callbacks.h"
#include "gmsecJNI_Jenv.h"

//TODO DMW #include <gmsec/internal/BaseConnection.h>
#include <gmsec/Connection.h>
#include <gmsec/ConnectionFactory.h>

using gmsec::Callback;
using gmsec::Config;
using gmsec::Connection;
using gmsec::ConnectionFactory;
using gmsec::Message;
using gmsec::Status;


#ifdef __cplusplus
extern "C"
{
#endif

	/** @brief Deletes a Connection object. */
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_delete_1Connection(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		try
		{
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				//TODO DMW gmsec::internal::ConnectionBuddy::destroy(connection);
				ConnectionFactory::Destroy(connection);
			}
		}
		JNI_CATCH
	}


	/** @brief Connects a connection. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Connect(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				status = connection->Connect();
				//TODO: Is this needed?
				//connection->RegisterErrorCallback("*", new JNIErrorCallback());
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Disconnects a connection. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Disconnect(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				status = connection->Disconnect();
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Subscribes to the specified subject on the specified connection. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Subscribe(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				if (jvmOk(jenv, "Connection.Subscribe"))
				{
					status = connection->Subscribe(subject.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Subscribes to the specified subject on the specified connection, and calls Callback when Message is available. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Subscribe_1Via_1Callback(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject, jobject jCallback)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				JNICallback *callback = new JNICallback(jenv, jConnection, jSubject, jCallback);
				if (jvmOk(jenv, "Connection.Subscribe"))
				{
					status = connection->Subscribe(subject.c_str(), callback);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** Unsubscribes from the specified subject on the specified connection. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1UnSubscribe(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				if (jvmOk(jenv, "Connection.UnSubscribe"))
				{
					status = connection->UnSubscribe(subject.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1UnSubscribe_1Via_1Callback(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject, jobject jCallback)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jCallback)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Callback object is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				if (jvmOk(jenv, "Connection.UnSubscribe"))
				{
					Callback* callback = Cache::getCache().getAndRemoveCallback(jenv, jSubject, jCallback);
					if (callback != NULL)
					{
						status = connection->UnSubscribe(subject.c_str(), callback);
						delete callback;
						created = new Status(status);
					}
					else
					{
						created = new Status(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CALLBACK, "No subscription for given Callback");
					}
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1RegisterErrorCallback(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jEvent, jobject jCallback)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jCallback)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ErrorCallback object is null");
			}
			else
			{
				JStringManager event(jenv, jEvent);
				if (jvmOk(jenv, "Connection.RegisterErrorCallback"))
				{
					JNIErrorCallback *callback = new JNIErrorCallback(jenv, jCallback);
					status = connection->RegisterErrorCallback(event.c_str(), callback);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1CreateMessage__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnection_2Lgov_nasa_gsfc_gmsecapi_jni_JNIMessage_2(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConnection, jobject jMessage)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(ptr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jMessage)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message object is null");
			}
			else
			{
				Message *message = NULL;
				status = connection->CreateMessage(message);

				// set swigCPtr to the C++ Connection object pointer
				jenv->SetLongField(jMessage, Cache::getCache().fieldMessage_swigCPtr,
				                   JNI_POINTER_TO_JLONG(message));
				jvmOk(jenv, "Connection.CreateMessage");

				// set swigCMemOwn to true
				jboolean z = message ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jMessage, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "Connection.CreateMessage");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1CreateMessage__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnection_2Ljava_lang_String_2ILgov_nasa_gsfc_gmsecapi_jni_JNIMessage_2(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject, jint jKind, jobject jMessage)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jMessage)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message object is null");
			}
			else if (!jSubject)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				Message *message = NULL;
				status = connection->CreateMessage(subject.c_str(), (unsigned short) jKind, message);

				if (jvmOk(jenv, "Connection.CreateMessage"))
				{
					// set swigCPtr to the C++ Connection object pointer
					jenv->SetLongField(jMessage,
					                   Cache::getCache().fieldMessage_swigCPtr,
					                   JNI_POINTER_TO_JLONG(message));
					jvmOk(jenv, "Connection.CreateMessage");

					// set swigCMemOwn to true
					jboolean z = message ? JNI_TRUE : JNI_FALSE;
					jenv->SetBooleanField(jMessage,
					                      Cache::getCache().fieldMessage_swigCMemOwn,
					                      z);
					jvmOk(jenv, "Connection.CreateMessage");

					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Creates a message from the specified Config. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1CreateMessageCFG(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject, jint jKind, jobject jMessage, jlong jCfgPtr, jobject jConfig)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jMessage)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message object is null");
			}
			else if (!jSubject)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				Message *message = NULL;
				Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);
				if (jvmOk(jenv, "Connection.CreateMessage"))
				{
					status = connection->CreateMessage(subject.c_str(), (unsigned short) jKind, message, config);

					// set swigCPtr to the C++ Message object pointer
					jenv->SetLongField(jMessage,
					                   Cache::getCache().fieldMessage_swigCPtr,
					                   JNI_POINTER_TO_JLONG(message));
					jvmOk(jenv, "Connection.CreateMessage");

					// set swigCMemOwn to true
					jboolean z = message ? JNI_TRUE : JNI_FALSE;
					jenv->SetBooleanField(jMessage,
					                      Cache::getCache().fieldMessage_swigCMemOwn,
					                      z);
					jvmOk(jenv, "Connection.CreateMessage");
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/**  @brief Destroys a message. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1DestroyMessage(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jlong jMsgPtr, jobject jMessage)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
				status = connection->DestroyMessage(message);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function will publish a message to the middleware. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Publish(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jlong jMsgPtr, jobject jMessage)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
				status = connection->Publish(message);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function will send a request, wait for the specified timeout, and return the received reply
		 * This function will block until the reply is received or the timeout is reached.
		 * The timeout value is expressed in milliseconds. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Request(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jlong jReqMsgPtr, jobject jRequest, jint jTimeout, jobject jReplyMsg)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jReplyMsg)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message object is null");
			}
			else
			{
				Message *request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);

				Message *reply = NULL;
				status = connection->Request(request, (GMSEC_I32) jTimeout, reply);

				// set swigCPtr to the C++ Message object pointer
				jenv->SetLongField(jReplyMsg,
				                   Cache::getCache().fieldMessage_swigCPtr,
				                   JNI_POINTER_TO_JLONG(reply));
				jvmOk(jenv, "Connection.Request");

				// set swigCMemOwn to true
				jboolean z = reply ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jReplyMsg,
				                      Cache::getCache().fieldMessage_swigCMemOwn,
				                      z);
				jvmOk(jenv, "Connection.Request");

				created = new Status(status);
			}

		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function will send a request, wait for the specified timeout, and register the
	     * specified callback. The timeout value is expressed in milliseconds. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Request_1Via_1Callback(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jlong jReqMsgPtr, jobject jRequest, jint jTimeout, jobject jCallback, jboolean replyCallback)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jCallback)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Callback object is null");
			}
			else
			{
				Message *request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
				if (!request)
				{
					SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request object is null");
				}
				else if (replyCallback)
				{

					JNIReplyCallback *callback = new JNIReplyCallback(jenv, jCallback);
					status = connection->Request(request, (GMSEC_I32) jTimeout, callback);

					created = new Status(status);
				}
				else
				{
					JNICallback *callback = new JNICallback(jenv, jConnection, jCallback);
					status = connection->Request(request, (GMSEC_I32) jTimeout, callback);

					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function will send a reply to a given request. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1Reply(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jlong jReqMsgPtr, jobject jRequest, jlong jReplyMsgPtr, jobject jReply)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				Message *request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
				Message *reply = JNI_JLONG_TO_MESSAGE(jReplyMsgPtr);

				status = connection->Reply(request, reply);

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function will cause the any callbacks that are registered with matching
		 * patterns to be called. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1GetNextMsg(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jobject jMessage, jint jTimeout)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jMessage)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message object is null");
			}
			else
			{
				Message *message = NULL;
				status = connection->GetNextMsg(message, (GMSEC_I32) jTimeout);

				// set swigCPtr to the C++ Message object pointer
				jenv->SetLongField(jMessage,
				                   Cache::getCache().fieldMessage_swigCPtr,
				                   JNI_POINTER_TO_JLONG(message));
				jvmOk(jenv, "Connection.GetNextMsg");

				// set swigCMemOwn to true
				jboolean z = message ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jMessage,
				                      Cache::getCache().fieldMessage_swigCMemOwn,
				                      z);
				jvmOk(jenv, "Connection.GetNextMsg");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1StartAutoDispatch(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				status = connection->StartAutoDispatch();
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1StopAutoDispatch(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				status = connection->StopAutoDispatch();
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1DispatchMsg(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jlong jMsgPtr, jobject jMsg)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jMsgPtr)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message object is null");
			}
			else
			{
				Message *message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

				status = connection->DispatchMsg(message);

				jvmOk(jenv, "Connection.DispatchMsg");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1GetLastDispatcherStatus(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				status = connection->GetLastDispatcherStatus();
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1ExcludeSubject(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			if(!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				if (jvmOk(jenv, "Connection.ExcludeSubject"))
				{
					status = connection->ExcludeSubject(subject.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1RemoveExcludedSubject(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jstring jSubject)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			if(!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				JStringManager subject(jenv, jSubject);
				if (jvmOk(jenv, "Connection.RemoveExcludedSubject"))
				{
					status = connection->RemoveExcludedSubject(subject.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief This function identifies the root library name and therefore the
		 * connection type that this connection is associated with. For example,
		 * the root library name for the icsswb middleware library is "gmsec_icsswb"
		 * and matches the name of the windows library (gmsec_icsswb.dll) and the
		 * UNIX library (libgmsec_icsswb.so). */
	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1GetLibraryRootName(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		jstring s = 0;

		try
		{
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			const char *tmp = "";

			if (connection == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				tmp = connection->GetLibraryRootName();
				s = makeJavaString(jenv, tmp);
				jvmOk(jenv, "Connection.GetLibraryRootName");
			}
		}
		JNI_CATCH

		return s;
	}


	/** @brief This function returns a string containing the version information for
	 * this connection's associated middleware. */
	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1GetLibraryVersion(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		jstring s = 0;

		try
		{
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			const char *tmp = "";

			if (connection == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				tmp = connection->GetLibraryVersion();
				s = makeJavaString(jenv, tmp);
				jvmOk(jenv, "Connection.GetLibraryVersion");
			}
		}
		JNI_CATCH

		return s;
	}


	JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1GetPublishQueueMessageCount
		(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
	{
		jlong jCount = 0;

		try
		{
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
			{
				jCount = (jlong) connection->GetPublishQueueMessageCount();
			}
		}
		JNI_CATCH

		return jCount;
	}


	/** @brief Returns whether the specified connection is connected. */
	JNIEXPORT jint JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1IsConnected(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		int isConnected = 0;

		try
		{
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else
				isConnected = connection->IsConnected();
		}
		JNI_CATCH

		return (jint) isConnected;
	}


	/** @brief This function copies a message without knowing what type it is. This
		 * function can be used to copy a message's contents.  */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Connection_1CloneMessage(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection, jlong jInMsgPtr, jobject jInMsg, jobject jOutMsgPtr)
	{
		Status *created = 0;

		try
		{
			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);

			if (!connection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
			}
			else if (!jInMsgPtr)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Input Message object is null");
			}
			else if (!jOutMsgPtr)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Output Message object is null");
			}
			else
			{
				Message *inMessage = JNI_JLONG_TO_MESSAGE(jInMsgPtr);
				Message *outMessage = NULL;

				status = connection->CloneMessage(inMessage, outMessage);

				// set swigCPtr to the C++ Message object pointer
				jenv->SetLongField(jOutMsgPtr,
				                   Cache::getCache().fieldMessage_swigCPtr,
				                   JNI_POINTER_TO_JLONG(outMessage));
				jvmOk(jenv, "Connection.CloneMessage");

				// set swigCMemOwn to true
				jboolean z = outMessage ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jOutMsgPtr,
				                      Cache::getCache().fieldMessage_swigCMemOwn,
				                      z);
				jvmOk(jenv, "Connection.CloneMessage");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}

#ifdef __cplusplus
}
#endif
