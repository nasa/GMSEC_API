/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.F64Field;


public class JNIF64Field extends JNIField
{
	public JNIF64Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIF64Field(String name, double data)
	{
		this(gmsecJNI.new_F64Field(name, data), true);
	}


	public JNIF64Field(JNIF64Field other)
	{
		this(gmsecJNI.new_F64Field_Copy(JNIF64Field.getCPtr(other), other), true);
	}


	public double getValue()
	{
		return gmsecJNI.F64Field_GetValue(JNIField.getCPtr(this), this);
	}
}
