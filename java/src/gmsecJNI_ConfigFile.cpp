/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// ConfigFile class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Config.h>
#include <gmsec5/ConfigFile.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1ConfigFile
  (JNIEnv *jenv, jclass jcls)
{
	jlong created = JNI_POINTER_TO_JLONG(new ConfigFile());

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1ConfigFileUsingConfig
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jlong created = 0;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	if (config != NULL)
	{
		try
		{
			created = JNI_POINTER_TO_JLONG(new ConfigFile(*config));
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
	else
	{
		created = JNI_POINTER_TO_JLONG(new ConfigFile());
	}

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1ConfigFile
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	delete cfgFile;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1Load
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jFilePath)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager filePath(jenv, jFilePath);

	if (jvmOk(jenv, "ConfigFile.load"))
	{
		try
		{
			cfgFile->load(filePath.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1Save
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jFilePath, jboolean jCompact)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager filePath(jenv, jFilePath);

	if (jvmOk(jenv, "ConfigFile.save"))
	{
		try
		{
			cfgFile->save(filePath.c_str(), (jCompact == JNI_TRUE));
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1FromXML
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jXML)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager xml(jenv, jXML);

	if (jvmOk(jenv, "ConfigFile.fromXML"))
	{
		try
		{
			cfgFile->fromXML(xml.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	const char* xml = cfgFile->toXML();

	// XML string will always be non-NULL, thus no need for a try-catch block.
	jstring jXML = makeJavaString(jenv, xml);

	jvmOk(jenv, "ConfigFile.toXML");

	return jXML;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1IsLoaded
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	jboolean result = (cfgFile->isLoaded() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1AddConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName, jlong jConfigPtr, jobject jConfig)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);
	Config*     cfg     = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.addConfig"))
	{
		cfgFile->addConfig(name.c_str(), *cfg);
	}
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1LookupConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jlong jCfg = 0;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.lookupConfig"))
	{
		try
		{
			Config config = cfgFile->lookupConfig(name.c_str());

			jCfg = JNI_POINTER_TO_JLONG(new Config(config));
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jCfg;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1RemoveConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jboolean result = JNI_FALSE;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.removeConfig"))
	{
		result = (cfgFile->removeConfig(name.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1AddMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName, jlong jMsgPtr, jobject jMsg)
{
	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);
	Message*    msg     = JNI_JLONG_TO_MESSAGE(jMsgPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.addMessage"))
	{
		cfgFile->addMessage(name.c_str(), *msg);
	}
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1LookupMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jlong jMsg = 0;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.lookupMessage"))
	{
		try
		{
			Message msg = cfgFile->lookupMessage(name.c_str());

			jMsg = JNI_POINTER_TO_JLONG(new Message(msg));
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jMsg;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1RemoveMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jboolean result = JNI_FALSE;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.removeMessage"))
	{
		result = (cfgFile->removeMessage(name.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1LookupSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jstring jTopic = NULL;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.lookupSubscription"))
	{
		try {
			const ConfigFile::SubscriptionEntry& entry = cfgFile->lookupSubscriptionEntry(name.c_str());

			jTopic = makeJavaString(jenv, entry.getPattern());
		}
		catch (...) {
			// desired subscription was not found.
		}

		jvmOk(jenv, "ConfigFile.lookupSubscription");
	}

	return jTopic;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1AddSubscriptionEntry
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jlong jSubEntryPtr, jobject jSubEntry)
{
	ConfigFile*                    cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);
	ConfigFile::SubscriptionEntry* entry   = JNI_JLONG_TO_SUBSCRIPTION_ENTRY(jSubEntryPtr);

	cfgFile->addSubscriptionEntry(*entry);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1LookupSubscriptionEntry
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jlong jSubscriptionEntry = 0;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.lookupSubscriptionEntry"))
	{
		try
		{
			const ConfigFile::SubscriptionEntry& entry = cfgFile->lookupSubscriptionEntry(name.c_str());
 
			jSubscriptionEntry = JNI_POINTER_TO_JLONG(&entry);
		}
		catch (const GmsecException& e)
		{ 
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jSubscriptionEntry;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1RemoveSubscriptionEntry
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jboolean result = JNI_FALSE;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "ConfigFile.removeSubscriptionEntry"))
	{
		result = (cfgFile->removeSubscriptionEntry(name.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_ConfigFile_1GetIterator
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	jlong jIterator = 0;

	ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

	ConfigFileIterator& iter = cfgFile->getIterator();

	jIterator = JNI_POINTER_TO_JLONG(&iter);

	return jIterator;
}


#ifdef __cplusplus
}
#endif
