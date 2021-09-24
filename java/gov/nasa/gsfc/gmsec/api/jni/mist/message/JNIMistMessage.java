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

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.util.Log;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;

import gov.nasa.gsfc.gmsec.api.jni.mist.ArrayListConverter;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;

import java.util.ArrayList;
import java.util.List;


public class JNIMistMessage extends JNIMessage
{
	public static String buildSchemaID(Message.MessageKind kind,
	                                   String type,
	                                   String subtype,
	                                   String otherType,
	                                   int version)
	{
		StringBuilder sb = new StringBuilder();
		sb.append(version);
		sb.insert(4, ".");

		sb.append(".GMSEC.");

		switch (kind)
		{
			case PUBLISH: sb.append("MSG"); break;
			case REQUEST: sb.append("REQ"); break;
			case REPLY  : sb.append("RESP"); break;
		}

		if (type != null && !type.isEmpty())
		{
			sb.append(".").append(type);
		}

		if (subtype != null && !subtype.isEmpty())
		{
			sb.append(".").append(subtype);
		}

		if (otherType != null && !otherType.isEmpty())
		{
			sb.append(".").append(otherType);
		}

		return sb.toString();
	}


	public static Specification buildSpecification(int version)
	{
		Config config = new Config();
		config.addValue("gmsec-specification-version", Integer.toString(version));

		Specification spec = null;

		try
		{
			spec = new Specification(config);
		}
		catch (GMSEC_Exception e)
		{
			Log.warning("Invalid version number given; using GMSEC_ISD_CURRENT instead.");
		}

		return spec;
	}


	protected JNIMistMessage(long cPtr, boolean cMemoryOwn)
	{
		super(cPtr, cMemoryOwn);
	}


	public JNIMistMessage(String subject, String schemaID, JNISpecification jSpec) throws GMSEC_Exception
	{
		this(gmsecJNI.new_MistMessage(subject, schemaID, JNISpecification.getCPtr(jSpec), jSpec), true);
	}


	public JNIMistMessage(String subject, String schemaID, JNIConfig jConfig, JNISpecification jSpec) throws GMSEC_Exception
	{
		this(gmsecJNI.new_MistMessageWithConfig(subject, schemaID, JNIConfig.getCPtr(jConfig), jConfig, JNISpecification.getCPtr(jSpec), jSpec), true);
	}


	public JNIMistMessage(String data) throws GMSEC_Exception
	{
		this(gmsecJNI.new_MistMessageWithData(data), true);
	}


	public JNIMistMessage(JNISpecification jSpec, String data) throws GMSEC_Exception
	{
		this(gmsecJNI.new_MistMessageWithSpec(JNISpecification.getCPtr(jSpec), jSpec, data), true);
	}


	public JNIMistMessage(JNIMistMessage jOther)
	{
		this(gmsecJNI.new_MistMessageCopy(JNIMistMessage.getCPtr(jOther), jOther), true);
	}


	public JNIMistMessage(JNIMessage jMsg, JNIConfig jSpecConfig) 
	{
		this(gmsecJNI.new_MistMessageFromMessage(JNIMessage.getCPtr(jMsg), jMsg, JNIConfig.getCPtr(jSpecConfig), jSpecConfig), true);
	}


	public static void setStandardFields(List<Field> standardFields)
	{
		long[]     jFieldPtrs = ArrayListConverter.listToFieldPtrs(standardFields);
		JNIField[] jFields    = ArrayListConverter.listToJNIFields(standardFields);
		int        numFields  = (standardFields == null ? 0 : standardFields.size());

		gmsecJNI.MistMessageSetStandardFields(jFieldPtrs, jFields, numFields);
	}


	public static void clearStandardFields()
	{
		gmsecJNI.MistMessageClearStandardFields();
	}


	public String getSchemaID()
	{
		return gmsecJNI.MistMessageGetSchemaID(swigCPtr, this);
	}


	public void setValue(String fieldName, String value) throws GMSEC_Exception
	{
		gmsecJNI.MistMessageSetValueString(swigCPtr, this, fieldName, value);
	}


	public void setValue(String fieldName, long value) throws GMSEC_Exception
	{
		gmsecJNI.MistMessageSetValueLong(swigCPtr, this, fieldName, value);
	}


	public void setValue(String fieldName, double value) throws GMSEC_Exception
	{
		gmsecJNI.MistMessageSetValueDouble(swigCPtr, this, fieldName, value);
	}
}
