/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Message.java
 */

package gmsec;


/**
 * The Message object is a container for GMSEC Messages.
 * The methods of this class allow the construction and manipulation of the data in a message.
 */
public class Message
{
	/**
	 * Enumerated list of possible Message types.
	 *
	 * Note:
     * The message kind is auto-deduced using the MESSAGE-TYPE field;
     * if the field is not present in the message, or does not contain a
     * value of MSG, REQ, or RESP, then the message kind will default to
     * PUBLISH.
	 */
	public enum Kind
	{
		/** For messages that will be published. */
		PUBLISH(1),

		/** For messages that will be issued as requests. */
		REQUEST(2),

		/** For messages that will serve as a reply (to a request). */
		REPLY(3);
	}


	/**
	 * Enumerated values that can be used to indicate the status of a request.
	 */
	public enum ResponseStatus
	{
		/** Acknowledge request. */
		ACKNOWLEDGEMENT(1),

		/** Still processing request. */
		WORKING_KEEP_ALIVE(2),

		/** Successful completion of request. */
		SUCCESSFUL_COMPLETION(3),

		/** Failed to complete request. */
		FAILED_COMPLETION(4),

		/** Invalid request. */
		INVALID_REQUEST(5),

		/** Final reply message to request. */
		FINAL_MESSAGE(6);
	}


	/**
	 * Constructor.
	 *
	 * @see MessageFactory
	 */
	public Message();


	/**
	 * Copy constructor.
	 *
	 * @param other The other message object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given Message object is null.
	 */
	public Message(other);


    /**
     * Acknowledges the processing of the message that has been received from the
	 * GMSEC Bus.
     * Some middleware can retain a delivered message in a queue until said time
     * the message has been acknowledged as being processed. Should the message not be
     * acknowledged, the middleware can deliver the message to another subscriber client.
     */
    public acknowledge();


	/**
	 * Returns a string that identifies the schema ID, if known, from which the Message is based.
	 * @return A string that identifies the schema ID (if known), otherwise an empty string.
	 */
	public getSchemaID();


	/**
	 * Returns the specification version number that was used to create the message.
	 * @return The specification version number that was used to create the message.
	 */
	public getVersion();


	/**
	 * Returns the specification schema level that was used to create the message.
	 * @return Specification schema level that was used to create the message.
	 */
    public getSchemaLevel();


	/**
	 * The contents of the message are compared to the template to ensure the
	 * message is in compliance with the message specification.
	 *
	 * @return A Status object to indicate whether the message is valid. If invalid,
	 * a list of errors will be provided of issues found within the message will be
	 * reported.
	 *
	 * @see Status
	 */
	public isCompliant();


	/**
	 * Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * Note: This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName Name of the field to be modified/created
	 * @param strValue  The value of the field; the value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name is an empty string or is otherwise non-compliant.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required by the field.
	 */
	public setFieldValue(fieldName, strValue);


	/**
	 * Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * Note: This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName Name of the field to be modified/created
	 * @param intValue  The value of the field; the value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name is an empty string or is otherwise non-compliant.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required by the field.
	 */
	public setFieldValue(fieldName, intValue);


	/**
	 * Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * Note: This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName Name of the field to be modified/created
	 * @param longValue The value of the field; the value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name is an empty string or is otherwise non-compliant.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required by the field.
	 */
	public setFieldValue(fieldName, longValue);


	/**
	 * Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * Note: This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName   Name of the field to be modified/created
	 * @param doubleValue The value of the field; the value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name is an empty string or is otherwise non-compliant.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required by the field.
	 */
	public setFieldValue(fieldName, doubleValue);


	/**
	 * Used to associate configuration object with the message.
	 * <p>
	 * Note: Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 *
	 * @param config The configuration object.
	 *
	 * @throws SWIG_Exception Thrown if the given Config object is null.
	 */
	public setConfig(config);


	/**
	 * Returns the configuration associcated with the message.
	 *
	 * @return A Config object.
	 */
	public getConfig();


	/**
	 * Returns a String containing the subject/topic associated with the message.
	 *
	 * @return The message subject.
	 */
	public getSubject();


	/**
	 * Allows for the setting/changing of the message subject.
	 *
	 * @param subject The subject/topic to associate with the message
	 *
	 * @throws SWIG_Exception Thrown if the subject is null.
	 * @throws GmsecException Thrown if the subject contains illegal content.
	 */
	public setSubject(subject);


	/**
	 * Allows for the setting of individual subject elements. The name of the elements are defined
	 * by the message's corresponding message template. This value will be overridden by automatic subject
	 * generation if the subject element is defined by an existing field in the message, or if the subject
	 * was manually defined with setSubject.
	 *
	 * @param name - the name of the subject element
	 * @param value - the value of the subject element. An empty or null value will be seen as FILL in the subject line.
	 *
	 * @throws SWIG_Exception Thrown if the name is null or an empty string.
	 * @throws GmsecException Thrown if the name does not match a subject 
	 * element name defined the message template, or if the message does not 
	 * have a corresponding message template.
	 */
	public setSubjectElement(name, value);


	/**
	 * This method sets the message kind. The API can auto-deduce the message kind, but there
	 * may be cases where this method may be useful. For most common Use Cases, this method
	 * should not need to be called.
	 *
	 * @param kind - the message kind
	 */
	public setKind(kind);


	/**
	 * Returns an enumerated value representing the message kind associated with the message.
	 *
	 * @return The message kind.
	 */
	public getKind();


	/**
	 * Adds the given Field object to the Message.
	 *
	 * @param field The Field object to add to the message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given Field object is null.
	 */
	public addField(field);


	/**
	 * Adds each Field in the given Collection to the Message.
	 *
	 * @param fields A FieldArray of Field objects.
	 *
	 * @return Returns true if any existing field in the Message has been replaced; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given FieldArray is null.
	 */
	public addFields(fields);


	/**
	 * Removes all Fields from the Message.
	 */
	public clearFields();


	/**
	 * Removes the Field from this message by name.
	 *
	 * @param name The name of the Field to remove.
	 *
	 * @return Returns true if the field was removed; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the given field name is an empty string.
	 */
	public clearField(name);


	/**
	 * Attempts to convert the field value into string representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a string value.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found.
	 * @throws GmsecException Thrown if the field cannot be successfully be converted to a string value.
	 */
	public getStringValue(fieldName);


	/**
	 * Attempts to convert the field value into boolean representation.
	 * Any non-zero value will be interpreted as true, otherwise as false.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a boolean value.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found.
	 * @throws GmsecException Thrown if the field cannot be successfully be converted to a boolean value.
	 */
	public getBooleanValue(fieldName);


	/**
	 * Attempts to convert the field value into a 16-bit integer number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an integer value.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found.
	 * @throws GmsecException Thrown if the field cannot be successfully be converted to an integer.
	 */
	public getIntegerValue(fieldName);


	/**
	 * Attempts to convert the field value into an 64-bit floating-point number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a floating-point value.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found.
	 * @throws GmsecException Thrown if the field cannot be successfully be converted to a float-point value.
	 */
	public getDoubleValue(fieldName);


    /**
     * Reports whether the field, identified by name, is present in the Message.
     *
     * @param fieldName The name of field to find
     *
     * @return True if the field is found; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
     */
    public hasField(fieldName);


	/**
	 * This function will return the named Field object contained within the Message
	 * object, if it exists. Otherwise a null handle is returned.
	 *
	 * @param fieldName The name of the Field to reference.
	 *
	 * @return A handle to the Field, if it is found; null otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 */
	public getField(fieldName);


	/**
	 * Returns the field type associated with the named field.
	 *
	 * @param fieldName The name of the Field to reference.
	 *
	 * @return A Field.Type value.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found.
	 */
	public getFieldType(fieldName);


	/**
	 * Returns a handle to the BinaryField referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A BinaryField.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a BinaryField.
	 */
	public getBinaryField(fieldName);


	/**
	 * Returns a handle to the BooleanField referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A BooleanField.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a BooleanField.
	 */
	public getBooleanField(fieldName);


	/**
	 * Returns a handle to the CharField referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A CharField.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a CharField.
	 */
	public getCharField(fieldName);


	/**
	 * Returns a handle to the F32Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return An F32Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not an F32Field.
	 */
	public getF32Field(fieldName);


	/**
	 * Returns a handle to the F64Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return An F64Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not an F32Field.
	 */
	public getF64Field(fieldName);


	/**
	 * Returns a handle to the I8Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return An I8Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not an I8Field.
	 */
	public getI8Field(fieldName);


	/**
	 * Returns a handle to the I16Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return An I16Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not an I16Field.
	 */
	public getI16Field(fieldName);


	/**
	 * Returns a handle to the I32Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return An I32Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not an I32Field.
	 */
	public getI32Field(fieldName);


	/**
	 * Returns a handle to the I64Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return An I64Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not an I64Field.
	 */
	public getI64Field(fieldName);


	/**
	 * Returns a handle to the U8Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A U8Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a U8Field.
	 */
	public getU8Field(fieldName);


	/**
	 * Returns a handle to the U16Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A U16Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a U16Field.
	 */
	public getU16Field(fieldName);


	/**
	 * Returns a handle to the U32Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A U32Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a U16Field.
	 */
	public getU32Field(fieldName);


	/**
	 * Returns a handle to the U64Field referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A U64Field.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a U16Field.
	 */
	public getU64Field(fieldName);


	/**
	 * Returns a handle to the StringField referenced by the given name.
	 *
	 * @param fieldName The name of the field to retrieve.
	 *
	 * @return A StringField.
	 *
	 * @throws SWIG_Exception Thrown if the given field name is null.
	 * @throws GmsecException Thrown if the field cannot be found or is not a StringField.
	 */
	public getStringField(fieldName);


	/**
	 * Returns the number of fields associated with this message.
	 *
	 * @return The number of fields in the message.
	 */
	public getFieldCount();


	/**
	 * This method will copy all fields from this message to the given message.
	 * <p>
	 * Note: The fields are NOT copied by reference. As such, the original message and its fields
	 * can be destroyed or modified after it has been copied to the new message.
	 *
	 * @param toMsg The message into which to copy the fields
	 *
	 * @throws SWIG_Exception Thrown if the given Message object is null.
	 */
	public copyFields(toMsg);


	/**
	 * Returns an XML string representation of the message.
	 *
	 * @return The XML string representation of the message.
	 */
	public toXML();


	/**
	 * Returns an JSON string representation of the message.
	 *
	 * @return The JSON string representation of the message.
	 */
	public toJSON();


	/**
	 * Returns the physical storage size of the message in bytes. Note the
	 * additional tracking data fields are added to messages at the time
	 * of publication, which will affect the size of the message.
	 *
	 * @return The message size.
	 */
	public getSize();


	/**
	 * Method that allows the callee to get the MessageFieldIterator associated with the Message.
	 * This iterator will allow for applications to iterate over the Field objects stored within the Message.
	 * The iterator is reset each time getFieldIterator() is called. The iterator itself is destroyed when the
	 * Message object is destroyed.
	 * <p>
	 * Note: Only one MessageFieldIterator object is associated with a Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object. Each call will reset the iterator.
	 *
	 * @return A MessageFieldIterator.
	 */
	public getFieldIterator();


	/**
	 * Method that allows the callee to get the MessageFieldIterator associated with the Message.
	 * This iterator will allow for applications to iterate over the Field objects stored within the Message.
	 * The iterator is reset each time getFieldIterator() is called. The iterator itself is destroyed when the
	 * Message object is destroyed.
	 * <p>
	 * Note: Only one MessageFieldIterator object is associated with a Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object. Each call will reset the iterator.
	 *
	 * @param selector The type of MessageFieldIterator to return.
	 *
	 * @return A MessageFieldIterator.
	 */
	public getFieldIterator(selector);
}
