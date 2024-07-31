/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Field.java
 */

package gmsec;


/**
 * Base-class for all field classes.
 */
public class Field
{
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
	}


	protected Field()
	{
	}


	/**
	 * Returns the name of this Field object.
	 *
	 * @return The name of the field.
	 */
	public getName();


	/**
	 * Returns the type of this Field object.
	 *
	 * @return A Field.Type.
	 */
	public getType();


	/**
	 * Returns a string containing the XML representation of the field object.
	 *
	 * @return An XML string.
	 */
	public toXML();


	/**
	 * Returns a string containing the JSON representation of the field object.
	 *
	 * @return A JSON string.
	 */
	public toJSON();


	/**
	 * Attempts to convert the field value into string representation.
	 *
	 * @return Returns the field value as a string.
	 */
	public getStringValue();


	/**
	 * Attempts to convert the field value into a boolean representation.
	 *
	 * @return Returns the field value as a boolean.
	 *
	 * @throws GmsecException Thrown if the field cannot successfully be converted to a boolean.
	 */
	public getBooleanValue();


	/**
	 * Attempts to convert the field value into an integer representation.
	 *
	 * @return Returns the field value as an integer.
	 *
	 * @throws GmsecException Thrown if the field cannot successfully be converted to an integer.
	 */
	public getIntegerValue();


	/**
	 * Attempts to convert the field value into a floating point number representation.
	 *
	 * @return Returns the field value as a floating point number.
	 *
	 * @throws GmsecException Thrown if the field cannot successfully be converted to a double.
	 */
	public getDoubleValue();


	/**
	 * Returns whether the Field is a non-header or header field.
	 *
	 * @return True if a header field, false otherwise.
	 */
	public isHeader();


	/**
	 * Returns whether the Field is a GMSEC API tracking field.
	 *
	 * @return True if a tracking field, false otherwise.
	 */
	public isTracking();


	/**
	 * Downcasts the Field object into a BinaryField.
	 *
	 * @param field The field object to downcast to a BinaryField.
	 *
	 * @return A BinaryField object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a BinaryField object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toBinaryField(field);


	/**
	 * Downcasts the Field object into a BooleanField.
	 *
	 * @param field The field object to downcast to a BooleanField.
	 *
	 * @return A BooleanField object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a BooleanField object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toBooleanField(field);


	/**
	 * Downcasts the Field object into a CharField.
	 *
	 * @param field The field object to downcast to a CharField.
	 *
	 * @return A CharField object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a CharField object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toCharField(field);


	/**
	 * Downcasts the Field object into an F32Field.
	 *
	 * @param field The field object to downcast to an F32Field.
	 *
	 * @return An F32Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to an F32Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toF32Field(field);


	/**
	 * Downcasts the Field object into an F64Field.
	 *
	 * @param field The field object to downcast to an F64Field.
	 *
	 * @return An F64Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to an F64Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toF64Field(field);


	/**
	 * Downcasts the Field object into an I16Field.
	 *
	 * @param field The field object to downcast to an I16Field.
	 *
	 * @return An I16Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to an I16Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toI16Field(field);


	/**
	 * Downcasts the Field object into an I32Field.
	 *
	 * @param field The field object to downcast to an I32Field.
	 *
	 * @return An I32Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to an I32Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toI32Field(field);


	/**
	 * Downcasts the Field object into an I64Field.
	 *
	 * @param field The field object to downcast to an I64Field.
	 *
	 * @return An I64Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to an I64Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toI64Field(field);


	/**
	 * Downcasts the Field object into an I8Field.
	 *
	 * @param field The field object to downcast to an I8Field.
	 *
	 * @return An I8Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to an I8Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toI8Field(field);


	/**
	 * Downcasts the Field object into a StringField.
	 *
	 * @param field The field object to downcast to a StringField.
	 *
	 * @return A StringField object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a StringField object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toStringField(field);


	/**
	 * Downcasts the Field object into a U16Field.
	 *
	 * @param field The field object to downcast to a U16Field.
	 *
	 * @return A U16Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a U16Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toU16Field(field);


	/**
	 * Downcasts the Field object into a U32Field.
	 *
	 * @param field The field object to downcast to a U32Field.
	 *
	 * @return A U32Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a U32Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toU32Field(field);


	/**
	 * Downcasts the Field object into a U64Field.
	 *
	 * @param field The field object to downcast to a U64Field.
	 *
	 * @return A U64Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a U64Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toU64Field(field);


	/**
	 * Downcasts the Field object into a U8Field.
	 *
	 * @param field The field object to downcast to a U8Field.
	 *
	 * @return A U8Field object.
	 *
	 * @throws SWIG_Exception The given Field object is null.
	 * @throws SWIG_Exception The given Field object cannot be converted to a U8Field object.
	 * @throws SWIG_Exception The given object is not a Field object.
	 */
	public static toU8Field(field);
}
