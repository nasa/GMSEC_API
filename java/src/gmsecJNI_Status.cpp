/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Status class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

using gmsec::Status;


#ifdef __cplusplus
extern "C"
{
#endif

	/** @brief Creates a new Status object and returns a pointer to it.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Status(JNIEnv *jenv, jclass jcls)
	{
		Status *created = 0;

		try
		{
			created = new Status();
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Creates a new status object with the specified error class, error
	 * code, and message, and returns a pointer to it.
	 */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Status_1Set(JNIEnv *jenv, jclass jcls, jint jeclass, jlong jcode, jstring jtext)
	{
		Status *created = 0;

		try
		{
			JStringManager text(jenv, jtext);
			if (jvmOk(jenv, "Status.Set"))
				created = new Status((unsigned short) jeclass, (unsigned int) jcode, text.c_str());

		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Creates a new Status object as a copy of the specified Status and return a pointer to the new object.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Status_1Copy(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		Status *created = 0;

		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

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


	/** @brief Deletes a Status object.
	*/
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_delete_1Status(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
				delete status;
		}
		JNI_CATCH
	}


	/** @brief Retrieves the string detailed description from the specified status and returns it.
	*/
	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1GetString(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
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
				tmp = status->GetString();
				s = makeJavaString(jenv, tmp);
				jvmOk(jenv, "Status GetString");
			}
		}
		JNI_CATCH

		return s;
	}


	/** @brief Returns a string representation of the specified Status object.
	*/
	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1Get(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		jstring s  = 0;

		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);
			const char *tmp = "";

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				tmp = status->Get();
				s = makeJavaString(jenv, tmp);
				jvmOk(jenv, "Status Get");
			}
		}
		JNI_CATCH

		return s;
	}


	/** @brief Returns the error class of the specified Status object.
	*/
	JNIEXPORT jint JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1GetClass(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		jint result = 0;

		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				result = status->GetClass();
			}
		}
		JNI_CATCH

		return result;
	}


	/** @brief Returns the error code of the specified Status object.
	*/
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1GetCode(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		jlong result = 0;

		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				result = status->GetCode();
			}
		}
		JNI_CATCH

		return result;
	}


	/** @brief Returns the custom error code of the specified Status object.
	*/
	JNIEXPORT jint JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1GetCustomCode(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		jint result = 0;

		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				result = status->GetCustomCode();
			}
		}
		JNI_CATCH

		return result;
	}


	/** @brief If the error class of the specified Status object is
	 * GMSEC_STATUS_NO_ERROR, returns false; otherwise returns true.
	 * @note deprecated, use IsError instead
	 */
	JNIEXPORT jint JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1isError(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		jint error = 0;

		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

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

	/** @brief If the error class of the specified Status object is
	 * GMSEC_STATUS_NO_ERROR, returns false; otherwise returns true.
	 */
	JNIEXPORT jint JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1IsError(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		jint error = 0;

		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				error = status->IsError();
			}
		}
		JNI_CATCH

		return error;
	}


	/** @brief Sets the string detailed description of the specified Status object to the specified string.
	*/
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1SetString(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jstring jtext)
	{
		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				JStringManager text(jenv, jtext);
				if (text.c_str())
					status->SetString(text.c_str());
			}
		}
		JNI_CATCH
	}


	/** @brief Sets the error class of the specified Status object to the specified class.
	*/
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1SetClass(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass)
	{
		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				status->SetClass((unsigned short) jeclass);
			}
		}
		JNI_CATCH
	}


	/** @brief Sets the error code of the specified Status object the specified error code.
	*/
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1SetCode(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jlong jcode)
	{
		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				status->SetCode((unsigned int) jcode);
			}
		}
		JNI_CATCH
	}


	/** @brief Sets the custom error code of the specified Status object.
	*/
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1SetCustomCode(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jcode)
	{
		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				status->SetCustomCode(jcode);
			}
		}
		JNI_CATCH
	}


	/** @brief Sets the error class, error code, and string detailed description of the specified Status object.
	*/
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1Set(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass, jlong jcode, jstring jtext)
	{
		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				JStringManager text(jenv, jtext);
				if (jvmOk(jenv, "Status.Set"))
					status->Set((unsigned short) jeclass, (unsigned int) jcode, text.c_str());
			}
		}
		JNI_CATCH
	}


	/** @brief Sets the error class to GMSEC_STATUS_NO_ERROR, the error code to 0,
	 * and the string detailed description to NULL.
	 */
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Status_1ReSet(JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
	{
		try
		{
			Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

			if (status == NULL)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Status reference is null");
			}
			else
			{
				status->ReSet();
			}
		}
		JNI_CATCH
	}

#ifdef __cplusplus
}
#endif
