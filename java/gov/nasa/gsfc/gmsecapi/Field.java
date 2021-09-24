/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



package gov.nasa.gsfc.gmsecapi;


/** @class Field
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is the base class for GMSEC message fields.
 *
 */
@Deprecated
public class Field
{
	protected static final int DEFAULT_XML_FIELD_SIZE = 150;

	protected static final String XML_FIELD_CHAR_HEAD = "<FIELD TYPE=\"CHAR\" NAME=\"";

	protected static final String XML_FIELD_BOOL_HEAD = "<FIELD TYPE=\"BOOL\" NAME=\"";

	protected static final String XML_FIELD_SHORT_HEAD = "<FIELD TYPE=\"SHORT\" NAME=\"";

	protected static final String XML_FIELD_USHORT_HEAD = "<FIELD TYPE=\"USHORT\" NAME=\"";

	protected static final String XML_FIELD_LONG_HEAD = "<FIELD TYPE=\"LONG\" NAME=\"";

	protected static final String XML_FIELD_ULONG_HEAD = "<FIELD TYPE=\"ULONG\" NAME=\"";

	protected static final String XML_FIELD_FLOAT_HEAD = "<FIELD TYPE=\"FLOAT\" NAME=\"";

	protected static final String XML_FIELD_DOUBLE_HEAD = "<FIELD TYPE=\"DOUBLE\" NAME=\"";

	protected static final String XML_FIELD_STRING_HEAD = "<FIELD TYPE=\"STRING\" NAME=\"";

	protected static final String XML_FIELD_BIN_HEAD = "<FIELD TYPE=\"BIN\" NAME=\"";

	protected static final String XML_FIELD_I8_HEAD = "<FIELD TYPE=\"I8\" NAME=\"";

	protected static final String XML_FIELD_U8_HEAD = "<FIELD TYPE=\"U8\" NAME=\"";

	protected static final String XML_FIELD_U16_HEAD = "<FIELD TYPE=\"U16\" NAME=\"";

	protected static final String XML_FIELD_I16_HEAD = "<FIELD TYPE=\"I16\" NAME=\"";

	protected static final String XML_FIELD_U32_HEAD = "<FIELD TYPE=\"U32\" NAME=\"";

	protected static final String XML_FIELD_I32_HEAD = "<FIELD TYPE=\"I32\" NAME=\"";

	protected static final String XML_FIELD_I64_HEAD = "<FIELD TYPE=\"I64\" NAME=\"";

	protected static final String XML_FIELD_F32_HEAD = "<FIELD TYPE=\"F32\" NAME=\"";

	protected static final String XML_FIELD_F64_HEAD = "<FIELD TYPE=\"F64\" NAME=\"";

	protected static final String XML_FIELD_HEAD_END = "\">";

	protected static final String XML_FIELD_CLOSE = "</FIELD>";

	protected static final String XML_FIELD_BOOL_TRUE = "TRUE";

	protected static final String XML_FIELD_BOOL_FALSE = "FALSE";

	private int fType = gmsecAPI.GMSEC_TYPE_UNSET;

	private String fName = new String();

	private GMSEC_Bin fBinValue = null;

	private GMSEC_Bool fBoolValue = null;

	private GMSEC_String fStringValue = null;

	private GMSEC_Char fCharValue = null;

	private GMSEC_U8 fU8Value = null;

	private GMSEC_I8 fI8Value = null;

	private GMSEC_U16 fU16Value = null;

	private GMSEC_I16 fI16Value = null;

	private GMSEC_U32 fU32Value = null;

	private GMSEC_I32 fI32Value = null;

	private GMSEC_I64 fI64Value = null;

	private GMSEC_F32 fF32Value = null;

	private GMSEC_F64 fF64Value = null;

	/**
	 * @brief Create an empty field of type UNSET
	 */
	public Field()
	{
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueCHAR(String name, byte value)
	{
		fType = gmsecAPI.GMSEC_TYPE_CHAR;
		fName = name;
		fCharValue = new GMSEC_Char(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * 
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueBOOL(String name, boolean value)
	{
		fType = gmsecAPI.GMSEC_TYPE_BOOL;
		fName = name;
		fBoolValue = new GMSEC_Bool(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * 
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueBYTE(String name, byte value)
	{
		return SetFieldValueI8(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * 
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueUBYTE(String name, int value)
	{
		return SetFieldValueU8(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * 
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueSHORT(String name, short value)
	{
		return SetFieldValueI16(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * 
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueUSHORT(String name, int value)
	{
		return SetFieldValueU16(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * 
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueLONG(String name, int value)
	{
		return SetFieldValueI32(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueULONG(String name, long value)
	{
		return SetFieldValueU32(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueFLOAT(String name, float value)
	{
		return SetFieldValueF32(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueDOUBLE(String name, double value)
	{
		return SetFieldValueF64(name, value);
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueSTRING(String name, String value)
	{
		fType = gmsecAPI.GMSEC_TYPE_STRING;
		fName = name;
		fStringValue = new GMSEC_String(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueBIN(String name, byte[] value)
	{
		fType = gmsecAPI.GMSEC_TYPE_BIN;
		fName = name;
		fBinValue = new GMSEC_Bin(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueU8(String name, int value)
	{
		fType = gmsecAPI.GMSEC_TYPE_U8;
		fName = name;
		fU8Value = new GMSEC_U8(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI8(String name, byte value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I8;
		fName = name;
		fI8Value = new GMSEC_I8(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueU16(String name, int value)
	{
		fType = gmsecAPI.GMSEC_TYPE_U16;
		fName = name;
		fU16Value = new GMSEC_U16(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI16(String name, short value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I16;
		fName = name;
		fI16Value = new GMSEC_I16(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueU32(String name, long value)
	{
		fType = gmsecAPI.GMSEC_TYPE_U32;
		fName = name;
		fU32Value = new GMSEC_U32(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI32(String name, int value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I32;
		fName = name;
		fI32Value = new GMSEC_I32(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueI64(String name, long value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I64;
		fName = name;
		fI64Value = new GMSEC_I64(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueF32(String name, float value)
	{
		fType = gmsecAPI.GMSEC_TYPE_F32;
		fName = name;
		fF32Value = new GMSEC_F32(value);
		return new Status();
	}

	/**
	 * @brief Assign field name, type, and value
	 * @param name - field name
	 * @param value - value of field
	 * @return Status - operational value
	 */
	public Status SetFieldValueF64(String name, double value)
	{
		fType = gmsecAPI.GMSEC_TYPE_F64;
		fName = name;
		fF64Value = new GMSEC_F64(value);
		return new Status();
	}

	/**
	 * @brief Copy constuctor
	 */
	public Field(Field fd)
	{
		Set(fd);
	}

	public void Set(Field fd)
	{
		// IMPORTANT:
		// Don't assume fd is a Field object; it may be a JNIField object,
		// which is a subclass of Field, and which implements its own
		// version of the Set/Get accessor methods.
		//
		// The JNIField sets and gets values via the JNI interface, thus
		// none of its base-class attributes are ever set.  For this reason,
		// it is necessary to perform Get-attribute operations when attempting
		// to access data regarding the field object that is passed to this method.
		//
		// In summary, do NOT attempt/bother to directly access the member
		// data of the Field object that is passed to this method.  Use the Get-
		// methods.

		if (fd == null)
		{
			return;
		}

		GMSEC_String name = new GMSEC_String();
		fd.GetName(name);
		this.SetName( (name.Get() == null ? "" : name.Get()) );

		GMSEC_UShort ftype = new GMSEC_UShort();
		fd.GetType(ftype);
		this.SetType(ftype.Get());

		switch (ftype.Get())
		{
			case gmsecAPI.GMSEC_TYPE_UNSET:
				break;
			case gmsecAPI.GMSEC_TYPE_BOOL:
			{
				GMSEC_Bool c = new GMSEC_Bool();
				fd.GetValueBOOL(c);
				this.SetValueBOOL(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_CHAR:
			{
				GMSEC_Char c = new GMSEC_Char();
				fd.GetValueCHAR(c);
				this.SetValueCHAR(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_STRING:
			{
				GMSEC_String c = new GMSEC_String();
				fd.GetValueSTRING(c);
				if (c.Get() != null)
				{
					this.SetValueSTRING(c.Get());
				}
				break;
			}
			case gmsecAPI.GMSEC_TYPE_BIN:
			{
				GMSEC_Bin c = new GMSEC_Bin();
				fd.GetValueBIN(c);
				if (c.Get() != null)
				{
					this.SetValueBIN(c.Get());
				}
				break;
			}
			case gmsecAPI.GMSEC_TYPE_U8:
			{
				GMSEC_U8 c = new GMSEC_U8();
				fd.GetValueU8(c);
				this.SetValueU8(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_I8:
			{
				GMSEC_I8 c = new GMSEC_I8();
				fd.GetValueI8(c);
				this.SetValueI8(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_U16:
			{
				GMSEC_U16 c = new GMSEC_U16();
				fd.GetValueU16(c);
				this.SetValueU16(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_I16:
			{
				GMSEC_I16 c = new GMSEC_I16();
				fd.GetValueI16(c);
				this.SetValueI16(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_U32:
			{
				GMSEC_U32 c = new GMSEC_U32();
				fd.GetValueU32(c);
				this.SetValueU32(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_I32:
			{
				GMSEC_I32 c = new GMSEC_I32();
				fd.GetValueI32(c);
				this.SetValueI32(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_I64:
			{
				GMSEC_I64 c = new GMSEC_I64();
				fd.GetValueI64(c);
				this.SetValueI64(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_F32:
			{
				GMSEC_F32 c = new GMSEC_F32();
				fd.GetValueF32(c);
				this.SetValueF32(c.Get());
				break;
			}
			case gmsecAPI.GMSEC_TYPE_F64:
			{
				GMSEC_F64 c = new GMSEC_F64();
				fd.GetValueF64(c);
				this.SetValueF64(c.Get());
				break;
			}
		}
	}

	/**
	 * @brief Get the name of the field
	 * @param name - storage for name
	 * @return Status - operational Status
	 */
	public Status GetName(GMSEC_String name)
	{
		if (name == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR,
					gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
					"Name is null");
		}

		name.Set(fName);
		return new Status();
	}

	/**
	 * @brief set the name of the field
	 * 
	 * @param name - field name
	 * @return Status - operational Status
	 */
	public Status SetName(String name)
	{
		if (name == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR,
					gmsecAPI.GMSEC_UNINITIALIZED_OBJECT,
					"Name is null");
		}

		fName = name;
		return new Status();
	}

	/**
	 * @brief set the type of field
	 * @param ft - enumeration of the field type
	 * @return Status - operational result
	 */
	public Status SetType(int ft)
	{
		fType = ft;
		return new Status();
	}

	/**
	 * @brief get the type of field
	 * @param ftype - storage for type
	 * @return Status - operational result
	 */
	public Status GetType(GMSEC_UShort ftype)
	{
		ftype.Set(fType);
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueCHAR(GMSEC_Char value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_CHAR)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fCharValue == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fCharValue.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueBOOL(GMSEC_Bool value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_BOOL)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fBoolValue == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fBoolValue.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueBYTE(GMSEC_I8 value)
	{
		return GetValueI8(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueUBYTE(GMSEC_U8 value)
	{
		return GetValueU8(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueSHORT(GMSEC_Short value)
	{
		return GetValueI16(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueUSHORT(GMSEC_UShort value)
	{
		return GetValueU16(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueLONG(GMSEC_Long value)
	{
		return GetValueI32(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueULONG(GMSEC_ULong value)
	{
		return GetValueU32(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueFLOAT(GMSEC_Float value)
	{
		return GetValueF32(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueDOUBLE(GMSEC_Double value)
	{
		return GetValueF64(value);
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueSTRING(GMSEC_String value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_STRING)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fStringValue == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fStringValue.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueBIN(GMSEC_Bin value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_BIN)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fBinValue == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fBinValue.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueU8(GMSEC_U8 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_U8)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fU8Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fU8Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI8(GMSEC_I8 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_I8)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fI8Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fI8Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueU16(GMSEC_U16 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_U16)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fU16Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fU16Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI16(GMSEC_I16 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_I16)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fI16Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fI16Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueU32(GMSEC_U32 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_U32)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fU32Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fU32Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI32(GMSEC_I32 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_I32)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fI32Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fI32Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueI64(GMSEC_I64 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_I64)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fI64Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fI64Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueF32(GMSEC_F32 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_F32)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fF32Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fF32Value.Get());
		return new Status();
	}

	/**
	 * @brief Get the type from the field
	 * @param value - storage for type
	 * @return Status - operational result
	 */
	public Status GetValueF64(GMSEC_F64 value)
	{
		if (fType != gmsecAPI.GMSEC_TYPE_F64)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_FIELD_TYPE_MISMATCH, "Field is not of requested type");
		}
		if (fF64Value == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Field value has not been set");
		}
		value.Set(fF64Value.Get());
		return new Status();
	}

	/**
	 * @brief Set char value
	 * @param value - byte; This method calls the C++ JNI method, where char types
	 *                are set as signed single-byte values, hence the byte
	 * @return Status - the operational result
	 */
	public Status SetValueCHAR(byte value)
	{
		fType = gmsecAPI.GMSEC_TYPE_CHAR;
		fCharValue = new GMSEC_Char(value);
		return new Status();
	}

	/**
	 * @brief Set boolean value
	 * @param value - short 0 == false !0 == true
	 * @return Status - the operational result
	 */
	public Status SetValueBOOL(boolean value)
	{
		fType = gmsecAPI.GMSEC_TYPE_BOOL;
		fBoolValue = new GMSEC_Bool(value);
		return new Status();
	}

	/**
	 * @brief Set signed 8-bit value
	 * @param value - byte value
	 * @return Status - the operational result
	 */
	public Status SetValueBYTE(byte value)
	{
		return SetValueI8(value);
	}

	/**
	 * @brief Set unsigned 8-bit value
	 * @param value - int
	 * @return Status - the operational result
	 */
	public Status SetValueUBYTE(int value)
	{
		return SetValueU8(value);
	}

	/**
	 * @brief Set short value
	 * @param value - short
	 * @return Status - the operational result
	 */
	public Status SetValueSHORT(short value)
	{
		return SetValueI16(value);
	}

	/**
	 * @brief Set unsigned short value
	 * @param value - int
	 * @return Status - the operational result
	 */
	public Status SetValueUSHORT(int value)
	{
		return SetValueU16(value);
	}

	/**
	 * @brief Set long value
	 * @param value - int
	 * @return Status - the operational result
	 */
	public Status SetValueLONG(int value)
	{
		return SetValueI32(value);
	}

	/**
	 * @brief Set unsigned long
	 * @param value - long
	 * @return Status - the operational result
	 */
	public Status SetValueULONG(long value)
	{
		return SetValueU32(value);
	}

	/**
	 * @brief Set float value
	 * @param value - float
	 * @return Status - the operational result
	 */
	public Status SetValueFLOAT(float value)
	{
		return SetValueF32(value);
	}

	/**
	 * @brief Set double value
	 * @param value - double
	 * @return Status - the operational result
	 */
	public Status SetValueDOUBLE(double value)
	{
		return SetValueF64(value);
	}

	/**
	 * @brief Set String value
	 * @param value - String
	 * @return Status - the operational result
	 */
	public Status SetValueSTRING(String value)
	{
		fType = gmsecAPI.GMSEC_TYPE_STRING;
		fStringValue = new GMSEC_String(value);
		return new Status();
	}

	/**
	 * @brief Set binary (BLOB) value
	 * @param value - byte array
	 * @return Status - the operational result
	 */
	public Status SetValueBIN(byte[] value)
	{
		fType = gmsecAPI.GMSEC_TYPE_BIN;
		fBinValue = new GMSEC_Bin(value);
		return new Status();
	}

	public Status SetValueU8(int value)
	{
		fType = gmsecAPI.GMSEC_TYPE_U8;
		fU8Value = new GMSEC_U8(value);
		return new Status();
	}

	public Status SetValueI8(byte value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I8;
		fI8Value = new GMSEC_I8(value);
		return new Status();
	}

	public Status SetValueU16(int value)
	{
		fType = gmsecAPI.GMSEC_TYPE_U16;
		fU16Value = new GMSEC_U16(value);
		return new Status();
	}

	public Status SetValueI16(short value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I16;
		fI16Value = new GMSEC_I16(value);
		return new Status();
	}

	public Status SetValueU32(long value)
	{
		fType = gmsecAPI.GMSEC_TYPE_U32;
		fU32Value = new GMSEC_U32(value);
		return new Status();
	}

	public Status SetValueI32(int value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I32;
		fI32Value = new GMSEC_I32(value);
		return new Status();
	}

	public Status SetValueI64(long value)
	{
		fType = gmsecAPI.GMSEC_TYPE_I64;
		fI64Value = new GMSEC_I64(value);
		return new Status();
	}

	public Status SetValueF32(float value)
	{
		fType = gmsecAPI.GMSEC_TYPE_F32;
		fF32Value = new GMSEC_F32(value);
		return new Status();
	}

	public Status SetValueF64(double value)
	{
		fType = gmsecAPI.GMSEC_TYPE_F64;
		fF64Value = new GMSEC_F64(value);
		return new Status();
	}

	/**
	 * @brief Recycles this field by unsetting the type and the value
	 * @return Status - the operatinal result
	 */
	public Status UnSet()
	{
		fName      = "";
		fType      = gmsecAPI.GMSEC_TYPE_UNSET;
		fBinValue  = null;
		fBoolValue = null;
		fCharValue = null;
		fU8Value   = null;
		fI8Value   = null;
		fU16Value  = null;
		fI16Value  = null;
		fU32Value  = null;
		fI32Value  = null;
		fI64Value  = null;
		fF32Value  = null;
		fF64Value  = null;
		fStringValue = null;
		return new Status();
	}

	public Status ToXML(GMSEC_String outst)
	{
		Status result = new Status();
		StringBuffer xmlBuf = new StringBuffer(DEFAULT_XML_FIELD_SIZE);

		String name = "";
		if (null != fName)
			name = fName;

		GMSEC_UShort ftype = new GMSEC_UShort();
		GetType(ftype);

		switch (ftype.Get())
		{
			case gmsecAPI.GMSEC_TYPE_BOOL:
				if (fBoolValue.Get())
					xmlBuf.append(XML_FIELD_BOOL_HEAD).append(name).append(XML_FIELD_HEAD_END).append(XML_FIELD_BOOL_TRUE).append(XML_FIELD_CLOSE);
				else
					xmlBuf.append(XML_FIELD_BOOL_HEAD).append(name).append(XML_FIELD_HEAD_END).append(XML_FIELD_BOOL_FALSE).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_CHAR:
				xmlBuf.append(XML_FIELD_CHAR_HEAD).append(name).append(XML_FIELD_HEAD_END);
				if (fCharValue.Get() == 0)
					xmlBuf.append("nul");
				else
					xmlBuf.append((char)fCharValue.Get());
				xmlBuf.append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_STRING:
				xmlBuf.append(XML_FIELD_STRING_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fStringValue).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_BIN:
				xmlBuf.append(XML_FIELD_BIN_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fBinValue).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_U8:
				xmlBuf.append(XML_FIELD_U8_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fU8Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_I8:
				xmlBuf.append(XML_FIELD_I8_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fI8Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_U16:
				xmlBuf.append(XML_FIELD_U16_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fU16Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_I16:
				xmlBuf.append(XML_FIELD_I16_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fI16Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_U32:
				xmlBuf.append(XML_FIELD_U32_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fU32Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_I32:
				xmlBuf.append(XML_FIELD_I32_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fI32Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_I64:
				xmlBuf.append(XML_FIELD_I64_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fI64Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_F32:
				xmlBuf.append(XML_FIELD_F32_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fF32Value.Get()).append(XML_FIELD_CLOSE);
				break;
			case gmsecAPI.GMSEC_TYPE_F64:
				xmlBuf.append(XML_FIELD_F64_HEAD).append(name).append(XML_FIELD_HEAD_END).append(fF64Value.Get()).append(XML_FIELD_CLOSE);
				break;
			default:
				result.Set(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_UNKNOWN_FIELD_TYPE, "Field has unknown or unset type.");
				break;

		}

		outst.Set(xmlBuf.toString());
		return result;
	}

	public static int LookupType(String ftype)
	{
		// TODO: use a table lookup
		if (ftype == null)
			return gmsecAPI.GMSEC_TYPE_UNSET;
		else if (ftype.equalsIgnoreCase("CHAR"))
			return gmsecAPI.GMSEC_TYPE_CHAR;
		else if (ftype.equalsIgnoreCase("BOOL"))
			return gmsecAPI.GMSEC_TYPE_BOOL;
		else if (ftype.equalsIgnoreCase("SHORT"))
			return gmsecAPI.GMSEC_TYPE_SHORT;
		else if (ftype.equalsIgnoreCase("USHORT"))
			return gmsecAPI.GMSEC_TYPE_USHORT;
		else if (ftype.equalsIgnoreCase("LONG"))
			return gmsecAPI.GMSEC_TYPE_LONG;
		else if (ftype.equalsIgnoreCase("ULONG"))
			return gmsecAPI.GMSEC_TYPE_ULONG;
		else if (ftype.equalsIgnoreCase("FLOAT"))
			return gmsecAPI.GMSEC_TYPE_FLOAT;
		else if (ftype.equalsIgnoreCase("DOUBLE"))
			return gmsecAPI.GMSEC_TYPE_DOUBLE;
		else if (ftype.equalsIgnoreCase("STRING"))
			return gmsecAPI.GMSEC_TYPE_STRING;
		else if (ftype.equalsIgnoreCase("BIN"))
			return gmsecAPI.GMSEC_TYPE_BIN;
		else if (ftype.equalsIgnoreCase("I8"))
			return gmsecAPI.GMSEC_TYPE_I8;
		else if (ftype.equalsIgnoreCase("U8"))
			return gmsecAPI.GMSEC_TYPE_U8;
		else if (ftype.equalsIgnoreCase("U16"))
			return gmsecAPI.GMSEC_TYPE_U16;
		else if (ftype.equalsIgnoreCase("I16"))
			return gmsecAPI.GMSEC_TYPE_I16;
		else if (ftype.equalsIgnoreCase("U32"))
			return gmsecAPI.GMSEC_TYPE_U32;
		else if (ftype.equalsIgnoreCase("I32"))
			return gmsecAPI.GMSEC_TYPE_I32;
		else if (ftype.equalsIgnoreCase("I64"))
			return gmsecAPI.GMSEC_TYPE_I32;
		else if (ftype.equalsIgnoreCase("F32"))
			return gmsecAPI.GMSEC_TYPE_F32;
		else if (ftype.equalsIgnoreCase("F64"))
			return gmsecAPI.GMSEC_TYPE_F64;
		else
			return gmsecAPI.GMSEC_TYPE_UNSET;
	}

	private char lookupHexDigit(char digit)
	{
		switch (digit)
		{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				return (char)((digit - 'a') + 10);
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return (char)((digit - 'A') + 10);
			default:
				return (char)(digit - '0');
		}
	}

	public Status SetValue(int ftype, String fvalue)
	{
		Status result = new Status();

		switch (ftype)
		{
			case gmsecAPI.GMSEC_TYPE_BOOL:
				this.SetValueBOOL(Boolean.valueOf(fvalue).booleanValue());
				break;
			case gmsecAPI.GMSEC_TYPE_CHAR:
			{
				byte b = 0;
				if (fvalue.equals("nul"))
					b = 0;
				else if (fvalue.length() > 0)
					b = fvalue.getBytes()[0];
				else
					result.Set(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_INVALID_FIELD_VALUE, "Invalid CHAR value");
				this.SetValueCHAR(b);
				break;
			}
			case gmsecAPI.GMSEC_TYPE_STRING:
				this.SetValueSTRING(fvalue);
				break;
			case gmsecAPI.GMSEC_TYPE_BIN:
				if (fvalue.length() % 2 != 0)
				{
					result.Set(gmsecAPI.GMSEC_STATUS_FIELD_ERROR, gmsecAPI.GMSEC_INVALID_FIELD_VALUE, "Invalid hex value.  An even number of hex digits is required.");
					return result;
				}

				byte blob[] = new byte[fvalue.length() / 2];
				for (int i = 0; i < (fvalue.length() / 2); i++)
				{
					blob[i] = (byte)((lookupHexDigit(fvalue.charAt(i * 2)) * 16) + lookupHexDigit(fvalue.charAt((i * 2) + 1)));
				}

				this.SetValueBIN(blob);
				break;
			case gmsecAPI.GMSEC_TYPE_U8:
				this.SetValueU8(Integer.parseInt(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_I8:
				this.SetValueI8(Byte.parseByte(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_U16:
				this.SetValueU16(Integer.parseInt(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_I16:
				this.SetValueI16(Short.parseShort(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_U32:
				this.SetValueU32(Long.parseLong(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_I32:
				this.SetValueI32(Integer.parseInt(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_I64:
				this.SetValueI32(Integer.parseInt(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_F32:
				this.SetValueF32(Float.parseFloat(fvalue));
				break;
			case gmsecAPI.GMSEC_TYPE_F64:
				this.SetValueF64(Double.parseDouble(fvalue));
				break;
			default:
				break;
		}

		return result;
	}

	public String toString()
	{
		GMSEC_String xml = new GMSEC_String();
		this.ToXML(xml);
		return xml.Get();
	}
}
