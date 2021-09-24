/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST Mnemonic class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicIterator.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/mist/message/MnemonicMessage.h>

#include <gmsec4/util/Deprecated.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage__Ljava_lang_String_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jstring jSchemaID, jlong jSpecPtr, jobject jSpec)
{
	jlong jCreated = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			JStringManager schemaID(jenv, jSchemaID);

			if (jvmOk(jenv, "MnemonicMessage"))
			{
				MnemonicMessage* mnemMsg = new MnemonicMessage(subject.c_str(), schemaID.c_str(), *spec);

				jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage__Ljava_lang_String_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jstring jSchemaID, jlong jConfigPtr, jobject jConfig, jlong jSpecPtr, jobject jSpec)
{
	jlong jCreated = 0;

	try
	{
		Config*        config = JNI_JLONG_TO_CONFIG(jConfigPtr);
		Specification* spec   = JNI_JLONG_TO_SPECIFICATION(jSpecPtr); 
		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);
			JStringManager schemaID(jenv, jSchemaID);

			if (jvmOk(jenv, "MnemonicMessage"))
			{
				MnemonicMessage* mnemMsg = new MnemonicMessage(subject.c_str(), schemaID.c_str(), *config, *spec);

				jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage__Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	jlong jCreated = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "MnemonicMessage"))
		{
			MnemonicMessage* mnemMsg = new MnemonicMessage(data.c_str());

			jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
		}
	}
	JNI_CATCH

	return jCreated;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jlong jSpecPtr, jobject jSpec, jstring jData)
{
	jlong jCreated = 0;

	try
	{
		Specification* spec = JNI_JLONG_TO_SPECIFICATION(jSpecPtr);

		if (!spec)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Specification reference is null");
		}
		else
		{
			JStringManager data(jenv, jData);

			if (jvmOk(jenv, "MnemonicMessage"))
			{
				MnemonicMessage* mnemMsg = new MnemonicMessage(*spec, data.c_str());

				jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jOtherPtr, jobject jOther)
{
	jlong jCreated = 0;

	try
	{
		MnemonicMessage* other = JNI_JLONG_TO_MNEMONIC_MSG(jOtherPtr);

		if (!other)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicMessage reference is null");
		}
		else
		{
			MnemonicMessage* mnemMsg = new MnemonicMessage(*other);

			jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicMessage_1AddMnemonic
  (JNIEnv *jenv, jclass jcls, jlong jMnemMsgPtr, jobject jMnemMsg, jlong jMnemPtr, jobject jMnem)
{
	try
	{
		MnemonicMessage* mnemMsg  = JNI_JLONG_TO_MNEMONIC_MSG(jMnemMsgPtr);
		Mnemonic*        mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemPtr);

		if (!mnemMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicMessage reference is null");
		}
		else if (!mnemonic)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Mnemonic reference is null");
		}
		else
		{
			mnemMsg->addMnemonic(*mnemonic);
		}
	}
	JNI_CATCH
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicMessage_1GetNumMnemonics
  (JNIEnv *jenv, jclass jcls, jlong jMnemMsgPtr, jobject jMnemMsg)
{
	jint jNum = 0;

	try
	{
		MnemonicMessage* mnemMsg = JNI_JLONG_TO_MNEMONIC_MSG(jMnemMsgPtr);

		if (!mnemMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicMessage reference is null");
		}
		else
		{
			jNum = (jint) mnemMsg->getNumMnemonics();
		}
	}
	JNI_CATCH

	return jNum;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicMessage_1GetMnemonic
  (JNIEnv *jenv, jclass jcls, jlong jMnemMsgPtr, jobject jMnemMsg, jint jIndex)
{
	jlong jMnemonic = 0;

	try
	{
		MnemonicMessage* mnemMsg = JNI_JLONG_TO_MNEMONIC_MSG(jMnemMsgPtr);

		if (!mnemMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicMessage reference is null");
		}
		else
		{
			const Mnemonic& mnemonic = mnemMsg->getMnemonic((size_t) jIndex);

			jMnemonic = JNI_POINTER_TO_JLONG(new Mnemonic(mnemonic));
		}
	}
	JNI_CATCH

	return jMnemonic;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicMessage_1GetMnemonicIterator
  (JNIEnv *jenv, jclass jcls, jlong jMnemMsgPtr, jobject jMnemMsg)
{
	jlong jIterator = 0;

	try
	{
		MnemonicMessage* mnemMsg = JNI_JLONG_TO_MNEMONIC_MSG(jMnemMsgPtr);

		if (!mnemMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicMessage reference is null");
		}
		else
		{
			MnemonicIterator& iter = mnemMsg->getMnemonicIterator();

			jIterator = JNI_POINTER_TO_JLONG(&iter);
		}
	}
	JNI_CATCH

	return jIterator;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicMessage_1ConvertMessage
  (JNIEnv *jenv, jclass jcls, jlong jMsgPtr, jobject jMsg)
{
	jlong jCreated = 0;

	try
	{
		Message* msg = JNI_JLONG_TO_MESSAGE(jMsgPtr);

		if (!msg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Message reference is null");
		}
		else
		{
			MnemonicMessage mnemMsg = MnemonicMessage::convertMessage(*msg);

			jCreated = JNI_POINTER_TO_JLONG(new MnemonicMessage(mnemMsg));
		}
	}
	JNI_CATCH

	return jCreated;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


#ifdef __cplusplus
}
#endif
