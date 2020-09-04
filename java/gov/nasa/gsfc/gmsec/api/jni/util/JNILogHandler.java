/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.util.LogHandler;


public class JNILogHandler
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNILogHandler(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_LogHandler(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNILogHandler obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNILogHandler(LogHandler handler)
	{
		this(gmsecJNI.new_LogHandler(handler), true);
	}
}
