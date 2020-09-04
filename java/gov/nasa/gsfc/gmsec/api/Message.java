/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Message.java
 *
 * @brief The Message object is a container for GMSEC Messages.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;


/**
 * @class Message
 *
 * @brief The Message object is a container for GMSEC Messages.
 * The methods of this class allow the construction and manipulation of the data in a message.
 */
public class Message
{
	private JNIMessage m_jniMessage = null;


	public Message(JNIMessage child)
	{
		m_jniMessage = child;
	}


	public static JNIMessage getInternal(Message msg)
	{
		return (msg == null ? null : msg.m_jniMessage);
	}


	protected void registerChild(JNIMessage jMsg)
	{
		m_jniMessage = jMsg;
	}


	/**
	 * @enum MessageKind
	 *
	 * @desc Enumerated list of possible Message types.
	 */
	public enum MessageKind
	{
		PUBLISH,  ///< For messages that will be published
		REQUEST,  ///< For messages that will be issued as requests
		REPLY     ///< For messages that will serve as a reply (to a request)
	}


	/**
	 * @fn Message(String subject, MessageKind kind)
	 *
	 * @brief Default constructor.
	 *
	 * Example creation and use:
	 * @code
	 * // create message with subject and kind
	 * //
	 * Config config = new Config(args);
	 * Message message = new Message("GMSEC.TEST.JAVA.PUBLISH", Message.MessageKind.PUBLISH);
	 *
	 * // create field
	 * //
	 * message.addField("EXAMPLE-STRING-NAME", "Example String");
	 * message.addField("EXAMPLE-BOOL-FIELD", true);
	 *
	 * // do something with message (i.e. publish using connection object)
	 *
	 * Connection conn = Connection.create(config);
	 * conn.connect();
	 * ...
	 *
	 * conn.publish(message);
	 *
	 * ...
	 *
	 * Connection.destroy(conn);
	 * @endcode
	 *
	 * @param subject - the GMSEC subject to associate with the message.
	 * @param kind - the kind of message to create.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty string.
	 */
	public Message(String subject, MessageKind kind) throws IllegalArgumentException
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}

		m_jniMessage = new JNIMessage(subject, kind);
	}


	/**
	 * @fn Message(String subject, MessageKind kind, Config config)
	 *
	 * @brief Specialized constructor that accepts a Config object.
	 *
	 * @param subject - the GMSEC subject to associate with the message.
	 * @param kind - the kind of message to create.
	 * @param config - a configuration object that contains message-specific options.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty string,
	 * or if the Config object is null.
	 */
	public Message(String subject, MessageKind kind, Config config) throws IllegalArgumentException
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniMessage = new JNIMessage(subject, kind, config);
	}


	/**
	 * @fn Message(String data)
	 *
	 * @brief Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * An XML string should have a format similar to the following:
	 * @code
	 * <MESSAGE SUBJECT="GMSEC.TEST.SUBJECT" KIND="PUBLISH">
	 *     <FIELD TYPE="STRING" NAME="FACILITY">GMSEC LAB</FIELD>
	 *     <FIELD TYPE="I32" NAME="COUNT">10</FIELD>
	 * </MESSAGE>
	 * @endcode
	 *
	 * A JSON string should have a format similar to the following:
	 * @code
	 *    "MESSAGE":{
	 *        "KIND":"PUBLISH",
	 *        "SUBJECT":"GMSEC.MISSION.CONST.SAT.EVT.MSG",
	 *        "FIELD":[
	 *            {
	 *                "NAME":"FACILITY",
	 *                "TYPE":"STRING",
	 *                "VALUE":"GMSEC LAB"
	 *            },
	 *            {
	 *                "NAME":"COUNT",
	 *                "TYPE":"I32",
	 *                "VALUE":"10"
	 *            },
	 *        ]
	 *    }
	 * @endcode
	 *
	 * @param data - the XML or JSON string that represents a GMSEC message.
	 *
	 * @throws An IllegalArgumentException is thrown if the data string is null, or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the data string cannot be parsed.
	 *
	 * @sa toXML()
	 * @sa toJSON()
	 */
	public Message(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("The XML/JSON string is null or contains an empty string");
		}

		m_jniMessage = new JNIMessage(data);
	}


	/**
	 * @fn Message(Message other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - the other message object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 */
	public Message(Message other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		m_jniMessage = new JNIMessage(Message.getInternal(other));
	}


	/**
	 * @fn void addConfig(Config config)
	 *
	 * @brief Used to associate configuration object with the message.
	 *
	 * @param config - the configuration object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Config object is null.
	 */
	public void addConfig(Config config) throws IllegalArgumentException
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniMessage.addConfig(config);
	}


	/**
	 * @fn Config getConfig()
	 *
	 * @return A reference to the Config object associated with the message.
	 */
	public Config getConfig()
	{
		return m_jniMessage.getConfig();
	}


	/**
	 * @fn String getSubject()
	 *
	 * @return A String containing the subject/topic associated with the message.
	 */
	public String getSubject()
	{
		return m_jniMessage.getSubject();
	}


	/**
	 * @fn MessageKind getKind()
	 *
	 * @return An enumerated value representing the MessageKind associated with the message.
	 */
	public MessageKind getKind()
	{
		return m_jniMessage.getKind();
	}


	/**
	 * @fn boolean addField(Field field)
	 *
	 * @brief Adds the given Field object to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Field object is null.
	 */
	public boolean addField(Field field) throws IllegalArgumentException
	{
		if (field == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		return m_jniMessage.addField(field);
	}


	/**
	 * @fn boolean addField(String name, byte[] data)
	 *
	 * @brief Adds a BinaryField to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - data object
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given byte array is null.
	 */
	public boolean addField(String name, byte[] data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (data == null)
		{
			throw new IllegalArgumentException("Byte array is null");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, boolean data)
	 *
	 * @brief Adds a BooleanField to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, boolean data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, char data)
	 *
	 * @brief Adds a CharField to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, char data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, byte data)
	 *
	 * @brief Adds an I8Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, byte data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, short data)
	 *
	 * @brief Adds an I16Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, short data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, int data)
	 *
	 * @brief Adds an I32Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, int data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, long data)
	 *
	 * @brief Adds an I64Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, long data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, U8 data)
	 *
	 * @brief Adds an U8Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given data object is null.
	 */
	public boolean addField(String name, U8 data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (data == null)
		{
			throw new IllegalArgumentException("U8 data object is null");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, U16 data)
	 *
	 * @brief Adds an U16Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - data object
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given data object is null.
	 */
	public boolean addField(String name, U16 data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (data == null)
		{
			throw new IllegalArgumentException("U16 data object is null");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, U32 data)
	 *
	 * @brief Adds an U32Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - data object
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given data object is null.
	 */
	public boolean addField(String name, U32 data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (data == null)
		{
			throw new IllegalArgumentException("U32 data object is null");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, U64 data)
	 *
	 * @brief Adds an U64Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - data object
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given data object is null.
	 */
	public boolean addField(String name, U64 data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (data == null)
		{
			throw new IllegalArgumentException("U64 data object is null");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, float data)
	 *
	 * @brief Adds an F32Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, float data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, double data)
	 *
	 * @brief Adds an F64Field to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean addField(String name, double data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn boolean addField(String name, String data)
	 *
	 * @brief Adds a StringField to the message using the given data.
	 *
	 * @param name - field name
	 * @param data - string data
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given data value is null or contains an empty string.
	 */
	public boolean addField(String name, String data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data value is null or contains an empty string");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * @fn void clearFields()
	 *
	 * @brief Removes all Fields from the Message.
	 */
	public void clearFields()
	{
		m_jniMessage.clearFields();
	}


	/**
	 * @fn boolean clearField(String name)
	 *
	 * @brief Removes the Field from this message by name.
	 *
	 * @param name - the name of the Field to remove.
	 *
	 * @return Returns true if the field was removed; false otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public boolean clearField(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.clearField(name);
	}


	/**
	 * @fn long getIntegerValue(String fieldName)
	 *
	 * @brief Attempts to convert the field value into an 64-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an integer value.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an integer.
	 */
	public long getIntegerValue(String fieldName) throws GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getIntegerValue(fieldName);
	}


	/**
	 * @fn U64 getUnsignedIntegerValue(String fieldName)
	 *
	 * @brief Attempts to convert the field value into a U64 number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a U64 value.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a U64.
	 */
	public U64 getUnsignedIntegerValue(String fieldName) throws GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getUnsignedIntegerValue(fieldName);
	}


	/**
	 * @fn double getDoubleValue(String fieldName)
	 *
	 * @brief Attempts to convert the field value into an 64-bit floating-point number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a floating-point value.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a floating-point value.
	 */
	public double getDoubleValue(String fieldName) throws GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getDoubleValue(fieldName);
	}


	/**
	 * @fn String getStringValue(String fieldName)
	 *
	 * @brief Attempts to convert the field value into string representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a string value.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a string value.
	 */
	public String getStringValue(String fieldName) throws GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getStringValue(fieldName);
	}


	/**
	 * @fn Field getField(String name)
	 *
	 * @brief This function will return the named Field object contained within the Message
	 * object, if it exists. Otherwise a null handle is returned.
	 *
	 * @param name - the name of the Field to reference.
	 *
	 * @return A handle to the field, if it is found; a null handle otherwise.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 */
	public Field getField(String name) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getField(name);
	}


	/**
	 * @fn FieldType getFieldType(String name)
	 *
	 * @brief Returns the FieldType associated with the named field.
	 *
	 * @param name - the name of the Field to reference.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found.
	 */
	public Field.FieldType getFieldType(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getFieldType(name);
	}


	/**
	 * @fn BinaryField getBinaryField(String name)
	 *
	 * @brief Returns a handle to the BinaryField referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a BinaryField.
	 */
	public BinaryField getBinaryField(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getBinaryField(name);
	}


	/**
	 * @fn BooleanField getBooleanField(String name)
	 *
	 * @brief Returns a handle to the BooleanField referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a BooleanField.
	 */
	public BooleanField getBooleanField(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getBooleanField(name);
	}


	/**
	 * @fn CharField getCharField(String name)
	 *
	 * @brief Returns a handle to the CharField referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a CharField.
	 */
	public CharField getCharField(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getCharField(name);
	}


	/**
	 * @fn F32Field getF32Field(String name)
	 *
	 * @brief Returns a handle to the F32Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not an F32Field.
	 */
	public F32Field getF32Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getF32Field(name);
	}


	/**
	 * @fn F64Field getF64Field(String name)
	 *
	 * @brief Returns a handle to the F64Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not an F32Field.
	 */
	public F64Field getF64Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getF64Field(name);
	}


	/**
	 * @fn I8Field getI8Field(String name)
	 *
	 * @brief Returns a handle to the I8Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not an I8Field.
	 */
	public I8Field getI8Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getI8Field(name);
	}


	/**
	 * @fn I16Field getI16Field(String name)
	 *
	 * @brief Returns a handle to the I16Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not an I16Field.
	 */
	public I16Field getI16Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getI16Field(name);
	}


	/**
	 * @fn I32Field getI32Field(String name)
	 *
	 * @brief Returns a handle to the I32Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not an I32Field.
	 */
	public I32Field getI32Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getI32Field(name);
	}


	/**
	 * @fn I64Field getI64Field(String name)
	 *
	 * @brief Returns a handle to the I64Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not an I64Field.
	 */
	public I64Field getI64Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getI64Field(name);
	}


	/**
	 * @fn U8Field getU8Field(String name)
	 *
	 * @brief Returns a handle to the U8Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a U8Field.
	 */
	public U8Field getU8Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getU8Field(name);
	}


	/**
	 * @fn U16Field getU16Field(String name)
	 *
	 * @brief Returns a handle to the U16Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a U16Field.
	 */
	public U16Field getU16Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getU16Field(name);
	}


	/**
	 * @fn U32Field getU32Field(String name)
	 *
	 * @brief Returns a handle to the U32Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a U16Field.
	 */
	public U32Field getU32Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getU32Field(name);
	}


	/**
	 * @fn U64Field getU64Field(String name)
	 *
	 * @brief Returns a handle to the U64Field referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a U16Field.
	 */
	public U64Field getU64Field(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getU64Field(name);
	}


	/**
	 * @fn StringField getStringField(String name)
	 *
	 * @brief Returns a handle to the StringField referenced by the given name.
	 *
	 * @throws An IllegalArgumentException is thrown if the given field name is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the field cannot be found or is not a StringField.
	 */
	public StringField getStringField(String name) throws IllegalArgumentException, GMSEC_Exception
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getStringField(name);
	}


	/**
	 * @fn int getFieldCount()
	 *
	 * @brief Returns the number of fields associated with this message.
	 */
	public int getFieldCount()
	{
		return m_jniMessage.getFieldCount();
	}


	/**
	 * @fn void copyFields(Message toMsg)
	 *
	 * @brief This method will copy all fields from this message to the given message.
	 *
	 * @note The fiels are NOT copied by reference.  As such, the original message and its fields
	 * can be destroyed or modified after it has been copieid to the new message.
	 *
	 * @param toMsg - the message into which to copy the fields
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 */
	public void	copyFields(Message toMsg) throws IllegalArgumentException
	{
		if (toMsg == null)
		{
			throw new IllegalArgumentException("Message object is null");
		}

		m_jniMessage.copyFields(Message.getInternal(toMsg));
	}


	/**
	 * @fn String toXML()
	 *
	 * @brief Returns an XML string representation of the message.
	 */
	public String toXML()
	{
		return m_jniMessage.toXML();
	}


	/**
	 * @fn String toJSON()
	 *
	 * @brief Returns an JSON string representation of the message.
	 */
	public String toJSON()
	{
		return m_jniMessage.toJSON();
	}


	/**
	 * @fn long getSize()
	 *
	 * @brief Returns the physical storage size of the message in bytes.  Please note that
	 * additional tracking data fields are added to messages at the time of publication,
	 * which will affect the size of the message.
	 */
	public long getSize()
	{
		return m_jniMessage.getSize();
	}


	/**
	 * @fn MessageFieldIterator getFieldIterator()
	 *
	 * @brief Method that allows the callee to get the MessageFieldIterator associated with the %Message.
	 * This iterator will allow for applications to iterate over the Field objects stored within the %Message.
	 * The iterator is reset each time getFieldIterator() is called.  The iterator itself is destroyed when the
	 * %Message object is destroyed.
	 *
	 * @note Only one MessageFieldIterator object is associated with a %Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a MessageFieldIterator object.
	 */
	public MessageFieldIterator getFieldIterator()
	{
		return m_jniMessage.getFieldIterator();
	}


	/**
	 * @fn MessageFieldIterator getFieldIterator(MessageFieldIterator.Selector selector)
	 *
	 * @brief Method that allows the callee to get the MessageFieldIterator associated with the %Message.
	 * This iterator will allow for applications to iterate over the Field objects stored within the %Message.
	 * The iterator is reset each time getFieldIterator() is called.  The iterator itself is destroyed when the
	 * %Message object is destroyed.
	 *
	 * @note Only one MessageFieldIterator object is associated with a %Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object.  Each call will reset the iterator.
	 *
	 * @param selector - the type of MessageFieldIterator to return.
	 *
	 * @return A reference to a MessageFieldIterator object.
	 */
	public MessageFieldIterator getFieldIterator(MessageFieldIterator.Selector selector)
	{
		return m_jniMessage.getFieldIterator(selector);
	}
}
