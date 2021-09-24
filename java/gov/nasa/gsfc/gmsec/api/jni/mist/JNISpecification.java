/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.mist.MessageSpecification;
import gov.nasa.gsfc.gmsec.api.mist.MessageValidator;
import gov.nasa.gsfc.gmsec.api.mist.SchemaIDIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNISchemaIDIterator;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import java.util.List;


public class JNISpecification
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;

	private MessageValidator validator;


	protected JNISpecification(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
		validator   = null;
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
		validator   = null;
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


	public void validateMessage(JNIMessage jMsg) throws GMSEC_Exception
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


	public Specification.SchemaLevel getSchemaLevel()
	{
		int level = gmsecJNI.Specification_GetSchemaLevel(swigCPtr, this);

		return Specification.SchemaLevel.values()[level];
	}


	public List<MessageSpecification> getMessageSpecifications()
	{
		return gmsecJNI.Specification_GetMessageSpecifications(swigCPtr, this);
	}


    public void registerMessageValidator(MessageValidator val)
    {
        long valPtr = JNIMessageValidator.getCPtr(MessageValidator.getInternal(val));

        gmsecJNI.Specification_RegisterMessageValidator(swigCPtr, this, valPtr);

        // We store a handle to the MessageValidator (in case the user has declared it
        // as an anonymous object) so that the JVM garbage collector does not dispose of it after it
        // has been sent over the JNI layer.
        validator = val;
    }


	public String getTemplateXML(String subject, String schemaID)
	{
		return gmsecJNI.Specification_GetTemplateXML(swigCPtr, this, subject, schemaID);
	}
}
