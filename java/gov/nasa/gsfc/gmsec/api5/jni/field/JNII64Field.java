/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.I64Field;


public class JNII64Field extends JNIField
{
	public JNII64Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNII64Field(String name, long data, boolean isHeader)
	{
		this(gmsecJNI.new_I64Field(name, data, isHeader), true);
	}


	public JNII64Field(JNII64Field other)
	{
		this(gmsecJNI.new_I64Field_Copy(JNII64Field.getCPtr(other), other), true);
	}


	public long getValue()
	{
		return gmsecJNI.I64Field_GetValue(JNIField.getCPtr(this), this);
	}
}
