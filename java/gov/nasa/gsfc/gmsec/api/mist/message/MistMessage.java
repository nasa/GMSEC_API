/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MistMessage.java
 */

package gov.nasa.gsfc.gmsec.api.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMistMessage;


/**
 * This Message object shares all of the functionality of standard messages,
 * but can also be auto-populated based on how the message schema identifies itself within
 * the given specification.
 *
 * @see Message
 * @see Config
 * @see Specification
 * @see Field
 * @see gov.nasa.gsfc.gmsec.api.MessageFieldIterator
 */
public class MistMessage extends Message
{
	protected MistMessage(JNIMistMessage jMistMsg)
	{
		super(jMistMsg);
	}


	/**
	 * Constructor - Initializes the message instance with a template determined by ID and spec.
	 *
	 * @param subject  The subject string for the message.
	 * @param schemaID The string used to identify the message schema in C2MS or other message specification
	 * (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
	 * @param spec The specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if any of the input parameters are invalid.
	 * @throws GMSEC_Exception Thrown if an invalid schema ID is given.
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
	 * Constructor - initializes an empty message with no template
	 *
	 * @param subject  The subject string for the message.
	 * @param schemaID The string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2019.00.C2MS.MSG.HB).
	 * You may also use the shorthand notation of messageKind.messageType.messageSubType (e.g. MSG.HB) 
	 * @param config   A configuration to associate with the message.
	 * @param spec     The specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if any of the input parameters are invalid.
	 * @throws GMSEC_Exception Thrown if an invalid schema ID is given.
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
	 * Initializes a MistMessage using an XML or JSON string
	 * <p>
	 * Note: This constructor has been deprecated; use {@link #MistMessage(Specification, String)} instead.
	 *
	 * @param data XML or JSON string used to initialize the message
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null.
	 * @throws GMSEC_Exception Thrown if data string contains invalid an XML or JSON statement.
	 *
	 * @deprecated This constructor has been deprecated; use MistMessage(Specification, String) instead.
	 */
	@Deprecated
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
	 * Initializes a MistMessage using an XML or JSON string, and ensures the message adheres
	 * to the provided specification.
	 *
	 * @param spec The specification this message's schema will adhere to.
	 * @param data XML or JSON string used to initialize the message
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null.
	 * @throws IllegalArgumentException Thrown if the Specification is null.
	 * @throws GMSEC_Exception Thrown if data string contains invalid an XML or JSON statement.
	 *
	 * @see Message#toXML()
	 * @see Message#toJSON()
	 */
	public MistMessage(Specification spec, String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMistMessage) null);

		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIMistMessage(Specification.getInternal(spec), data));
	}


	/**
	 * Copy constructor.
	 *
	 * @param other The other MIST message object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given MIST message object is null.
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
	 * Special constructor that constructs a MistMessage using a simple Message.
	 *
	 * @param msg The simple Message to reference while building the MistMessage
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 * @throws GMSEC_Exception Thrown if the given Message does not house sufficient information to construct a MistMessage.
	 */
	public MistMessage(Message msg) throws IllegalArgumentException, GMSEC_Exception
	{
		this(msg, new Config());
	}


	/**
	 * Special constructor that builds a MistMessage using a simple Message.
	 *
	 * @param msg The simple Message to reference while building the MistMessage
	 * @param specConfig The specification configuration to apply when constructing the MistMessage
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 * @throws GMSEC_Exception Thrown if the given Message does not house sufficient information to construct a MistMessage.
	 */
	public MistMessage(Message msg, Config specConfig) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMistMessage) null);

		if (msg == null)
		{
			throw new IllegalArgumentException("Message cannot be null");
		}
		if (specConfig == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}

		registerChild(new JNIMistMessage(Message.getInternal(msg), Config.getInternal(specConfig)));
	}


	/**
	 * Sets the internal list of fields which are to be automatically placed in all
	 * MistMessage objects that are created.  Internal copies of the Fields are made, and
	 * thus ownership of the fields that are provided are not retained by MistMessage.
	 *
	 * @param standardFields The array of fields to be copied to the internal set of fields,
	 * which will in turn be included with all MistMessage objects that are created.
	 */
	public static void setStandardFields(java.util.List<Field> standardFields)
	{
		if (standardFields != null && standardFields.size() > 0)
		{
			JNIMistMessage.setStandardFields(standardFields);
		}
	}


	/**
	 * Destroys the lists of standard fields that are included with MistMessage objects.
	 */
	public static void clearStandardFields()
	{
		JNIMistMessage.clearStandardFields();
	}


	/**
	 * Returns a string that identifies the schema that the Message is based off of.
	 *
	 * @return A schema ID string.
	 */
	public String getSchemaID()
	{
		return ((JNIMistMessage) getInternal(this)).getSchemaID();
	}


	/**
	 * Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new StringField will be created with the specified name 
	 * and value.
	 * 
	 * @param fieldName Name of the field to be modified/created
	 * @param value     The value of the field.  The value's type is automatically determined based on the message schema.
	 *
	 * @throws IllegalArgumentException Thrown if either the field name or the value are null.  Note
	 * field name cannot contain an empty string.
	 * @throws GMSEC_Exception Thrown if the input value's type cannot be converted into the type required
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
	 * Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new I64Field will be created with the specified name 
	 * and value.
	 * 
	 * @param fieldName Name of the field to be modified/created
	 * @param value     The value of the field.  The value's type is automatically determined based on the message schema.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the input value's type cannot be converted into the type required
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
	 * Sets the value for a field indicated by user.  The value's type is automatically
	 * determined by the function depending on the message schema being used.  If the field 
	 * does not have a required type, a new F64Field will be created with the specified name 
	 * and value.
	 * 
	 * @param fieldName Name of the field to be modified/created
	 * @param value     The value of the field.  The value's type is automatically determined based on the message schema.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the input value's type cannot be converted into the type required
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
