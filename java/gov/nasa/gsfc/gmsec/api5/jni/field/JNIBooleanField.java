/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.BooleanField;


public class JNIBooleanField extends JNIField
{
	public JNIBooleanField(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIBooleanField(String name, boolean data, boolean isHeader)
	{
		this(gmsecJNI.new_BooleanField(name, data, isHeader), true);
	}


	public JNIBooleanField(JNIBooleanField other)
	{
		this(gmsecJNI.new_BooleanField_Copy(JNIBooleanField.getCPtr(other), other), true);
	}


	public boolean getValue()
	{
		return gmsecJNI.BooleanField_GetValue(JNIField.getCPtr(this), this);
	}
}
