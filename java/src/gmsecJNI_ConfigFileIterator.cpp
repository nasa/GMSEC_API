/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// ConfigFileIterator class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Config.h>
#include <gmsec5/ConfigFileIterator.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFileIterator_1HasNextConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

	jboolean result = (iter->hasNextConfig() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFileIterator_1HasNextMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

	jboolean result = (iter->hasNextMessage() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFileIterator_1HasNextSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

	jboolean result = (iter->hasNextSubscription() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFileIterator_1NextConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

	jlong jEntry = 0;

	try
	{
		const ConfigFile::ConfigEntry& entry = iter->nextConfig();

		jEntry = JNI_POINTER_TO_JLONG(new ConfigFile::ConfigEntry(entry));
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jEntry;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFileIterator_1NextMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

	jlong jEntry = 0;

	try
	{
		const ConfigFile::MessageEntry& entry = iter->nextMessage();

		jEntry = JNI_POINTER_TO_JLONG(new ConfigFile::MessageEntry(entry));
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jEntry;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFileIterator_1NextSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

	jlong jEntry = 0;

	try
	{
		const ConfigFile::SubscriptionEntry& entry = iter->nextSubscription();

		jEntry = JNI_POINTER_TO_JLONG(new ConfigFile::SubscriptionEntry(entry));
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jEntry;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFileIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jCfgFileIterPtr, jobject jCfgFileIter)
{
	ConfigFileIterator* iter = JNI_JLONG_TO_CONFIG_FILE_ITER(jCfgFileIterPtr);

	iter->reset();
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1ConfigEntry
  (JNIEnv *jenv, jclass jcls, jlong jCfgEntryPtr, jobject jCfgEntry)
{
	ConfigFile::ConfigEntry* entry = JNI_JLONG_TO_CONFIG_ENTRY(jCfgEntryPtr);

	delete entry;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigEntry_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jCfgEntryPtr, jobject jCfgEntry)
{
	ConfigFile::ConfigEntry* entry = JNI_JLONG_TO_CONFIG_ENTRY(jCfgEntryPtr);

	const char* name = entry->getName();

	jstring jName = makeJavaString(jenv, name);

	jvmOk(jenv, "ConfigEntry::getName");

	return jName;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigEntry_1GetConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgEntryPtr, jobject jCfgEntry)
{
	ConfigFile::ConfigEntry* entry = JNI_JLONG_TO_CONFIG_ENTRY(jCfgEntryPtr);

	const Config& cfg = entry->getConfig();

	jlong jConfig = JNI_POINTER_TO_JLONG(&cfg);

	return jConfig;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1MessageEntry
  (JNIEnv *jenv, jclass jcls, jlong jMsgEntryPtr, jobject jMsgEntry)
{
	ConfigFile::MessageEntry* entry = JNI_JLONG_TO_MESSAGE_ENTRY(jMsgEntryPtr);

	delete entry;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageEntry_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jMsgEntryPtr, jobject jMsgEntry)
{
	ConfigFile::MessageEntry* entry = JNI_JLONG_TO_MESSAGE_ENTRY(jMsgEntryPtr);

	const char* name = entry->getName();

	jstring jName = makeJavaString(jenv, name);

	jvmOk(jenv, "MessageEntry::getName");

	return jName;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageEntry_1GetMessage
  (JNIEnv *jenv, jclass jcls, jlong jMsgEntryPtr, jobject jMsgEntry)
{
	ConfigFile::MessageEntry* entry = JNI_JLONG_TO_MESSAGE_ENTRY(jMsgEntryPtr);

	const Message& msg = entry->getMessage();

	jlong jMessage = JNI_POINTER_TO_JLONG(&msg);

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1SubscriptionEntry
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jPattern)
{
	jlong jEntry = 0;

	JStringManager name(jenv, jName);
	JStringManager pattern(jenv, jPattern);

	if (jvmOk(jenv, "ConfigFile.lookupSubscription"))
	{
		jEntry = JNI_POINTER_TO_JLONG(new ConfigFile::SubscriptionEntry(name.c_str(), pattern.c_str()));
	}

	return jEntry;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1SubscriptionEntry
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

	delete entry;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SubscriptionEntry_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

	const char* name = entry->getName();

	jstring jName = makeJavaString(jenv, name);

	jvmOk(jenv, "SubscriptionEntry::getName");

	return jName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SubscriptionEntry_1GetSubject
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	return Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SubscriptionEntry_1GetPattern(jenv, jcls, jSubEntryPtr, jSubEntry);
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SubscriptionEntry_1GetPattern
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

	const char* pattern = entry->getPattern();

	jstring jPattern = makeJavaString(jenv, pattern);

	jvmOk(jenv, "SubscriptionEntry::getPattern");

	return jPattern;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SubscriptionEntry_1AddExcludedPattern
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry, jstring jPattern)
{
	ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

	JStringManager pattern(jenv, jPattern);

	if (jvmOk(jenv, "SubscriptionEntry.addExcludedPattern"))
	{
		entry->addExcludedPattern(pattern.c_str());
	}
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SubscriptionEntry_1HasNextExcludedPattern
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

	jboolean result = (entry->hasNextExcludedPattern() ? JNI_TRUE : JNI_FALSE);

	return result;
}

JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_SubscriptionEntry_1NextExcludedPattern
  (JNIEnv *jenv, jclass jcls, jlong jSubEntryPtr, jobject jSubEntry)
{
	ConfigFile::SubscriptionEntry* entry = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

	jstring jPattern = 0;

	try
	{
		const char* pattern = entry->nextExcludedPattern();

		jPattern = makeJavaString(jenv, pattern);

		jvmOk(jenv, "SubscriptionEntry::nextExcludedPattern");
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jPattern;
}


#ifdef __cplusplus
}
#endif
