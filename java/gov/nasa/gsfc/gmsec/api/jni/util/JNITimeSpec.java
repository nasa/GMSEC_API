/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.util;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


public class JNITimeSpec extends gov.nasa.gsfc.gmsec.api.util.TimeSpec
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNITimeSpec(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_TimeSpec(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNITimeSpec obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNITimeSpec()
	{
		this(gmsecJNI.new_TimeSpec(), true);
	}


	public long getSeconds()
	{
		return gmsecJNI.TimeSpec_GetSeconds(swigCPtr, this);
	}


	public long getNanoseconds()
	{
		return gmsecJNI.TimeSpec_GetNanoseconds(swigCPtr, this);
	}
}
