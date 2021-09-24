/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


public class JNIConnMgrCallback
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNIConnMgrCallback(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_ConnectionManagerCallback(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIConnMgrCallback obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConnMgrCallback(ConnectionManagerCallback cb)
	{
		this(gmsecJNI.new_ConnectionManagerCallback(cb), true);
	}
}
