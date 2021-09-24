/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





// ConnectionFactory class functions
//

#include "gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h"
#include "gmsecJNI_Cache.h"
#include "gmsecJNI_Jenv.h"

#include <gmsec/Config.h>
#include <gmsec/Connection.h>
#include <gmsec/ConnectionFactory.h>
#include <gmsec/Status.h>

using gmsec::Config;
using gmsec::ConnectionFactory;
using gmsec::Connection;
using gmsec::Status;


#ifdef __cplusplus
extern "C"
{
#endif

	/** @brief Creates a Connection. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionFactory_1Create(JNIEnv *jenv, jclass jcls, jlong jCfgPtr, jobject jConfig, jobject jConnection)
	{
		Status *created = 0;

		try
		{
			Status status;
			Config *config = JNI_JLONG_TO_CONFIG(jCfgPtr);

			if (!jConnection)
			{
				SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Connection object is null");
			}
			else
			{
				Connection *connection = NULL;
				status = ConnectionFactory::Create(config, connection);

				if (!status.IsError())
				{
					// set swigCPtr to the C++ Connection object pointer
					jenv->SetLongField(jConnection,
					                   Cache::getCache().fieldConnection_swigCPtr,
					                   JNI_POINTER_TO_JLONG(connection));
					jvmOk(jenv, "ConnectionFactory.Create");

					// set swigCMemOwn to true
					jenv->SetBooleanField(jConnection,
					                      Cache::getCache().fieldConnection_swigCMemOwn,
					                      JNI_TRUE);
					jvmOk(jenv, "ConnectionFactory.Create");

					Cache::getCache().putConnection(connection, jenv->NewGlobalRef(jConnection));
				}

				created = new Status(status);
			}
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Destroys a connection. */
	JNIEXPORT jlong JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionFactory_1Destroy(JNIEnv *jenv, jclass jcls, jlong jConnPtr, jobject jConnection)
	{
		Status *created = 0;

		try
		{
			// TODO: Revamp design of JNI layer so that a cache is associated with each connection object.
			// Currently a global cache is used, and because of such, we need to employ a hokie alternative
			// that examines each entry in the callbacks map that is associated with the given connection
			// object.
			Cache::getCache().destroyCallbacks(jenv, jConnection);

			Status status;
			Connection *connection = JNI_JLONG_TO_CONNECTION(jConnPtr);
			status = ConnectionFactory::Destroy(connection);
			created = new Status(status);
		}
		JNI_CATCH

		return JNI_POINTER_TO_JLONG(created);
	}


	/** @brief Returns the API version. */
	JNIEXPORT jstring JNICALL
	Java_gov_nasa_gsfc_gmsecapi_jni_gmsecJNI_ConnectionFactory_1GetAPIVersion(JNIEnv *jenv, jclass jcls)
	{
		jstring s = 0;
		try
		{
			s = makeJavaString(jenv, ConnectionFactory::GetAPIVersion());
			jvmOk(jenv, "ConnectionFactory.GetAPIVersion");
		}
		JNI_CATCH

		return s;
	}

#ifdef __cplusplus
}
#endif
