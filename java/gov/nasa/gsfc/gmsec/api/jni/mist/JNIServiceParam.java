/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIFieldConverter;


public class JNIServiceParam
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIServiceParam(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_ServiceParam(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIServiceParam obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIServiceParam(String name, Field field)
	{
		this(gmsecJNI.new_ServiceParam(name, JNIField.getCPtr(Field.getInternal(field)), Field.getInternal(field)), true);
	}


	public JNIServiceParam(JNIServiceParam other)
	{
		this(gmsecJNI.new_ServiceParam_Copy(JNIServiceParam.getCPtr(other), other), true);
	}


	public String getName()
	{
		return gmsecJNI.ServiceParam_GetName(swigCPtr, this);
	}


	public Field getValue()
	{
		long cPtr = gmsecJNI.ServiceParam_GetValue(swigCPtr, this);

		return JNIFieldConverter.cloneField(cPtr);
	}
}
