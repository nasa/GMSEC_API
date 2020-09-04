/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.field.Field;


public class JNIField
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIField(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Field(swigCPtr, this);
			swigCMemOwn = false;
		}
		swigCPtr = 0;
	}


	protected static long getCPtr(JNIField obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public String getName()
	{
		return gmsecJNI.Field_GetName(swigCPtr, this);
	}


	public Field.FieldType getType()
	{
		int type = gmsecJNI.Field_GetType(swigCPtr, this);

		return Field.FieldType.values()[type];
	}


	public String toXML()
	{
		return gmsecJNI.Field_ToXML(swigCPtr, this);
	}


	public String toJSON()
	{
		return gmsecJNI.Field_ToJSON(swigCPtr, this);
	}


	public void isHeader(boolean header)
	{
		gmsecJNI.Field_IsHeader(swigCPtr, this, header);
	}


	public boolean isHeader()
	{
		return gmsecJNI.Field_IsHeader(swigCPtr, this);
	}
}
