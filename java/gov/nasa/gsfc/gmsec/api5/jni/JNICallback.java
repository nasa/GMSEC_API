/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.Callback;


public class JNICallback
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNICallback(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_Callback(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNICallback obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNICallback(Callback cb)
	{
		this(gmsecJNI.new_Callback(cb), true);
	}
}
