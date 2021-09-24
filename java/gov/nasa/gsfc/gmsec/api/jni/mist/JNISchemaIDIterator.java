/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


public class JNISchemaIDIterator
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNISchemaIDIterator(long cPtr, boolean cMemoryOwn)
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


	protected static long getCPtr(JNISchemaIDIterator obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public boolean hasNext()
	{
		return gmsecJNI.SchemaIDIterator_HasNext(swigCPtr, this);
	}


	public String next() throws GMSEC_Exception
	{
		return gmsecJNI.SchemaIDIterator_Next(swigCPtr, this);
	}


	public void reset()
	{
		gmsecJNI.SchemaIDIterator_Reset(swigCPtr, this);
	}
}
