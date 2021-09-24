/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST Mnemonic class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicSample.h>

#include <gmsec4/util/DataList.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Mnemonic__Ljava_lang_String_2_3J_3Lgov_nasa_gsfc_gmsec_api_jni_mist_JNIMnemonicSample_2I
  (JNIEnv *jenv, jclass jcls, jstring jName, jlongArray jMnemonicSamplePtrs, jobjectArray jMnemonicSamples, jint jNumSamples)
{
	jlong created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "Mnemonic"))
		{
			DataList<MnemonicSample*> samples;

			if ((int) jNumSamples > 0)
			{
				jlong* mnemonicSamplePtrs = jenv->GetLongArrayElements(jMnemonicSamplePtrs, JNI_FALSE);

				for (int i = 0; i < (int) jNumSamples; ++i)
				{
					samples.add(JNI_JLONG_TO_MNEMONIC_SAMPLE(mnemonicSamplePtrs[i]));
				}
			}

			created = JNI_POINTER_TO_JLONG(new Mnemonic(name.c_str(), samples));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Mnemonic_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	jlong created = 0;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new Mnemonic(*mnemonic));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Mnemonic
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			delete mnemonic;
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	jstring jName = 0;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			const char* name = mnemonic->getName();

			jName = makeJavaString(jenv, name);

			jvmOk(jenv, "Mnemonic.getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1StatusAvailable
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	jboolean result = JNI_FALSE;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			result = (mnemonic->statusAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1GetStatus
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	jlong jField = 0;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			try
			{
				const Field& field = mnemonic->getStatus();

				jField = JNI_POINTER_TO_JLONG(&field);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jField;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1SetStatus
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic, jlong jFieldPtr, jobject jField)
{
	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);
		Field*    field    = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			mnemonic->setStatus(*field);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1UnitsAvailable
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	jboolean result = JNI_FALSE;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			result = (mnemonic->unitsAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1GetUnits
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	jstring jUnits = 0;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			const char* units = mnemonic->getUnits();

			jUnits = makeJavaString(jenv, units);

			jvmOk(jenv, "Mnemonic.getUnits");
		}
	}
	JNI_CATCH

	return jUnits;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1SetUnits
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic, jstring jUnits)
{
	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			JStringManager units(jenv, jUnits);

			if (jvmOk(jenv, "Mnemonic.setUnits"))
			{
				mnemonic->setUnits(units.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1GetSampleCount
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic)
{
	jlong count = 0;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			count = (jlong) mnemonic->getSampleCount();
		}
	}
	JNI_CATCH

	return count;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1GetSample
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic, jlong jIndex)
{
	jlong jSample = 0;

	try
	{
		Mnemonic* mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			try
			{
				const MnemonicSample& sample = mnemonic->getSample((size_t) jIndex);

				jSample = JNI_POINTER_TO_JLONG(new MnemonicSample(sample));
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jSample;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Mnemonic_1AddSample
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicPtr, jobject jMnemonic, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	try
	{
		Mnemonic*       mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);
		MnemonicSample* sample   = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			mnemonic->addSample(*sample);
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
