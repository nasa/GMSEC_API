/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Errors.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    new_Status
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Status
  (JNIEnv *jenv, jclass jcls)
{
	Status *created = 0;

	try
	{
		created = new Status();
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    new_Status_Set
 * Signature: (IILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Status_1Set__IILjava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jint jeclass, jint jecode, jstring jemsg)
{
	Status* created = 0;

	try
	{
		JStringManager tmp(jenv, jemsg);

		if (jvmOk(jenv, "Status.Set"))
		{
			created = new Status((StatusClass) jeclass, (StatusCode) jecode, tmp.c_str());
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    new_Status_Set
 * Signature: (IILjava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Status_1Set__IILjava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jint jeclass, jint jecode, jstring jemsg, jint jecustom)
{
	Status* created = 0;

	try
	{
		JStringManager tmp(jenv, jemsg);

		if (jvmOk(jenv, "Status.Set"))
		{
			created = new Status((StatusClass) jeclass, (StatusCode) jecode, tmp.c_str(), (int) jecustom);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    new_Status_Copy
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Status_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* created = 0;

	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			created = new Status(*status);
		}
	}
	JNI_CATCH

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    delete_Status
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Status
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			delete status;
		}
	}
	JNI_CATCH
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_IsError
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1IsError
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	jint error = 0;

	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			error = status->isError();
		}
	}
	JNI_CATCH

	return error;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_GetString
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1Get
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	jstring s = 0;

	try
	{
		const char *tmp = "";
		Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			tmp = status->get();
			s = makeJavaString(jenv, tmp);
			jvmOk(jenv, "Status GetString");
		}
	}
	JNI_CATCH

	return s;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_GetClassification
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1GetClassification
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	jint result = 0;

	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			result = status->getClass();
		}
	}
	JNI_CATCH

	return result;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_SetClassification
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1SetClassification
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			status->setClass((StatusClass) jeclass);
		}
	}
	JNI_CATCH
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_GetCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1GetCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	jint result = 0;

	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			result = status->getCode();
		}
	}
	JNI_CATCH

	return result;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_SetCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1SetCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jecode)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			status->setCode((StatusCode) jecode);
		}
	}
	JNI_CATCH
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_GetReason
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1GetReason
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	jstring s = 0;

	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);
		const char* tmp = "";

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			tmp = status->getReason();
			s = makeJavaString(jenv, tmp);
			jvmOk(jenv, "Status GetErorReason");
		}
	}
	JNI_CATCH

	return s;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_SetErrorReason
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1SetErrorReason
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jstring jemsg)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			JStringManager text(jenv, jemsg);
			if (text.c_str())
			{
				status->setReason(text.c_str());
			}
		}
	}
	JNI_CATCH
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_GetCustomCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1GetCustomCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	jint result = 0;

	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			result = status->getCustomCode();
		}
	}
	JNI_CATCH

	return result;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_SetCustomCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1SetCustomCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jcustom)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			status->setCustomCode((int) jcustom);
		}
	}
	JNI_CATCH
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_Set
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1Set__JLgov_nasa_gsfc_gmsec_api_jni_JNIStatus_2IILjava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass, jint jecode, jstring jemsg)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			JStringManager tmp(jenv, jemsg);

			if (jvmOk(jenv, "Status.Set"))
			{
				status->set((StatusClass) jeclass, (StatusCode) jecode, tmp.c_str());
			}
		}
	}
	JNI_CATCH
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_Set
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;IILjava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1Set__JLgov_nasa_gsfc_gmsec_api_jni_JNIStatus_2IILjava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass, jint jecode, jstring jemsg, jint jcustom)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			JStringManager tmp(jenv, jemsg);

			if (jvmOk(jenv, "Status.Set"))
			{
				status->set((StatusClass) jeclass, (StatusCode) jecode, tmp.c_str(), (int) jcustom);
			}
		}
	}
	JNI_CATCH
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api_jni_gmsecJNI
 * Method:    Status_Reset
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Status_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	try
	{
		Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

		if (status == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
		}
		else
		{
			status->reset();
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
