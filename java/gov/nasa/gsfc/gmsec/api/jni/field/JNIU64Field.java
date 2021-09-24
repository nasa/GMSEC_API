/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.U64Field;
import gov.nasa.gsfc.gmsec.api.U64;


public class JNIU64Field extends JNIField
{
	public JNIU64Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIU64Field(String name, U64 data)
	{
		this(gmsecJNI.new_U64Field(name, data.toString()), true);
	}


	public JNIU64Field(JNIU64Field other)
	{
		this(gmsecJNI.new_U64Field_Copy(JNIU64Field.getCPtr(other), other), true);
	}


	public U64 getValue()
	{
		return new U64(gmsecJNI.U64Field_GetValue(JNIField.getCPtr(this), this));
	}
}
