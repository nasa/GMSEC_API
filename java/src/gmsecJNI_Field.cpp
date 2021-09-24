/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// Field class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec/Field.h>
#include <gmsec/Status.h>

using gmsec::Field;
using gmsec::Status;


#ifdef __cplusplus
extern "C"
{
#endif

	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Field(JNIEnv *jenv, jclass jcls)
	{
		Field * created = 0;

		try
		{
			created = new Field();
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueCHAR(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jbyte jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueCHAR"))
				{
					status = field->SetName(name.c_str());

					if (!status.isError())
						status = field->SetValue((GMSEC_CHAR) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueBOOL(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jboolean jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);

				if (jvmOk(jenv, "Field.SetFieldValueCHAR"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_BOOL) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueI8(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jbyte jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueI8"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_I8) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueU8(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jint jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueU8"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_U8) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueI16(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jshort jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueI16"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_I16) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueU16(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jint jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueU16"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_U16) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH
		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueI32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jint jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueI32"))
				{
					status = field->SetName(name.c_str());

					if (!status.isError())
						status = field->SetValue((GMSEC_I32) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH
		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueU32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jlong jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueU32"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_U32) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueI64(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jlong jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueI64"))
				{
					status = field->SetName(name.c_str());

					if (!status.isError())
						status = field->SetValue((GMSEC_I64) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH
		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueF32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jfloat jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueF32"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_F32) jValue);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH
		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueF64(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jdouble jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetFieldValueF64"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_F64) jValue);
					created = new Status(status);
				}

			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueSTRING(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jstring jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				JStringManager value(jenv, jValue, true);
				if (jvmOk(jenv, "Field.SetFieldValueSTRING"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_STRING) value.c_str());
					created = new Status(status);
				}

			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetFieldValueBIN(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName, jbyteArray jValue, jlong count)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JByteArrayManager manager(jenv, jValue);
				JStringManager name(jenv, jName, true);
				if (jvmOk(jenv, "Field.SetFieldValueBIN"))
				{
					status = field->SetName(name.c_str());
					if (!status.isError())
						status = field->SetValue((GMSEC_BIN) manager.jbytes(), (GMSEC_U32) count);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH
		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_new_1Field_1Copy(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField)
	{
		Field *created = 0;

		try
		{
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				created = new Field(*field);
			}
		}
		JNI_CATCH
		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_delete_1Field(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField)
	{
		try
		{
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

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


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetName(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jGname)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				const char *name = NULL;
				status = field->GetName(name);
				if (name != NULL)
				{
					jstring s = makeJavaString(jenv, name);
					if (jvmOk(jenv, "Field.GetName"))
						jenv->SetObjectField(jGname, Cache::getCache().fieldString_value, s);
				}

				if (jvmOk(jenv, "Field.GetName"))
					created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetName(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jName)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager name(jenv, jName);
				if (jvmOk(jenv, "Field.SetName"))
					status = field->SetName(name.c_str());

				if (jvmOk(jenv, "Field.SetName"))
					created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetType(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jint jType)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetType((unsigned short) jType);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetType(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jGtype)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				unsigned short type = 0;
				status = field->GetType(type);

				jenv->SetIntField(jGtype, Cache::getCache().fieldU16_value, (jint) type);

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueCHAR(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_CHAR value = 0;
				status = field->GetValue(value);

				jenv->SetByteField(jValue, Cache::getCache().fieldChar_value, (jbyte) value);
				jvmOk(jenv, "Field.GetValueCHAR");
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueBOOL(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_BOOL value;
				status = field->GetValue(value);

				// GMSEC_BOOL can take values other than JNI_TRUE / JNI_FALSE
				jenv->SetBooleanField(jValue, Cache::getCache().fieldBool_value, (jboolean) value);
				jvmOk(jenv, "Field.GetValueBOOL");
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueI8(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_I8 value = 0;
				status = field->GetValue(value);

				jenv->SetByteField(jValue, Cache::getCache().fieldI8_value, (jbyte) value);
				jvmOk(jenv, "Field.GetValueI8");
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueU8(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_U8 value = 0;
				status = field->GetValue(value);

				jenv->SetIntField(jValue, Cache::getCache().fieldU8_value, (jint) value);
				jvmOk(jenv, "Field.GetValueU8");
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueI16(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_I16 value = 0;
				status = field->GetValue(value);

				jenv->SetShortField(jValue, Cache::getCache().fieldI16_value, (jshort) value);
				jvmOk(jenv, "Field.GetValueI16");
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueU16(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_U16 value = 0;
				status = field->GetValue(value);

				jenv->SetIntField(jValue, Cache::getCache().fieldU16_value, (jint) value);
				jvmOk(jenv, "Field.GetValueU16");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueI32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_I32 value = 0;
				status = field->GetValue(value);

				jenv->SetIntField(jValue, Cache::getCache().fieldI32_value, (jint) value);
				jvmOk(jenv, "Field.GetValueI32");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueU32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_U32 value = 0;
				status = field->GetValue(value);

				jenv->SetLongField(jValue, Cache::getCache().fieldU32_value, (jlong) value);
				jvmOk(jenv, "Field.GetValueU32");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueI64(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_I64 value = 0;
				status = field->GetValue(value);

				jenv->SetLongField(jValue, Cache::getCache().fieldI64_value, (jlong) value);
				jvmOk(jenv, "Field.GetValueI64");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueF32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_F32 value = 0;
				status = field->GetValue(value);

				jenv->SetFloatField(jValue, Cache::getCache().fieldF32_value, (jfloat) value);
				jvmOk(jenv, "Field.GetValueF32");
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueF64(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_F64 value = 0;
				status = field->GetValue(value);

				jenv->SetDoubleField(jValue, Cache::getCache().fieldF64_value, (jdouble) value);
				jvmOk(jenv, "Field.GetValueF64");

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueSTRING(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_STRING value = NULL;
				status = field->GetValue(value);

				if (!status.isError())
				{
					jstring s = makeJavaString(jenv, value);
					if (jvmOk(jenv, "Field.GetValueSTRING"))
						jenv->SetObjectField(jValue, Cache::getCache().fieldString_value, s);
				}

				if (jvmOk(jenv, "Field.GetValueSTRING"))
					created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1GetValueBIN(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jobject jValue, jobject jSize)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				GMSEC_BIN value = NULL;
				GMSEC_U32 size = 0;
				status = field->GetValue(&value, size);

				if (!status.isError())
				{
					jbyteArray dataIn = jenv->NewByteArray((jsize) size);
					if (jvmOk(jenv, "Field.GetValueBIN"))
					{
						jenv->SetByteArrayRegion(dataIn, 0, (jsize) size, (jbyte *) value);
						jenv->SetObjectField(jValue, Cache::getCache().fieldBin_value, dataIn);

						jenv->SetLongField(jSize, Cache::getCache().fieldU32_value, (jlong) size);
					}
				}

				if (jvmOk(jenv, "Field.GetValueBIN"))
					created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueCHAR(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jbyte jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_CHAR) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueBOOL(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jboolean jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_BOOL) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueI8(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jbyte jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_I8) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueU8(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jint jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_U8) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueI16(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jshort jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_I16) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueU16(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jint jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_U16) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueI32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jint jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_I32) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueU32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jlong jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_U32) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueI64(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jlong jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_I64) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueF32(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jfloat jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_F32) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueF64(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jdouble jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->SetValue((GMSEC_F64) jValue);
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueSTRING(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jstring jValue)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JStringManager value(jenv, jValue);
				if (jvmOk(jenv, "Field.SetValueSTRING"))
				{
					status = field->SetValue((GMSEC_STRING) value.c_str());
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1SetValueBIN(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField, jbyteArray jValue, jlong count)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				JByteArrayManager manager(jenv, jValue);
				if (jvmOk(jenv, "Field.SetValueBIN"))
				{
					status = field->SetValue((GMSEC_BIN) manager.jbytes(), (GMSEC_U32) count);
					created = new Status(status);
				}
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_Field_1UnSet(JNIEnv *jenv, jclass jcls, jlong jFldPtr, jobject jField)
	{
		Status *created = 0;

		try
		{
			Status status;
			Field *field = JNI_JLONG_TO_FIELD(jFldPtr);

			if (!field)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
			}
			else
			{
				status = field->UnSet();
				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}

#ifdef __cplusplus
}
#endif
