/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Fields.h>

#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C" {
#endif


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
			name = makeJavaString(jenv, tmp);
			jvmOk(jenv, "Field.getName");
		}
	}
	JNI_CATCH

	return name;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1SetName
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField, jstring jFieldName)
{
	try
	{
		Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (field == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			JStringManager fieldName(jenv, jFieldName);

			if (jvmOk(jenv, "Field.setName"))
			{
				try
				{
					field->setName(fieldName.c_str());
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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1ToXML
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


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1ToJSON
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1GetIntegerValue
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
			result = field->getIntegerValue();
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1GetUnsignedIntegerValue
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
			GMSEC_U64 value = field->getUnsignedIntegerValue();

			std::ostringstream oss;
			oss << value;

			jstring valueAsString = makeJavaString(jenv, oss.str().c_str());

			result = jenv->NewObject(Cache::getCache().classU64, Cache::getCache().methodU64Init, valueAsString);

			if (!gmsec::api::jni::jvmOk(jenv, "Field::getUnsignedIntegerValue: new U64") || !result)
			{
				GMSEC_WARNING << "Unable to create U64 object";
				result = 0;
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1GetDoubleValue
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
			result = field->getDoubleValue();
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1GetStringValue
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1IsHeader__JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2Z
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


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Field_1IsHeader__JLgov_nasa_gsfc_gmsec_api_jni_field_JNIField_2
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
