/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerReplyCallback;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


public class JNIConnMgrReplyCallback
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNIConnMgrReplyCallback(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_ConnectionManagerReplyCallback(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIConnMgrReplyCallback obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConnMgrReplyCallback(ConnectionManagerReplyCallback cb)
	{
		this(gmsecJNI.new_ConnectionManagerReplyCallback(cb), true);
	}
}
