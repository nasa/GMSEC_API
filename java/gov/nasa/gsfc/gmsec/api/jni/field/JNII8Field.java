/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.I8Field;


public class JNII8Field extends JNIField
{
	public JNII8Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNII8Field(String name, byte data)
	{
		this(gmsecJNI.new_I8Field(name, data), true);
	}


	public JNII8Field(JNII8Field other)
	{
		this(gmsecJNI.new_I8Field_Copy(JNII8Field.getCPtr(other), other), true);
	}


	public byte getValue()
	{
		return gmsecJNI.I8Field_GetValue(JNIField.getCPtr(this), this);
	}
}
