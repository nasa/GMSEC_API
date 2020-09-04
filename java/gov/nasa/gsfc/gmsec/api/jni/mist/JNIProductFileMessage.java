/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;
import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.mist.ProductFileIterator;
import gov.nasa.gsfc.gmsec.api.mist.ProductFileMessage;


public class JNIProductFileMessage extends JNIMessage
{
	protected JNIProductFileMessage(long cPtr, boolean cMemoryOwn)
	{
		super(cPtr, cMemoryOwn);
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
			gmsecJNI.delete_ProductFileMessage(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIProductFileMessage obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIProductFileMessage(String subject, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
	{
		this(gmsecJNI.new_ProductFileMessage(subject, status.getValue(), productType, productSubtype, version), true);
	}


	public JNIProductFileMessage(String subject, JNIConfig jConfig, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
	{
		this(gmsecJNI.new_ProductFileMessage(subject, JNIConfig.getCPtr(jConfig), jConfig, status.getValue(), productType, productSubtype, version), true);
	}


	public JNIProductFileMessage(String data)
	{
		this(gmsecJNI.new_ProductFileMessage(data), true);
	}


	public JNIProductFileMessage(JNIProductFileMessage jProductFileMsg)
	{
		this(gmsecJNI.new_ProductFileMessage_Copy(JNIProductFileMessage.getCPtr(jProductFileMsg), jProductFileMsg), true);
	}


	public void addProductFile(JNIProductFile jProductFile)
	{
		gmsecJNI.ProductFileMessage_AddProductFile(swigCPtr, this, JNIProductFile.getCPtr(jProductFile), jProductFile);
	}


	public int getNumProductFiles()
	{
		return gmsecJNI.ProductFileMessage_GetNumProductFiles(swigCPtr, this);
	}


	public ProductFile getProductFile(int index)
	{
		long cPtr = gmsecJNI.ProductFileMessage_GetProductFile(swigCPtr, this, index);

		return new ProductFile( new JNIProductFile(cPtr, true) );
	}


	public ProductFileIterator getProductFileIterator()
	{
		long cPtr = gmsecJNI.ProductFileMessage_GetProductFileIterator(swigCPtr, this);

		return new ProductFileIterator(new JNIProductFileIterator(cPtr, false));
	}


	public gmsecMIST.ResponseStatus getResponseStatus()
	{
		int status = gmsecJNI.ProductFileMessage_GetResponseStatus(swigCPtr, this);

		return gmsecMIST.ResponseStatus.getUsingValue(status);
	}


	public String getProductType()
	{
		return gmsecJNI.ProductFileMessage_GetProductType(swigCPtr, this);
	}


	public String getProductSubtype()
	{
		return gmsecJNI.ProductFileMessage_GetProductSubtype(swigCPtr, this);
	}


	public static ProductFileMessage convertMessage(JNIMessage jMsg)
	{
		long cPtr = gmsecJNI.ProductFileMessage_ConvertMessage(JNIMessage.getCPtr(jMsg), jMsg);

		return new ProductFileMessage( new JNIProductFileMessage(cPtr, true) );
	}
}
