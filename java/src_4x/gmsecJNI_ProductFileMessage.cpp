/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST Device class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ProductFileIterator.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/mist/message/ProductFileMessage.h>

#include <gmsec4/util/Deprecated.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jResponseStatus, jint jKind, jstring jProductType, jstring jProductSubtype, jlong jSpecPtr, jobject jSpec)
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
			JStringManager productType(jenv, jProductType);
			JStringManager productSubtype(jenv, jProductSubtype);

			if (jvmOk(jenv, "ProductTypeMessage"))
			{
				ResponseStatus::Response status = static_cast<ResponseStatus::Response>((int) jResponseStatus);
				Message::MessageKind     kind   = messageKindToNative(jenv, jKind);

				ProductFileMessage* prodMsg = new ProductFileMessage(subject.c_str(), status, kind, productType.c_str(), productSubtype.c_str(), *spec);

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2IILjava_lang_String_2Ljava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jResponseStatus, jint jKind, jstring jProductType, jstring jProductSubtype, jlong jConfigPtr, jobject jConfig, jlong jSpecPtr, jobject jSpec)
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
			JStringManager productType(jenv, jProductType);
			JStringManager productSubtype(jenv, jProductSubtype);

			if (jvmOk(jenv, "ProductTypeMessage"))
			{
				ResponseStatus::Response status = static_cast<ResponseStatus::Response>((int) jResponseStatus);
				Message::MessageKind     kind   = messageKindToNative(jenv, jKind);

				ProductFileMessage* prodMsg = new ProductFileMessage(subject.c_str(), status, kind, productType.c_str(), productSubtype.c_str(), *config, *spec);

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2ILjava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jResponseStatus, jstring jSchemaID, jlong jSpecPtr, jobject jSpec)
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

			if (jvmOk(jenv, "ProductTypeMessage"))
			{
				ResponseStatus::Response status = static_cast<ResponseStatus::Response>((int) jResponseStatus);

				ProductFileMessage* prodMsg = new ProductFileMessage(subject.c_str(), status, schemaID.c_str(), *spec);

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2ILjava_lang_String_2JLgov_nasa_gsfc_gmsec_api_jni_JNIConfig_2JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2
  (JNIEnv *jenv, jclass jcls, jstring jSubject, jint jResponseStatus, jstring jSchemaID, jlong jConfigPtr, jobject jConfig, jlong jSpecPtr, jobject jSpec)
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

			if (jvmOk(jenv, "ProductTypeMessage"))
			{
				ResponseStatus::Response status = static_cast<ResponseStatus::Response>((int) jResponseStatus);

				ProductFileMessage* prodMsg = new ProductFileMessage(subject.c_str(), status, schemaID.c_str(), *config, *spec);

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jData)
{
	jlong jCreated = 0;

	try
	{
		JStringManager data(jenv, jData);

		if (jvmOk(jenv, "ProductTypeMessage"))
		{
			ProductFileMessage* prodMsg = new ProductFileMessage(data.c_str());

			jCreated = JNI_POINTER_TO_JLONG(prodMsg);
		}
	}
	JNI_CATCH

	return jCreated;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage__JLgov_nasa_gsfc_gmsec_api_jni_mist_JNISpecification_2Ljava_lang_String_2
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

			if (jvmOk(jenv, "ProductTypeMessage"))
			{
				ProductFileMessage* prodMsg = new ProductFileMessage(*spec, data.c_str());

				jCreated = JNI_POINTER_TO_JLONG(prodMsg);
			}
		}
	}
	JNI_CATCH

	return jCreated;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFileMessage_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jOtherPtr, jobject jOther)
{
	jlong jCreated = 0;

	try
	{
		ProductFileMessage* other = JNI_JLONG_TO_PRODUCT_FILE_MSG(jOtherPtr);

		if (!other)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileMessage reference is null");
		}
		else
		{
			ProductFileMessage* prodMsg = new ProductFileMessage(*other);

			jCreated = JNI_POINTER_TO_JLONG(prodMsg);
		}
	}
	JNI_CATCH

	return jCreated;
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


GMSEC_DISABLE_DEPRECATED_WARNINGS
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
GMSEC_ENABLE_DEPRECATED_WARNINGS


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

			jType = makeJavaString(jenv, type);

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

			jSubtype = makeJavaString(jenv, subtype);

			jvmOk(jenv, "ProductFileMessage::getProductSubtype");
		}
	}
	JNI_CATCH

	return jSubtype;
}


GMSEC_DISABLE_DEPRECATED_WARNINGS
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
			ProductFileMessage prodMsg = ProductFileMessage::convertMessage(*msg);

			jCreated = JNI_POINTER_TO_JLONG(new ProductFileMessage(prodMsg));
		}
	}
	JNI_CATCH

	return jCreated;
}
GMSEC_ENABLE_DEPRECATED_WARNINGS


#ifdef __cplusplus
}
#endif
