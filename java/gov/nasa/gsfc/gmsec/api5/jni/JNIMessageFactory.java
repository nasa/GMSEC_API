/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFactory.java
 */

package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.MessageValidator;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.field.*;

import gov.nasa.gsfc.gmsec.api5.jni.JNIMessage;
import gov.nasa.gsfc.gmsec.api5.jni.field.*;

import java.util.Collection;


public class JNIMessageFactory
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;

	private MessageValidator validator;


	protected JNIMessageFactory(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.MessageFactory_Delete(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIMessageFactory obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIMessageFactory()
	{
		this(gmsecJNI.MessageFactory_Create(), true);
	}


	public JNIMessageFactory(Config config)
	{
		this(gmsecJNI.MessageFactory_CreateUsingConfig(JNIConfig.getCPtr(Config.getInternal(config)), Config.getInternal(config)), true);
	}


	public static void destroy(MessageFactory factory)
	{
		MessageFactory.getInternal(factory).delete();
	}


	public void setStandardFields(java.util.List<Field> standardFields)
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(standardFields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(standardFields);
		int        numFields  = (standardFields == null ? 0 : standardFields.size());

		gmsecJNI.MessageFactory_SetStandardFields(swigCPtr, this, jFieldPtrs, jFields, numFields);
	}


	public void clearStandardFields()
	{
		gmsecJNI.MessageFactory_ClearStandardFields(swigCPtr, this);
	}


	public void setMessageConfig(Config msgConfig) throws IllegalArgumentException
	{
		gmsecJNI.MessageFactory_SetMessageConfig(swigCPtr, this, JNIConfig.getCPtr(Config.getInternal(msgConfig)), Config.getInternal(msgConfig));
	}


	public Message createMessage()
	{
		long cptr = gmsecJNI.MessageFactory_CreateMessage(swigCPtr, this);

		return new Message(new JNIMessage(cptr, true));
	}


	public Message createMessage(String schemaID)
		throws GmsecException
	{
		long cptr = gmsecJNI.MessageFactory_CreateMessage(swigCPtr, this, schemaID);

		return new Message(new JNIMessage(cptr, true));
	}


	public Message fromData(String data, Gmsec.DataType type)
		throws GmsecException
	{
		long cptr = gmsecJNI.MessageFactory_FromData(swigCPtr, this, data, type.getValue());

		return new Message(new JNIMessage(cptr, true));
	}


	public Specification getSpecification()
	{
		long cptr = gmsecJNI.MessageFactory_GetSpecification(swigCPtr, this);

		return new Specification(new JNISpecification(cptr, false));
	}


	public void registerMessageValidator(MessageValidator val)
	{
		long valPtr = JNIMessageValidator.getCPtr(MessageValidator.getInternal(val));

		gmsecJNI.MessageFactory_RegisterMessageValidator(swigCPtr, this, valPtr);

		// We store a handle to the MessageValidator (in case the user has declared it
		// as an anonymous object) so that the JVM garbage collector does not dispose of it after it
		// has been sent over the JNI layer.
		validator = val;
	}
}
