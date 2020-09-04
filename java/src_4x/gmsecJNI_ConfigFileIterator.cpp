/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// ConfigFileIterator class functions
//

#include "gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFileIterator.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1HasNextConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			result = (iter->hasNextConfig() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1HasNextMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			result = (iter->hasNextMessage() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1HasNextSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			result = (iter->hasNextSubscription() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1HasNextCustomElement
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			result = (iter->hasNextCustomElement() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1NextConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jlong jEntry = 0;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			try
			{
				const ConfigFile::ConfigEntry& entry = iter->nextConfig();

				jEntry = JNI_POINTER_TO_JLONG(&entry);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jEntry;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1NextMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jlong jEntry = 0;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			try
			{
				const ConfigFile::MessageEntry& entry = iter->nextMessage();

				jEntry = JNI_POINTER_TO_JLONG(&entry);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jEntry;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1NextSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jlong jEntry = 0;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			try
			{
				const ConfigFile::SubscriptionEntry& entry = iter->nextSubscription();

				jEntry = JNI_POINTER_TO_JLONG(&entry);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jEntry;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1NextCustomElement
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	jstring jCustom = NULL;

	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			try
			{
				const char* custom = iter->nextCustomElement();

				jCustom = jenv->NewStringUTF(custom);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jCustom;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFileIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	try
	{
		ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

		if (!iter)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFileIterator reference is null");
		}
		else
		{
			iter->reset();
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigEntry_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jCfgEntryPtr, jobject jCfgEntry)
{
	jstring jName = 0;

	try
	{
		ConfigFile::ConfigEntry* entry = JNI_JLONG_TO_CONFIG_ENTRY(jCfgEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigEntry reference is null");
		}
		else
		{
			const char* name = entry->getName();

			jName = jenv->NewStringUTF(name);

			jvmOk(jenv, "ConfigEntry::getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigEntry_1GetConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgEntryPtr, jobject jCfgEntry)
{
	jlong jConfig = 0;

	try
	{
		ConfigFile::ConfigEntry* entry = JNI_JLONG_TO_CONFIG_ENTRY(jCfgEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigEntry reference is null");
		}
		else
		{
			const Config& cfg = entry->getConfig();

			jConfig = JNI_POINTER_TO_JLONG(&cfg);
		}
	}
	JNI_CATCH

	return jConfig;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MessageEntry_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jMsgEntryPtr, jobject jMsgEntry)
{
	jstring jName = 0;

	try
	{
		ConfigFile::MessageEntry* entry = JNI_JLONG_TO_MESSAGE_ENTRY(jMsgEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageEntry reference is null");
		}
		else
		{
			const char* name = entry->getName();

			jName = jenv->NewStringUTF(name);

			jvmOk(jenv, "MessageEntry::getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MessageEntry_1GetMessage
  (JNIEnv *jenv, jclass jcls, jlong jMsgEntryPtr, jobject jMsgEntry)
{
	jlong jMessage = 0;

	try
	{
		ConfigFile::MessageEntry* entry = JNI_JLONG_TO_MESSAGE_ENTRY(jMsgEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageEntry reference is null");
		}
		else
		{
			const Message& msg = entry->getMessage();

			jMessage = JNI_POINTER_TO_JLONG(&msg);
		}
	}
	JNI_CATCH

	return jMessage;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SubscriptionEntry_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	jstring jName = 0;

	try
	{
		ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionEntry reference is null");
		}
		else
		{
			const char* name = entry->getName();

			jName = jenv->NewStringUTF(name);

			jvmOk(jenv, "SubscriptionEntry::getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SubscriptionEntry_1GetSubject
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	jstring jSubject = 0;

	try
	{
		ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionEntry reference is null");
		}
		else
		{
			const char* subject = entry->getSubject();

			jSubject = jenv->NewStringUTF(subject);

			jvmOk(jenv, "SubscriptionEntry::getSubject");
		}
	}
	JNI_CATCH

	return jSubject;
}


#ifdef __cplusplus
}
#endif
