/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.U8Field;
import gov.nasa.gsfc.gmsec.api.U8;


public class JNIU8Field extends JNIField
{
	public JNIU8Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIU8Field(String name, U8 data)
	{
		this(gmsecJNI.new_U8Field(name, data.get()), true);
	}


	public JNIU8Field(JNIU8Field other)
	{
		this(gmsecJNI.new_U8Field_Copy(JNIU8Field.getCPtr(other), other), true);
	}


	public U8 getValue()
	{
		return new U8(gmsecJNI.U8Field_GetValue(JNIField.getCPtr(this), this));
	}
}
