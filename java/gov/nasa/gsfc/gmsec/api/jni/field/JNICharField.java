/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.CharField;


public class JNICharField extends JNIField
{
	public JNICharField(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNICharField(String name, char data)
	{
		this(gmsecJNI.new_CharField(name, data), true);
	}


	public JNICharField(JNICharField other)
	{
		this(gmsecJNI.new_CharField_Copy(JNICharField.getCPtr(other), other), true);
	}


	public char getValue()
	{
		return gmsecJNI.CharField_GetValue(JNIField.getCPtr(this), this);
	}
}
