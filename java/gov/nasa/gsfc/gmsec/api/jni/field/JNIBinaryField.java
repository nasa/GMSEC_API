/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.BinaryField;


public class JNIBinaryField extends JNIField
{
	public JNIBinaryField(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIBinaryField(String name, byte[] data)
	{
		this(gmsecJNI.new_BinaryField(name, data, data.length), true);
	}


	public JNIBinaryField(JNIBinaryField other)
	{
		this(gmsecJNI.new_BinaryField_Copy(JNIBinaryField.getCPtr(other), other), true);
	}


	public byte[] getValue()
	{
		return gmsecJNI.BinaryField_GetValue(JNIField.getCPtr(this), this);
	}
}
