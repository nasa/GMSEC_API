/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Config class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/Config.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Errors.h>

#include <gmsec5_defs.h>

#include <vector>


using namespace gmsec::api5;
using namespace gmsec::api5::util;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Config
  (JNIEnv *jenv, jclass jcls)
{
	Config* created = new Config();

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Config_1Args
  (JNIEnv *jenv, jclass jcls, jobjectArray jargs)
{
	jlong jConfig = 0;

	jint size = jenv->GetArrayLength(jargs);

	if (size < 1)
	{
		jConfig = Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Config(jenv, jcls);
	}
	else
	{
		std::vector<char*> args;

		for (int i = 0; jvmOk(jenv, "new Config(String[])") && i < size; ++i)
		{
			jstring s = (jstring) jenv->GetObjectArrayElement(jargs, i);

			JStringManager manager(jenv, s);

			if (manager.c_str())
			{
				args.push_back(StringUtil::stringNew(manager.c_str()));
			}
		}

		if (jvmOk(jenv, "new Config(String[])"))
		{
			Config* created = new Config((int) args.size(), &args[0]);

			jConfig = JNI_POINTER_TO_JLONG(created);
		}

		for (size_t j = 0; j < args.size(); ++j)
		{
			StringUtil::stringDestroy(args[j]);
		}
	}

	return jConfig;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Config_1Data
  (JNIEnv *jenv, jclass jcls, jstring jData, jint jType)
{
	jlong jConfig = 0;

	JStringManager data(jenv, jData);

	if (jvmOk(jenv, "new Config(const char*)") && data.c_str())
	{
		Config* created = new Config(data.c_str(), static_cast<DataType>(jType));

		jConfig = JNI_POINTER_TO_JLONG(created);
	}

	return jConfig;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Config_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jOtherPtr, jobject jOther)
{
	Config* config = JNI_JLONG_TO_CONFIG(jOtherPtr);

	Config* created = new Config(*config);

	jlong jConfig = JNI_POINTER_TO_JLONG(created);

	return jConfig;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1Config
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	Config *config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	delete config;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1AddValue
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jstring jValue)
{
	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);
	JStringManager value(jenv, jValue, true);

	if (jvmOk(jenv, "Config.AddValue") && name.c_str() && value.c_str())
	{
		config->addValue(name.c_str(), value.c_str());
	}
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1ClearValue
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jboolean result = JNI_FALSE;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.ClearValue"))
	{
		result = (config->clearValue(name.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1Clear
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	config->clear();
}


JNIEXPORT jobjectArray JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1KeySet
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jobjectArray jKeys = NULL;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	// Get native key set
	std::vector<std::string> keys;
	const char*              key = 0;
	const char*              value = 0;

	bool found = config->getFirst(key, value);

	while (found)
	{
		keys.push_back(key);

		found = config->getNext(key, value);
	}

	// Convert native key set to JNI managed set
	jclass stringClass = jenv->FindClass("java/lang/String");
	jKeys = jenv->NewObjectArray(static_cast<jsize>(keys.size()), stringClass, 0);

	int i = 0;
	for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it)
	{
		key = it->c_str();

		jstring jKey = makeJavaString(jenv, key);

		jenv->SetObjectArrayElement(jKeys, i++, jKey);
	}

	return jKeys;
}


JNIEXPORT jobjectArray JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1Values
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jobjectArray jValues = NULL;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	// Get native key set
	std::vector<std::string> values;
	const char*              key = 0;
	const char*              value = 0;

	bool found = config->getFirst(key, value);

	while (found)
	{
		values.push_back(value);

		found = config->getNext(key, value);
	}

	// Convert native key set to JNI managed set
	jclass stringClass = jenv->FindClass("java/lang/String");
	jValues = jenv->NewObjectArray((jsize) values.size(), stringClass, 0);

	int i = 0;
	for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
	{
		value = it->c_str();

		jstring jValue = makeJavaString(jenv, value);

		jenv->SetObjectArrayElement(jValues, i++, jValue);
	}

	return jValues;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jstring result = NULL;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.GetValue"))
	{
		const char* value = config->getValue(name.c_str());

		if (value)
		{
			// Value will always be non-NULL, thus no need to use a try-catch block.
			result = makeJavaString(jenv, value);

			jvmOk(jenv, "Config.GetValue");
		}
	}

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jstring jDefaultValue)
{
	jstring result = NULL;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);
	JStringManager defaultValue(jenv, jDefaultValue);

	if (jvmOk(jenv, "Config.GetValue"))
	{
		const char* value = config->getValue(name.c_str(), defaultValue.c_str());

		if (value)
		{
			// Value will always be non-NULL, thus no need to use a try-catch block.
			result = makeJavaString(jenv, value);

			jvmOk(jenv, "Config.GetValue");
		}
	}

	return result;
}



JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetBooleanValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jboolean result = JNI_FALSE;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.GetBooleanValue"))
	{
		try
		{
			result = (config->getBooleanValue(name.c_str()) ? JNI_TRUE : JNI_FALSE);
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetBooleanValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2Z
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jboolean jDefaultResult)
{
	jboolean result = JNI_FALSE;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.GetBooleanValue"))
	{
		try
		{
			result = (config->getBooleanValue(name.c_str(), (jDefaultResult == JNI_TRUE)) ? JNI_TRUE : JNI_FALSE);
		}
		catch (...)
		{
			result = jDefaultResult; 
		}
	}

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetIntegerValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jint result = 0;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.GetIntegerValue"))
	{
		try
		{
			result = config->getIntegerValue(name.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetIntegerValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jint jDefaultResult)
{
	jint result = 0;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.GetIntegerValue"))
	{
		try
		{
			result = config->getIntegerValue(name.c_str(), static_cast<int>(jDefaultResult));
		}
		catch (...)
		{
			result = jDefaultResult;
		}
	}

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetDoubleValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jdouble result = 0;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.GetDoubleValue"))
	{
		try
		{
			result = config->getDoubleValue(name.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetDoubleValue__JLgov_nasa_gsfc_gmsec_api5_jni_JNIConfig_2Ljava_lang_String_2D
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jdouble jDefaultResult)
{
	jdouble result = 0;

	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager name(jenv, jName);

	if (jvmOk(jenv, "Config.GetDoubleValue"))
	{
		try
		{
			result = config->getDoubleValue(name.c_str(), static_cast<double>(jDefaultResult));
		}
		catch (...)
		{
			result = jDefaultResult;
		}
	}

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1Merge
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jlong jOtherConfigPtr, jobject jOtherConfig, jboolean overwriteExisting)
{
	Config* config      = JNI_JLONG_TO_CONFIG(jConfigPtr);
	Config* otherConfig = JNI_JLONG_TO_CONFIG(jOtherConfigPtr);

	config->merge(*otherConfig, (overwriteExisting == JNI_TRUE));
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	const char* xml = config->toXML();

	// XML content will never be NULL, thus no need for a try-catch block.
	jstring result = makeJavaString(jenv, xml);

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1FromXML
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring inxml)
{
	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	JStringManager xml(jenv, inxml);

	if (jvmOk(jenv, "Config.FromXML"))
	{
		try
		{
			config->fromXML(xml.c_str());
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1GetFromFile
  (JNIEnv *jenv, jclass jcls, jstring jFilename, jstring jConfigName)
{
	jlong jConfig = 0;

	JStringManager filename(jenv, jFilename);
	JStringManager configName(jenv, jConfigName);

	if (jvmOk(jenv, "Config.getFromFile"))
	{
		try
		{
			Config config = Config::getFromFile(filename.c_str(), configName.c_str());

			jConfig = JNI_POINTER_TO_JLONG(new Config(config));
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jConfig;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Config_1ToJSON
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	const char* json = config->toJSON();

	// JSON content will never be NULL, thus no need for a try-catch block.
	jstring result = makeJavaString(jenv, json);

	return result;
}


#ifdef __cplusplus
}
#endif
