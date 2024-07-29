/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.field;


import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.U16;
import gov.nasa.gsfc.gmsec.api5.U32;
import gov.nasa.gsfc.gmsec.api5.U64;
import gov.nasa.gsfc.gmsec.api5.field.Field;


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


	public Field.Type getType()
	{
		int type = gmsecJNI.Field_GetType(swigCPtr, this);

		return Field.Type.valueOf(type);
	}


	public String toXML()
	{
		return gmsecJNI.Field_ToXML(swigCPtr, this);
	}


	public String toJSON()
	{
		return gmsecJNI.Field_ToJSON(swigCPtr, this);
	}


	public String getStringValue()
	{
		return gmsecJNI.Field_GetStringValue(swigCPtr, this);
	}


	public boolean getBooleanValue() throws GmsecException
	{
		return gmsecJNI.Field_GetBooleanValue(swigCPtr, this);
	}


	public short getI16Value() throws GmsecException
	{
		return gmsecJNI.Field_GetI16Value(swigCPtr, this);
	}


	public int getI32Value() throws GmsecException
	{
		return gmsecJNI.Field_GetI32Value(swigCPtr, this);
	}


	public long getI64Value() throws GmsecException
	{
		return gmsecJNI.Field_GetI64Value(swigCPtr, this);
	}


	public U16 getU16Value() throws GmsecException
	{
		return gmsecJNI.Field_GetU16Value(swigCPtr, this);
	}


	public U32 getU32Value() throws GmsecException
	{
		return gmsecJNI.Field_GetU32Value(swigCPtr, this);
	}


	public U64 getU64Value() throws GmsecException
	{
		return gmsecJNI.Field_GetU64Value(swigCPtr, this);
	}


	public double getF64Value() throws GmsecException
	{
		return gmsecJNI.Field_GetF64Value(swigCPtr, this);
	}


	public boolean isHeader()
	{
		return gmsecJNI.Field_IsHeader(swigCPtr, this);
	}


	public boolean isTracking()
	{
		return gmsecJNI.Field_IsTracking(swigCPtr, this);
	}


	public static boolean isFieldNameCompliant(String fieldName)
	{
		return gmsecJNI.Field_IsFieldNameCompliant(fieldName);
	}
}
