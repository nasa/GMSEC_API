/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.Message;


public class JNIMessageEntry
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIMessageEntry(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_MessageEntry(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIMessageEntry entry)
	{
		return (entry == null) ? 0 : entry.swigCPtr;
	}


	public String getName()
	{
		return gmsecJNI.MessageEntry_GetName(swigCPtr, this);
	}


	public Message getMessage()
	{
		long cPtr = gmsecJNI.MessageEntry_GetMessage(swigCPtr, this);

		return new Message(new JNIMessage(cPtr, false));
	}
}
