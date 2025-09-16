/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;

import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.I16Field;


public class JNII16Field extends JNIField
{
    public JNII16Field(long cPtr, boolean owned)
    {
        super(cPtr, owned);
    }


	public JNII16Field(String name, short data, boolean isHeader)
	{
		this(gmsecJNI.new_I16Field(name, data, isHeader), true);
	}


	public JNII16Field(JNII16Field other)
	{
		this(gmsecJNI.new_I16Field_Copy(JNII16Field.getCPtr(other), other), true);
	}


	public short getValue()
	{
		return gmsecJNI.I16Field_GetValue(JNIField.getCPtr(this), this);
	}
}
