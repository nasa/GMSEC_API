/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.U16Field;
import gov.nasa.gsfc.gmsec.api.U16;


public class JNIU16Field extends JNIField
{
	public JNIU16Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIU16Field(String name, U16 data)
	{
		this(gmsecJNI.new_U16Field(name, data.get()), true);
	}


	public JNIU16Field(JNIU16Field other)
	{
		this(gmsecJNI.new_U16Field_Copy(JNIU16Field.getCPtr(other), other), true);
	}


	public U16 getValue()
	{
		return new U16(gmsecJNI.U16Field_GetValue(JNIField.getCPtr(this), this));
	}
}
