/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.MessageFieldIterator;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIFieldConverter;


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
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		// We don't own the C++ object, so nothing to do here.
		swigCPtr    = 0;
		swigCMemOwn = false;
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

		return JNIFieldConverter.cloneField(cPtr);
	}


	public void reset()
	{
		gmsecJNI.MessageFieldIterator_Reset(swigCPtr, this);
	}
}
