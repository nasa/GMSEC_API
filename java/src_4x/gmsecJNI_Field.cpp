/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Fields.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif

static int fieldTypeToJava(JNIEnv* jenv, Field::FieldType type)
{
	switch (type)
	{
	case Field::BIN_TYPE:    return 0;
	case Field::BOOL_TYPE:   return 1;
	case Field::CHAR_TYPE:   return 2;
	case Field::I8_TYPE:     return 3;
	case Field::I16_TYPE:    return 4;
	case Field::I32_TYPE:    return 5;
	case Field::I64_TYPE:    return 6;
	case Field::F32_TYPE:    return 7;
	case Field::F64_TYPE:    return 8;
	case Field::STRING_TYPE: return 9;
	case Field::U8_TYPE:     return 10;
	case Field::U16_TYPE:    return 11;
	case Field::U32_TYPE:    return 12;
	case Field::U64_TYPE:    return 13;
	}

	ThrowGmsecException(jenv, "Unknown field type");

	// We will never reach here, but the compiler will be happy nonetheless.
	return -1;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1GetType
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jint result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (field == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = (jint) fieldTypeToJava(jenv, field->getType());
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jstring name;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (field == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			const char* tmp = field->getName();
			name = jenv->NewStringUTF(tmp);
			jvmOk(jenv, "Field.getName");
		}
	}
	JNI_CATCH

	return name;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jstring result;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = jenv->NewStringUTF(field->toXML());
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1ToJSON
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jstring result;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = jenv->NewStringUTF(field->toJSON());
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1IsHeader__JLgov_nasa_gsfc_gmsec_api_jni_JNIField_2Z
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField, jboolean jHeader)
{
	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			field->isHeader(jHeader == JNI_TRUE);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1IsHeader__JLgov_nasa_gsfc_gmsec_api_jni_JNIField_2
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jboolean result = JNI_FALSE;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = (field->isHeader() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Field
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			delete field;
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
