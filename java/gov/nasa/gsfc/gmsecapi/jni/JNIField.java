/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.jni;

import java.lang.reflect.Array;
import gov.nasa.gsfc.gmsecapi.*;

/**
 * @class Field
 * @brief This is the base class for GMSEC message fields.
 */
public class JNIField extends gov.nasa.gsfc.gmsecapi.Field
{
	private long swigCPtr;
	protected boolean swigCMemOwn;

	protected JNIField(long cPtr, boolean cMemoryOwn)
	{
		swigCMemOwn = cMemoryOwn;
		swigCPtr = cPtr;
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

	/**
	 * @brief Create an empty field of type UNSET
	 */
	public JNIField()
	{
		this(gmsecJNI.new_Field(), true);
	}

	public JNIField(Field fd)
	{
		this(gmsecJNI.new_Field(), true);
		super.Set(fd);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueCHAR(String name, byte value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueCHAR(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and @value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueBOOL(String name, boolean value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueBOOL(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueBYTE(String name, byte value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueI8(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueUBYTE(String name, int value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueU8(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueSHORT(String name, short value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueI16(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueUSHORT(String name, int value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueU16(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueLONG(String name, int value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueI32(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueULONG(String name, long value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueU32(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueFLOAT(String name, float value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueF32(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueDOUBLE(String name, double value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueF64(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI8(String name, byte value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueI8(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueU8(String name, int value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueU8(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI16(String name, short value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueI16(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueU16(String name, int value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueU16(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI32(String name, int value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueI32(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueU32(String name, long value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueU32(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI64(String name, long value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueI64(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueF32(String name, float value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueF32(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueF64(String name, double value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		long cPtr = gmsecJNI.Field_SetFieldValueF64(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueSTRING(String name, String value)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		if (value == null)
			throw new IllegalArgumentException("Null value");
		long cPtr = gmsecJNI.Field_SetFieldValueSTRING(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @param size - length of binary field
	 * @return Status - operational value
	 */
	public Status SetFieldValueBIN(String name, byte[] value, long size)
	{
		if (name == null)
			throw new IllegalArgumentException("Null name");
		if (value == null)
			throw new IllegalArgumentException("Null value");
		long cPtr = gmsecJNI.Field_SetFieldValueBIN(swigCPtr, this, name, value, size);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Copy constuctor
	 */
	public JNIField(JNIField fd)
	{
		this(gmsecJNI.new_Field_Copy(JNIField.getCPtr(fd), fd), true);
	}

	/**
	 * @brief Get the name of the field
	 * @param name - storage for name
	 * @return Status - operational Status
	 */
	public Status GetName(GMSEC_String name)
	{
		long cPtr = gmsecJNI.Field_GetName(swigCPtr, this, name);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief set the name of the field
	 * @param name - field name
	 * @return Status - operational Status
	 */
	public Status SetName(String name)
	{
		long cPtr = gmsecJNI.Field_SetName(swigCPtr, this, name);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief set the type of field
	 * @param ftype - enumeration of the field type
	 * @return Status - operational result
	 */
	public Status SetType(int ftype)
	{
		long cPtr = gmsecJNI.Field_SetType(swigCPtr, this, ftype);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief get the type of field
	 * @param ftype - storage for type
	 * @return Status - operational result
	 */
	public Status GetType(GMSEC_UShort ftype)
	{
		long cPtr = gmsecJNI.Field_GetType(swigCPtr, this, ftype);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueCHAR(GMSEC_Char value)
	{
		long cPtr = gmsecJNI.Field_GetValueCHAR(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueBOOL(GMSEC_Bool value)
	{
		long cPtr = gmsecJNI.Field_GetValueBOOL(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueBYTE(GMSEC_I8 value)
	{
		long cPtr = gmsecJNI.Field_GetValueI8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueUBYTE(GMSEC_U8 value)
	{
		long cPtr = gmsecJNI.Field_GetValueU8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueSHORT(GMSEC_Short value)
	{
		long cPtr = gmsecJNI.Field_GetValueI16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueUSHORT(GMSEC_UShort value)
	{
		long cPtr = gmsecJNI.Field_GetValueU16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueLONG(GMSEC_Long value)
	{
		long cPtr = gmsecJNI.Field_GetValueI32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueULONG(GMSEC_ULong value)
	{
		long cPtr = gmsecJNI.Field_GetValueU32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueFLOAT(GMSEC_Float value)
	{
		long cPtr = gmsecJNI.Field_GetValueF32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueDOUBLE(GMSEC_Double value)
	{
		long cPtr = gmsecJNI.Field_GetValueF64(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI8(GMSEC_I8 value)
	{
		long cPtr = gmsecJNI.Field_GetValueI8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueU8(GMSEC_U8 value)
	{
		long cPtr = gmsecJNI.Field_GetValueU8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI16(GMSEC_I16 value)
	{
		long cPtr = gmsecJNI.Field_GetValueI16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueU16(GMSEC_U16 value)
	{
		long cPtr = gmsecJNI.Field_GetValueU16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI32(GMSEC_I32 value)
	{
		long cPtr = gmsecJNI.Field_GetValueI32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueU32(GMSEC_U32 value)
	{
		long cPtr = gmsecJNI.Field_GetValueU32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI64(GMSEC_I64 value)
	{
		long cPtr = gmsecJNI.Field_GetValueI64(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueF32(GMSEC_F32 value)
	{
		long cPtr = gmsecJNI.Field_GetValueF32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueF64(GMSEC_F64 value)
	{
		long cPtr = gmsecJNI.Field_GetValueF64(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueSTRING(GMSEC_String value)
	{
		long cPtr = gmsecJNI.Field_GetValueSTRING(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @param size - length of binary data
	 * @return Status - operational result
	 */
	public Status GetValueBIN(GMSEC_Bin value)
	{
		GMSEC_ULong ul = new GMSEC_ULong();
		long cPtr = gmsecJNI.Field_GetValueBIN(swigCPtr, this, value, ul);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set char value
	 * @param value - byte; This method calls the C++ JNI method, where char
	 *            types are set as signed single-byte values, hence the byte
	 * @return Status - the operational result
	 */
	public Status SetValueCHAR(byte value)
	{
		long cPtr = gmsecJNI.Field_SetValueCHAR(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set boolean value
	 * @param value - short 0 == false !0 == true
	 * @return Status - the operational result
	 */
	public Status SetValueBOOL(boolean value)
	{
		long cPtr = gmsecJNI.Field_SetValueBOOL(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set signed 8-bit integer value
	 * @param value - byte
	 * @return Status - the operational result
	 */
	public Status SetValueBYTE(byte value)
	{
		long cPtr = gmsecJNI.Field_SetValueI8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set unsigned 8-bit integer value
	 * @param value - int
	 * @return Status - the operational result
	 */
	public Status SetValueUBYTE(int value)
	{
		long cPtr = gmsecJNI.Field_SetValueU8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set short value
	 * @param value - short
	 * @return Status - the operational result
	 */
	public Status SetValueSHORT(short value)
	{
		long cPtr = gmsecJNI.Field_SetValueI16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set unsigned short value
	 * @param value - int
	 * @return Status - the operational result
	 */
	public Status SetValueUSHORT(int value)
	{
		long cPtr = gmsecJNI.Field_SetValueU16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set long value
	 * @param value - int
	 * @return Status - the operational result
	 */
	public Status SetValueLONG(int value)
	{
		long cPtr = gmsecJNI.Field_SetValueI32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set unsigned long
	 * @param value - long
	 * @return Status - the operational result
	 */
	public Status SetValueULONG(long value)
	{
		long cPtr = gmsecJNI.Field_SetValueU32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set float value
	 * @param value - float
	 * @return Status - the operational result
	 */
	public Status SetValueFLOAT(float value)
	{
		long cPtr = gmsecJNI.Field_SetValueF32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set double value
	 * @param value - double
	 * @return Status - the operational result
	 */
	public Status SetValueDOUBLE(double value)
	{
		long cPtr = gmsecJNI.Field_SetValueF64(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set String value
	 * @param value - String
	 * @return Status - the operational result
	 */
	public Status SetValueSTRING(String value)
	{
		long cPtr = gmsecJNI.Field_SetValueSTRING(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Set binary (BLOB) value
	 * @param value - byte array
	 * @param size - length of byte array
	 * @return Status - the operational result
	 */
	public Status SetValueBIN(byte[] value)
	{
		long cPtr = gmsecJNI.Field_SetValueBIN(swigCPtr, this, value, Array.getLength(value));
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueI8(byte value)
	{
		long cPtr = gmsecJNI.Field_SetValueI8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueU8(int value)
	{
		long cPtr = gmsecJNI.Field_SetValueU8(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueI16(short value)
	{
		long cPtr = gmsecJNI.Field_SetValueI16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueU16(int value)
	{
		long cPtr = gmsecJNI.Field_SetValueU16(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueI32(int value)
	{
		long cPtr = gmsecJNI.Field_SetValueI32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueU32(long value)
	{
		long cPtr = gmsecJNI.Field_SetValueU32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueI64(long value)
	{
		long cPtr = gmsecJNI.Field_SetValueI64(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueF32(float value)
	{
		long cPtr = gmsecJNI.Field_SetValueF32(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	public Status SetValueF64(double value)
	{
		long cPtr = gmsecJNI.Field_SetValueF64(swigCPtr, this, value);
		return new JNIStatus(cPtr, true);
	}

	/**
	 * @brief Recycles this field by unsetting the type and the value
	 * @return Status - the operatinal result
	 */
	public Status UnSet()
	{
		long cPtr = gmsecJNI.Field_UnSet(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}

}
