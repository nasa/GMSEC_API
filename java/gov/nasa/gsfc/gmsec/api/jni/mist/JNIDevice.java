/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.Device;
import gov.nasa.gsfc.gmsec.api.mist.DeviceParam;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIFieldConverter;



public class JNIDevice
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	public JNIDevice(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	protected void finalize() throws Throwable
	{
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Device(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIDevice obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIDevice(String name, Device.DeviceStatus status)
	{
		this(gmsecJNI.new_Device(name, status.ordinal()), true);
	}


	public JNIDevice(String name, Device.DeviceStatus status, java.util.List<DeviceParam> params)
	{
		this(gmsecJNI.new_Device(name, status.ordinal(),
				ArrayListConverter.listToDeviceParamPtrs(params),
				ArrayListConverter.listToJNIDeviceParams(params),
				(params == null ? 0 : params.size())), true);
	}


	public JNIDevice(JNIDevice other)
	{
		this(gmsecJNI.new_Device_Copy(JNIDevice.getCPtr(other), other), true);
	}


	public String getName()
	{
		return gmsecJNI.Device_GetName(swigCPtr, this);
	}


	public boolean numberAvailable()
	{
		return gmsecJNI.Device_NumberAvailable(swigCPtr, this);
	}


	public Field getNumber()
	{
		long cPtr = gmsecJNI.Device_GetNumber(swigCPtr, this);

		return JNIFieldConverter.cloneField(cPtr);
	}


	public void setNumber(Field number)
	{
		JNIField jField = Field.getInternal(number);

		gmsecJNI.Device_SetNumber(swigCPtr, this, JNIField.getCPtr(jField), jField);
	}


	public boolean modelAvailable()
	{
		return gmsecJNI.Device_ModelAvailable(swigCPtr, this);
	}


	public String getModel()
	{
		return gmsecJNI.Device_GetModel(swigCPtr, this);
	}


	public void setModel(String model)
	{
		gmsecJNI.Device_SetModel(swigCPtr, this, model);
	}


	public boolean serialAvailable()
	{
		return gmsecJNI.Device_SerialAvailable(swigCPtr, this);
	}


	public String getSerial()
	{
		return gmsecJNI.Device_GetSerial(swigCPtr, this);
	}


	public void setSerial(String serial)
	{
		gmsecJNI.Device_SetSerial(swigCPtr, this, serial);
	}


	public boolean versionAvailable()
	{
		return gmsecJNI.Device_VersionAvailable(swigCPtr, this);
	}


	public String getVersion()
	{
		return gmsecJNI.Device_GetVersion(swigCPtr, this);
	}


	public void setVersion(String version)
	{
		gmsecJNI.Device_SetVersion(swigCPtr, this, version);
	}


	public boolean groupAvailable()
	{
		return gmsecJNI.Device_GroupAvailable(swigCPtr, this);
	}


	public String getGroup()
	{
		return gmsecJNI.Device_GetGroup(swigCPtr, this);
	}


	public void setGroup(String group)
	{
		gmsecJNI.Device_SetGroup(swigCPtr, this, group);
	}


	public boolean roleAvailable()
	{
		return gmsecJNI.Device_RoleAvailable(swigCPtr, this);
	}


	public String getRole()
	{
		return gmsecJNI.Device_GetRole(swigCPtr, this);
	}


	public void setRole(String role)
	{
		gmsecJNI.Device_SetRole(swigCPtr, this, role);
	}


	public Device.DeviceStatus getStatus()
	{
		int status = gmsecJNI.Device_GetStatus(swigCPtr, this);

		return Device.DeviceStatus.values()[status];
	}


	public void setStatus(Device.DeviceStatus status)
	{
		gmsecJNI.Device_SetStatus(swigCPtr, this, status.ordinal());
	}


	public boolean infoAvailable()
	{
		return gmsecJNI.Device_InfoAvailable(swigCPtr, this);
	}


	public Field getInfo()
	{
		long cPtr = gmsecJNI.Device_GetInfo(swigCPtr, this);

		return JNIFieldConverter.cloneField(cPtr);
	}


	public void setInfo(Field info)
	{
		JNIField jField = Field.getInternal(info);

		gmsecJNI.Device_SetInfo(swigCPtr, this, JNIField.getCPtr(jField), jField);
	}


	public long getParamCount()
	{
		return gmsecJNI.Device_GetParamCount(swigCPtr, this);
	}


	public DeviceParam getParam(long index)
	{
		long cPtr = gmsecJNI.Device_GetParam(swigCPtr, this, index);

		return new DeviceParam(new JNIDeviceParam(cPtr, false));
	}


	public void addParam(DeviceParam param)
	{
		JNIDeviceParam jParam = DeviceParam.getInternal(param);

		gmsecJNI.Device_AddParam(swigCPtr, this, JNIDeviceParam.getCPtr(jParam), jParam);
	}
}
