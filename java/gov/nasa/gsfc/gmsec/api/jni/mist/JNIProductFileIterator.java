/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


public class JNIProductFileIterator
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	public JNIProductFileIterator(long cPtr, boolean cMemoryOwn)
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


	protected static long getCPtr(JNIProductFileIterator obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public boolean hasNext()
	{
		return gmsecJNI.ProductFileIterator_HasNext(swigCPtr, this);
	}


	public ProductFile next() throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.ProductFileIterator_Next(swigCPtr, this);

		return new ProductFile(new JNIProductFile(cPtr, false));
	}


	public void reset()
	{
		gmsecJNI.ProductFileIterator_Reset(swigCPtr, this);
	}
}
