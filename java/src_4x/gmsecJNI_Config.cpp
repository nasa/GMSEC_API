/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Config class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <vector>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Config
  (JNIEnv *jenv, jclass jcls)
{
	Config *created = 0;

	try
	{
		created = new Config();
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Config_1Args
  (JNIEnv *jenv, jclass jcls, jobjectArray jargs)
{
	jlong jConfig = 0;

	try
	{
		jint size = jenv->GetArrayLength(jargs);

		if (size < 1)
		{
			jConfig = Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Config(jenv, jcls);
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
	}
	JNI_CATCH

	return jConfig;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Config_1Data
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	jlong jConfig = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "new Config(const char*)") && data.c_str())
		{
			Config* created = new Config(data.c_str());

			jConfig = JNI_POINTER_TO_JLONG(created);
		}
	}
	JNI_CATCH

	return jConfig;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Config_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jOtherPtr, jobject jOther)
{
	jlong jConfig = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jOtherPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			Config* created = new Config(*config);

			jConfig = JNI_POINTER_TO_JLONG(created);
		}
	}
	JNI_CATCH

	return jConfig;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Config
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	try
	{
		Config *config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			delete config;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1AddValue
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jstring jValue)
{
	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);
			JStringManager value(jenv, jValue, true);

			if (jvmOk(jenv, "Config.AddValue") && name.c_str() && value.c_str())
			{
				config->addValue(name.c_str(), value.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1ClearValue
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jboolean result = JNI_FALSE;

	try
	{
		Status status;
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);
			if (jvmOk(jenv, "Config.ClearValue"))
			{
				result = (config->clearValue(name.c_str()) ? JNI_TRUE : JNI_FALSE);
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1Clear
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			config->clear();
		}
	}
	JNI_CATCH
}


JNIEXPORT jobjectArray JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1KeySet
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jobjectArray jKeys = NULL;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
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
			jKeys = jenv->NewObjectArray((jsize) keys.size(), stringClass, 0);

			int i = 0;
			for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it)
			{
				key = it->c_str();

				jstring jKey = makeJavaString(jenv, key);

				jenv->SetObjectArrayElement(jKeys, i++, jKey);
			}
		}
	}
	JNI_CATCH

	return jKeys;
}


JNIEXPORT jobjectArray JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1Values
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jobjectArray jValues = NULL;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
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
		}
	}
	JNI_CATCH

	return jValues;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1GetValue__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jstring jDefaultValue)
{
	jstring result = NULL;

	try
	{
		Status status;
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);
			JStringManager defaultValue(jenv, jDefaultValue);

			if (jvmOk(jenv, "Config.GetValue"))
			{
				const char* value = config->getValue(name.c_str(), defaultValue.c_str());

				if (value)
				{
					result = makeJavaString(jenv, value);

					jvmOk(jenv, "Config.GetValue");
				}
			}
		}
	}
	JNI_CATCH

	return result;
}



JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1GetBooleanValue__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jboolean result = JNI_FALSE;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Config.GetBooleanValue"))
			{
				try
				{
					result = (config->getBooleanValue(name.c_str()) ? JNI_TRUE : JNI_FALSE);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1GetBooleanValue__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2Z
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jboolean jDefaultResult)
{
	jboolean result = JNI_FALSE;

	try
	{
		Status status;
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
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
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1GetIntegerValue__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jint result = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Config.GetIntegerValue"))
			{
				try
				{
					result = config->getIntegerValue(name.c_str());
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1GetIntegerValue__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jint jDefaultResult)
{
	jint result = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Config.GetIntegerValue"))
			{
				try
				{
					result = config->getIntegerValue(name.c_str(), (int) jDefaultResult);
				}
				catch (...)
				{
					result = jDefaultResult;
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1GetDoubleValue__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName)
{
	jdouble result = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Config.GetDoubleValue"))
			{
				try
				{
					result = config->getDoubleValue(name.c_str());
				}
				catch (const Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1GetDoubleValue__JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2Ljava_lang_String_2D
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring jName, jdouble jDefaultResult)
{
	jdouble result = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager name(jenv, jName);

			if (jvmOk(jenv, "Config.GetDoubleValue"))
			{
				try
				{
					result = config->getDoubleValue(name.c_str(), (double) jDefaultResult);
				}
				catch (...)
				{
					result = jDefaultResult;
				}
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1Merge
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jlong jOtherConfigPtr, jobject jOtherConfig, jboolean overwriteExisting)
{
	try
	{
		Config* config      = JNI_JLONG_TO_CONFIG(jConfigPtr);
		Config* otherConfig = JNI_JLONG_TO_CONFIG(jOtherConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else if (!otherConfig)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Other Config reference is null");
		}
		else
		{
			config->merge(*otherConfig, (overwriteExisting == JNI_TRUE));
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jstring result;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			const char* xml = config->toXML();

			result = makeJavaString(jenv, xml);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1FromXML
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig, jstring inxml)
{
	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager xml(jenv, inxml);

			if (jvmOk(jenv, "Config.FromXML"))
			{
				try
				{
					config->fromXML(xml.c_str());
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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Config_1ToJSON
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jstring result;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			const char* json = config->toJSON();

			result = makeJavaString(jenv, json);
		}
	}
	JNI_CATCH

	return result;
}


#ifdef __cplusplus
}
#endif
