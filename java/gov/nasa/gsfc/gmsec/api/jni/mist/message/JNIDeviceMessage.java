/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.mist.Device;
import gov.nasa.gsfc.gmsec.api.mist.DeviceIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDevice;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDeviceIterator;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;


public class JNIDeviceMessage extends JNIMistMessage
{
	private JNIDeviceMessage(long cPtr, boolean own)
	{
		super(cPtr, own);
	}


	public JNIDeviceMessage(String subject, Specification spec)
	{
		this(subject, Specification.getInternal(spec));
	}


	public JNIDeviceMessage(String subject, Config config, Specification spec)
	{
		this(subject, Config.getInternal(config), Specification.getInternal(spec));
	}


	public JNIDeviceMessage(String subject, JNISpecification jSpec)
	{
		super(gmsecJNI.new_DeviceMessage(subject, JNISpecification.getCPtr(jSpec), jSpec), true);
	}


	public JNIDeviceMessage(String subject, JNIConfig jConfig, JNISpecification jSpec)
	{
		super(gmsecJNI.new_DeviceMessage(subject, JNIConfig.getCPtr(jConfig), jConfig, JNISpecification.getCPtr(jSpec), jSpec), true);
	}


	public JNIDeviceMessage(String data) throws GMSEC_Exception
	{
		super(gmsecJNI.new_DeviceMessage(data), true);
	}


	public JNIDeviceMessage(JNISpecification jSpec, String data) throws GMSEC_Exception
	{
		super(gmsecJNI.new_DeviceMessage(JNISpecification.getCPtr(jSpec), jSpec, data), true);
	}


	public JNIDeviceMessage(JNIDeviceMessage jOther) throws IllegalArgumentException
	{
		super(gmsecJNI.new_DeviceMessage_Copy(JNIDeviceMessage.getCPtr(jOther), jOther), true);
	}


	public void addDevice(JNIDevice jDevice) throws IllegalArgumentException
	{
		gmsecJNI.DeviceMessage_AddDevice(swigCPtr, this, JNIDevice.getCPtr(jDevice), jDevice);
	}


	public int getNumDevices()
	{
		return gmsecJNI.DeviceMessage_GetNumDevices(swigCPtr, this);
	}


	public Device getDevice(int index) throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.DeviceMessage_GetDevice(swigCPtr, this, index);

		return new Device(new JNIDevice(cPtr, false));
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
