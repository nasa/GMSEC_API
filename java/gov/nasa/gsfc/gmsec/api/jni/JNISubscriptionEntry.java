/*
 * Copyright 2007-2021 United States Government as represented by the
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
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_SubscriptionEntry(swigCPtr, this);
		}

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

	public String getPattern()
	{
		return gmsecJNI.SubscriptionEntry_GetPattern(swigCPtr, this);
	}

	public boolean hasNextExcludedPattern()
	{
		return gmsecJNI.SubscriptionEntry_HasNextExcludedPattern(swigCPtr, this);
	}

	public String nextExcludedPattern()
	{
		return gmsecJNI.SubscriptionEntry_NextExcludedPattern(swigCPtr, this);
	}
}
