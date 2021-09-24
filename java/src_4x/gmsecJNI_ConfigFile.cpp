/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Config class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFile.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ConfigFile
  (JNIEnv *jenv, jclass jcls)
{
	jlong created = 0;

	try
	{
		created = JNI_POINTER_TO_JLONG(new ConfigFile());
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ConfigFile
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			delete cfgFile;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1Load
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jFilePath)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager filePath(jenv, jFilePath);

			if (jvmOk(jenv, "ConfigFile.load"))
			{
				try
				{
					cfgFile->load(filePath.c_str());
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1Save
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jFilePath, jboolean jCompact)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager filePath(jenv, jFilePath);

			if (jvmOk(jenv, "ConfigFile.save"))
			{
				try
				{
					cfgFile->save(filePath.c_str(), (jCompact == JNI_TRUE));
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1FromXML
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jXML)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager xml(jenv, jXML);

			if (jvmOk(jenv, "ConfigFile.fromXML"))
			{
				try
				{
					cfgFile->fromXML(xml.c_str());
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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	jstring jXML = NULL;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			const char* xml = cfgFile->toXML();

			jXML = makeJavaString(jenv, xml);

			jvmOk(jenv, "ConfigFile.toXML");
		}
	}
	JNI_CATCH

	return jXML;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1IsLoaded
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			result = (cfgFile->isLoaded() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1LookupConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jlong jCfg = 0;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.lookupConfig"))
			{
				try
				{
					Config config = cfgFile->lookupConfig(name.c_str());

					jCfg = JNI_POINTER_TO_JLONG(new Config(config));
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jCfg;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1LookupMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jlong jMsg = 0;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.lookupMessage"))
			{
				try
				{
					Message msg = cfgFile->lookupMessage(name.c_str());

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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1LookupSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jstring jTopic = NULL;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
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
		}
	}
	JNI_CATCH

	return jTopic;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1LookupSubscriptionEntry
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jlong jSubscriptionEntry = 0;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.lookupSubscriptionEntry"))
			{
				try
				{
					const ConfigFile::SubscriptionEntry& entry = cfgFile->lookupSubscriptionEntry(name.c_str());
 
					jSubscriptionEntry = JNI_POINTER_TO_JLONG(&entry);
				}
				catch (const Exception& e)
				{ 
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jSubscriptionEntry;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1AddSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName, jstring jTopic)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);
			JStringManager topic(jenv, jTopic);

			if (jvmOk(jenv, "ConfigFile.addSubscription"))
			{
				cfgFile->addSubscription(name.c_str(), topic.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1AddConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName, jlong jConfigPtr, jobject jConfig)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);
		Config*     cfg     = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else if (!cfg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.addConfig"))
			{
				cfgFile->addConfig(name.c_str(), *cfg);
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1AddMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);
		Message*    msg     = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.addMessage"))
			{
				cfgFile->addMessage(name.c_str(), *msg);
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1AddCustomXML
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jXML)
{
	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager xml(jenv, jXML);

			if (jvmOk(jenv, "ConfigFile.addCustomXML"))
			{
				cfgFile->addCustomXML(xml.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1RemoveSubscription
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.removeSubscription"))
			{
				result = (cfgFile->removeSubscription(name.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1RemoveConfig
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.removeConfig"))
			{
				result = (cfgFile->removeConfig(name.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1RemoveMessage
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jName)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "ConfigFile.removeMessage"))
			{
				result = (cfgFile->removeMessage(name.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1RemoveCustomXML
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile, jstring jXML)
{
	jboolean result = JNI_FALSE;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			JStringManager xml(jenv, jXML);

			if (jvmOk(jenv, "ConfigFile.removeCustomXML"))
			{
				result = (cfgFile->removeCustomXML(xml.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ConfigFile_1GetIterator
  (JNIEnv *jenv, jclass jcls, jlong jCfgFilePtr, jobject jCfgFile)
{
	jlong jIterator = 0;

	try
	{
		ConfigFile* cfgFile = JNI_JLONG_TO_CONFIG_FILE(jCfgFilePtr);

		if (!cfgFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
		}
		else
		{
			ConfigFileIterator& iter = cfgFile->getIterator();

			jIterator = JNI_POINTER_TO_JLONG(&iter);
		}
	}
	JNI_CATCH

	return jIterator;
}


#ifdef __cplusplus
}
#endif
