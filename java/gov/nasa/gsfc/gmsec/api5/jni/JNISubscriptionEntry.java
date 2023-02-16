/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;


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


	public JNISubscriptionEntry(String name, String pattern)
	{
		this(gmsecJNI.new_SubscriptionEntry(name, pattern), true);
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


	public void addExcludedPattern(String pattern)
	{
		gmsecJNI.SubscriptionEntry_AddExcludedPattern(swigCPtr, this, pattern);
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
