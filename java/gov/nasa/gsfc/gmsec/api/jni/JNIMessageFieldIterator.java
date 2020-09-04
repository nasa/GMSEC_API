/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.MessageFieldIterator;


public class JNIMessageFieldIterator
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNIMessageFieldIterator(long cPtr, boolean cMemoryOwn)
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
			// We don't own the C++ object, so nothing to do here.
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIMessageFieldIterator obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public boolean hasNext()
	{
		return gmsecJNI.MessageFieldIterator_HasNext(swigCPtr, this);
	}


	public Field next() throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.MessageFieldIterator_Next(swigCPtr, this);

		return JNIFieldConverter.createFieldReference(cPtr);
	}


	public void reset()
	{
		gmsecJNI.MessageFieldIterator_Reset(swigCPtr, this);
	}
}
