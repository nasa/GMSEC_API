/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// JNI initialization functions
//

#include "gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"


using namespace gmsec::api::jni;


#ifdef __cplusplus
extern "C"
{
#endif

	/*
	    @brief GMSEC JNI initialization
	    gmsecJNI.initialize is to be called once.  This is done in the gmsecJNI
	    static block right after the library is loaded.
	    This function stores global references to the classes, fields and methods
	    which are used frequently by the GMSEC API.  This way we know at load time
	    whether there are any mis-matches between the Java code and the JNI code.
	    And it means that symbolic lookups do not need to be performed repeatedly.
	 */
	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_initialize(JNIEnv *jenv, jclass cls)
	{
		try
		{
			Cache::getCache().initialize(jenv);
		}
		JNI_CATCH
	}


	JNIEXPORT void JNICALL
	Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_setEnableDetach (JNIEnv *jenv, jclass jcls, jboolean enableDetach)
	{
		Cache::getCache().enableDetach = enableDetach;
	}

#ifdef __cplusplus
}
#endif
