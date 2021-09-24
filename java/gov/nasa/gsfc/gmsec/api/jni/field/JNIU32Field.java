/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.field.Field;
import gov.nasa.gsfc.gmsec.api.field.U32Field;
import gov.nasa.gsfc.gmsec.api.U32;


public class JNIU32Field extends JNIField
{
	public JNIU32Field(long cPtr, boolean owned)
	{
		super(cPtr, owned);
	}


	public JNIU32Field(String name, U32 data)
	{
		this(gmsecJNI.new_U32Field(name, data.get()), true);
	}


	public JNIU32Field(JNIU32Field other)
	{
		this(gmsecJNI.new_U32Field_Copy(JNIU32Field.getCPtr(other), other), true);
	}


	public U32 getValue()
	{
		return new U32(gmsecJNI.U32Field_GetValue(JNIField.getCPtr(this), this));
	}
}
