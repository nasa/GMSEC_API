/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Errors.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    new_Status
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Status
  (JNIEnv *jenv, jclass jcls)
{
	Status* created = new Status();

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    new_Status_Set
 * Signature: (IILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Status_1Set__IILjava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jint jeclass, jint jecode, jstring jemsg)
{
	Status* created = 0;

	JStringManager tmp(jenv, jemsg);

	if (jvmOk(jenv, "Status.Set"))
	{
		created = new Status((StatusClass) jeclass, (StatusCode) jecode, tmp.c_str());
	}

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    new_Status_Set
 * Signature: (IILjava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Status_1Set__IILjava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jint jeclass, jint jecode, jstring jemsg, jint jecustom)
{
	Status* created = 0;

	JStringManager tmp(jenv, jemsg);

	if (jvmOk(jenv, "Status.Set"))
	{
		created = new Status((StatusClass) jeclass, (StatusCode) jecode, tmp.c_str(), (int) jecustom);
	}

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    new_Status_Copy
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)J
 */
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Status_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	Status* created = new Status(*status);

	return JNI_POINTER_TO_JLONG(created);
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    delete_Status
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1Status
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	delete status;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_IsError
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1HasError
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	jint error = status->hasError();

	return error;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_GetString
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1Get
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status *status = JNI_JLONG_TO_STATUS(jStatusPtr);

	const char* tmp = status->get();
	jstring s = makeJavaString(jenv, tmp);
	jvmOk(jenv, "Status GetString");

	return s;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_GetClassification
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1GetClassification
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	jint result = static_cast<jint>(status->getClass());

	return result;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_SetClassification
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1SetClassification
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	status->setClass(static_cast<StatusClass>(jeclass));
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_GetCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1GetCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	jint result = static_cast<jint>(status->getCode());

	return result;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_SetCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1SetCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jecode)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	status->setCode(static_cast<StatusCode>(jecode));
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_GetReason
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1GetReason
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	const char* tmp = status->getReason();
	jstring s = makeJavaString(jenv, tmp);
	jvmOk(jenv, "Status GetErorReason");

	return s;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_SetErrorReason
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1SetErrorReason
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jstring jemsg)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	JStringManager text(jenv, jemsg);
	if (text.c_str())
	{
		status->setReason(text.c_str());
	}
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_GetCustomCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)I
 */
JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1GetCustomCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	jint result = static_cast<jint>(status->getCustomCode());

	return result;
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_SetCustomCode
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1SetCustomCode
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jcustom)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	status->setCustomCode(static_cast<int>(jcustom));
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_Set
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1Set__JLgov_nasa_gsfc_gmsec_api5_jni_JNIStatus_2IILjava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass, jint jecode, jstring jemsg)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	JStringManager tmp(jenv, jemsg);

	if (jvmOk(jenv, "Status.Set"))
	{
		status->set(static_cast<StatusClass>(jeclass), static_cast<StatusCode>(jecode), tmp.c_str());
	}
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_Set
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;IILjava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1Set__JLgov_nasa_gsfc_gmsec_api5_jni_JNIStatus_2IILjava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus, jint jeclass, jint jecode, jstring jemsg, jint jcustom)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	JStringManager tmp(jenv, jemsg);

	if (jvmOk(jenv, "Status.Set"))
	{
		status->set(static_cast<StatusClass>(jeclass), static_cast<StatusCode>(jecode), tmp.c_str(), static_cast<int>(jcustom));
	}
}


/*
 * Class:     gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI
 * Method:    Status_Reset
 * Signature: (JLgov/nasa/gsfc/gmsec/api/jni/JNIStatus;)V
 */
JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Status_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jStatusPtr, jobject jStatus)
{
	Status* status = JNI_JLONG_TO_STATUS(jStatusPtr);

	status->reset();
}


#ifdef __cplusplus
}
#endif
