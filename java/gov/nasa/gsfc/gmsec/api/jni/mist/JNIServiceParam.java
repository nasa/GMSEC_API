/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.mist.*;


public class JNIServiceParam extends ServiceParam
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
			gmsecJNI.delete_ServiceParam(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
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

		return JNIFieldConverter.createFieldReference(cPtr);
	}
}
