/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Field class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5/Fields.h>

#include <sstream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::jni;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetType
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jint result = static_cast<jint>( field->getType() );

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	const char* tmp = field->getName();
	jstring name = makeJavaString(jenv, tmp);
	jvmOk(jenv, "Field.getName");

	return name;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1ToXML
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jstring result = makeJavaString(jenv, field->toXML());

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1ToJSON
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jstring result = makeJavaString(jenv, field->toJSON());

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetStringValue
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jstring result = makeJavaString(jenv, field->getStringValue());

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetBooleanValue
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jboolean result = 0;

	try
	{
		result = (field->getBooleanValue() ? JNI_TRUE : JNI_FALSE);
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jshort JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetI16Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jshort result = 0;

	try
	{
		result = field->getI16Value();
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetI32Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jint result = 0;

	try
	{
		result = field->getI32Value();
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetI64Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jlong result = 0;

	try
	{
		result = field->getI64Value();
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetU16Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jobject result = 0;

	try
	{
		GMSEC_U16 value = field->getU16Value();

		result = jenv->NewObject(Cache::getCache().classU16, Cache::getCache().methodU16Init, static_cast<jint>(value));

		if (!jvmOk(jenv, "Field::getU32Value: new U16") || !result)
		{
			GMSEC_WARNING << "Unable to create U16 object";
			result = 0;
		}
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetU32Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jobject result = 0;

	try
	{
		GMSEC_U32 value = field->getU32Value();

		result = jenv->NewObject(Cache::getCache().classU32, Cache::getCache().methodU32Init, static_cast<jlong>(value));

		if (!jvmOk(jenv, "Field::getU32Value: new U32") || !result)
		{
			GMSEC_WARNING << "Unable to create U32 object";
			result = 0;
		}
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetU64Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jobject result = 0;

	try
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
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jdouble JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1GetF64Value
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jdouble result = 0;

	try
	{
		result = field->getF64Value();
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1IsHeader
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jboolean result = (field->isHeader() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1IsTracking
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	jboolean result = (field->isTracking() ? JNI_TRUE : JNI_FALSE);

	return result;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Field_1IsFieldNameCompliant
  (JNIEnv *jenv, jclass jcls, jstring jFieldName)
{
	jboolean result = JNI_FALSE;

	JStringManager fieldName(jenv, jFieldName);

	if (jvmOk(jenv, "Field.isFieldNameCompliant"))
	{
		result = (InternalField::isFieldNameCompliant(fieldName.c_str()) ? JNI_TRUE : JNI_FALSE);
	}

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1Field
  (JNIEnv *jenv, jclass jcls, jlong jFieldPtr, jobject jField)
{
	Field* field = JNI_JLONG_TO_FIELD(jFieldPtr);

	delete field;
}


#ifdef __cplusplus
}
#endif
