/*
 * Copyright 2007-2024 United States Government as represented by the
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
	Config* cfg = JNI_JLONG_TO_CONFIG(jConfigPtr);

	jlong jConn = 0;

	try
	{
		// A GmsecException can be thrown if the Connection cannot be created
		Connection* conn = new Connection(*cfg);

		jConn = JNI_POINTER_TO_JLONG(conn);

		jenv->SetLongField(jConnection, Cache::getCache().fieldConnection_swigCPtr, jConn);

		if (jvmOk(jenv, "Connection.create"))
		{
			jenv->SetBooleanField(jConnection, Cache::getCache().fieldConnection_swigCMemOwn, JNI_TRUE);

			if (jvmOk(jenv, "Connection.create"))
			{
				Cache::getCache().putConnection(conn, jConnection);
			}
			else
			{
				delete conn;
				jConn = 0;
			}
		}
		else
		{
			delete conn;
			jConn = 0;
		}
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jConn;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1CreateWithFactory
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jlong jFactoryPtr, jobject jFactory, jobject jConnection)
{
	Config*         cfg     = JNI_JLONG_TO_CONFIG(jConfigPtr);
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jFactoryPtr);

	jlong jConn = 0;

	try
	{
		// A GmsecException can be thrown if the Connection cannot be created
		Connection* conn = new Connection(*cfg, *factory);

		jConn = JNI_POINTER_TO_JLONG(conn);

		jenv->SetLongField(jConnection, Cache::getCache().fieldConnection_swigCPtr, jConn);

		if (jvmOk(jenv, "Connection.create"))
		{
			jenv->SetBooleanField(jConnection, Cache::getCache().fieldConnection_swigCMemOwn, JNI_TRUE);

			if (jvmOk(jenv, "Connection.create"))
			{
				Cache::getCache().putConnection(conn, jConnection);
			}
			else
			{
				delete conn;
				jConn = 0;
			}
		}
		else
		{
			delete conn;
			jConn = 0;
		}
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jConn;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Destroy
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	Cache::getCache().removeConnection(conn);

	delete conn;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetAPIVersion
  (JNIEnv *jenv, jclass jcls)
{
	const char* tmp = Connection::getAPIVersion();

	jstring jVersion = makeJavaString(jenv, tmp);

	jvmOk(jenv, "Connection.getAPIVersion");

	return jVersion;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Connect
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	try
	{
		// A GmsecException can be thrown if we fail to connect for any reason
		conn->connect();
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Disconnect
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	conn->disconnect();
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetLibraryName
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	const char* name = conn->getLibraryName();

	jstring jName = makeJavaString(jenv, name);

	jvmOk(jenv, "Connection.getLibraryName");

	return jName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetLibraryVersion
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	const char* version = conn->getLibraryVersion();

	jstring jVersion = makeJavaString(jenv, version);

	jvmOk(jenv, "Connection.getLibraryVersion");

	return jVersion;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetConfig
(JNIEnv* jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	Config& config = conn->getConfig();

	return JNI_POINTER_TO_JLONG(&config);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetMessageFactory
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	MessageFactory& factory = conn->getMessageFactory();

	return JNI_POINTER_TO_JLONG(&factory);
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1RegisterEventCallback
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jint jEvent, jlong jEventCallbackPtr)
{
	Connection*       conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Connection::Event event    = static_cast<Connection::Event>(jEvent);
	EventCallback*    callback = reinterpret_cast<EventCallback*>(jEventCallbackPtr);

	conn->registerEventCallback(event, callback);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	jlong jInfo = 0;

	JStringManager subject(jenv, jSubject);

	if (jvmOk(jenv, "Connection.subscribe(subject)"))
	{
		try
		{
			// A GmsecException can be thrown for any of the following reasons:
			//   * we are not connected
			//   * the subject is not compliant
			SubscriptionInfo* info = conn->subscribe(subject.c_str());

			jInfo = JNI_POINTER_TO_JLONG(info);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2J
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject, jlong jCallbackPtr)
{
	Connection* conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Callback*   callback = reinterpret_cast<Callback*>(jCallbackPtr);

	jlong jInfo = 0;

	JStringManager subject(jenv, jSubject);

	if (jvmOk(jenv, "Connection.subscribe(subject, callback)"))
	{
		try
		{
			// A GmsecException can be thrown for any of the following reasons:
			//   * we are not connected
			//   * the subject is not compliant
			SubscriptionInfo* info = conn->subscribe(subject.c_str(), callback);

			jInfo = JNI_POINTER_TO_JLONG(info);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject, jlong jConfigPtr, jobject jConfig)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Config*     cfg  = JNI_JLONG_TO_CONFIG(jConfigPtr);

	jlong jInfo = 0;

	JStringManager subject(jenv, jSubject);

	if (jvmOk(jenv, "Connection.subscribe(subject, callback)"))
	{
		try
		{
			// A GmsecException can be thrown for any of the following reasons:
			//   * we are not connected
			//   * the subject is not compliant
			SubscriptionInfo* info = conn->subscribe(subject.c_str(), *cfg);

			jInfo = JNI_POINTER_TO_JLONG(info);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Subscribe__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2J
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject, jlong jConfigPtr, jobject jConfig, jlong jCallbackPtr)
{
	Connection* conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Config*     cfg      = JNI_JLONG_TO_CONFIG(jConfigPtr);
	Callback*   callback = reinterpret_cast<Callback*>(jCallbackPtr);

	jlong jInfo = 0;

	JStringManager subject(jenv, jSubject);

	if (jvmOk(jenv, "Connection.subscribe(subject, callback)"))
	{
		try
		{
			// A GmsecException can be thrown for any of the following reasons:
			//   * we are not connected
			//   * the subject is not compliant
			SubscriptionInfo* info = conn->subscribe(subject.c_str(), *cfg, callback);

			jInfo = JNI_POINTER_TO_JLONG(info);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jInfo;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Unsubscribe
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jSubInfoPtr, jobject jSubInfo)
{
	Connection*       conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
	SubscriptionInfo* info = JNI_JLONG_TO_SUBSCRIPTION_INFO(jSubInfoPtr);

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * the SubscriptionInfo object did not originate from the Connection object
		conn->unsubscribe(info);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1StartAutoDispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	jboolean jResult = JNI_FALSE;

	try
	{
		// A GmsecException can be thrown if we are not connected
		jResult = (conn->startAutoDispatch() ? JNI_TRUE : JNI_FALSE);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jResult;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1StopAutoDispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jboolean jWaitForCompletion)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	jboolean jResult = JNI_FALSE;

	try
	{
		// A GmsecException can be thrown if we are not connected
		jResult = (conn->stopAutoDispatch(jWaitForCompletion == JNI_TRUE) ? JNI_TRUE : JNI_FALSE);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jResult;
}



JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Publish__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jMsgPtr, jobject jMsg)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Message*    msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * the message is not a PUBLISH message
		//   * the message lacks a subject
		//   * the message is not compliant (when validation is enabled)
		//   * the middleware can throw an exception
		conn->publish(*msg);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Publish__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jMsgPtr, jobject jMsg, jlong jConfigPtr, jobject jConfig)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Message*    msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);
	Config*     cfg  = JNI_JLONG_TO_CONFIG(jConfigPtr);

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * the message is not a PUBLISH message
		//   * the message lacks a subject
		//   * the message is not compliant (when validation is enabled)
		//   * the middleware can throw an exception
		conn->publish(*msg, *cfg);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Request__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2IJI
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jlong jReplyCallbackPtr, jint jRepublish_ms)
{
	Connection*    conn     = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Message*       reqMsg   = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
	ReplyCallback* callback = reinterpret_cast<ReplyCallback*>(jReplyCallbackPtr);

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * the request message is not a REQUEST message
		//   * the request message lacks a subject
		//   * the request message is not compliant (when validation is enabled)
		//   * the middleware can throw an exception
		conn->request(*reqMsg, static_cast<int>(jTimeout), callback, static_cast<int>(jRepublish_ms));
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Request__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConnection_2JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessage_2II
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jint jRepublish_ms)
{
	Connection* conn   = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Message*    reqMsg = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);

	jlong jRepMsg = 0;

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * the request message is not a REQUEST message
		//   * the request message lacks a subject
		//   * the request message is not compliant (when validation is enabled)
		//   * the middleware can throw an exception
		Message* repMsg = conn->request(*reqMsg, static_cast<int>(jTimeout), static_cast<int>(jRepublish_ms));

		if (repMsg)
		{
			jRepMsg = JNI_POINTER_TO_JLONG(repMsg);
		}
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jRepMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Reply
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jReqMsgPtr, jobject jReqMsg, jlong jRepMsgPtr, jobject jRepMsg)
{
	Connection* conn   = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Message*    reqMsg = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
	Message*    repMsg = JNI_JLONG_TO_MESSAGE(jRepMsgPtr);

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * the request message is not a REQUEST message
		//   * the reply message is not a REPLY message
		//   * the reply message lacks a subject
		//   * the reply message is not compliant (when validation is enabled)
		//   * the middleware can throw an exception
		conn->reply(*reqMsg, *repMsg);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Dispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jlong jMsgPtr, jobject jMsg)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);
	Message*    msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * the auto-dispatcher is running
		//   * the message lacks a subject
		//   * the message is not compliant (when validation is enabled)
		conn->dispatch(*msg);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1Receive
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jint jTimeout)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	jlong jMsg = 0;

	try
	{
		// A GmsecException can be thrown for any of the following reasons:
		//   * we are not connected
		//   * a middleware issue occurs
		Message* msg = conn->receive(static_cast<int>(jTimeout));

		if (msg)
		{
			jMsg = JNI_POINTER_TO_JLONG(msg);
		}
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1ExcludeSubject
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	JStringManager subject(jenv, jSubject);

	if (jvmOk(jenv, "Connection.excludeSubject()"))
	{
		try
		{
			// A GmsecException can be thrown for any of the following reasons:
			//   * we are not connected
			//   * the subject is non-compliant
			conn->excludeSubject(subject.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1RemoveExcludedSubject
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jSubject)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	JStringManager subject(jenv, jSubject);

	if (jvmOk(jenv, "Connection.removeExcludedSubject()"))
	{
		try
		{
			// A GmsecException can be thrown for any of the following reasons:
			//   * we are not connected
			//   * the subject is non-compliant
			conn->removeExcludedSubject(subject.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	const char* tmp = conn->getName();

	jstring jName = makeJavaString(jenv, tmp);

	jvmOk(jenv, "Connection.getName");

	return jName;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1SetName
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn, jstring jName)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Connection.setName"))
	{
		conn->setName(name.c_str());
	}
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetID
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	const char* tmp = conn->getID();

	jstring jID = makeJavaString(jenv, tmp);

	jvmOk(jenv, "Connection.getID");

	return jID;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetMWInfo
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	const char* tmp = conn->getMWInfo();

	jstring jInfo = makeJavaString(jenv, tmp);

	jvmOk(jenv, "Connection.getMWInfo");

	return jInfo;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetConnectionEndpoint
  (JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	const char* endpoint = conn->getConnectionEndpoint();

	jstring jEndpoint = makeJavaString(jenv, endpoint);

	jvmOk(jenv, "Connection.getConnectionEndpoint");

	return jEndpoint;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1GetPublishQueueMessageCount
	(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConn)
{
	Connection* conn = JNI_JLONG_TO_CONNECTION(jConnPtr);

	return static_cast<jlong>(conn->getPublishQueueMessageCount());
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1ShutdownAllMiddlewares
  (JNIEnv *jenv, jclass jcls)
{
	Connection::shutdownAllMiddlewares();
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Connection_1ShutdownMiddleware
  (JNIEnv *jenv, jclass jcls, jstring jName)
{
	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Connection.shutdownMiddleware"))
	{
		Connection::shutdownMiddleware(name.c_str());
	}
}


#ifdef __cplusplus
}
#endif
