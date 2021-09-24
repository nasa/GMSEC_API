/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;
import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.mist.ProductFileIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIProductFile;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIProductFileIterator;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;


public class JNIProductFileMessage extends JNIMistMessage
{
	private JNIProductFileMessage(long cPtr, boolean own)
	{
		super(cPtr, own);
	}


	public JNIProductFileMessage(String subject,
	                             gmsecMIST.ResponseStatus responseStatus,
	                             Message.MessageKind kind,
	                             String productType,
	                             String productSubtype,
	                             Specification spec)
	{
        super(gmsecJNI.new_ProductFileMessage(subject,
		                                      responseStatus.getValue(),
		                                      kind.ordinal(),
		                                      productType,
		                                      productSubtype,
		                                      JNISpecification.getCPtr(Specification.getInternal(spec)),
		                                      Specification.getInternal(spec)),
		                                      true);
	}


	public JNIProductFileMessage(String subject,
	                             gmsecMIST.ResponseStatus responseStatus,
	                             Message.MessageKind kind,
	                             String productType,
	                             String productSubtype,
	                             Config config,
	                             Specification spec)
	{
        super(gmsecJNI.new_ProductFileMessage(subject,
		                                      responseStatus.getValue(),
		                                      kind.ordinal(),
		                                      productType,
		                                      productSubtype,
		                                      JNIConfig.getCPtr(Config.getInternal(config)),
		                                      Config.getInternal(config),
		                                      JNISpecification.getCPtr(Specification.getInternal(spec)),
		                                      Specification.getInternal(spec)),
		                                      true);
	}


	public JNIProductFileMessage(String subject,
	                             gmsecMIST.ResponseStatus responseStatus,
	                             String schemaID,
	                             Specification spec)
	{
        super(gmsecJNI.new_ProductFileMessage(subject,
		                                      responseStatus.getValue(),
		                                      schemaID,
		                                      JNISpecification.getCPtr(Specification.getInternal(spec)),
		                                      Specification.getInternal(spec)),
		                                      true);
	}


	public JNIProductFileMessage(String subject,
	                             gmsecMIST.ResponseStatus responseStatus,
	                             String schemaID,
	                             Config config,
	                             Specification spec)
	{
        super(gmsecJNI.new_ProductFileMessage(subject,
		                                      responseStatus.getValue(),
		                                      schemaID,
		                                      JNIConfig.getCPtr(Config.getInternal(config)),
		                                      Config.getInternal(config),
		                                      JNISpecification.getCPtr(Specification.getInternal(spec)),
		                                      Specification.getInternal(spec)),
		                                      true);
	}


	public JNIProductFileMessage(String data) throws GMSEC_Exception
	{
        super(gmsecJNI.new_ProductFileMessage(data), true);
	}


	public JNIProductFileMessage(JNISpecification jSpec, String data) throws GMSEC_Exception
	{
		super(gmsecJNI.new_ProductFileMessage(JNISpecification.getCPtr(jSpec), jSpec, data), true);
	}


	public JNIProductFileMessage(JNIProductFileMessage jOther)
	{
        super(gmsecJNI.new_ProductFileMessage_Copy(JNIProductFileMessage.getCPtr(jOther), jOther), true);
	}


	public void addProductFile(JNIProductFile jProductFile)
	{
		gmsecJNI.ProductFileMessage_AddProductFile(swigCPtr, this, JNIProductFile.getCPtr(jProductFile), jProductFile);
	}


	public int getNumProductFiles()
	{
		return gmsecJNI.ProductFileMessage_GetNumProductFiles(swigCPtr, this);
	}


	public ProductFile getProductFile(int index) throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.ProductFileMessage_GetProductFile(swigCPtr, this, index);

		return new ProductFile(new JNIProductFile(cPtr, false));
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


	public ProductFileIterator getProductFileIterator()
	{
		long cPtr = gmsecJNI.ProductFileMessage_GetProductFileIterator(swigCPtr, this);

		return new ProductFileIterator(new JNIProductFileIterator(cPtr, false));
	}


	public static ProductFileMessage convertMessage(JNIMessage jMsg)
	{
		long cPtr = gmsecJNI.ProductFileMessage_ConvertMessage(JNIMessage.getCPtr(jMsg), jMsg);

		return new ProductFileMessage(new JNIProductFileMessage(cPtr, true));
	}
}
