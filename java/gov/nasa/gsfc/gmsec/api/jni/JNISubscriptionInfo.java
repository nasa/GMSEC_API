/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.Callback;
import gov.nasa.gsfc.gmsec.api.SubscriptionInfo;


public class JNISubscriptionInfo
{
	protected long   swigCPtr;
	private boolean  swigCMemOwn;
	private String   swigSubject;
	private Callback swigCallback;


	protected JNISubscriptionInfo(long cPtr, boolean cMemoryOwn, String subject, Callback callback)
	{
		swigCPtr     = cPtr;
		swigCMemOwn  = cMemoryOwn;
		swigSubject  = subject;
		swigCallback = callback;
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
			// We don't own the C++ object, so nothing to delete here.
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNISubscriptionInfo obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public String getSubject()
	{
		return swigSubject;
	}


	public Callback getCallback()
	{
		return swigCallback;
	}
}
