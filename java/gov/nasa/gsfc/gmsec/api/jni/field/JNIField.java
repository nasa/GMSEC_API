/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.field;


import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.U64;
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
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Field(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIField obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public String getName()
	{
		return gmsecJNI.Field_GetName(swigCPtr, this);
	}


	public void setName(String name)
	{
		gmsecJNI.Field_SetName(swigCPtr, this, name);
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


	public long getIntegerValue() throws GMSEC_Exception
	{
		return gmsecJNI.Field_GetIntegerValue(swigCPtr, this);
	}


	public U64 getUnsignedIntegerValue() throws GMSEC_Exception
	{
		return gmsecJNI.Field_GetUnsignedIntegerValue(swigCPtr, this);
	}


	public double getDoubleValue() throws GMSEC_Exception
	{
		return gmsecJNI.Field_GetDoubleValue(swigCPtr, this);
	}


	public String getStringValue() throws GMSEC_Exception
	{
		return gmsecJNI.Field_GetStringValue(swigCPtr, this);
	}


	public void isHeader(boolean header)
	{
		gmsecJNI.Field_IsHeader(swigCPtr, this, header);
	}


	public boolean isHeader()
	{
		return gmsecJNI.Field_IsHeader(swigCPtr, this);
	}


	public Field clone()
	{
		return JNIFieldConverter.cloneField(swigCPtr);
	}
}
