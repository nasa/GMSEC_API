/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.BinaryField;


public class JNIBinaryField extends JNIField
{
	public JNIBinaryField(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIBinaryField(String name, byte[] data, boolean isHeader)
	{
		this(gmsecJNI.new_BinaryField(name, data, data.length, isHeader), true);
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
