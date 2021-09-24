/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Config class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec/Config.h>
#include <gmsec/Status.h>
#include <gmsec/internal/strutil.h>

#include <vector>

using gmsec::Config;
using gmsec::Status;


#ifdef __cplusplus
extern "C"
{
#endif

	/** @brief Creates a new Config object and returns a pointer to it.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Config(JNIEnv *jenv, jclass jcls)
	{
		Config *created = 0;

		try
		{
			created = new Config();
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Creates a new Config object with the specified arguments and returns a pointer to it.
	 * @param jargs The arguments to create the Config with.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Config_1String(JNIEnv *jenv, jclass jcls, jobjectArray jargs)
	{
		Config *created = 0;
		try
		{
			jint size = jenv->GetArrayLength(jargs);
			if (size < 1)
				return Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Config(jenv, jcls);

			std::vector<char*> args;
			int i;
			for (i = 0; jvmOk(jenv, "new Config(String[])") && i < size; ++i)
			{
				jstring s = (jstring) jenv->GetObjectArrayElement(jargs, i);
				JStringManager manager(jenv, s);
				if (manager.c_str())
					args.push_back(gmsec::util::stringNew(manager.c_str()));
			}

			if (jvmOk(jenv, "new Config(String[])"))
				created = new Config((int) args.size(), &args[0]);

			for (size_t j = 0; j < args.size(); ++j)
				gmsec::util::stringDestroy(args[j]);
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Creates a Config object as a copy of the specified Config.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Config_1Copy(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig)
	{
		Config *created = 0;
		try
		{
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				created = new Config(*config);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Deletes a Config.
	*/
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_delete_1Config(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig)
	{
		try
		{
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

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


	/** @brief Adds the specified name-value pair to a Config object.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1AddValue(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig, jstring jName, jstring jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				JStringManager value(jenv, jValue, true);

				if (jvmOk(jenv, "Config.AddValue") && name.c_str() && value.c_str())
					status = config->AddValue(name.c_str(), value.c_str());
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Clears the name-value pair at the specified name in the specified Config.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1ClearValue(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig, jstring jName)
	{
		Status *created = 0;
		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Config.ClearValue"))
					status = config->ClearValue(name.c_str());
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Gets the value associated with the specified name in the specified Config.
	 * @param [out] jGvalue The value at the specified name.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1GetValue(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig, jstring jName, jobject jGvalue)
	{
		Status * created = 0;
		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				const char *value = NULL;
				JStringManager name(jenv, jName);

				if (jvmOk(jenv, "Config.GetValue"))
				{
					status = config->GetValue(name.c_str(), value);

					if (!status.isError())
					{
						jstring s = makeJavaString(jenv, value);
						if (jvmOk(jenv, "Config.GetValue"))
						{

							jenv->SetObjectField(jGvalue, Cache::getCache().fieldString_value, s);
						}
					}
				}

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Clears all name-value pairs in the specified Config.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1Clear(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig)
	{
		Status *created = 0;

		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				status = config->Clear();

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Gets the first name-value pair out of the specified config object.
	 * @param [out] jGname The name of the first pair.
	 * @param [out] jGvalue The value of the first pair.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1GetFirst(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig, jobject jGname, jobject jGvalue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				const char *name = NULL;
				const char *value = NULL;

				status = config->GetFirst(name, value);
				if (!status.isError())
				{

					jstring jName = makeJavaString(jenv, name);
					if (jvmOk(jenv, "Config.GetFirst"))
						jenv->SetObjectField(jGname, Cache::getCache().fieldString_value, jName);

					jstring jValue = 0;
					if (jvmOk(jenv, "Config.GetFirst"))
						jValue = makeJavaString(jenv, value);

					if (jvmOk(jenv, "Config.GetFirst"))
						jenv->SetObjectField(jGvalue, Cache::getCache().fieldString_value, jValue);
				}

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Gets a name-value pair from the specified Config object.  Subsequent
	 * calls to this method on the same Config will get subsequent names and
	 * values.
	 * @param [out] name The name of the next name-value pair.
	 * @param [out] value The value of the next name-value pair.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1GetNext(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig, jobject jGname, jobject jGvalue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				const char *name = NULL;
				const char *value = NULL;

				status = config->GetNext(name, value);

				if (!status.isError())
				{

					jstring jName = makeJavaString(jenv, name);
					if (jvmOk(jenv, "Config.GetNext"))
						jenv->SetObjectField(jGname, Cache::getCache().fieldString_value, jName);

					jstring jValue = 0;
					if (jvmOk(jenv, "Config.GetNext"))
						jValue = makeJavaString(jenv, value);
					if (jvmOk(jenv, "Config.GetNext"))
						jenv->SetObjectField(jGvalue, Cache::getCache().fieldString_value, jValue);
				}

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Converts the specified Config object to XML.
	 * @param [out] outxml The XML representation of this Config.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1ToXML(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfig, jobject outxml)
	{
		Status *created = 0;

		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(ptr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				const char *tmp = NULL;
				status = config->ToXML(tmp);

				if (!status.isError())
				{
					jstring s = makeJavaString(jenv, tmp);
					if (jvmOk(jenv, "Config.ToXML"))
					{
						jenv->SetObjectField(outxml, Cache::getCache().fieldString_value, s);
						jvmOk(jenv, "Config.ToXML");
					}
				}
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Creates a Config from an XML representation.
	 * @param [out] jConfig The new Config object.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Config_1FromXML(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfig, jstring inxml)
	{
		Status *created = 0;

		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(ptr);

			if (!config)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
			}
			else
			{
				JStringManager xml(jenv, inxml);
				if (jvmOk(jenv, "Config.FromXML"))
					status = config->FromXML(xml.c_str());
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}

#ifdef __cplusplus
}
#endif
