/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Field.java
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.U64;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;



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
	 */
	public enum FieldType
	{
		/** Field containing a raw data */
		BIN_TYPE,

		/** Field containing a boolean value */
		BOOL_TYPE,

		/** Field containing a character value */
		CHAR_TYPE,

		/** Field containing a signed 8-bit value */
		I8_TYPE,

		/** Field containing a signed 16-bit value */
		I16_TYPE,

		/** Field containing a signed 32-bit value */
		I32_TYPE,

		/** Field containing a signed 64-bit value */
		I64_TYPE,

		/** Field containing a 32-bit float value */
		F32_TYPE,

		/** Field containing a 64-bit float value */
		F64_TYPE,

		/** Field containing a string */
		STRING_TYPE,

		/** Field containing an unsigned 8-bit value */
		U8_TYPE,

		/** Field containing an unsigned 16-bit value */
		U16_TYPE,

		/** Field containing an unsigned 32-bit value */
		U32_TYPE,

		/** Field containing an unsigned 64-bit value */
		U64_TYPE
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
	 * Sets the field name of the Field object.
	 *
	 * @param name The name of the field.
	 *
	 * @throws IllegalArgumentException Thrown if the name string is null or is empty.
	 */
	public void setName(String name)
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name cannot be null or contain empty string");
		}

		m_jniField.setName(name);
	}


	/**
	 * Returns the type of this Field object.
	 *
	 * @return A field type.
	 */
	public Field.FieldType getType()
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
	 * Attempts to convert the field value into long integer number representation.
	 *
	 * @return Returns the field value as an integer.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api.GMSEC_Exception Thrown if the field cannot successfully be converted to an integer.
	 */
	public long getIntegerValue() throws GMSEC_Exception
	{
		return m_jniField.getIntegerValue();
	}


	/**
	 * Attempts to convert the field value into U64 representation.
	 *
	 * @return Returns the field value as an integer.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api.GMSEC_Exception Thrown if the field cannot successfully be converted to a U64 object."
	 */
	public U64 getUnsignedIntegerValue() throws GMSEC_Exception
	{
		return m_jniField.getUnsignedIntegerValue();
	}


	/**
	 * Attempts to convert the field value into a 64-bit floating point number representation.
	 *
	 * @return Returns the field value as a floating point number.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api.GMSEC_Exception Thrown if the field cannot successfully be converted to an double.
	 */
	public double getDoubleValue() throws GMSEC_Exception
	{
		return m_jniField.getDoubleValue();
	}


	/**
	 * Attempts to convert the field value into string representation.
	 *
	 * @return Returns the field value as a string.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api.GMSEC_Exception Thrown if the field cannot successfully be converted to a string.
	 */
	public String getStringValue() throws GMSEC_Exception
	{
		return m_jniField.getStringValue();
	}


	/**
	 * Attempts to convert the field value into string representation.
	 *
	 * @return Returns the field value as a string.
	 *
	 * @throws gov.nasa.gsfc.gmsec.api.GMSEC_Exception Thrown if the field cannot successfully be converted to a string.
	 *
	 * @deprecated As of release 4.3, replaced by {@link Field#getStringValue()}.
	 */
	@Deprecated
	public String getString() throws GMSEC_Exception
	{
		return m_jniField.getStringValue();
	}


	/**
	 * Can be used to denote whether this is a non-header or header field.
	 * By default, fields are constructed as non-header.
	 *
	 * @param header Flag to denote whether this is a header field or not.
	 */
	public void isHeader(boolean header)
	{
		m_jniField.isHeader(header);
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


	/**
	 * Creates/returns a clone of the Field object.
	 *
	 * @return A clone of the Field object.
	 */
	public Field clone()
	{
		return m_jniField.clone();
	}
}
