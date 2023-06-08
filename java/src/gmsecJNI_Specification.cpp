/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Specification class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/FieldSpecification.h>
#include <gmsec5/MessageSpecification.h>
#include <gmsec5/Specification.h>

#include <gmsec5/util/List.h>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;
using namespace gmsec::api5::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Specification
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jlong spec = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (config == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			spec = JNI_POINTER_TO_JLONG(new Specification(*config));
		}
	}
	JNI_CATCH

	return spec;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_new_1Specification_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jlong spec = 0;

	try
	{
		Specification* other = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (other == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			spec = JNI_POINTER_TO_JLONG(new Specification(*other));
		}
	}
	JNI_CATCH

	return spec;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_delete_1Specification
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			delete spec;
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Specification_1GetSchemaIDIterator
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jlong jIter = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			SchemaIDIterator& iter = spec->getSchemaIDIterator();

			jIter = JNI_POINTER_TO_JLONG(&iter);
		}
	}
	JNI_CATCH

	return jIter;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Specification_1GetVersion
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jint version = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			version = (jint) spec->getVersion();
		}
	}
	JNI_CATCH

	return version;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Specification_1GetSchemaLevel
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jint level = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			level = (jint) spec->getSchemaLevel();
		}
	}
	JNI_CATCH

	return level;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Specification_1GetMessageSpecifications
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec)
{
	jobject jMsgSpecs = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			static jclass    jArrayList     = static_cast<jclass>(jenv->NewGlobalRef(jenv->FindClass("java/util/ArrayList")));
			static jmethodID jArrayListCtor = jenv->GetMethodID(jArrayList, "<init>", "(I)V");
			static jmethodID jArrayListAdd  = jenv->GetMethodID(jArrayList, "add", "(Ljava/lang/Object;)Z");

			static jclass    jMsgSpec       = static_cast<jclass>(jenv->NewGlobalRef(jenv->FindClass("gov/nasa/gsfc/gmsec/api5/MessageSpecification")));
			static jmethodID jMsgSpecCtor   = jenv->GetMethodID(jMsgSpec, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/util/ArrayList;)V");

			static jclass    jFldSpec       = static_cast<jclass>(jenv->NewGlobalRef(jenv->FindClass("gov/nasa/gsfc/gmsec/api5/FieldSpecification")));
			static jmethodID jFldSpecCtor   = jenv->GetMethodID(jFldSpec, "<init>",
			                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

			const List<MessageSpecification*>& msgSpecs = spec->getMessageSpecifications();

			// Create ArrayList for the number of MessageSpecification objects we have
			jMsgSpecs = jenv->NewObject(jArrayList, jArrayListCtor, msgSpecs.size());

			for (List<MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
			{
				const MessageSpecification* msgSpec = *it;

				const List<FieldSpecification*>& fldSpecs = msgSpec->getFieldSpecifications();

				// Create ArrayList for the number of FieldSpecification objects we have
				jobject jFldSpecs = jenv->NewObject(jArrayList, jArrayListCtor, fldSpecs.size());

				for (List<FieldSpecification*>::const_iterator it2 = fldSpecs.begin(); it2 != fldSpecs.end(); ++it2)
				{
					const FieldSpecification* fldSpec = *it2;

					jstring name  = makeJavaString(jenv, fldSpec->getName());
					jstring type  = makeJavaString(jenv, fldSpec->getType());
					jstring mode  = makeJavaString(jenv, fldSpec->getMode());
					jstring clazz = makeJavaString(jenv, fldSpec->getClassification());
					jstring value = makeJavaString(jenv, fldSpec->getValue());
					jstring desc  = makeJavaString(jenv, fldSpec->getDescription());

					// Instantiate new FieldSpecification object
					jobject jNewFldSpec = jenv->NewObject(jFldSpec, jFldSpecCtor, name, type, mode, clazz, value, desc);

					// Add FieldSpecification object to the ArrayList
					jenv->CallBooleanMethod(jFldSpecs, jArrayListAdd, jNewFldSpec);
				}

				jstring schemaID        = makeJavaString(jenv, msgSpec->getSchemaID());
				jstring subjectTemplate = makeJavaString(jenv, msgSpec->getSubjectTemplate());

				// Instantiate new MessageSpecification object
				jobject jNewMsgSpec = jenv->NewObject(jMsgSpec, jMsgSpecCtor, schemaID, subjectTemplate, jFldSpecs);

				// Add MessageSpecification object to the ArrayList that we will return
				jenv->CallBooleanMethod(jMsgSpecs, jArrayListAdd, jNewMsgSpec);
			}
		}
	}
	JNI_CATCH

	return jMsgSpecs;
}


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_Specification_1GetHeaderFieldNames
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jstring jschemaName)
{
	jobject jHeaders = 0;
	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			static jclass    jArrayList = static_cast<jclass>(jenv->NewGlobalRef(jenv->FindClass("java/util/ArrayList")));
			static jmethodID jArrayListCtor = jenv->GetMethodID(jArrayList, "<init>", "(I)V");
			static jmethodID jArrayListAdd = jenv->GetMethodID(jArrayList, "add", "(Ljava/lang/Object;)Z");

			JStringManager schemaName(jenv, jschemaName);
			const List<const char*>& headers = spec->getHeaderFieldNames(schemaName.c_str());

			// Create ArrayList for the number of field names we have
			jHeaders = jenv->NewObject(jArrayList, jArrayListCtor, headers.size());

			// Load headers into newly created Array
			for (List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
			{
				const char* fieldName = *it;

				// Instantiate new String object
				jstring jFieldName = makeJavaString(jenv, fieldName);

				// Add String object to the ArrayList that we will return
				jenv->CallBooleanMethod(jHeaders, jArrayListAdd, jFieldName);
			}
		}
	}
	JNI_CATCH

	return jHeaders;
}


#ifdef __cplusplus
}
#endif
