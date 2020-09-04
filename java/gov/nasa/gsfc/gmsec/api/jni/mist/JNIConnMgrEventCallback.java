/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerEventCallback;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


public class JNIConnMgrEventCallback
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNIConnMgrEventCallback(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_ConnectionManagerEventCallback(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIConnMgrEventCallback obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConnMgrEventCallback(ConnectionManagerEventCallback cb)
	{
		this(gmsecJNI.new_ConnectionManagerEventCallback(cb), true);
	}
}
