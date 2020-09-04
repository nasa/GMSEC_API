/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;


public class JNISubscriptionEntry
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNISubscriptionEntry(long cPtr, boolean cMemoryOwn)
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
		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNISubscriptionEntry entry)
	{
		return (entry == null) ? 0 : entry.swigCPtr;
	}


	public String getName()
	{
		return gmsecJNI.SubscriptionEntry_GetName(swigCPtr, this);
	}


	public String getSubject()
	{
		return gmsecJNI.SubscriptionEntry_GetSubject(swigCPtr, this);
	}
}
