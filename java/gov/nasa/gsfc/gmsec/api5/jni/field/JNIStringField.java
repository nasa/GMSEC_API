/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.StringField;


public class JNIStringField extends JNIField
{
	public JNIStringField(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIStringField(String name, String data, boolean isHeader)
	{
		this(gmsecJNI.new_StringField(name, data, isHeader), true);
	}


	public JNIStringField(JNIStringField other)
	{
		this(gmsecJNI.new_StringField_Copy(JNIStringField.getCPtr(other), other), true);
	}


	public String getValue()
	{
		return gmsecJNI.StringField_GetValue(JNIField.getCPtr(this), this);
	}
}
