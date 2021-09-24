/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;
import gov.nasa.gsfc.gmsec.api.mist.SubscriptionInfo;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


public class JNIConnMgrSubscriptionInfo
{
	protected long                    swigCPtr;
	private boolean                   swigCMemOwn;
	private String                    swigSubject;
	private ConnectionManagerCallback swigCallback;


	protected JNIConnMgrSubscriptionInfo(long cPtr, boolean cMemoryOwn, String subject, ConnectionManagerCallback callback)
	{
		swigCPtr     = cPtr;
		swigCMemOwn  = cMemoryOwn;
		swigSubject  = subject;
		swigCallback = callback;
	}


	protected void finalize() throws Throwable
	{
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		// We don't own the C++ object, so nothing to delete here.
		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIConnMgrSubscriptionInfo obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public String getSubject()
	{
		return swigSubject;
	}


	public ConnectionManagerCallback getCallback()
	{
		return swigCallback;
	}
}
