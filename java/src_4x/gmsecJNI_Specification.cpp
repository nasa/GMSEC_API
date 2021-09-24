/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// SchemaIDIterator class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"
#include "gmsecJNI_CustomSpecification.h"

#include <gmsec4/mist/FieldSpecification.h>
#include <gmsec4/mist/MessageSpecification.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Exception.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/Deprecated.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Specification
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
			spec = JNI_POINTER_TO_JLONG(new CustomSpecification(*config));
		}
	}
	JNI_CATCH

	return spec;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Specification_1Copy
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
			spec = JNI_POINTER_TO_JLONG(new CustomSpecification(*other));
		}
	}
	JNI_CATCH

	return spec;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Specification
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1ValidateMessage
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jlong jMsgPtr, jobject jMsg)
{
	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);
		Message*       msg  = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else if (msg == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			// We need to be explicit in calling the base-class validateMessage()
			// for cases where a user may have registered their own custom Specification
			// object using ConnectionManager.setSpecification().
			//
			// In such cases, the user's custom Specification's validateMessage() will be
			// called automatically when message validation needs to take place, and should
			// that method in turn attempt to call the base-class' version of the method,
			// we need to be explicit with the call below so as to avoid a severe case of
			// recursion.
			//
			spec->Specification::validateMessage(*msg);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetSchemaIDIterator
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


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetVersion
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


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetSchemaLevel
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


JNIEXPORT jobject JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetMessageSpecifications
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

			static jclass    jMsgSpec       = static_cast<jclass>(jenv->NewGlobalRef(jenv->FindClass("gov/nasa/gsfc/gmsec/api/mist/MessageSpecification")));
			static jmethodID jMsgSpecCtor   = jenv->GetMethodID(jMsgSpec, "<init>", "(Ljava/lang/String;Ljava/util/ArrayList;)V");

			static jclass    jFldSpec       = static_cast<jclass>(jenv->NewGlobalRef(jenv->FindClass("gov/nasa/gsfc/gmsec/api/mist/FieldSpecification")));
			static jmethodID jFldSpecCtor   = jenv->GetMethodID(jFldSpec, "<init>",
			                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

			const DataList<MessageSpecification*>& msgSpecs = spec->getMessageSpecifications();

			// Create ArrayList for the number of MessageSpecification objects we have
			jMsgSpecs = jenv->NewObject(jArrayList, jArrayListCtor, msgSpecs.size());

			for (DataList<MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
			{
				const MessageSpecification* msgSpec = *it;

				const DataList<FieldSpecification*>& fldSpecs = msgSpec->getFieldSpecifications();

				// Create ArrayList for the number of FieldSpecification objects we have
				jobject jFldSpecs = jenv->NewObject(jArrayList, jArrayListCtor, fldSpecs.size());

				for (DataList<FieldSpecification*>::const_iterator it2 = fldSpecs.begin(); it2 != fldSpecs.end(); ++it2)
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

				jstring schemaID = makeJavaString(jenv, msgSpec->getSchemaID());

				// Instantiate new MessageSpecification object
				jobject jNewMsgSpec = jenv->NewObject(jMsgSpec, jMsgSpecCtor, schemaID, jFldSpecs);

				// Add MessageSpecification object to the ArrayList that we will return
				jenv->CallBooleanMethod(jMsgSpecs, jArrayListAdd, jNewMsgSpec);
			}
		}
	}
	JNI_CATCH

	return jMsgSpecs;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1RegisterMessageValidator
  (JNIEnv *jenv, jclass jclass, jlong jSpecPtr, jobject jSpec, jlong jValPtr)
{
	try
	{
		Specification*    spec      = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);
        MessageValidator* validator = reinterpret_cast<MessageValidator*>(jValPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else if (!validator)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageValidator reference is null");
		}
		else
		{
			spec->registerMessageValidator(validator);
		}
	}
	JNI_CATCH
}



GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Specification_1GetTemplateXML
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jstring jSubject, jstring jSchemaID)
{
	jstring xml = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (spec == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			JStringManager schemaID(jenv, jSchemaID);

			const char* tmp = spec->getTemplateXML(subject.c_str(), schemaID.c_str());

			xml = makeJavaString(jenv, tmp);

			jvmOk(jenv, "Specification.getTemplateXML");
		}
	}
	JNI_CATCH

	return xml;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


#ifdef __cplusplus
}
#endif
