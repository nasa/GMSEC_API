/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;


public class JNIStatus extends gov.nasa.gsfc.gmsec.api.Status
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;

	public JNIStatus(long cPtr, boolean cMemoryOwn)
	{
		swigCMemOwn = cMemoryOwn;
		swigCPtr    = cPtr;
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
			gmsecJNI.delete_Status(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIStatus obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIStatus()
	{
		this(gmsecJNI.new_Status(), true);
	}


	private JNIStatus(int clazz, int code, String reason, int customCode)
	{
		this(StatusClassification.getUsingValue(clazz), StatusCode.getUsingValue(code), reason, customCode);
	}

	public JNIStatus(StatusClassification clazz, StatusCode code, String reason)
	{
		this(clazz, code, reason, 0);
	}


	public JNIStatus(StatusClassification clazz, StatusCode code, String reason, int customCode)
	{
		this(gmsecJNI.new_Status_Set(clazz.getValue(), code.getValue(), reason, customCode), true);
	}


	public JNIStatus(JNIStatus other)
	{
		this(gmsecJNI.new_Status_Copy(JNIStatus.getCPtr(other), other), true);
	}


	public boolean isError()
	{
		return gmsecJNI.Status_IsError(swigCPtr, this) != 0;
	}


	public String get()
	{
		return gmsecJNI.Status_Get(swigCPtr, this);
	}


	public void set(StatusClassification clazz, StatusCode code, String reason)
	{
		set(clazz, code, reason, 0);
	}


	public void set(StatusClassification clazz, StatusCode code, String reason, int customCode)
	{
		gmsecJNI.Status_Set(swigCPtr, this, clazz.getValue(), code.getValue(), reason, customCode);
	}


	public StatusClassification getClassification()
	{
		int clazz = gmsecJNI.Status_GetClassification(swigCPtr, this);

		return StatusClassification.getUsingValue(clazz);
	}


	public void setClassification(StatusClassification clazz)
	{
		gmsecJNI.Status_SetClassification(swigCPtr, this, clazz.getValue());
	}


	public StatusCode getCode()
	{
		int code = gmsecJNI.Status_GetCode(swigCPtr, this);

		return StatusCode.getUsingValue(code);
	}


	public void setCode(StatusCode code)
	{
		gmsecJNI.Status_SetCode(swigCPtr, this, code.getValue());
	}


	public String getReason()
	{
		return gmsecJNI.Status_GetReason(swigCPtr, this);
	}


	public void setReason(String reason)
	{
		gmsecJNI.Status_SetReason(swigCPtr, this, reason);
	}


	public int getCustomCode()
	{
		return gmsecJNI.Status_GetCustomCode(swigCPtr, this);
	}


	public void setCustomCode(int customCode)
	{
		gmsecJNI.Status_SetCustomCode(swigCPtr, this, customCode);
	}


	public void reset()
	{
		gmsecJNI.Status_Reset(swigCPtr, this);
	}
}
