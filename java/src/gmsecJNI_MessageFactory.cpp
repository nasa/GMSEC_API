/*
 * Copyright 2007-2024 United States Government as represented by the
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
	jlong jFactory = JNI_POINTER_TO_JLONG( new MessageFactory() );

	return jFactory;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1CreateUsingConfig
  (JNIEnv *jenv, jclass jcls, jlong jConfigPtr, jobject jConfig)
{
	Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

	jlong jFactory = 0;

	try
	{
		jFactory = JNI_POINTER_TO_JLONG( new MessageFactory(*config) );
	}
	catch (const GmsecException& e)
	{
		ThrowGmsecException(jenv, e.what());
	}

	return jFactory;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1Delete
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

	delete factory;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1SetStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jlongArray jFieldPtrs, jobjectArray jFields, jint jNumFields)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

	size_t numFields = static_cast<size_t>(jNumFields);

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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1ClearStandardFields
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

	factory->clearStandardFields();
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1SetMessageConfig
  (JNIEnv *jenv, jclass jcsl, jlong jMsgFactoryPtr, jobject jMsgFactory, jlong jConfigPtr, jobject jConfig)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);
	Config*         config  = JNI_JLONG_TO_CONFIG(jConfigPtr);

	factory->setMessageConfig(*config);
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1CreateMessage__JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessageFactory_2
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

	jlong jMessage = JNI_POINTER_TO_JLONG( new Message(factory->createMessage()) );

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1CreateMessage__JLgov_nasa_gsfc_gmsec_api5_jni_JNIMessageFactory_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jstring jSchemaID)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

	jlong jMessage = 0;

	JStringManager schemaID(jenv, jSchemaID);

	if (jvmOk(jenv, "MessageFactory.createMessage"))
	{
		try
		{
			jMessage = JNI_POINTER_TO_JLONG( new Message(factory->createMessage(schemaID.c_str())) );
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1FromData
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jstring jData, jint jType)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

	jlong jMessage = 0;

	JStringManager data(jenv, jData);

	if (jvmOk(jenv, "MessageFactory.fromData"))
	{
		try
		{
			jMessage = JNI_POINTER_TO_JLONG( new Message(factory->fromData(data.c_str(), static_cast<DataType>(jType))) );
		}
		catch (const GmsecException& e)
		{
			ThrowGmsecException(jenv, e.what());
		}
	}

	return jMessage;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1GetSpecification
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory)
{
	MessageFactory* factory = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);

	const Specification& spec = factory->getSpecification();

	jlong jSpec = JNI_POINTER_TO_JLONG( &spec );

	return jSpec;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI_MessageFactory_1RegisterMessageValidator
  (JNIEnv *jenv, jclass jcls, jlong jMsgFactoryPtr, jobject jMsgFactory, jlong jValPtr)
{
	MessageFactory*   factory   = JNI_JLONG_TO_MESSAGE_FACTORY(jMsgFactoryPtr);
	MessageValidator* validator = JNI_JLONG_TO_MESSAGE_VALIDATOR(jValPtr);

	factory->registerMessageValidator(validator);
}

#ifdef __cplusplus
}
#endif
