/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Field.java
 */

package gov.nasa.gsfc.gmsec.api5.field;

import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.U16;
import gov.nasa.gsfc.gmsec.api5.U32;
import gov.nasa.gsfc.gmsec.api5.U64;
import gov.nasa.gsfc.gmsec.api5.jni.field.JNIField;

import java.util.HashMap;
import java.util.Map;



/**
 * Base-class for all field classes.
 */
public class Field
{
	protected JNIField m_jniField = null;


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param field A Field object.
	 * @return A JNIField object.
	 */
	public static JNIField getInternal(Field field)
	{
		return (field == null ? null : field.m_jniField);
	}


	/**
	 * Enumerated list of possible field types.
	 *
	 * Note: Field numbering was chosen to match that of API 3.x
	 * and is preserved so that message encoding/decoding between
	 * APIs 3.x, 4.x, and 5.x are possible.
	 */
	public enum Type
	{
		/** Field containing a character value */
		CHAR(1),

		/** Field containing a boolean value */
		BOOL(2),

		/** Field containing a signed 16-bit value */
		I16(3),

		/** Field containing an unsigned 16-bit value */
		U16(4),

		/** Field containing a signed 32-bit value */
		I32(5),

		/** Field containing an unsigned 32-bit value */
		U32(6),

		/** Field containing a 32-bit float value */
		F32(7),

		/** Field containing a 64-bit float value */
		F64(8),

		/** Field containing a string */
		STRING(9),

		/** Field containing a raw data */
		BINARY(10),

		/** Field containing a signed 8-bit value */
		I8(20),

		/** Field containing an unsigned 8-bit value */
		U8(21),

		/** Field containing a signed 64-bit value */
		I64(22),

		/** Field containing an unsigned 64-bit value */
		U64(23);

		private final int value;

		private static Map<Integer, Type> map = new HashMap<>();

		static {
			for (Type type : Type.values()) {
				map.put(type.value, type);
			}
		}

		private Type(int value) {
			this.value = value;
		}

		/**
		 * Converts the enumerated Field Type to an integer.
		 * @return An integer value.
		 */
		public int getValue() {
			return this.value;
		}

		/**
		 * Converts the integer to a Field Type.
		 * @param type An integer value that represents a Field Type enumerated type.
		 * @return A Field type enumerated value.
		 */
		public static Type valueOf(int type) {
			return map.get(type);
		}
	}


	protected Field()
	{
	}


	protected void validateName(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
	}


	protected void setInternal(JNIField jField)
	{
		m_jniField = jField;
	}


	protected JNIField getInternal()
	{
		return m_jniField;
	}


	/**
	 * Returns the name of this Field object.
	 *
	 * @return The name of the field.
	 */
	public String getName()
	{
		return m_jniField.getName();
	}


	/**
	 * Returns the type of this Field object.
	 *
	 * @return A field type.
	 */
	public Field.Type getType()
	{
		return m_jniField.getType();
	}


	/**
	 * Returns a string containing the XML representation of the field object.
	 *
	 * @return A string.
	 */
	public String toXML()
	{
		return m_jniField.toXML();
	}


	/**
	 * Returns a string containing the JSON representation of the field object.
	 *
	 * @return A string.
	 */
	public String toJSON()
	{
		return m_jniField.toJSON();
	}


	/**
	 * Attempts to convert the field value into string representation.
	 *
	 * @return Returns the field value as a string.
	 */
	public String getStringValue()
	{
		return m_jniField.getStringValue();
	}


	/**
	 * Attempts to convert the field value into a boolean representation.
	 *
	 * @return Returns the field value as a boolean.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to a boolean.
	 */
	public boolean getBooleanValue() throws GmsecException
	{
		return m_jniField.getBooleanValue();
	}


	/**
	 * Attempts to convert the field value into a signed 16-bit integer representation.
	 *
	 * @return Returns the field value as an integer.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to an integer.
	 */
	public int getI16Value() throws GmsecException
	{
		return m_jniField.getI16Value();
	}


	/**
	 * Attempts to convert the field value into a signed 32-bit integer representation.
	 *
	 * @return Returns the field value as an integer.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to an integer.
	 */
	public int getI32Value() throws GmsecException
	{
		return m_jniField.getI32Value();
	}


	/**
	 * Attempts to convert the field value into long integer number representation.
	 *
	 * @return Returns the field value as a long integer.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to an integer.
	 */
	public long getI64Value() throws GmsecException
	{
		return m_jniField.getI64Value();
	}


	/**
	 * Attempts to convert the field value into U16 representation.
	 *
	 * @return Returns the field value as a U16 object.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to a U16 object.
	 */
	public U16 getU16Value() throws GmsecException
	{
		return m_jniField.getU16Value();
	}


	/**
	 * Attempts to convert the field value into U32 representation.
	 *
	 * @return Returns the field value as a U32 object.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to a U32 object.
	 */
	public U32 getU32Value() throws GmsecException
	{
		return m_jniField.getU32Value();
	}


	/**
	 * Attempts to convert the field value into U64 representation.
	 *
	 * @return Returns the field value as a U64 object.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to a U64 object.
	 */
	public U64 getU64Value() throws GmsecException
	{
		return m_jniField.getU64Value();
	}


	/**
	 * Attempts to convert the field value into a 64-bit floating point number representation.
	 *
	 * @return Returns the field value as a floating point number.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api5.GmsecException Thrown if the field cannot successfully be converted to a double.
	 */
	public double getF64Value() throws GmsecException
	{
		return m_jniField.getF64Value();
	}


	/**
	 * Returns whether the Field is a non-header or header field.
	 *
	 * @return True if header field, false otherwise.
	 */
	public boolean isHeader()
	{
		return m_jniField.isHeader();
	}
}
