/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST Mnemonic class functions
//

#include "gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Exception.h>
#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicIterator.h>
#include <gmsec4/mist/MnemonicMessage.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage__Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jVersion)
{
	jlong jCreated = 0;

	try
	{
		JStringManager subject(jenv, jSubject);

		if (jvmOk(jenv, "MnemonicMessage"))
		{
			try
			{
				MnemonicMessage* mnemMsg = new MnemonicMessage(subject.c_str(), (unsigned int) jVersion);

				jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage__Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2I
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jlong jConfigPtr, jobject jConfig, jint jVersion)
{
	jlong jCreated = 0;

	try
	{
		Config* config = JNI_JLONG_TO_CONFIG(jConfigPtr);

		if (!config)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Config reference is null");
		}
		else
		{
			JStringManager subject(jenv, jSubject);

			if (jvmOk(jenv, "MnemonicMessage"))
			{
				try
				{
					MnemonicMessage* mnemMsg = new MnemonicMessage(subject.c_str(), *config, (unsigned int) jVersion);

					jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
				}
				catch (Exception& e)
				{
					ThrowGmsecException(jenv, e.what());
				}
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage__Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	jlong jCreated = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "MnemonicMessage"))
		{
			try
			{
				MnemonicMessage* mnemMsg = new MnemonicMessage(data.c_str());

				jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1MnemonicMessage_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jMnemMsgPtr, jobject jMnemMsg)
{
	jlong jCreated = 0;

	try
	{
		MnemonicMessage* other = JNI_JLONG_TO_MNEMONIC_MSG(jMnemMsgPtr);

		if (!other)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicMessage reference is null");
		}
		else
		{
			try
			{
				MnemonicMessage* mnemMsg = new MnemonicMessage(*other);

				jCreated = JNI_POINTER_TO_JLONG(mnemMsg);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1MnemonicMessage
  (JNIEnv *jenv, jclass jcls, jlong jMnemMsgPtr, jobject jMnemMsg)
{
	try
	{
		MnemonicMessage* mnemMsg = JNI_JLONG_TO_MNEMONIC_MSG(jMnemMsgPtr);

		if (!mnemMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "MnemonicMessage reference is null");
		}
		else
		{
			delete mnemMsg;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_MnemonicMessage_1AddMnemonic
  (JNIEnv *jenv, jclass jcls, jlong jMnemMsgPtr, jobject jMnemMsg, jlong jMnemonicPtr, jobject jMnemonic)
{
	try
	{
		MnemonicMessage* mnemMsg  = JNI_JLONG_TO_MNEMONIC_MSG(jMnemMsgPtr);
		Mnemonic*        mnemonic = JNI_JLONG_TO_MNEMONIC(jMnemonicPtr);

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
			try
			{
				const Mnemonic& mnemonic = mnemMsg->getMnemonic((size_t) jIndex);

				jMnemonic = JNI_POINTER_TO_JLONG(new Mnemonic(mnemonic));
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jMnemonic;
}


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
			try
			{
				MnemonicMessage mnemMsg = MnemonicMessage::convertMessage(*msg);

				jCreated = JNI_POINTER_TO_JLONG(new MnemonicMessage(mnemMsg));
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


#ifdef __cplusplus
}
#endif
