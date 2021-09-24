/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST MnemonicSample class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/mist/MnemonicSample.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicSample
  (JNIEnv *jenv, jclass jcls, jstring jTimestamp, jlong jFieldPtr, jobject jField)
{
	jlong created = 0;

	try
	{
		JStringManager timestamp(jenv, jTimestamp);

		if (jvmOk(jenv, "MnemonicSample"))
		{
			Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				created = JNI_POINTER_TO_JLONG(new MnemonicSample(timestamp.c_str(), *field));
			}
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicSample_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jlong created = 0;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new MnemonicSample(*sample));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1MnemonicSample
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			delete sample;
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetTimestamp
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jstring jTimestamp = 0;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			const char* timestamp = sample->getTimestamp();

			jTimestamp = makeJavaString(jenv, timestamp);

			jvmOk(jenv, "MnemonicSample.getTimestamp");
		}
	}
	JNI_CATCH

	return jTimestamp;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetRawValue
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jlong jFieldPtr = 0;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			const Field& field = sample->getRawValue();

			jFieldPtr = JNI_POINTER_TO_JLONG(&field);
		}
	}
	JNI_CATCH

	return jFieldPtr;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1HasEUValue
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (sample->hasEUValue() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetEUValue
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jlong jFieldPtr = 0;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			try
			{
				const Field& field = sample->getEUValue();

				jFieldPtr = JNI_POINTER_TO_JLONG(&field);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jFieldPtr;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1SetEUValue
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample, jlong jFieldPtr, jobject jField)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);
		Field*          field  = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			sample->setEUValue(*field);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1HasTextValue
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (sample->hasTextValue() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetTextValue
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jstring jValue = 0;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			try
			{
				const char* value = sample->getTextValue();

				jValue = makeJavaString(jenv, value);

				jvmOk(jenv, "MnemonicSample.getTextValue");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jValue;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1SetTextValue
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample, jstring jValue)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			JStringManager value(jenv, jValue);

			if (jvmOk(jenv, "MnemonicSample.setTextValue"))
			{
				sample->setTextValue(value.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1HasFlags
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (sample->hasFlags() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetFlags
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jint jFlags = 0;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			try
			{
				jFlags = (jint) sample->getFlags();
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jFlags;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1SetFlags
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample, jint jFlags)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			sample->setFlags((int) jFlags);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1HasLimitEnableDisable
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (sample->hasLimitEnableDisable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetLimitEnableDisable
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			try
			{
				jResult = (sample->getLimitEnableDisable() ? JNI_TRUE : JNI_FALSE);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1SetLimitEnableDisable
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample, jboolean jFlag)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			sample->setLimitEnableDisable(jFlag == JNI_TRUE);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1HasLimit
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (sample->hasLimit() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetLimit
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jint jResult = 0;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (jint) sample->getLimit();
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1SetLimit
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample, jint jFlag)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			MnemonicSample::LimitFlag flag = static_cast<MnemonicSample::LimitFlag>((int) jFlag);

			sample->setLimit(flag);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1HasStalenessStatus
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (sample->hasStalenessStatus() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetStalenessStatus
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			try
			{
				jResult = (sample->getStalenessStatus() ? JNI_TRUE : JNI_FALSE);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1SetStalenessStatus
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample, jboolean jStatus)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			sample->setStalenessStatus(jStatus == JNI_TRUE);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1HasQuality
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			jResult = (sample->hasQuality() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1GetQuality
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			try
			{
				jResult = (sample->getQuality() ? JNI_TRUE : JNI_FALSE);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicSample_1SetQuality
  (JNIEnv *jenv, jclass jcls, jlong jMnemonicSamplePtr, jobject jMnemonicSample, jboolean jQuality)
{
	try
	{
		MnemonicSample* sample = JNI_JLONG_TO_MNEMONIC_SAMPLE(jMnemonicSamplePtr);

		if (!sample)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicSample reference is null");
		}
		else
		{
			sample->setQuality(jQuality == JNI_TRUE);
		}
	}
	JNI_CATCH
}



#ifdef __cplusplus
}
#endif
