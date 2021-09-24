/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.mist.*;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIFieldConverter;


public class JNIDeviceParam
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIDeviceParam(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_DeviceParam(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIDeviceParam obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIDeviceParam(String name, String timestamp, Field field)
	{
		this(gmsecJNI.new_DeviceParam(name, timestamp, JNIField.getCPtr(Field.getInternal(field)), Field.getInternal(field)), true);
	}


	public JNIDeviceParam(JNIDeviceParam other)
	{
		this(gmsecJNI.new_DeviceParam_Copy(JNIDeviceParam.getCPtr(other), other), true);
	}


	public String getName()
	{
		return gmsecJNI.DeviceParam_GetName(swigCPtr, this);
	}


	public String getTimestamp()
	{
		return gmsecJNI.DeviceParam_GetTimestamp(swigCPtr, this);
	}


	public Field getValue()
	{
		long cPtr = gmsecJNI.DeviceParam_GetValue(swigCPtr, this);

		return JNIFieldConverter.cloneField(cPtr);
	}
}
