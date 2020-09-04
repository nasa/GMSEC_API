/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Field.java
 *
 * @brief Base-class for all field classes.
 */

package gov.nasa.gsfc.gmsec.api.field;

import gov.nasa.gsfc.gmsec.api.U64;
import gov.nasa.gsfc.gmsec.api.jni.JNIField;



/**
 * @class Field
 *
 * @brief Base-class for all field classes.
 */
public class Field
{
	protected JNIField m_jniField = null;


	public static JNIField getInternal(Field field)
	{
		return (field == null ? null : field.m_jniField);
	}


	/**
	 * @enum FieldType
	 *
	 * @desc Enumerated list of possible field types.
	 */
	public enum FieldType
	{
		BIN_TYPE,      ///< Field containing a raw data
		BOOL_TYPE,     ///< Field containing a boolean value
		CHAR_TYPE,     ///< Field containing a character value
		I8_TYPE,       ///< Field containing a signed 8-bit value
		I16_TYPE,      ///< Field containing a signed 16-bit value
		I32_TYPE,      ///< Field containing a signed 32-bit value
		I64_TYPE,      ///< Field containing a signed 64-bit value
		F32_TYPE,      ///< Field containing a 16-bit float value
		F64_TYPE,      ///< Field containing a 64-bit float value
		STRING_TYPE,   ///< Field containing a string
		U8_TYPE,       ///< Field containing an unsigned 8-bit value
		U16_TYPE,      ///< Field containing an unsigned 16-bit value
		U32_TYPE,      ///< Field containing an unsigned 32-bit value
		U64_TYPE       ///< Field containing an unsigned 64-bit value
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
	 * @fn String getName()
	 *
	 * @return Returns the name of this Field object.
	 */
	public String getName()
	{
		return m_jniField.getName();
	}


	/**
	 * @fn Field.FieldType getType()
	 *
	 * @return Returns the type of this Field object.
	 */
	public Field.FieldType getType()
	{
		return m_jniField.getType();
	}


	/**
	 * @fn String toXML()
	 *
	 * @return Returns a string containing the XML representation of the field object.
	 */
	public String toXML()
	{
		return m_jniField.toXML();
	}


	/**
	 * @fn String toJSON()
	 *
	 * @return Returns a string containing the JSON representation of the field object.
	 */
	public String toJSON()
	{
		return m_jniField.toJSON();
	}


	/**
	 * @fn long getIntegerValue() const
	 *
	 * @brief Attempts to convert the field value into long integer number representation.
	 *
	 * @return Returns the field value as an integer.
	 *
	 * @throws An Exception is thrown if the field cannot be successfully converted to a long integer.
	 */
	public long getIntegerValue()
	{
		return m_jniField.getIntegerValue();
	}


	/**
	 * @fn U64 getUnsignedIntegerValue() const
	 *
	 * @brief Attempts to convert the field value into U64 representation.
	 *
	 * @return Returns the field value as an integer.
	 *
	 * @throws An Exception is thrown if the field cannot be successfully converted to U64.
	 */
	public U64 getUnsignedIntegerValue()
	{
		return m_jniField.getUnsignedIntegerValue();
	}


	/**
	 * @fn double getDoubleValue() const
	 *
	 * @brief Attempts to convert the field value into a 64-bit floating point number representation.
	 *
	 * @return Returns the field value as a floating point number.
	 *
	 * @throws An Exception is thrown if the field cannot be successfully converted to a double.
	 */
	public double getDoubleValue()
	{
		return m_jniField.getDoubleValue();
	}


	/**
	 * @fn String getStringValue()
	 *
	 * @brief Attempts to convert the field value into string representation.
	 *
	 * @return Returns the field value as a string.
	 *
	 * @throws An Exception is thrown if the field cannot be successfully converted to a string.
	 */
	public String getString()
	{
		return m_jniField.getStringValue();
	}


	/**
	 * @fn void isHeader(boolean header)
	 *
	 * @brief Can be used to denote whether this is a non-header or header field.
	 * By default, fields are constructed as non-header.
	 */
	public void isHeader(boolean header)
	{
		m_jniField.isHeader(header);
	}


	/**
	 * @fn boolean isHeader()
	 *
	 * @brief Can be used to determine if this is a non-header or header field.
	 */
	public boolean isHeader()
	{
		return m_jniField.isHeader();
	}
}
