/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.U64Field;
import gov.nasa.gsfc.gmsec.api5.U64;


public class JNIU64Field extends JNIField
{
	public JNIU64Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIU64Field(String name, U64 data, boolean isHeader)
	{
		this(gmsecJNI.new_U64Field(name, data.toString(), isHeader), true);
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
