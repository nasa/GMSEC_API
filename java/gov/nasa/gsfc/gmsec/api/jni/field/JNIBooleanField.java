/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.BooleanField;


public class JNIBooleanField extends JNIField
{
	public JNIBooleanField(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIBooleanField(String name, boolean data)
	{
		this(gmsecJNI.new_BooleanField(name, data), true);
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
