/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.F32Field;


public class JNIF32Field extends JNIField
{
	public JNIF32Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIF32Field(String name, float data, boolean isHeader)
	{
		this(gmsecJNI.new_F32Field(name, data, isHeader), true);
	}


	public JNIF32Field(JNIF32Field other)
	{
		this(gmsecJNI.new_F32Field_Copy(JNIF32Field.getCPtr(other), other), true);
	}


	public float getValue()
	{
		return gmsecJNI.F32Field_GetValue(JNIField.getCPtr(this), this);
	}
}
