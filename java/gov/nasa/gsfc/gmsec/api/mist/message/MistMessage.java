/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MistMessage.java
 *
 * @brief The file contains the public interface to generic MIST Messages.
 */

package gov.nasa.gsfc.gmsec.api.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMistMessage;


/**
 * @class MistMessage
 *
 * @brief This Message object shares all of the functionality of standard messages,
 * but can also be auto-populated based on how the message schema identifies itself within
 * the given specification.
 *
 * @sa Message
 * @sa Config
 * @sa Specification
 * @sa Field
 * @sa MsgFieldIterator
 */
public class MistMessage extends Message
{
	protected MistMessage(JNIMistMessage jMistMsg)
	{
		super(jMistMsg);
	}


	/**
	 * @fn MistMessage(String subject, String schemaID, Specification spec)
	 *
	 * @brief default constructor - Initializes the message instance with a template determined by ID and spec.
	 *
	 * @param subject  - the subject string for the message.
	 * @param schemaID - the string used to identify the message schema in the GMSEC ISD.
	 * @param spec     - the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if any of the input parameters are invalid.
	 * @throws A GMSEC_Exception is thrown if an invalid schema ID is given.
	 */
	public MistMessage(String subject, String schemaID, Specification spec)
		throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMistMessage) null);

		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("Schema ID cannot be null, nor contain an empty string");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIMistMessage(subject, schemaID, Specification.getInternal(spec)));
	}


	/**
	 * @fn MistMessage(String subject, String schemaID, Config config, Specification spec)
	 *
	 * @brief default constructor - initializes an empty message with no template
	 *
	 * @param subject  - the subject string for the message.
	 * @param schemaID - the string used to identify the message schema in the GMSEC ISD.
	 * @param config   - A configuration to associate with the message.
	 * @param spec     - the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if any of the input parameters are invalid.
	 * @throws A GMSEC_Exception is thrown if an invalid schema ID is given.
	 */
	public MistMessage(String subject, String schemaID, Config config, Specification spec)
		throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMistMessage) null);

		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("Schema ID cannot be null, nor contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIMistMessage(subject, schemaID, Config.getInternal(config), Specification.getInternal(spec)));
	}


	/**
	 * @fn MistMessage(String data)
	 *
	 * @brief Initializes a MistMessage using an XML or JSON string
	 *
	 * @param data - XML or JSON string used to initialize the message
	 *
	 * @throws An IllegalArgumentException is thrown if any of the data string is invalid.
	 * @throws A GMSEC_Exception is thrown if data string contains invalid XML or JSON statement(s).
	 *
	 * @sa Message.toXML()
	 * @sa Message.toJSON()
	 */
	public MistMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMistMessage) null);

		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIMistMessage(data));
	}


	/**
	 * @fn MistMessage(MistMessage other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other MIST message object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given MIST message object is null.
	 */
	public MistMessage(MistMessage other) throws IllegalArgumentException
	{
		super((JNIMistMessage) null);

		if (other == null)
		{
			throw new IllegalArgumentException("MistMessage cannot be null");
		}

		registerChild(new JNIMistMessage((JNIMistMessage) getInternal(other)));
	}


	/**
	 * @fn String getSchemaID()
	 *
	 * @brief Returns a string that identifies the schema that the Message is based off of.
	 */
	public String getSchemaID()
	{
		return ((JNIMistMessage) getInternal(this)).getSchemaID();
	}


	/**
	 * @fn void setValue(String fieldName, String value) throws IllegalArgumentException, GMSEC_Exception
	 *
	 * @brief Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new StringField will be created with the specified name 
	 * and value.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field.  The value's type is automatically determined based on the message schema.
	 *
	 * @throws An IllegalArgumentException is thrown if either the field name or the value are null.  Note
	 * field name cannot contain an empty string.
	 * @throws An GMSEC_Exception is thrown if the input value's type cannot be converted into the type required
	 * by the field, or if the string is too big when converting to char.
	 */
	public void setValue(String fieldName, String value) throws IllegalArgumentException, GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field Name string is null or contains an empty string");
		}
		if (value == null)
		{
			throw new IllegalArgumentException("Value string is null or contains an empty string");
		}

		((JNIMistMessage) getInternal(this)).setValue(fieldName, value);
	}


	/**
	 * @fn void setValue(String fieldName, long value) throws IllegalArgumentException, GMSEC_Exception
	 *
	 * @brief Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new I64Field will be created with the specified name 
	 * and value.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field.  The value's type is automatically determined based on the message schema.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null or contains an empty string.
	 * @throws An GMSEC_Exception is thrown if the input value's type cannot be converted into the type required
	 * by the field, or if the string is too big when converting to char.
	 */
	public void setValue(String fieldName, long value) throws IllegalArgumentException, GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field Name string is null or contains an empty string");
		}

		((JNIMistMessage) getInternal(this)).setValue(fieldName, value);
	}


	/**
	 * @fn void setValue(String fieldName, double value) throws IllegalArgumentException, GMSEC_Exception
	 *
	 * @brief Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new F64Field will be created with the specified name 
	 * and value.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field.  The value's type is automatically determined based on the message schema.
	 *
	 * @throws An IllegalArgumentException is thrown if the field name is null or contains an empty string.
	 * @throws An GMSEC_Exception is thrown if the input value's type cannot be converted into the type required
	 * by the field, or if the string is too big when converting to char.
	 */
	public void setValue(String fieldName, double value) throws IllegalArgumentException, GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field Name string is null or contains an empty string");
		}

		((JNIMistMessage) getInternal(this)).setValue(fieldName, value);
	}
}
