/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// MIST ProductFile class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/Exception.h>

#include <gmsec4_defs.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif



JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFile__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jDescription, jstring jVersion, jstring jFormat, jstring jURI)
{
	jlong created = 0;

	try
	{
		JStringManager name(jenv, jName);
		JStringManager description(jenv, jDescription);
		JStringManager version(jenv, jVersion);
		JStringManager format(jenv, jFormat);
		JStringManager uri(jenv, jURI);

		if (jvmOk(jenv, "ProductFile"))
		{
			try
			{
				created = JNI_POINTER_TO_JLONG(new ProductFile(name.c_str(), description.c_str(), version.c_str(), format.c_str(), uri.c_str()));
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFile__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2_3BJ
  (JNIEnv *jenv, jclass jcls, jstring jName, jstring jDescription, jstring jVersion, jstring jFormat, jbyteArray jData, jlong jDataLength)
{
	jlong created = 0;

	try
	{
		JStringManager    name(jenv, jName);
		JStringManager    description(jenv, jDescription);
		JStringManager    version(jenv, jVersion);
		JStringManager    format(jenv, jFormat);
		JByteArrayManager data(jenv, jData);

		if (jvmOk(jenv, "ProductFile"))
		{
			try
			{
				created = JNI_POINTER_TO_JLONG(new ProductFile(name.c_str(), description.c_str(), version.c_str(), format.c_str(), (GMSEC_BIN) data.jbytes(), (size_t) jDataLength));
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1ProductFile_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jlong created = 0;

	try
	{
		ProductFile* other = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!other)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new ProductFile(*other));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1ProductFile
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			delete pf;
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jstring jName = 0;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			const char* name = pf->getName();

			jName = makeJavaString(jenv, name);

			jvmOk(jenv, "ProductFile.getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1GetDescription
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jstring jDesc = 0;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			const char* desc = pf->getDescription();

			jDesc = makeJavaString(jenv, desc);

			jvmOk(jenv, "ProductFile.getDescription");
		}
	}
	JNI_CATCH

	return jDesc;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1GetVersion
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jstring jVersion = 0;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			const char* version = pf->getVersion();

			jVersion = makeJavaString(jenv, version);

			jvmOk(jenv, "ProductFile.getDescription");
		}
	}
	JNI_CATCH

	return jVersion;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1GetFormat
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jstring jFormat = 0;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			const char* format = pf->getFormat();

			jFormat = makeJavaString(jenv, format);

			jvmOk(jenv, "ProductFile.getFormat");
		}
	}
	JNI_CATCH

	return jFormat;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1IsURIAvailable
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			jResult = (pf->uriAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1GetURI
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jstring jURI = 0;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			const char* uri = pf->getURI();

			if (uri)
			{
				jURI = makeJavaString(jenv, uri);

				jvmOk(jenv, "ProductFile.getURI");
			}
		}
	}
	JNI_CATCH

	return jURI;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1ContentsAvailable
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jboolean jResult = JNI_FALSE;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			jResult = (pf->contentsAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return jResult;
}


JNIEXPORT jbyteArray JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFile_1GetContents
  (JNIEnv *jenv, jclass jcls, jlong jProductFilePtr, jobject jProductFile)
{
	jbyteArray jContents = NULL;

	try
	{
		ProductFile* pf = JNI_JLONG_TO_PRODUCT_FILE(jProductFilePtr);

		if (!pf)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFile reference is null");
		}
		else
		{
			try
			{
				GMSEC_BIN contents;
				size_t    length = pf->getContents(contents);

				jContents = jenv->NewByteArray((jsize) length);

				jenv->SetByteArrayRegion(jContents, 0, (jsize) length, (jbyte*) contents);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jContents;
}


#ifdef __cplusplus
}
#endif
