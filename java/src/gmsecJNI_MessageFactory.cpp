/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MessageFactory class functions
//

#include "gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageFactory.h>

#include <gmsec5/util/List.h>


using namespace gmsec::api5;
using namespace gmsec::api5::jni;
using namespace gmsec::api5::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1Create
  (JNIEnv *jenv, jclass jcls)
{
	jlong jFactory = 0;

	try
	{
		jFactory = JNI_POINTER_TO_JLONG( new MessageFactory() );
	}
	JNI_CATCH

	return jFactory;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1CreateUsingConfig
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	jlong jFactory = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (config == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			jFactory = JNI_POINTER_TO_JLONG( new MessageFactory(*config) );
		}
	}
	JNI_CATCH

	return jFactory;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1Delete
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			delete factory;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1SetStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			size_t numFields = (size_t) jNumFields;

			if (numFields > 0)
			{
				List<Field*> fields;

				jlong* fldptrs = jenv->GetLongArrayElements(jFieldPtrs, JNI_FALSE);

				for (size_t i = 0; i < numFields; ++i)
				{
					fields.push_back(JNI_JLONG_TO_FIELD(fldptrs[i]));
				}

				factory->setStandardFields(fields);

				jenv->ReleaseLongArrayElements(jFieldPtrs, fldptrs, 0);
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1ClearStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			factory->clearStandardFields();
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1SetMessageConfig
  (JNIEnv *jenv, jclass jcsl, jlong jMsgFactoryPtr, jobject jMsgFactory, jlong jConfigPtr, jobject jConfig)
{
	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);
		Config*         config  = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else if (config == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			factory->setMessageConfig(*config);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1CreateMessage__JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessageFactory_2
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	jlong jMessage = 0;

	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			jMessage = JNI_POINTER_TO_JLONG( new Message(factory->createMessage()) );
		}
	}
	JNI_CATCH

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1CreateMessage__JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessageFactory_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jstring jSchemaID)
{
	jlong jMessage = 0;

	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			JStringManager schemaID(jenv, jSchemaID);

			if (jvmOk(jenv, "MessageFactory.createMessage"))
			{
				jMessage = JNI_POINTER_TO_JLONG( new Message(factory->createMessage(schemaID.c_str())) );
			}
		}
	}
	JNI_CATCH

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1FromData
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jstring jData, jint jType)
{
	jlong jMessage = 0;

	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			JStringManager data(jenv, jData);

			if (jvmOk(jenv, "MessageFactory.fromData"))
			{
				jMessage = JNI_POINTER_TO_JLONG( new Message(factory->fromData(data.c_str(), static_cast<DataType>(jType))) );
			}
		}
	}
	JNI_CATCH

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1GetSpecification
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	jlong jSpec = 0;

	try
	{
		MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else
		{
			const Specification& spec = factory->getSpecification();

			jSpec = JNI_POINTER_TO_JLONG( &spec );
		}
	}
	JNI_CATCH

	return jSpec;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1RegisterMessageValidator
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jlong jValPtr)
{
	try
	{
		MessageFactory*   factory   = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);
		MessageValidator* validator = JNI_JLONG_TO_MESSAGE_VALIDATOR(jValPtr);

		if (factory == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageFactory reference is null");
		}
		else if (validator == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MessageValidator reference is null");
		}
		else
		{
			factory->registerMessageValidator(validator);
		}
	}
	JNI_CATCH
}

#ifdef __cplusplus
}
#endif
