/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.mist.SchemaIDIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNISchemaIDIterator;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;


public class JNISpecification
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNISpecification(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_Specification(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	public static long getCPtr(JNISpecification obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNISpecification(JNIConfig jConfig) throws GMSEC_Exception
	{
		this(gmsecJNI.new_Specification(JNIConfig.getCPtr(jConfig), jConfig), true);
	}


	public JNISpecification(JNISpecification jOther) throws GMSEC_Exception
	{
		this(gmsecJNI.new_Specification_Copy(JNISpecification.getCPtr(jOther), jOther), true);
	}


	public void validateMessage(JNIMessage jMsg)
	{
		gmsecJNI.Specification_ValidateMessage(swigCPtr, this, JNIMessage.getCPtr(jMsg), jMsg);
	}


	public SchemaIDIterator getSchemaIDIterator()
	{
		long cPtr = gmsecJNI.Specification_GetSchemaIDIterator(swigCPtr, this);

		return new SchemaIDIterator(new JNISchemaIDIterator(cPtr, false));
	}


	public int getVersion()
	{
		return gmsecJNI.Specification_GetVersion(swigCPtr, this);
	}


	public String getTemplateXML(String subject, String schemaID)
	{
		return gmsecJNI.Specification_GetTemplateXML(swigCPtr, this, subject, schemaID);
	}
}
