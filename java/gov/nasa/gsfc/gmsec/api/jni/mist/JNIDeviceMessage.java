/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.mist.Device;
import gov.nasa.gsfc.gmsec.api.mist.DeviceIterator;
import gov.nasa.gsfc.gmsec.api.mist.DeviceMessage;


public class JNIDeviceMessage extends JNIMessage
{
	protected JNIDeviceMessage(long cPtr, boolean cMemoryOwn)
	{
		super(cPtr, cMemoryOwn);
	}


	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_DeviceMessage(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIDeviceMessage obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIDeviceMessage(String subject, int version)
	{
		this(gmsecJNI.new_DeviceMessage(subject, version), true);
	}


	public JNIDeviceMessage(String subject, JNIConfig jConfig, int version)
	{
		this(gmsecJNI.new_DeviceMessage(subject, JNIConfig.getCPtr(jConfig), jConfig, version), true);
	}


	public JNIDeviceMessage(String data)
	{
		this(gmsecJNI.new_DeviceMessage(data), true);
	}


	public JNIDeviceMessage(JNIDeviceMessage other)
	{
		this(gmsecJNI.new_DeviceMessage_Copy(JNIDeviceMessage.getCPtr(other), other), true);
	}


	public void addDevice(JNIDevice jDevice)
	{
		gmsecJNI.DeviceMessage_AddDevice(swigCPtr, this, JNIDevice.getCPtr(jDevice), jDevice);
	}


	public int getNumDevices()
	{
		return gmsecJNI.DeviceMessage_GetNumDevices(swigCPtr, this);
	}


	public Device getDevice(int index)
	{
		long cPtr = gmsecJNI.DeviceMessage_GetDevice(swigCPtr, this, index);

		return new Device(new JNIDevice(cPtr, true));
	}


	public DeviceIterator getDeviceIterator()
	{
		long cPtr = gmsecJNI.DeviceMessage_GetDeviceIterator(swigCPtr, this);

		return new DeviceIterator(new JNIDeviceIterator(cPtr, false));
	}


	public static DeviceMessage convertMessage(JNIMessage jMsg)
	{
		long cPtr = gmsecJNI.DeviceMessage_ConvertMessage(JNIMessage.getCPtr(jMsg), jMsg);

		return new DeviceMessage(new JNIDeviceMessage(cPtr, true));
	}
}
