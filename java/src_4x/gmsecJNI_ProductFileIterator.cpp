/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Status class functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ProductFileIterator.h>

#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileIterator_1HasNext
  (JNIEnv *jenv, jclass jcls, jlong jProdFileIterPtr, jobject jProdFileIter)
{
	jboolean result = JNI_FALSE;

	try
	{
		ProductFileIterator* iter = JNI_JLONG_TO_PRODUCT_FILE_ITER(jProdFileIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileIterator reference is null");
		}
		else
		{
			result = (iter->hasNext() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileIterator_1Next
  (JNIEnv *jenv, jclass jcls, jlong jProdFileIterPtr, jobject jProdFileIter)
{
	jlong result = 0;

	try
	{
		ProductFileIterator* iter = JNI_JLONG_TO_PRODUCT_FILE_ITER(jProdFileIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileIterator reference is null");
		}
		else
		{
			try
			{
				const ProductFile& prodFile = iter->next();

				result = JNI_POINTER_TO_JLONG(&prodFile);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_ProductFileIterator_1Reset
  (JNIEnv *jenv, jclass jcls, jlong jProdFileIterPtr, jobject jProdFileIter)
{
	try
	{
		ProductFileIterator* iter = JNI_JLONG_TO_PRODUCT_FILE_ITER(jProdFileIterPtr);

		if (iter == NULL)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "ProductFileIterator reference is null");
		}
		else
		{
			iter->reset();
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
