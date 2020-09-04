/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.EventCallback;


public class JNIEventCallback
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNIEventCallback(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_EventCallback(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIEventCallback obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIEventCallback(EventCallback cb)
	{
		this(gmsecJNI.new_EventCallback(cb), true);
	}
}
