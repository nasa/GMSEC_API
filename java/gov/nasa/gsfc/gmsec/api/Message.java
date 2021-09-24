/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Message.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import java.util.Collection;


/**
 * The Message object is a container for GMSEC Messages.
 * The methods of this class allow the construction and manipulation of the data in a message.
 */
public class Message
{
	private JNIMessage m_jniMessage = null;


	/**
	 * This method is for internal GMSEC API use only.
	 * @param child A JNIMessage object.
	 */
	public Message(JNIMessage child)
	{
		m_jniMessage = child;
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param msg A Message object.
	 * @return A JNIMessage object.
	 */
	public static JNIMessage getInternal(Message msg)
	{
		return (msg == null ? null : msg.m_jniMessage);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jMsg A JNIMessage object.
	 */
	protected void registerChild(JNIMessage jMsg)
	{
		m_jniMessage = jMsg;
	}


	/**
	 * Enumerated list of possible Message types.
	 */
	public enum MessageKind
	{
		/** For messages that will be published. */
		PUBLISH,

		/** For messages that will be issued as requests. */
		REQUEST,

		/** For messages that will serve as a reply (to a request). */
		REPLY
	}


	/**
	 * Constructor.
	 * <p>
	 * Example creation and use:
	 * <pre>{@code
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
	 * }</pre>
	 *
	 * @param subject The GMSEC subject to associate with the message.
	 * @param kind The kind of message to create.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty string.
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
	 * Specialized constructor that accepts a Config object.
	 * <p>
	 * Note: Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 *
	 * @param subject The GMSEC subject to associate with the message.
	 * @param kind The kind of message to create.
	 * @param config A configuration object that contains message-specific options.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty string,
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
	 * Specialized constructor that initializes the message instance using either XML or JSON text.
	 * <p>
	 * An XML string should have a format similar to the following:
	 * <pre>{@code
	 * final String XML = "<MESSAGE SUBJECT=\"GMSEC.TEST.XML.SUBJECT\" KIND=\"PUBLISH\">" +
	 *		              "    <CONFIG>" +
	 *	     	          "        <PARAMETER NAME=\"gmsec-msgfld-store-type\">tree</PARAMETER>" +
	 *	     	          "        <PARAMETER NAME=\"gmsec-msgfld-store-size\">100</PARAMETER>" +
	 *	     	          "    </CONFIG>" +
	 *	     	          "    <FIELD TYPE=\"STRING\" NAME=\"EXAMPLE-STRING-NAME\">EXAMPLE TEXT</FIELD>" +
	 *	     	          "    <FIELD TYPE=\"BOOL\" NAME=\"EXAMPLE-BOOL-FIELD-NAME\">TRUE</FIELD>" +
	 *	     	          "</MESSAGE>";
	 * }</pre>
	 * <p>
	 * A JSON string should have a format similar to the following:
	 * <pre>{@code
	 * final String JSON = "{\"MESSAGE\":{" +
	 *	               "  \"KIND\":\"PUBLISH\"," +
	 *	               "  \"SUBJECT\":\"GMSEC.TEST.JSON.SUBJECT\"," +
	 *	               "  \"FIELD\":[" +
	 *		       "    {" +
	 *	               "      \"NAME\":\"CHAR-FIELD\"," +
	 *	               "      \"TYPE\":\"CHAR\"" +
	 *	               "      \"VALUE\":\"c\"" +
	 *	               "    }," +
	 *		       "    {" +
	 *	               "      \"NAME\":\"BOOL-FIELD\"," +
	 *	               "      \"TYPE\":\"BOOL\"" +
	 *	               "      \"VALUE\":\"TRUE\"" +
	 *	               "    }" +
	 *		       "  ]" +
	 *		       "  }" +
	 *			"}";
	 * }</pre>
	 *
	 * @param data The XML or JSON string that represents a GMSEC message.
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null, or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the data string cannot be parsed.
	 *
	 * @see Message#toXML()
	 * @see Message#toJSON()
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
	 * Copy constructor.
	 *
	 * @param other The other message object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
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
	 * Used to associate configuration object with the message.
	 * <p>
	 * Note: Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 *
	 * @param config The configuration object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
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
	 * Returns the reference to the Config object associated with the message.
	 *
	 * @return A Config object.
	 */
	public Config getConfig()
	{
		return m_jniMessage.getConfig();
	}


	/**
	 * Returns a String containing the subject/topic associated with the message.
	 *
	 * @return The message subject.
	 */
	public String getSubject()
	{
		return m_jniMessage.getSubject();
	}


	/**
	 * Allows for the setting/changing of the message subject.
	 *
	 * @param subject The subject/topic to associate with the message
	 *
	 * @throws IllegalArgumentException Thrown if the subject is null or contains
	 * an empty string.
	 */
	public void setSubject(String subject)
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}

		m_jniMessage.setSubject(subject);
	}


	/**
	 * Returns an enumerated value representing the MessageKind associated with the message.
	 *
	 * @return The message kind.
	 */
	public MessageKind getKind()
	{
		return m_jniMessage.getKind();
	}


	/**
	 * Adds the given Field object to the Message.
	 *
	 * @param field The field to add to the message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field object is null.
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
	 * Adds a BinaryField to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Byte array of data
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the given byte array is null.
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
	 * Adds a BooleanField to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds a CharField to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds an I8Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds an I16Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds an I32Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds an I64Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds an U8Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the given data object is null.
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
	 * Adds an U16Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Data object
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the given data object is null.
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
	 * Adds an U32Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Data object
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the given data object is null.
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
	 * Adds an U64Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Data object
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the given data object is null.
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
	 * Adds an F32Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds an F64Field to the message using the given data.
	 *
	 * @param name Field name
	 * @param data Value
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Adds a StringField to the message using the given data.
	 *
	 * @param name Field name
	 * @param data String data
	 *
	 * @return Returns true if the field is replacing one with the same name; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the given data value is null or contains an empty string.
	 */
	public boolean addField(String name, String data) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (data == null)
		{
			throw new IllegalArgumentException("Data value is null");
		}

		return m_jniMessage.addField(name, data);
	}


	/**
	 * Adds each Field in the given Collection to the Message.
	 *
	 * @param fields Collection of field objects
	 *
	 * @return Returns true if any existing field in the Message has been replaced; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given Collection is null.
	 */
	public boolean addFields(Collection<Field> fields) throws IllegalArgumentException
	{
		if (fields == null)
		{
			throw new IllegalArgumentException("Field collection is null");
		}

		return m_jniMessage.addFields(fields);
	}


	/**
	 * Removes all Fields from the Message.
	 */
	public void clearFields()
	{
		m_jniMessage.clearFields();
	}


	/**
	 * Removes the Field from this message by name.
	 *
	 * @param name the name of the Field to remove.
	 *
	 * @return Returns true if the field was removed; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Attempts to convert the field value into an 64-bit integer number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an integer value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found, or if it cannot successfully
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
	 * Attempts to convert the field value into a U64 number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a U64 value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found, or if it cannot successfully
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
	 * Attempts to convert the field value into an 64-bit floating-point number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a floating-point value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found, or if it cannot successfully
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
	 * Attempts to convert the field value into string representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a string value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found, or if it cannot successfully
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
     * Reports whether the field, identified by name, is present in the Message.
     *
     * @param name The name of field to find
     *
     * @return True if the field is found; false otherwise.
     *
     * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
     */
    public boolean hasField(String name) throws IllegalArgumentException
	{
		return (getField(name) != null);
	}


	/**
	 * This function will return the named Field object contained within the Message
	 * object, if it exists. Otherwise a null handle is returned.
	 *
	 * @param name The name of the Field to reference.
	 *
	 * @return A handle to the Field, if it is found; null otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
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
	 * Returns the FieldType associated with the named field.
	 *
	 * @param name The name of the Field to reference.
	 *
	 * @return A Field.FieldType value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found.
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
	 * Returns a handle to the BinaryField referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A BinaryField.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a BinaryField.
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
	 * Returns a handle to the BooleanField referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A BooleanField.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a BooleanField.
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
	 * Returns a handle to the CharField referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A CharField.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a CharField.
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
	 * Returns a handle to the F32Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return An F32Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not an F32Field.
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
	 * Returns a handle to the F64Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return An F64Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not an F32Field.
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
	 * Returns a handle to the I8Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return An I8Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not an I8Field.
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
	 * Returns a handle to the I16Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return An I16Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not an I16Field.
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
	 * Returns a handle to the I32Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return An I32Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not an I32Field.
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
	 * Returns a handle to the I64Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return An I64Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not an I64Field.
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
	 * Returns a handle to the U8Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A U8Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a U8Field.
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
	 * Returns a handle to the U16Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A U16Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a U16Field.
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
	 * Returns a handle to the U32Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A U32Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a U16Field.
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
	 * Returns a handle to the U64Field referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A U64Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a U16Field.
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
	 * Returns a handle to the StringField referenced by the given name.
	 *
	 * @param name The name of the field to retrieve.
	 *
	 * @return A StringField.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the field cannot be found or is not a StringField.
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
	 * Returns the number of fields associated with this message.
	 *
	 * @return The number of fields in the message.
	 */
	public int getFieldCount()
	{
		return m_jniMessage.getFieldCount();
	}


	/**
	 * This method will copy all fields from this message to the given message.
	 * <p>
	 * Note: The fields are NOT copied by reference.  As such, the original message and its fields
	 * can be destroyed or modified after it has been copied to the new message.
	 *
	 * @param toMsg The message into which to copy the fields
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
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
	 * Returns an XML string representation of the message.
	 *
	 * @return The XML string representation of the message.
	 */
	public String toXML()
	{
		return m_jniMessage.toXML();
	}


	/**
	 * Returns an JSON string representation of the message.
	 *
	 * @return The JSON string representation of the message.
	 */
	public String toJSON()
	{
		return m_jniMessage.toJSON();
	}


	/**
	 * Returns the physical storage size of the message in bytes.  Please note that
	 * additional tracking data fields are added to messages at the time of publication,
	 * which will affect the size of the message.
	 *
	 * @return The message size.
	 */
	public long getSize()
	{
		return m_jniMessage.getSize();
	}


	/**
	 * Method that allows the callee to get the MessageFieldIterator associated with the Message.
	 * This iterator will allow for applications to iterate over the Field objects stored within the Message.
	 * The iterator is reset each time getFieldIterator() is called.  The iterator itself is destroyed when the
	 * Message object is destroyed.
	 * <p>
	 * Note: Only one MessageFieldIterator object is associated with a Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object.  Each call will reset the iterator.
	 *
	 * @return A MessageFieldIterator.
	 */
	public MessageFieldIterator getFieldIterator()
	{
		return m_jniMessage.getFieldIterator();
	}


	/**
	 * Method that allows the callee to get the MessageFieldIterator associated with the Message.
	 * This iterator will allow for applications to iterate over the Field objects stored within the Message.
	 * The iterator is reset each time getFieldIterator() is called.  The iterator itself is destroyed when the
	 * Message object is destroyed.
	 * <p>
	 * Note: Only one MessageFieldIterator object is associated with a Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object.  Each call will reset the iterator.
	 *
	 * @param selector The type of MessageFieldIterator to return.
	 *
	 * @return A MessageFieldIterator.
	 */
	public MessageFieldIterator getFieldIterator(MessageFieldIterator.Selector selector)
	{
		return m_jniMessage.getFieldIterator(selector);
	}
}
