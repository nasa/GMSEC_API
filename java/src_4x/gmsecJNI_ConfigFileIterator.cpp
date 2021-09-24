/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// ConfigFileIterator class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
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

				jEntry = JNI_POINTER_TO_JLONG(new ConfigFile::ConfigEntry(entry));
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

				jEntry = JNI_POINTER_TO_JLONG(new ConfigFile::MessageEntry(entry));
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

				jEntry = JNI_POINTER_TO_JLONG(new ConfigFile::SubscriptionEntry(entry));
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

				jCustom = makeJavaString(jenv, custom);
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ConfigEntry
  (JNIEnv *jenv, jclass jcls, jlong jCfgEntryPtr, jobject jCfgEntry)
{
	try
	{
		ConfigFile::ConfigEntry* entry = JNI_JLONG_TO_CONFIG_ENTRY(jCfgEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigEntry reference is null");
		}
		else
		{
			delete entry;
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

			jName = makeJavaString(jenv, name);

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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1MessageEntry
  (JNIEnv *jenv, jclass jcls, jlong jMsgEntryPtr, jobject jMsgEntry)
{
	try
	{
		ConfigFile::MessageEntry* entry = JNI_JLONG_TO_MESSAGE_ENTRY(jMsgEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageEntry reference is null");
		}
		else
		{
			delete entry;
		}
	}
	JNI_CATCH
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

			jName = makeJavaString(jenv, name);

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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1SubscriptionEntry
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	try
	{
		ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionEntry reference is null");
		}
		else
		{
			delete entry;
		}
	}
	JNI_CATCH
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

			jName = makeJavaString(jenv, name);

			jvmOk(jenv, "SubscriptionEntry::getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SubscriptionEntry_1GetSubject
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	return Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SubscriptionEntry_1GetPattern(jenv, jcls, jSubEntryPtr, jSubEntry);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SubscriptionEntry_1GetPattern
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	jstring jPattern = 0;

	try
	{
		ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionEntry reference is null");
		}
		else
		{
			const char* pattern = entry->getPattern();

			jPattern = makeJavaString(jenv, pattern);

			jvmOk(jenv, "SubscriptionEntry::getPattern");
		}
	}
	JNI_CATCH

	return jPattern;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SubscriptionEntry_1HasNextExcludedPattern
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionEntry reference is null");
		}
		else
		{
			result = (entry->hasNextExcludedPattern() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}

JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_SubscriptionEntry_1NextExcludedPattern
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	jstring jPattern = 0;

	try
	{
		ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

		if (!entry)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "SubscriptionEntry reference is null");
		}
		else
		{
			try
			{
				const char* pattern = entry->nextExcludedPattern();

				jPattern = makeJavaString(jenv, pattern);

				jvmOk(jenv, "SubscriptionEntry::nextExcludedPattern");
			}
			catch (const Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jPattern;
}


#ifdef __cplusplus
}
#endif
