/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST Device class functions
//

#include "gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Exception.h>
#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ProductFileIterator.h>
#include <gmsec4/mist/ProductFileMessage.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2ILjava_lang_String_2Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jStatus, jstring jProductType, jstring jProductSubtype, jint jVersion)
{
	jlong jCreated = 0;

	try
	{
		JStringManager subject(jenv, jSubject);
		JStringManager productType(jenv, jProductType);
		JStringManager productSubtype(jenv, jProductSubtype);

		if (jvmOk(jenv, "ProductTypeMessage"))
		{
			try
			{
				ResponseStatus::Response status = static_cast<ResponseStatus::Response>((int) jStatus);

				ProductFileMessage* prodMsg = new ProductFileMessage(subject.c_str(), status, productType.c_str(), productSubtype.c_str(), (unsigned int) jVersion);

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2ILjava_lang_String_2Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jlong jConfigPtr, jobject jConfig, jint jStatus, jstring jProductType, jstring jProductSubtype, jint jVersion)
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
			JStringManager productType(jenv, jProductType);
			JStringManager productSubtype(jenv, jProductSubtype);

			if (jvmOk(jenv, "ProductTypeMessage"))
			{
				try
				{
					ResponseStatus::Response status = static_cast<ResponseStatus::Response>((int) jStatus);

					ProductFileMessage* prodMsg = new ProductFileMessage(subject.c_str(), *config, status, productType.c_str(), productSubtype.c_str(), (unsigned int) jVersion);

					jCreated = JNI_POINTER_TO_JLONG(prodMsg);
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	jlong jCreated = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "ProductTypeMessage"))
		{
			try
			{
				ProductFileMessage* prodMsg = new ProductFileMessage(data.c_str());

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
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


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg)
{
	jlong jCreated = 0;

	try
	{
		ProductFileMessage* other = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!other)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			try
			{
				ProductFileMessage* prodMsg = new ProductFileMessage(*other);

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
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


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ProductFileMessage
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg)
{
	try
	{
		ProductFileMessage* prodMsg = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			delete prodMsg;
		}
	}
	JNI_CATCH
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1AddProductFile
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg, jlong jProdFilePtr, jobject jProdFile)
{
	try
	{
		ProductFileMessage* prodMsg  = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);
		ProductFile*        prodFile = JNI_JLONG_TO_PRODUCT_FILE(jProdFilePtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else if (!prodFile)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			prodMsg->addProductFile(*prodFile);
		}
	}
	JNI_CATCH
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1GetNumProductFiles
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg)
{
	jint jNum = 0;

	try
	{
		ProductFileMessage* prodMsg = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			jNum = (jint) prodMsg->getNumProductFiles();
		}
	}
	JNI_CATCH

	return jNum;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1GetProductFile
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg, jint jIndex)
{
	jlong jProdFile = 0;

	try
	{
		ProductFileMessage* prodMsg = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			try
			{
				const ProductFile& prodFile = prodMsg->getProductFile((size_t) jIndex);

				jProdFile = JNI_POINTER_TO_JLONG(new ProductFile(prodFile));
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jProdFile;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1GetProductFileIterator
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg)
{
	jlong jIterator = 0;

	try
	{
		ProductFileMessage* prodMsg = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			ProductFileIterator& iter = prodMsg->getProductFileIterator();

			jIterator = JNI_POINTER_TO_JLONG(&iter);
		}
	}
	JNI_CATCH

	return jIterator;
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1GetResponseStatus
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg)
{
	jint jStatus = 0;

	try
	{
		ProductFileMessage* prodMsg = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			jStatus = (jint) prodMsg->getResponseStatus();
		}
	}
	JNI_CATCH

	return jStatus;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1GetProductType
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg)
{
	jstring jType = 0;

	try
	{
		ProductFileMessage* prodMsg = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			const char* type = prodMsg->getProductType();

			jType = jenv->NewStringUTF(type);

			jvmOk(jenv, "ProductFileMessage::getProductType");
		}
	}
	JNI_CATCH

	return jType;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1GetProductSubtype
  (JNIEnv *jenv, jclass jcls, jlong jProdFileMsgPtr, jobject jProdFileMsg)
{
	jstring jSubtype = 0;

	try
	{
		ProductFileMessage* prodMsg = JNI_JLONG_TO_PRODUCT_FILE_MSG(jProdFileMsgPtr);

		if (!prodMsg)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			const char* subtype = prodMsg->getProductSubtype();

			jSubtype = jenv->NewStringUTF(subtype);

			jvmOk(jenv, "ProductFileMessage::getProductSubType");
		}
	}
	JNI_CATCH

	return jSubtype;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileMessage_1ConvertMessage
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
				ProductFileMessage prodMsg = ProductFileMessage::convertMessage(*msg);

				jCreated = JNI_POINTER_TO_JLONG(new ProductFileMessage(prodMsg));
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
