/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST ConnectionManager class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"
#include "gmsecJNI_CustomSpecification.h"

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/ConnectionManagerCallback.h>
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceParam.h>
#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicSample.h>
#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ServiceParam.h>
#include <gmsec4/mist/SubscriptionInfo.h>

#include <gmsec4/util/Log.h>


using gmsec::api::Config;
using gmsec::api::Connection;
using gmsec::api::Field;
using gmsec::api::Exception;
using gmsec::api::Message;
using gmsec::api::Status;

using gmsec::api::util::DataList;

using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C"
{
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ConnectionManager__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jlong created = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new ConnectionManager(*config));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ConnectionManager__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Z
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jboolean jValidate)
{
	jlong created = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new ConnectionManager(*config, jValidate == JNI_TRUE));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ConnectionManager__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2ZI
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jboolean jValidate, jint jSpecVersion)
{
	jlong created = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new ConnectionManager(*config, jValidate == JNI_TRUE, (unsigned int) jSpecVersion));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ConnectionManager
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			delete connMgr;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Initialize
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				connMgr->initialize();
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Cleanup
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			connMgr->cleanup();
		}
	}
	JNI_CATCH
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetState
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jint jState = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (connMgr == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			jState = (jint) connMgr->getState();
		}
	}
	JNI_CATCH

	return jState;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetLibraryRootName
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jstring jRootName = NULL;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				const char* rootname = connMgr->getLibraryRootName();

				jRootName = makeJavaString(jenv, rootname);

				jvmOk(jenv, "ConnectionManager.getLibraryRootName");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jRootName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetLibraryVersion
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jstring jVersion = NULL;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				const char* version = connMgr->getLibraryVersion();

				jVersion = makeJavaString(jenv, version);

				jvmOk(jenv, "ConnectionManager.getLibraryVersion");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jVersion;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetSpecification
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jlong jSpec = 0;

	ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

	if (!connMgr)
	{
		SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
	}
	else
	{
		try
		{
			Specification& spec = connMgr->getSpecification();

			jSpec = JNI_POINTER_TO_JLONG(&spec);
		}
		JNI_CATCH
	}

	return jSpec;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1SetStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
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

				connMgr->setStandardFields(fields);

				jenv->ReleaseLongArrayElements(jFieldPtrs, fldptrs, 0);
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlongArray JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jlongArray standardFields = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			const DataList<Field*>& fields = connMgr->getStandardFields();

			GMSEC_U64* fieldAddrs = new GMSEC_U64[fields.size()];

			size_t n = 0;
			for (DataList<Field*>::iterator it = fields.begin(); it != fields.end(); ++it, ++n)
			{
				fieldAddrs[n] = GMSEC_U64(*it);
			}

			standardFields = jenv->NewLongArray(fields.size());
			jenv->SetLongArrayRegion(standardFields, 0, fields.size(), (jlong*) fieldAddrs);

			delete [] fieldAddrs;
		}
	}
	JNI_CATCH

	return standardFields;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1AddStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           msg     = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			connMgr->addStandardFields(*msg);
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RegisterMessageValidator
  (JNIEnv *jenv, jclass jclass, jlong jConnMgrPtr, jobject jConnMgr, jlong jValPtr)
{
	try
	{
		ConnectionManager* connMgr   = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
        MessageValidator*  validator = reinterpret_cast<MessageValidator*>(jValPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!validator)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageValidator reference is null");
		}
		else
		{
			connMgr->registerMessageValidator(validator);
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RegisterEventCallback
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jint jEvent, jlong jEventCallbackPtr)
{
	try
	{
		ConnectionManager*              connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Connection::ConnectionEvent     event    = static_cast<Connection::ConnectionEvent>(jEvent);
        ConnectionManagerEventCallback* callback = reinterpret_cast<ConnectionManagerEventCallback*>(jEventCallbackPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!callback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManagerEventCallback reference is null");
		}
		else
		{
			try
			{
				connMgr->registerEventCallback(event, callback);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Subscribe__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject)
{
	jlong jInfo = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.subscribe(subject)"))
			{
				try
				{
					SubscriptionInfo* info = connMgr->subscribe(subject.c_str());

					jInfo = JNI_POINTER_TO_JLONG(info);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Subscribe__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2J
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jCallbackPtr)
{
	jlong jInfo = 0;

	try
	{
		ConnectionManager*         connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
        ConnectionManagerCallback* callback = reinterpret_cast<ConnectionManagerCallback*>(jCallbackPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
        else if (callback == NULL)
        {
            SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManagerCallback reference is null");
        }
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.subscribe(subject, callback)"))
			{
				try
				{
					SubscriptionInfo* info = connMgr->subscribe(subject.c_str(), callback);

					jInfo = JNI_POINTER_TO_JLONG(info);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Subscribe__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jConfigPtr, jobject jConfig)
{
	jlong jInfo = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Config*            config  = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.subscribe(subject, config)"))
			{
				try
				{
					SubscriptionInfo* info = connMgr->subscribe(subject.c_str(), *config);

					jInfo = JNI_POINTER_TO_JLONG(info);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Subscribe__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2J
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jConfigPtr, jobject jConfig, jlong jCallbackPtr)
{
	jlong jInfo = 0;

	try
	{
		ConnectionManager*         connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Config*                    config   = JNI_JLONG_TO_CONFIG(jConfigPtr);
        ConnectionManagerCallback* callback = reinterpret_cast<ConnectionManagerCallback*>(jCallbackPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
        else if (callback == NULL)
        {
            SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManagerCallback reference is null");
        }
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.subscribe(subject, callback)"))
			{
				try
				{
					SubscriptionInfo* info = connMgr->subscribe(subject.c_str(), *config, callback);

					jInfo = JNI_POINTER_TO_JLONG(info);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Unsubscribe
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jInfoPtr, jobject jInfo)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		SubscriptionInfo*  info    = JNI_JLONG_TO_CONN_MGR_SUBSCRIPTION_INFO(jInfoPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!info)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionInfo reference is null");
		}
		else
		{
			connMgr->unsubscribe(info);
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Publish__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2JLgov_nasa_gsfc_gmsec_api_jni_JNIMessage_2
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           msg     = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			try
			{
				connMgr->publish(*msg);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Publish__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2JLgov_nasa_gsfc_gmsec_api_jni_JNIMessage_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg, jlong jConfigPtr, jobject jConfig)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           msg     = JNI_JLONG_TO_MESSAGE(jMsgPtr);
		Config*            config  = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			try
			{
				connMgr->publish(*msg, *config);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Request__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2JLgov_nasa_gsfc_gmsec_api_jni_JNIMessage_2IJI
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jlong jReplyCallbackPtr, jint jRepublish_ms)
{
	try
	{
		ConnectionManager*              connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*                        request  = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
        ConnectionManagerReplyCallback* callback = reinterpret_cast<ConnectionManagerReplyCallback*>(jReplyCallbackPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!request)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message reference is null");
		}
		else if (!callback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManagerReplyCallback reference is null");
		}
		else
		{
			try
			{
				connMgr->request(*request, (GMSEC_I32) jTimeout, callback, (GMSEC_I32) jRepublish_ms);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Request__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2JLgov_nasa_gsfc_gmsec_api_jni_JNIMessage_2II
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jint jRepublish_ms)
{
	jlong jRepMsg = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!request)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message reference is null");
		}
		else
		{
			try
			{
				Message* reply = connMgr->request(*request, (GMSEC_I32) jTimeout, (GMSEC_I32) jRepublish_ms);

				if (reply)
				{
					jRepMsg = JNI_POINTER_TO_JLONG(reply);
				}
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jRepMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1CancelRequest
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jReplyCallbackPtr)
{
	try
	{
		ConnectionManager*              connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
        ConnectionManagerReplyCallback* callback = reinterpret_cast<ConnectionManagerReplyCallback*>(jReplyCallbackPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!callback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManagerReplyCallback reference is null");
		}
		else
		{
			try
			{
				connMgr->cancelRequest(callback);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Reply
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jReqMsgPtr, jobject jReqMsg, jlong jRepMsgPtr, jobject jRepMsg)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
		Message*           reply   = JNI_JLONG_TO_MESSAGE(jRepMsgPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!request)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message reference is null");
		}
		else if (!reply)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Reply Message reference is null");
		}
		else
		{
			try
			{
				connMgr->reply(*request, *reply);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Dispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!message)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			try
			{
				connMgr->dispatch(*message);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1Receive
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jint jTimeout)
{
	jlong jMsg = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				Message* msg = connMgr->receive((int) jTimeout);

				if (msg)
				{
					jMsg = JNI_POINTER_TO_JLONG(msg);
				}
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jMsg;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1StartAutoDispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				result = (connMgr->startAutoDispatch() ? JNI_TRUE : JNI_FALSE);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1StopAutoDispatch
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jboolean jWaitForCompletion)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				result = (connMgr->stopAutoDispatch(jWaitForCompletion == JNI_TRUE) ? JNI_TRUE : JNI_FALSE);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1ExcludeSubject
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.excludeSubject()"))
			{
				connMgr->excludeSubject(subject.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RemoveExcludedSubject
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.removeExcludedSubject()"))
			{
				connMgr->removeExcludedSubject(subject.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jstring jName = NULL;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				const char* name = connMgr->getName();

				jName = makeJavaString(jenv, name);

				jvmOk(jenv, "ConnectionManager.getName");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1SetName
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jName)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (connMgr == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConnectionManager.setName"))
			{
				try
				{
					connMgr->setName(name.c_str());
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetID
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jstring jID = NULL;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				const char* id = connMgr->getID();

				jID = makeJavaString(jenv, id);

				jvmOk(jenv, "ConnectionManager.getID");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jID;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetMWInfo
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jstring jInfo = NULL;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				const char* info = connMgr->getMWInfo();

				jInfo = makeJavaString(jenv, info);

				jvmOk(jenv, "ConnectionManager.getMWInfo");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jInfo;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetConnectionEndpoint
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jstring jEndpoint = NULL;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			const char* endpoint = connMgr->getConnectionEndpoint();

			jEndpoint = makeJavaString(jenv, endpoint);

			jvmOk(jenv, "ConnectionManager.getConnectionEndpoint");
		}
	}
	JNI_CATCH

	return jEndpoint;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1GetPublishQueueMessageCount
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jlong jCount = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			jCount = (jlong) connMgr->getPublishQueueMessageCount();
		}
	}
	JNI_CATCH

	return jCount;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1CreateHeartbeatMessage
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	jlong jMsg = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
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

			if (jvmOk(jenv, "ConnectionManager.createHeartbeatMessage"))
			{
				try
				{
					Message msg = connMgr->createHeartbeatMessage(subject.c_str(), fields);

					jMsg = JNI_POINTER_TO_JLONG(new Message(msg));
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1StartHeartbeatService
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlongArray jFieldPtrs, jobjectArray jField, jint jNumFields)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
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

			if (jvmOk(jenv, "ConnectionManager.startHeartbeatService"))
			{
				try
				{
					connMgr->startHeartbeatService(subject.c_str(), fields);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1StopHeartbeatService
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				connMgr->stopHeartbeatService();
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1SetHeartbeatServiceField
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jFieldPtr, jobject jField)
{
	jlong jStatus = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*             field   = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			Status status = connMgr->setHeartbeatServiceField(*field);

			jStatus = JNI_POINTER_TO_JLONG(new Status(status));
		}
	}
	JNI_CATCH

	return jStatus;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1CreateLogMessage
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	jlong jMsg = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
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

			if (jvmOk(jenv, "ConnectionManager.createLogMessage"))
			{
				try
				{
					Message msg = connMgr->createLogMessage(subject.c_str(), fields);

					jMsg = JNI_POINTER_TO_JLONG(new Message(msg));
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1SetLoggingDefaults
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
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

			if (jvmOk(jenv, "ConnectionManager.setLoggingDefaults"))
			{
				try
				{
					connMgr->setLoggingDefaults(subject.c_str(), fields);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1PublishLog
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jFieldPtr, jobject jField)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*             field   = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.publishLog"))
			{
				try
				{
					connMgr->publishLog(subject.c_str(), *field);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RequestDirective__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2_3J_3Lgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2I
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jFieldPtr, jobject jField, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	try
	{
		ConnectionManager* connMgr   = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*             dirString = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!dirString)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Directive String Field reference is null");
		}
		else
		{
			DataList<Field*> fields;

			size_t numFields = (size_t) jNumFields;

			if (numFields > 0)
			{
				jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

				for (size_t i = 0; i < numFields; ++i)
				{
					fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
				}
			}

			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.requestDirective(1)"))
			{
				try
				{
					connMgr->requestDirective(subject.c_str(), *dirString, fields);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RequestDirective__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2_3J_3Lgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2IIJI
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jFieldPtr, jobject jField, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jint jTimeout, jlong jReplyCallbackPtr, jint jRepublish_ms)
{
	try
	{
		ConnectionManager*              connMgr   = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*                          dirString = JNI_JLONG_TO_FIELD(jFieldPtr);
        ConnectionManagerReplyCallback* callback  = reinterpret_cast<ConnectionManagerReplyCallback*>(jReplyCallbackPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!dirString)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Directive String Field reference is null");
		}
		else if (!callback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManagerReplyCallback reference is null");
		}
		else
		{
			DataList<Field*> fields;

			size_t numFields = (size_t) jNumFields;

			if (numFields > 0)
			{
				jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

				for (size_t i = 0; i < numFields; ++i)
				{
					fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
				}
			}

			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.requestDirective(1)"))
			{
				try
				{
					connMgr->requestDirective(subject.c_str(), *dirString, fields, (GMSEC_I32) jTimeout, callback, (GMSEC_I32) jRepublish_ms);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RequestDirective__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2_3J_3Lgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2III
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jFieldPtr, jobject jField, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jint jTimeout, jint jRepublish_ms)
{
	jlong jMsg = 0;

	try
	{
		ConnectionManager* connMgr   = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*             dirString = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!dirString)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Directive String Field reference is null");
		}
		else
		{
			DataList<Field*> fields;

			size_t numFields = (size_t) jNumFields;

			if (numFields > 0)
			{
				jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

				for (size_t i = 0; i < numFields; ++i)
				{
					fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
				}
			}

			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.requestDirective(2)"))
			{
				try
				{
					Message* msg = connMgr->requestDirective(subject.c_str(), *dirString, fields, (GMSEC_I32) jTimeout, (GMSEC_I32) jRepublish_ms);

					jMsg = JNI_POINTER_TO_JLONG(msg);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1AcknowledgeDirectiveRequest
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jReqMsgPtr, jobject jReqMsg, jint jStatus, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!request)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message reference is null");
		}
		else
		{
			DataList<Field*> fields;

			size_t numFields = (size_t) jNumFields;

			if (numFields > 0)
			{
				jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

				for (size_t i = 0; i < numFields; ++i)
				{
					fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
				}
			}

			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.requestDirective(3)"))
			{
				try
				{
					ResponseStatus::Response status = static_cast<ResponseStatus::Response>(jStatus);

					connMgr->acknowledgeDirectiveRequest(subject.c_str(), *request, status, fields);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1PublishResourceMessage
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jSampleInterval, jlong jAverageInterval)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.publishResourceMessage"))
			{
				try
				{
					connMgr->publishResourceMessage(subject.c_str(), (size_t) jSampleInterval, (size_t) jAverageInterval);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1CreateResourceMessage
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jSampleInterval, jlong jAverageInterval)
{
	jlong jMsg = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.createResourceMessage"))
			{
				try
				{
					Message msg = connMgr->createResourceMessage(subject.c_str(), (size_t) jSampleInterval, (size_t) jAverageInterval);

					jMsg = JNI_POINTER_TO_JLONG(new Message(msg));
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jMsg;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1StartResourceMessageService
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jIntervalSeconds, jlong jSampleInterval, jlong jAverageInterval)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.startResourceMessageService"))
			{
				try
				{
					connMgr->startResourceMessageService(subject.c_str(), (size_t) jIntervalSeconds, (size_t) jSampleInterval, (size_t) jAverageInterval);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1StopResourceMessageService
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			try
			{
				result = (connMgr->stopResourceMessageService() ? JNI_TRUE : JNI_FALSE);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1AcknowledgeSimpleService
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jReqMsgPtr, jobject jReqMsg, jint jStatus, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Message*           reqMsg  = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!reqMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.acknowledgeSimpleService"))
			{
				DataList<Field*> fields;

				size_t numFields = (size_t) jNumFields;

				if (numFields > 0)
				{
					jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

					for (size_t i = 0; i < numFields; ++i)
					{
						fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
					}
				}

				ResponseStatus::Response status = static_cast<ResponseStatus::Response>(jStatus);

				try
				{
					connMgr->acknowledgeSimpleService(subject.c_str(), *reqMsg, status, fields);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RequestSimpleService__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2_3J_3Lgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2I_3J_3Lgov_nasa_gsfc_gmsec_api_jni_mist_JNIServiceParam_2I
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jstring jOpName, jlong jOpNumFieldPtr, jobject jOpNumField, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jlongArray jServiceParamPtrs, jobjectArray jServiceParams, jint jNumServiceParams)
{
	try
	{
		ConnectionManager* connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*             opNumFld = JNI_JLONG_TO_FIELD(jOpNumFieldPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!opNumFld)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "OpNumber Field reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			JStringManager opName(jenv, jOpName);

			if (jvmOk(jenv, "ConnectionManager.requestSimpleService"))
			{
				DataList<Field*>        fields;
				DataList<ServiceParam*> params;

				size_t numFields = (size_t) jNumFields;
				size_t numParams = (size_t) jNumServiceParams;

				if (numFields > 0)
				{
					jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

					for (size_t i = 0; i < numFields; ++i)
					{
						fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
					}
				}

				if (numParams > 0)
				{
					jlong* paramPtrs = jenv->GetLongArrayElements(jServiceParamPtrs, JNI_FALSE);

					for (size_t i = 0; i < numParams; ++i)
					{
						params.add(JNI_JLONG_TO_SERVICE_PARAM(paramPtrs[i]));
					}
				}

				try
				{
					connMgr->requestSimpleService(subject.c_str(), opName.c_str(), *opNumFld, fields, params);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RequestSimpleService__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2_3J_3Lgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2I_3J_3Lgov_nasa_gsfc_gmsec_api_jni_mist_JNIServiceParam_2IIJI
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jstring jOpName, jlong jOpNumFieldPtr, jobject jOpNumField, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jlongArray jServiceParamPtrs, jobjectArray jServiceParams, jint jNumServiceParams, jint jTimeout, jlong jReplyCallbackPtr, jint jRepublish_ms)
{
	try
	{
		ConnectionManager*              connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*                          opNumFld = JNI_JLONG_TO_FIELD(jOpNumFieldPtr);
        ConnectionManagerReplyCallback* callback = reinterpret_cast<ConnectionManagerReplyCallback*>(jReplyCallbackPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!opNumFld)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "OpNumber Field reference is null");
		}
		else if (!callback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManagerReplyCallback reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			JStringManager opName(jenv, jOpName);

			if (jvmOk(jenv, "ConnectionManager.requestSimpleService"))
			{
				DataList<Field*>        fields;
				DataList<ServiceParam*> params;

				size_t numFields = (size_t) jNumFields;
				size_t numParams = (size_t) jNumServiceParams;

				if (numFields > 0)
				{
					jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

					for (size_t i = 0; i < numFields; ++i)
					{
						fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
					}
				}

				if (numParams > 0)
				{
					jlong* paramPtrs = jenv->GetLongArrayElements(jServiceParamPtrs, JNI_FALSE);

					for (size_t i = 0; i < numParams; ++i)
					{
						params.add(JNI_JLONG_TO_SERVICE_PARAM(paramPtrs[i]));
					}
				}

				try
				{
					connMgr->requestSimpleService(subject.c_str(), opName.c_str(), *opNumFld, fields, params, (GMSEC_I32) jTimeout, callback, (GMSEC_I32) jRepublish_ms);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConnectionManager_1RequestSimpleService__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNIConnectionManager_2Ljava_lang_String_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2_3J_3Lgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2I_3J_3Lgov_nasa_gsfc_gmsec_api_jni_mist_JNIServiceParam_2III
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jstring jOpName, jlong jOpNumFieldPtr, jobject jOpNumField, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jlongArray jServiceParamPtrs, jobjectArray jServiceParams, jint jNumServiceParams, jint jTimeout, jint jRepublish_ms)
{
	jlong jReply = 0;

	try
	{
		ConnectionManager* connMgr  = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);
		Field*             opNumFld = JNI_JLONG_TO_FIELD(jOpNumFieldPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else if (!opNumFld)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "OpNumber Field reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			JStringManager opName(jenv, jOpName);

			if (jvmOk(jenv, "ConnectionManager.requestSimpleService"))
			{
				DataList<Field*>        fields;
				DataList<ServiceParam*> params;

				size_t numFields = (size_t) jNumFields;
				size_t numParams = (size_t) jNumServiceParams;

				if (numFields > 0)
				{
					jlong* fldPtrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

					for (size_t i = 0; i < numFields; ++i)
					{
						fields.add(JNI_JLONG_TO_FIELD(fldPtrs[i]));
					}
				}

				if (numParams > 0)
				{
					jlong* paramPtrs = jenv->GetLongArrayElements(jServiceParamPtrs, JNI_FALSE);

					for (size_t i = 0; i < numParams; ++i)
					{
						params.add(JNI_JLONG_TO_SERVICE_PARAM(paramPtrs[i]));
					}
				}

				try
				{
					Message* reply = connMgr->requestSimpleService(subject.c_str(), opName.c_str(), *opNumFld, fields, params, (GMSEC_I32) jTimeout, (GMSEC_I32) jRepublish_ms);

					if (reply)
					{
						jReply = JNI_POINTER_TO_JLONG(reply);
					}
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jReply;
}


#ifdef __cplusplus
}
#endif
