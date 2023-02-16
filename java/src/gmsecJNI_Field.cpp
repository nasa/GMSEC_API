/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Fields.h>

#include <sstream>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetType
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
			result = static_cast<jint>( field->getType() );
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetName
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
			name = makeJavaString(jenv, tmp);
			jvmOk(jenv, "Field.getName");
		}
	}
	JNI_CATCH

	return name;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jstring result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = makeJavaString(jenv, field->toXML());
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1ToJSON
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jstring result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = makeJavaString(jenv, field->toJSON());
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetStringValue
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jstring result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = makeJavaString(jenv, field->getStringValue());
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetBooleanValue
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jboolean result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = (field->getBooleanValue() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetI16Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jshort result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = field->getI16Value();
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetI32Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jint result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = field->getI32Value();
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetI64Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jlong result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = field->getI64Value();
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetU16Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jobject result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			GMSEC_U16 value = field->getU16Value();

			result = jenv->NewObject(Cache::getCache().classU16, Cache::getCache().methodU16Init, static_cast<jint>(value));

			if (!jvmOk(jenv, "Field::getU32Value: new U16") || !result)
			{
				GMSEC_WARNING << "Unable to create U16 object";
				result = 0;
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetU32Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jobject result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			GMSEC_U32 value = field->getU32Value();

			result = jenv->NewObject(Cache::getCache().classU32, Cache::getCache().methodU32Init, static_cast<jlong>(value));

			if (!jvmOk(jenv, "Field::getU32Value: new U32") || !result)
			{
				GMSEC_WARNING << "Unable to create U32 object";
				result = 0;
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetU64Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jobject result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			GMSEC_U64 value = field->getU64Value();

			std::ostringstream oss;
			oss << value;

			jstring valueAsString = makeJavaString(jenv, oss.str().c_str());

			result = jenv->NewObject(Cache::getCache().classU64, Cache::getCache().methodU64Init, valueAsString);

			if (!jvmOk(jenv, "Field::getU64Value: new U64") || !result)
			{
				GMSEC_WARNING << "Unable to create U64 object";
				result = 0;
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetF64Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	jdouble result = 0;

	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			result = field->getF64Value();
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1IsHeader__JLgov_nasa_gsfc_gmsec_api5_jni_field_JNIField_2
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1Field
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
