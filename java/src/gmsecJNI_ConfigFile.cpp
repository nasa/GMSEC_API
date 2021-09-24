/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// ConfigFile class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec/Config.h>
#include <gmsec/ConfigFile.h>
#include <gmsec/Message.h>
#include <gmsec/Status.h>

using gmsec::Config;
using gmsec::ConfigFile;
using gmsec::Message;
using gmsec::Status;


#ifdef __cplusplus
extern "C"
{
#endif

	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1ConfigFile__Lgov_nasa_gsfc_gmsecapi_jni_JNIConfigFile_2(JNIEnv *jenv, jclass jcls, jobject jConfigFile)
	{
		ConfigFile *created = 0;
		try
		{
			if (jvmOk(jenv, "new ConfigFile"))
			{
				created = new ConfigFile();
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1ConfigFile__Lgov_nasa_gsfc_gmsecapi_jni_JNIConfigFile_2Ljava_lang_String_2(JNIEnv *jenv, jclass jcls, jobject jConfigFile, jstring filepath)
	{
		ConfigFile *created = 0;
		try
		{
			JStringManager manager(jenv, filepath);

			if (jvmOk(jenv, "new ConfigFile"))
			{
				created = new ConfigFile(manager.c_str());
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1Load__JLgov_nasa_gsfc_gmsecapi_jni_JNIConfigFile_2(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfigFile)
	{
		Status *created = 0;
		try
		{
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				Status status = cf->Load();
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1Load__JLgov_nasa_gsfc_gmsecapi_jni_JNIConfigFile_2Ljava_lang_String_2(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfigFile, jstring inFilePath)
	{
		Status *created = 0;
		try
		{
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager filePath(jenv, inFilePath);
				Status status = cf->Load(filePath.c_str());
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1Save__JLgov_nasa_gsfc_gmsecapi_jni_JNIConfigFile_2Ljava_lang_String_2Z
	(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfigFile, jstring inFilePath, jboolean jCompact)
	{
		Status *result = 0;
		try
		{
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				bool compact = (jCompact == JNI_TRUE);
				JStringManager filePath(jenv, inFilePath);
				Status status = cf->Save(filePath.c_str(), compact);
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_delete_1ConfigFile(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfigFile)
	{
		try
		{
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
				delete cf;
		}
		JNI_CATCH
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1FromXML(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfigFile, jstring inxml)
	{
		Status *created = 0;

		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (!cf)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager xml(jenv, inxml);
				if (jvmOk(jenv, "ConfigFile.FromXML"))
					status = cf->FromXML(xml.c_str());
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jboolean JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1IsLoaded(JNIEnv * jenv, jclass jcls, jlong ptr, jobject jConfigFile)
	{
		jboolean loaded = 0;

		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (!cf)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				loaded = (jboolean) cf->IsLoaded();
			}
		}
		JNI_CATCH

		return loaded;
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1LookupConfig(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jName, jlong cptr, jobject config)
	{
		Status *created = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				Config *cfg = JNI_JLONG_TO_CONFIG(cptr);
				if (cfg == NULL)
				{
					SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
				}
				else
				{
					JStringManager name(jenv, jName);
					if (name.c_str())
						status = cf->LookupConfig(name.c_str(), cfg);
				}

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1LookupMessage(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jName, jlong mptr, jobject message)
	{
		Status *created = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				Message *msg = JNI_JLONG_TO_MESSAGE(mptr);
				if (msg == NULL)
				{
					SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
				}
				else
				{
					JStringManager name(jenv, jName);
					if (name.c_str())
						status = cf->LookupMessage(name.c_str(), msg);

					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1ToXML(JNIEnv *jenv, jclass jcls, jlong ptr, jobject jConfigFile, jobject outxml)
	{
		Status *created = 0;

		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (!cf)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				const char *tmp = NULL;
				status = cf->ToXML(tmp);

				if (!status.isError())
				{
					jstring s = makeJavaString(jenv, tmp);
					if (jvmOk(jenv, "ConfigFile.ToXML"))
					{
						jenv->SetObjectField(outxml, Cache::getCache().fieldString_value, s);
						jvmOk(jenv, "ConfigFile.ToXML");
					}
				}
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1LookupSubscription(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jName, jobject pattern)
	{
		Status *created = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				const char *value = NULL;
				JStringManager name(jenv, jName);
				if (name.c_str())
					status = cf->LookupSubscription(name.c_str(), value);

				jstring s = 0;

				if (jvmOk(jenv, "ConfigFile.LookupSubscription"))
					s = makeJavaString(jenv, value);

				if (jvmOk(jenv, "ConfigFile.LookupSubscription"))
					jenv->SetObjectField(pattern, Cache::getCache().fieldString_value, s);

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1AddSubscription(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jName, jstring jPattern)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				JStringManager pattern(jenv, jPattern);
				status = cf->AddSubscription(name.c_str(), pattern.c_str());
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1AddConfig(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jlong cptr, jstring jName, jobject config)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);
			Config *cfg    = JNI_JLONG_TO_CONFIG(cptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				status = cf->AddConfig(name.c_str(), *cfg);
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1AddMessage(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jlong mptr, jstring jName, jobject message)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);
			Message *msg   = JNI_JLONG_TO_MESSAGE(mptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				status = cf->AddMessage(name.c_str(), *msg);
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1AddCustomXML(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jXml)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager xml(jenv, jXml);
				status = cf->AddCustomXML(xml.c_str());
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1RemoveSubscription(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jName)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				status = cf->RemoveSubscription(name.c_str());
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1RemoveConfig(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jName)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				status = cf->RemoveConfig(name.c_str());
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1RemoveMessage(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jName)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				status = cf->RemoveMessage(name.c_str());
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConfigFile_1RemoveCustomXML(JNIEnv *jenv, jclass jcls, jlong ptr, jobject configFile, jstring jXml)
	{
		Status *result = 0;
		try
		{
			Status status;
			ConfigFile *cf = JNI_JLONG_TO_CONFIGFILE(ptr);

			if (cf == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ConfigFile reference is null");
			}
			else
			{
				JStringManager xml(jenv, jXml);
				status = cf->RemoveCustomXML(xml.c_str());
				result = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(result);
	}

#ifdef __cplusplus
}
#endif
