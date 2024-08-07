/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.MessageFieldIterator;

import gov.nasa.gsfc.gmsec.api5.jni.field.JNIFieldConverter;

import java.util.NoSuchElementException;


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


	public Field next()
		throws NoSuchElementException
	{
		try {
			long cPtr = gmsecJNI.MessageFieldIterator_Next(swigCPtr, this);

			return JNIFieldConverter.cloneField(cPtr);
		}
		catch (GmsecException e) {
			throw new NoSuchElementException(e.getMessage());
		}
	}


	public void reset()
	{
		gmsecJNI.MessageFieldIterator_Reset(swigCPtr, this);
	}
}
