/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// MIST ConnectionManager class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Callbacks.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec/Config.h>
#include <gmsec/Field.h>
#include <gmsec/Message.h>
#include <gmsec/Status.h>
#include <gmsec/mist/ConnectionManager.h>
#include <gmsec/mist/ConnectionManagerCallback.h>
#include <gmsec/mist/ConnectionManagerReplyCallback.h>
#include <gmsec/mist/ConnectionManagerErrorCallback.h>

using gmsec::Config;
using gmsec::Field;
using gmsec::Message;
using gmsec::Status;
using gmsec::mist::ConnectionManager;
using gmsec::mist::ConnectionManagerCallback;
using gmsec::mist::ConnectionManagerReplyCallback;
using gmsec::mist::ConnectionManagerErrorCallback;


#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1ConnectionManager__JLgov_nasa_gsfc_gmsecapi_jni_JNIConfig_2IZ
	(JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jint jVersion, jboolean jValidate)
{
	ConnectionManager* connMgr = 0;

	try
	{
		if (jvmOk(jenv, "new ConnectionManager"))
		{
			Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);
			bool validate = (jValidate == JNI_TRUE);

			connMgr = new ConnectionManager(*config, (unsigned int) jVersion, validate);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(connMgr);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1ConnectionManager__JLgov_nasa_gsfc_gmsecapi_jni_JNIConfig_2
	(JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	ConnectionManager* connMgr = 0;

	try
	{
		if (jvmOk(jenv, "new ConnectionManager"))
		{
			Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

			connMgr = new ConnectionManager(*config);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(connMgr);
}


JNIEXPORT void JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_delete_1ConnectionManager
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			delete connMgr;
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1SetStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	Status* result = 0;

	ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

	if (!connMgr)
	{
		SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
	}

	size_t numFields = (size_t) jNumFields;
	Field** fields = 0;

	if (numFields > 0)
	{
		jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

		fields = new Field*[numFields];

		for (size_t i = 0; i < numFields; ++i)
		{
			Field* field = JNI_JLONG_TO_FIELD(fldptrs[i]);

			if (field)
			{
				fields[i] = new Field(*field);
			}
			else
			{
				fields[i] = NULL;
			}
		}
	}

	try
	{
		result = new Status(connMgr->SetStandardFields(fields, numFields));
	}
	JNI_CATCH

	for (size_t i = 0; i < numFields; ++i)
	{
		delete fields[i];
	}
	delete [] fields;

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Initialize
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			result = new Status(connMgr->Initialize());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Cleanup
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			Cache::getCache().destroyConnMgrCallbacks(jenv, jConnMgr);

			result = new Status(connMgr->Cleanup());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1RegisterErrorCallback
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jEvent, jobject jErrorCallback)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jEvent)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Event is null");
		}
		else if (!jErrorCallback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ErrorCallback is null");
		}
		else
		{
			JStringManager event(jenv, jEvent);
			if (jvmOk(jenv, "ConnectionManager.RegisterErrorCallback"))
			{
				JNIMistErrorCallback* callback = new JNIMistErrorCallback(jenv, jConnMgr, jErrorCallback);

				result = new Status(connMgr->RegisterErrorCallback(event.c_str(), callback));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Subscribe__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2Ljava_lang_String_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jSubject)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			if (jvmOk(jenv, "ConnectionManager.Subscribe"))
			{
				result = new Status(connMgr->Subscribe(subject.c_str()));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Subscribe__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2Ljava_lang_String_2Ljava_lang_Object_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jobject jCallback)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jSubject)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
		}
		else if (!jCallback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Callback is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.Subscribe"))
			{
				JNIMistCallback* callback = new JNIMistCallback(jenv, jConnMgr, jSubject, jCallback);

				result = new Status(connMgr->Subscribe(subject.c_str(), callback));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Subscribe__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsecapi_jni_JNIConfig_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jlong jConfigPtr, jobject jConfig)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jSubject)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
		}
		else if (!jConfig)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			if (jvmOk(jenv, "ConnectionManager.Subscribe"))
			{
				Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

				result = new Status(connMgr->Subscribe(subject.c_str(), *config));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1UnSubscribe__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2Ljava_lang_String_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jSubject)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			if (jvmOk(jenv, "ConnectionManager.UnSubscribe"))
			{
				result = new Status(connMgr->UnSubscribe(subject.c_str()));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1UnSubscribe__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2Ljava_lang_String_2Ljava_lang_Object_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jobject jCallback)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jSubject)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
		}
		else if (!jCallback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Callback is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "ConnectionManager.UnSubscribe"))
			{
				ConnectionManagerCallback* callback = Cache::getCache().getAndRemoveConnMgrCallback(jenv, jSubject, jCallback);
				result = new Status(connMgr->UnSubscribe(subject.c_str(), callback));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1CreateMessage
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject, jint jKind, jobject jMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jSubject)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Subject is null");
		}
		else if (!jMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message object is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			Message* message = NULL;

			Status status = connMgr->CreateMessage(subject.c_str(), (unsigned short) jKind, message);

			if (jvmOk(jenv, "ConnectionManager.CreateMessage") && !status.IsError())
			{
				jenv->SetLongField(jMsg, Cache::getCache().fieldMessage_swigCPtr, JNI_POINTER_TO_JLONG(message));
				jvmOk(jenv, "ConnectionManager.CreateMessage");

				jboolean z = message ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jMsg, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "ConnectionManager.CreateMessage");
			}

			result = new Status(status);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1ConvertMessage
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jInMsgPtr, jobject jInMsg, jlong jOutMsgPtr, jobject jOutMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jInMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "In Message object is null");
		}
		else if (!jOutMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Out Message object is null");
		}
		else
		{
			Message* inMsg  = JNI_JLONG_TO_MESSAGE(jInMsgPtr);
			Message* outMsg = NULL;

			Status status = connMgr->ConvertMessage(inMsg, outMsg);

			if (jvmOk(jenv, "ConnectionManager.ConvertMessage") && !status.IsError())
			{
				jenv->SetLongField(jOutMsg, Cache::getCache().fieldMessage_swigCPtr, JNI_POINTER_TO_JLONG(outMsg));
				jvmOk(jenv, "ConnectionManager.ConvertMessage");

				jboolean z = outMsg ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jOutMsg, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "ConnectionManager.ConvertMessage");
			}

			result = new Status(status);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1ConvertCleanup
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jInMsgPtr, jobject jInMsg, jlong jOutMsgPtr, jobject jOutMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jInMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "In Message object is null");
		}
		else if (!jOutMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Out Message object is null");
		}
		else
		{
			Message* inMsg  = JNI_JLONG_TO_MESSAGE(jInMsgPtr);
			Message* outMsg = JNI_JLONG_TO_MESSAGE(jOutMsgPtr);

			result = new Status(connMgr->ConvertCleanup(inMsg, outMsg));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1CloneMessage
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jInMsgPtr, jobject jInMsg, jlong jOutMsgPtr, jobject jOutMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jInMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "In Message object is null");
		}
		else if (!jOutMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Out Message object is null");
		}
		else
		{
			Message* inMsg  = JNI_JLONG_TO_MESSAGE(jInMsgPtr);
			Message* outMsg = NULL;

			Status status = connMgr->CloneMessage(inMsg, outMsg);

			if (jvmOk(jenv, "ConnectionManager.CloneMessage") && !status.IsError())
			{
				jenv->SetLongField(jOutMsg, Cache::getCache().fieldMessage_swigCPtr, JNI_POINTER_TO_JLONG(outMsg));
				jvmOk(jenv, "ConnectionManager.CloneMessage");

				jboolean z = outMsg ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jOutMsg, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "ConnectionManager.CloneMessage");
			}

			result = new Status(status);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1DestroyMessage
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message is null");
		}
		else
		{
			Message* message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			result = new Status(connMgr->DestroyMessage(message));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Publish__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2JLgov_nasa_gsfc_gmsecapi_jni_JNIMessage_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message is null");
		}
		else
		{
			Message* message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			result = new Status(connMgr->Publish(message));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Publish__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2JLgov_nasa_gsfc_gmsecapi_jni_JNIMessage_2JLgov_nasa_gsfc_gmsecapi_jni_JNIConfig_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg, jlong jConfigPtr, jobject jConfig)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message is null");
		}
		else if (!jConfig)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config is null");
		}
		else
		{
			Message* message = JNI_JLONG_TO_MESSAGE(jMsgPtr);
			Config*  config  = JNI_JLONG_TO_CONFIG(jConfigPtr);

			result = new Status(connMgr->Publish(message, *config));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Request__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2JLgov_nasa_gsfc_gmsecapi_jni_JNIMessage_2ILjava_lang_Object_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jobject jReplyCallback)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jReqMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message is null");
		}
		else if (!jReplyCallback)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ReplyCallback is null");
		}
		else
		{
			Message* request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);

			JNIMistReplyCallback* callback = new JNIMistReplyCallback(jenv, jConnMgr, jReplyCallback);

			result = new Status(connMgr->Request(request, (GMSEC_I32) jTimeout, callback));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Request__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2JLgov_nasa_gsfc_gmsecapi_jni_JNIMessage_2ILgov_nasa_gsfc_gmsecapi_jni_JNIMessage_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jReqMsgPtr, jobject jReqMsg, jint jTimeout, jobject jRepMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jReqMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message is null");
		}
		else if (!jRepMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Reply Message is null");
		}
		else
		{
			Message* request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
			Message* reply   = NULL;

			Status status = connMgr->Request(request, (GMSEC_I32) jTimeout, reply);

			if (jvmOk(jenv, "ConnectionManager.Request") && !status.IsError())
			{
				jenv->SetLongField(jRepMsg, Cache::getCache().fieldMessage_swigCPtr, JNI_POINTER_TO_JLONG(reply));
				jvmOk(jenv, "ConnectionManager.Request");

				jboolean z = reply ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jRepMsg, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "ConnectionManager.Request");
			}

			result = new Status(status);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1Reply
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jReqMsgPtr, jobject jReqMsg, jlong jRepMsgPtr, jobject jRepMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jReqMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Request Message is null");
		}
		else if (!jRepMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Reply Message is null");
		}
		else
		{
			Message* request = JNI_JLONG_TO_MESSAGE(jReqMsgPtr);
			Message* reply   = JNI_JLONG_TO_MESSAGE(jRepMsgPtr);

			result = new Status(connMgr->Reply(request, reply));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1StartAutoDispatch
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			result = new Status(connMgr->StartAutoDispatch());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1StopAutoDispatch__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			result = new Status(connMgr->StopAutoDispatch());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1StopAutoDispatch__JLgov_nasa_gsfc_gmsecapi_jni_JNIConnectionManager_2Z
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jboolean jWait)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			bool wait = (jWait == JNI_TRUE);
			result = new Status(connMgr->StopAutoDispatch(wait));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1GetNextMsg
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg, jint jTimeout)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message is null");
		}
		else
		{
			Message* message = NULL;

			Status status = connMgr->GetNextMsg(message, (GMSEC_I32) jTimeout);

			if (jvmOk(jenv, "ConnectionManager.GetNextMsg") && !status.IsError())
			{
				jenv->SetLongField(jMsg, Cache::getCache().fieldMessage_swigCPtr, JNI_POINTER_TO_JLONG(message));
				jvmOk(jenv, "ConnectionManager.GetNextMsg");

				jboolean z = message ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jMsg, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "ConnectionManager.GetNextMsg");
			}

			result = new Status(status);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1DispatchMsg
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jMsgPtr, jobject jMsg)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else if (!jMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message is null");
		}
		else
		{
			Message* message = JNI_JLONG_TO_MESSAGE(jMsgPtr);

			result = new Status(connMgr->DispatchMsg(message));
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1GetLastDispatcherStatus
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			result = new Status(connMgr->GetLastDispatcherStatus());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1ExcludeSubject
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject)
{
	Status* result = 0;

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
			if (jvmOk(jenv, "ConnectionManager.ExcludeSubject"))
			{
				result = new Status(connMgr->ExcludeSubject(subject.c_str()));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1RemoveExcludedSubject
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jSubject)
{
	Status* result = 0;

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
			if (jvmOk(jenv, "ConnectionManager.RemoveExcludedSubject"))
			{
				result = new Status(connMgr->RemoveExcludedSubject(subject.c_str()));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1GetHeartbeatMessage
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jlong jMsgPtr, jobject jMsg, jstring jSubject)
{
	Status* result = 0;

	ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

	if (!connMgr)
	{
		SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
	}

	size_t numFields = (size_t) jNumFields;
	Field** fields = 0;

	if (numFields > 0)
	{
		jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

		fields = new Field*[numFields];

		for (size_t i = 0; i < numFields; ++i)
		{
			Field* field = JNI_JLONG_TO_FIELD(fldptrs[i]);

			if (field)
			{
				fields[i] = new Field(*field);
			}
			else
			{
				fields[i] = NULL;
			}
		}
	}

	try
	{
		JStringManager subject(jenv, jSubject);

		if (jvmOk(jenv, "ConnectionManager.GetHeartbeatMessage"))
		{
			Message* message = NULL;

			Status status = connMgr->GetHeartbeatMessage(fields, numFields, message, subject.c_str());

			if (jvmOk(jenv, "ConnectionManager.GetHeartbeatMessage") && !status.IsError())
			{
				jenv->SetLongField(jMsg, Cache::getCache().fieldMessage_swigCPtr, JNI_POINTER_TO_JLONG(message));
				jvmOk(jenv, "ConnectionManager.GetHeartbeatMessage");

				jboolean z = message ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jMsg, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "ConnectionManager.GetHeartbeatMessage");
			}

			result = new Status(status);
		}
	}
	JNI_CATCH

	for (size_t i = 0; i < numFields; ++i)
	{
		delete fields[i];
	}
	delete [] fields;

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1StartHeartbeatService
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jstring jSubject)
{
	Status* result = 0;

	ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

	if (!connMgr)
	{
		SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
	}

	size_t numFields = (size_t) jNumFields;
	Field** fields = 0;

	if (numFields > 0)
	{
		jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

		fields = new Field*[numFields];

		for (size_t i = 0; i < numFields; ++i)
		{
			Field* field = JNI_JLONG_TO_FIELD(fldptrs[i]);

			if (field)
			{
				fields[i] = new Field(*field);
			}
			else
			{
				fields[i] = NULL;
			}
		}
	}

	try
	{
		JStringManager subject(jenv, jSubject);

		if (jvmOk(jenv, "ConnectionManager.StartHeartbeatService"))
		{
			result = new Status(connMgr->StartHeartbeatService(fields, numFields, subject.c_str()));
		}
	}
	JNI_CATCH

	for (size_t i = 0; i < numFields; ++i)
	{
		delete fields[i];
	}
	delete [] fields;

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1ChangeComponentStatus
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jFieldPtr, jobject jField)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field is null");
			}
			else
			{
				result = new Status(connMgr->ChangeComponentStatus(*field));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1ChangeComponentInfo
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jFieldPtr, jobject jField)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field is null");
			}
			else
			{
				result = new Status(connMgr->ChangeComponentInfo(*field));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1ChangeCPUMemory
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jFieldPtr, jobject jField)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field is null");
			}
			else
			{
				result = new Status(connMgr->ChangeCPUMemory(*field));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1ChangeCPUUtil
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlong jFieldPtr, jobject jField)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field is null");
			}
			else
			{
				result = new Status(connMgr->ChangeCPUUtil(*field));
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1StopHeartbeatService
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager reference is null");
		}
		else
		{
			result = new Status(connMgr->StopHeartbeatService());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1GetLogMessage
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jlong jMsgPtr, jobject jMsg, jstring jSubject)
{
	Status* result = 0;

	ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

	if (!connMgr)
	{
		SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
	}

	size_t numFields = (size_t) jNumFields;
	Field** fields = 0;

	if (numFields > 0)
	{
		jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

		fields = new Field*[numFields];

		for (size_t i = 0; i < numFields; ++i)
		{
			Field* field = JNI_JLONG_TO_FIELD(fldptrs[i]);

			if (field)
			{
				fields[i] = new Field(*field);
			}
			else
			{
				fields[i] = NULL;
			}
		}
	}

	try
	{
		JStringManager subject(jenv, jSubject);

		if (jvmOk(jenv, "ConnectionManager.GetLogMessage"))
		{
			Message* message = NULL;

			Status status = connMgr->GetLogMessage(fields, numFields, message, subject.c_str());

			if (jvmOk(jenv, "ConnectionManager.GetLogMessage") && !status.IsError())
			{
				jenv->SetLongField(jMsg, Cache::getCache().fieldMessage_swigCPtr, JNI_POINTER_TO_JLONG(message));
				jvmOk(jenv, "ConnectionManager.GetLogMessage");

				jboolean z = message ? JNI_TRUE : JNI_FALSE;
				jenv->SetBooleanField(jMsg, Cache::getCache().fieldMessage_swigCMemOwn, z);
				jvmOk(jenv, "ConnectionManager.GetLogMessage");
			}

			result = new Status(status);
		}
	}
	JNI_CATCH

	for (size_t i = 0; i < numFields; ++i)
	{
		delete fields[i];
	}
	delete [] fields;

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1SetLoggingDefaults
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields, jstring jSubject)
{
	Status* result = 0;

	ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

	if (!connMgr)
	{
		SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
	}

	size_t numFields = (size_t) jNumFields;
	Field** fields = 0;

	if (numFields > 0)
	{
		jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

		fields = new Field*[numFields];

		for (size_t i = 0; i < numFields; ++i)
		{
			Field* field = JNI_JLONG_TO_FIELD(fldptrs[i]);

			if (field)
			{
				fields[i] = new Field(*field);
			}
			else
			{
				fields[i] = NULL;
			}
		}
	}

	try
	{
		JStringManager subject(jenv, jSubject);

		if (jvmOk(jenv, "ConnectionManager.SetLoggingDefaults"))
		{
			result = new Status(connMgr->SetLoggingDefaults(fields, numFields, subject.c_str()));
		}
	}
	JNI_CATCH

	for (size_t i = 0; i < numFields; ++i)
	{
		delete fields[i];
	}
	delete [] fields;

	return JNI_POINTER_TO_JLONG(result);
}


JNIEXPORT jlong JNICALL
Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionManager_1QuickLog
	(JNIEnv *jenv, jclass jcls, jlong jConnMgrPtr, jobject jConnMgr, jstring jMsg, jlong jFieldPtr, jobject jField)
{
	Status* result = 0;

	try
	{
		ConnectionManager* connMgr = JNI_JLONG_TO_CONNECTION_MANAGER(jConnMgrPtr);

		if (!connMgr)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConnectionManager is null");
		}
		else
		{
			JStringManager msg(jenv, jMsg);

			if (jvmOk(jenv, "ConnectionManager.QuickLog"))
			{
				Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

				if (!field)
				{
					SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field is null");
				}
				else
				{
					result = new Status(connMgr->QuickLog(msg.c_str(), *field));
				}
			}
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(result);
}

#ifdef __cplusplus
}
#endif
