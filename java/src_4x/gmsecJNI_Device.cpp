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

#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>
#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/DeviceParam.h>

#include <gmsec4/util/DataList.h>


using namespace gmsec::api;
using namespace gmsec::api::jni;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Device__Ljava_lang_String_2I
  (JNIEnv *jenv, jclass jcls, jstring jName, jint jStatus)
{
	jlong created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "Device"))
		{
			created = JNI_POINTER_TO_JLONG(new Device(name.c_str(), static_cast<Device::DeviceStatus>(jStatus)));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Device__Ljava_lang_String_2I_3J_3Lgov_nasa_gsfc_gmsec_api_jni_mist_JNIDeviceParam_2I
  (JNIEnv *jenv, jclass jcls, jstring jName, jint jStatus, jlongArray jDeviceParamPtrs, jobjectArray jDeviceParams, jint jNumDeviceParams)
{
	jlong created = 0;

	try
	{
		JStringManager name(jenv, jName);

		if (jvmOk(jenv, "Device"))
		{
			DataList<DeviceParam*> params;

			if ((int) jNumDeviceParams > 0)
			{
				jlong* deviceParamPtrs = jenv->GetLongArrayElements(jDeviceParamPtrs, JNI_FALSE);

				for (int i = 0; i < (int) jNumDeviceParams; ++i)
				{
					DeviceParam* param = JNI_JLONG_TO_DEVICE_PARAM(deviceParamPtrs[i]);

					if (param)
					{
						params.add(param);
					}
				}
			}

			created = JNI_POINTER_TO_JLONG(new Device(name.c_str(), static_cast<Device::DeviceStatus>(jStatus), params));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_new_1Device_1Copy
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jlong created = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			created = JNI_POINTER_TO_JLONG(new Device(*device));
		}
	}
	JNI_CATCH

	return created;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_delete_1Device
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			delete device;
		}
	}
	JNI_CATCH
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetName
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jstring jName = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			const char* name = device->getName();

			jName = makeJavaString(jenv, name);

			jvmOk(jenv, "Device.getName");
		}
	}
	JNI_CATCH

	return jName;
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1NumberAvailable
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jboolean result = JNI_FALSE;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			result = (device->numberAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetNumber
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jlong jFieldPtr = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const Field& field = device->getNumber();

				jFieldPtr = JNI_POINTER_TO_JLONG(&field);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jFieldPtr;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetNumber
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jlong jFieldPtr, jobject jField)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);
		Field*  field  = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			device->setNumber(*field);
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1ModelAvailable
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jboolean result = JNI_FALSE;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			result = (device->modelAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetModel
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jstring jModel = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const char* model = device->getModel();

				jModel = makeJavaString(jenv, model);

				jvmOk(jenv, "Device.getModel");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jModel;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetModel
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jstring jModel)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			JStringManager model(jenv, jModel);

			if (jvmOk(jenv, "Device.setModel"))
			{
				device->setModel(model.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SerialAvailable
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jboolean result = JNI_FALSE;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			result = (device->serialAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetSerial
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jstring jSerial = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const char* serial = device->getSerial();

				jSerial = makeJavaString(jenv, serial);

				jvmOk(jenv, "Device.getSerial");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jSerial;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetSerial
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jstring jSerial)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			JStringManager serial(jenv, jSerial);

			if (jvmOk(jenv, "Device.setSerial"))
			{
				device->setSerial(serial.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1VersionAvailable
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jboolean result = JNI_FALSE;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			result = (device->versionAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetVersion
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jstring jVersion = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const char* version = device->getVersion();

				jVersion = makeJavaString(jenv, version);

				jvmOk(jenv, "Device.getVersion");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jVersion;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetVersion
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jstring jVersion)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			JStringManager version(jenv, jVersion);

			if (jvmOk(jenv, "Device.setVersion"))
			{
				device->setVersion(version.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GroupAvailable
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jboolean result = JNI_FALSE;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			result = (device->groupAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetGroup
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jstring jGroup = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const char* group = device->getGroup();

				jGroup = makeJavaString(jenv, group);

				jvmOk(jenv, "Device.getGroup");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jGroup;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetGroup
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jstring jGroup)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			JStringManager group(jenv, jGroup);

			if (jvmOk(jenv, "Device.setGroup"))
			{
				device->setGroup(group.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1RoleAvailable
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jboolean result = JNI_FALSE;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			result = (device->roleAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jstring JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetRole
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jstring jRole = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const char* role = device->getRole();

				jRole = makeJavaString(jenv, role);

				jvmOk(jenv, "Device.getRole");
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jRole;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetRole
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jstring jRole)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			JStringManager role(jenv, jRole);

			if (jvmOk(jenv, "Device.setRole"))
			{
				device->setRole(role.c_str());
			}
		}
	}
	JNI_CATCH
}


JNIEXPORT jint JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetStatus
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jint jStatus = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			jStatus = (jint) device->getStatus();
		}
	}
	JNI_CATCH

	return jStatus;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetStatus
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jint jStatus)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			device->setStatus(static_cast<Device::DeviceStatus>(jStatus));
		}
	}
	JNI_CATCH
}


JNIEXPORT jboolean JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1InfoAvailable
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jboolean result = JNI_FALSE;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			result = (device->infoAvailable() ? JNI_TRUE : JNI_FALSE);
		}
	}
	JNI_CATCH

	return result;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetInfo
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jlong jFieldPtr = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const Field& field = device->getInfo();

				jFieldPtr = JNI_POINTER_TO_JLONG(&field);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jFieldPtr;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1SetInfo
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jlong jFieldPtr, jobject jField)
{
	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);
		Field*  field  = JNI_JLONG_TO_FIELD(jFieldPtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else if (!field)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Field reference is null");
		}
		else
		{
			device->setInfo(*field);
		}
	}
	JNI_CATCH
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetParamCount
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice)
{
	jlong jParamCount = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			jParamCount = (jlong) device->getParamCount();
		}
	}
	JNI_CATCH

	return jParamCount;
}


JNIEXPORT jlong JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1GetParam
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jlong jIndex)
{
	jlong jDeviceParamPtr = 0;

	try
	{
		Device* device = JNI_JLONG_TO_DEVICE(jDevicePtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else
		{
			try
			{
				const DeviceParam& param = device->getParam((size_t) jIndex);

				jDeviceParamPtr = JNI_POINTER_TO_JLONG(&param);
			}
			catch (Exception& e)
			{
				ThrowGmsecException(jenv, e.what());
			}
		}
	}
	JNI_CATCH

	return jDeviceParamPtr;
}


JNIEXPORT void JNICALL Java_gov_nasa_gsfc_gmsec_api_jni_gmsecJNI_Device_1AddParam
  (JNIEnv *jenv, jclass jcls, jlong jDevicePtr, jobject jDevice, jlong jDeviceParamPtr, jobject jDeviceParam)
{
	try
	{
		Device*      device = JNI_JLONG_TO_DEVICE(jDevicePtr);
		DeviceParam* param  = JNI_JLONG_TO_DEVICE_PARAM(jDeviceParamPtr);

		if (!device)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Device reference is null");
		}
		else if (!param)
		{
			SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "DeviceParam reference is null");
		}
		else
		{
			device->addParam(*param);
		}
	}
	JNI_CATCH
}


#ifdef __cplusplus
}
#endif
