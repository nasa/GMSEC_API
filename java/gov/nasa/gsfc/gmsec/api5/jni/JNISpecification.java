/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.MessageSpecification;
import gov.nasa.gsfc.gmsec.api5.MessageValidator;
import gov.nasa.gsfc.gmsec.api5.SchemaIDIterator;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api5.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api5.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api5.jni.JNISchemaIDIterator;

import gov.nasa.gsfc.gmsec.api5.GmsecException;

import java.util.List;


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
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Specification(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNISpecification obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNISpecification(JNIConfig jConfig) throws GmsecException
	{
		this(gmsecJNI.new_Specification(JNIConfig.getCPtr(jConfig), jConfig), true);
	}


	public JNISpecification(JNISpecification jOther) throws GmsecException
	{
		this(gmsecJNI.new_Specification_Copy(JNISpecification.getCPtr(jOther), jOther), true);
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


	public Specification.SchemaLevel getSchemaLevel()
	{
		int level = gmsecJNI.Specification_GetSchemaLevel(swigCPtr, this);

		return Specification.SchemaLevel.values()[level];
	}


	public List<MessageSpecification> getMessageSpecifications()
	{
		return gmsecJNI.Specification_GetMessageSpecifications(swigCPtr, this);
	}

	public List<String> getHeaderFieldNames(String schemaName)
	{
		return gmsecJNI.Specification_GetHeaderFieldNames(swigCPtr, this, schemaName);
	}
}
