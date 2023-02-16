/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Connection class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Callback.h>
#include <gmsec5/Config.h>
#include <gmsec5/Connection.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/EventCallback.h>
#include <gmsec5/ReplyCallback.h>
#include <gmsec5/Message.h>
#include <gmsec5/Specification.h>
#include <gmsec5/SubscriptionInfo.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/StdSharedPtr.h>



using namespace gmsec::api5;
using namespace gmsec::api5::jni;
using namespace gmsec::api5::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Create
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jobject jConnection)
{
	jlong jConn = 0;

	try
	{
		Config* cfg = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}

		Connection* conn = new Connection(*cfg);

		jConn = JNI_POINTER_TO_JLONG(conn);

		jenv->SetLongField(jConnection, Cache::getCache().fieldConnection_swigCPtr, jConn);
		jvmOk(jenv, "Connection.create");

		jenv->SetBooleanField(jConnection, Cache::getCache().fieldConnection_swigCMemOwn, JNI_TRUE);
		jvmOk(jenv, "Connection.create");

		Cache::getCache().putConnection(conn, jConnection);
	}
	JNI_CATCH

	return jConn;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1CreateWithFactory
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jlong jFactoryPtr, jobject jFactory, jobject jConnection)
{
	jlong jConn = 0;

	try
	{
		Config*         cfg     = JNI_JLONG_TO_CONFIG(jConfigPtr);
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jFactoryPtr);

		if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}

		Connection* conn = new Connection(*cfg, *factory);

		jConn = JNI_POINTER_TO_JLONG(conn);

		jenv->SetLongField(jConnection, Cache::getCache().fieldConnection_swigCPtr, jConn);
		jvmOk(jenv, "Connection.create");

		jenv->SetBooleanField(jConnection, Cache::getCache().fieldConnection_swigCMemOwn, JNI_TRUE);
		jvmOk(jenv, "Connection.create");

		Cache::getCache().putConnection(conn, jConnection);
	}
	JNI_CATCH

	return jConn;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Destroy
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}

		Cache::getCache().removeConnection(conn);

		delete conn;
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetAPIVersion
  (JNIEnv *jenv, jclass jcls)
{
	jstring jVersion = NULL;

	try
	{
		const char* tmp = Connection::getAPIVersion();

		jVersion = makeJavaString(jenv, tmp);

		jvmOk(jenv, "Connection.getAPIVersion");
	}
	JNI_CATCH

	return jVersion;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Connect
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			conn->connect();
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Disconnect
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			conn->disconnect();
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetLibraryName
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jstring jName = NULL;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			const char* name = conn->getLibraryName();

			jName = makeJavaString(jenv, name);

			jvmOk(jenv, "Connection.getLibraryName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetLibraryVersion
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jstring jVersion = NULL;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			const char* version = conn->getLibraryVersion();

			jVersion = makeJavaString(jenv, version);

			jvmOk(jenv, "Connection.getLibraryVersion");
		}
	}
	JNI_CATCH

	return jVersion;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetMessageFactory
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jlong jMsgFactoryPtr = 0;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			MessageFactory& factory = conn->getMessageFactory();

			jMsgFactoryPtr = JNI_POINTER_TO_JLONG(&factory);
		}
	}
	JNI_CATCH

	return jMsgFactoryPtr;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1RegisterEventCallback
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jint jEvent, jlong jEventCallbackPtr)
{
	try
	{
		Connection*       conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Connection::Event event    = static_cast<Connection::Event>(jEvent);
		EventCallback*    callback = reinterpret_cast<EventCallback*>(jEventCallbackPtr);


		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (callback == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "EventCallback reference is null");
		}
		else
		{
			conn->registerEventCallback(event, callback);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject)
{
	jlong jInfo = 0;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "Connection.subscribe(subject)"))
			{
				SubscriptionInfo* info = conn->subscribe(subject.c_str());

				jInfo = JNI_POINTER_TO_JLONG(info);
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2J
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject, jlong jCallbackPtr)
{
	jlong jInfo = 0;

	try
	{
		Connection* conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Callback*   callback = reinterpret_cast<Callback*>(jCallbackPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (callback == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Callback reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "Connection.subscribe(subject, callback)"))
			{
				SubscriptionInfo* info = conn->subscribe(subject.c_str(), callback);

				jInfo = JNI_POINTER_TO_JLONG(info);
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject, jlong jConfigPtr, jobject jConfig)
{
	jlong jInfo = 0;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Config*     cfg  = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "Connection.subscribe(subject, callback)"))
			{
				SubscriptionInfo* info = conn->subscribe(subject.c_str(), *cfg);

				jInfo = JNI_POINTER_TO_JLONG(info);
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2J
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject, jlong jConfigPtr, jobject jConfig, jlong jCallbackPtr)
{
	jlong jInfo = 0;

	try
	{
		Connection* conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Config*     cfg      = JNI_JLONG_TO_CONFIG(jConfigPtr);
		Callback*   callback = reinterpret_cast<Callback*>(jCallbackPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else if (callback == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Callback reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "Connection.subscribe(subject, callback)"))
			{
				SubscriptionInfo* info = conn->subscribe(subject.c_str(), *cfg, callback);

				jInfo = JNI_POINTER_TO_JLONG(info);
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Unsubscribe
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jSubInfoPtr, jobject jSubInfo)
{
	try
	{
		Connection*       conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
		SubscriptionInfo* info = JNI_JLONG_TO_SUBSCRIPTION_INFO(jSubInfoPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (info == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionInfo reference is null");
		}
		else
		{
			conn->unsubscribe(info);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1StartAutoDispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			jResult = (conn->startAutoDispatch() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1StopAutoDispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jboolean jWaitForCompletion)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			jResult = (conn->stopAutoDispatch(jWaitForCompletion == JNI_TRUE) ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}



JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Publish__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Message*    msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			conn->publish(*msg);
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Publish__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jMsgPtr, jobject jMsg, jlong jConfigPtr, jobject jConfig)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Message*    msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);
		Config*     cfg  = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else if (cfg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			conn->publish(*msg, *cfg);
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Request__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2IJI
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jlong jReplyCallbackPtr, jint jRepublish_ms)
{
	try
	{
		Connection*    conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Message*       reqMsg   = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
		ReplyCallback* callback = reinterpret_cast<ReplyCallback*>(jReplyCallbackPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (reqMsg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message reference is null");
		}
		else if (callback == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ReplyCallback reference is null");
		}
		else
		{
			conn->request(*reqMsg, (int) jTimeout, callback, (int) jRepublish_ms);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Request__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2II
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jint jRepublish_ms)
{
	jlong jRepMsg = 0;

	try
	{
		Connection* conn   = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Message*    reqMsg = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (reqMsg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message reference is null");
		}
		else
		{
			Message* repMsg = conn->request(*reqMsg, (int) jTimeout, (int) jRepublish_ms);

			if (repMsg)
			{
				jRepMsg = JNI_POINTER_TO_JLONG(repMsg);
			}
		}
	}
	JNI_CATCH

	return jRepMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Reply
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jReqMsgPtr, jobject jReqMsg, jlong jRepMsgPtr, jobject jRepMsg)
{
	try
	{
		Connection* conn   = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Message*    reqMsg = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
		Message*    repMsg = JNI_JLONG_TO_MESSAGE(jRepMsgPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (reqMsg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message reference is null");
		}
		else if (repMsg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Reply Message reference is null");
		}
		else
		{
			conn->reply(*reqMsg, *repMsg);
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Dispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
		Message*    msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			conn->dispatch(*msg);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Receive
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jint jTimeout)
{
	jlong jMsg = 0;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			Message* msg = conn->receive((int) jTimeout);

			if (msg)
			{
				jMsg = JNI_POINTER_TO_JLONG(msg);
			}
		}
	}
	JNI_CATCH

	return jMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1ExcludeSubject
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "Connection.excludeSubject()"))
			{
				conn->excludeSubject(subject.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1RemoveExcludedSubject
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "Connection.removeExcludedSubject()"))
			{
				conn->removeExcludedSubject(subject.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jstring jName = NULL;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			const char* tmp = conn->getName();

			jName = makeJavaString(jenv, tmp);

			jvmOk(jenv, "Connection.getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1SetName
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jName)
{
	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Connection.setName"))
			{
				conn->setName(name.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetID
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jstring jID = NULL;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			const char* tmp = conn->getID();

			jID = makeJavaString(jenv, tmp);

			jvmOk(jenv, "Connection.getID");
		}
	}
	JNI_CATCH

	return jID;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetMWInfo
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jstring jInfo = NULL;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			const char* tmp = conn->getMWInfo();

			jInfo = makeJavaString(jenv, tmp);

			jvmOk(jenv, "Connection.getMWInfo");
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetConnectionEndpoint
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jstring jEndpoint = NULL;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (conn == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			const char* endpoint = conn->getConnectionEndpoint();

			jEndpoint = makeJavaString(jenv, endpoint);

			jvmOk(jenv, "Connection.getConnectionEndpoint");
		}
	}
	JNI_CATCH

	return jEndpoint;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetPublishQueueMessageCount
	(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	jlong jCount = 0;

	try
	{
		Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

		if (!conn)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection reference is null");
		}
		else
		{
			jCount = (jlong) conn->getPublishQueueMessageCount();
		}
	}
	JNI_CATCH

	return jCount;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1ShutdownAllMiddlewares
  (JNIEnv *jenv, jclass jcls)
{
	try
	{
		Connection::shutdownAllMiddlewares();
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1ShutdownMiddleware
  (JNIEnv *jenv, jclass jcls, jstring jName)
{
	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "Connection.shutdownMiddleware"))
		{
			Connection::shutdownMiddleware(name.c_str());
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
