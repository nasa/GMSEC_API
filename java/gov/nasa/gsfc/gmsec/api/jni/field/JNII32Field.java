/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.I32Field;


public class JNII32Field extends JNIField
{
	public JNII32Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNII32Field(String name, int data)
	{
		this(gmsecJNI.new_I32Field(name, data), true);
	}


	public JNII32Field(JNII32Field other)
	{
		this(gmsecJNI.new_I32Field_Copy(JNII32Field.getCPtr(other), other), true);
	}


	public int getValue()
	{
		return gmsecJNI.I32Field_GetValue(JNIField.getCPtr(this), this);
	}
}
