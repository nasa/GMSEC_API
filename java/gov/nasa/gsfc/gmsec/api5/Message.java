/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Message.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.jni.JNIMessage;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;


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

		private final int value;

		private static Map<Integer, Kind> map = new HashMap<>();

		static {
			for (Kind kind : Kind.values()) {
				map.put(kind.value, kind);
			}
		}

		private Kind(int value) {
			this.value = value;
		}

		/**
		 * Converts the Message kind enumerated value to an integer.
		 * @return An integer value.
		 */
		public int getValue() {
			return this.value;
		}

		/**
		 * Converts an integer value to a Message Kind enumerated value.
		 * @param kind An integer value representing a Message Kind.
		 * @return A Message Kind enumerated value, or null if the conversion fails.
		 */
		public static Kind valueOf(int kind) {
			return map.get(kind);
		}
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

		private int value;

		private static Map<Integer, ResponseStatus> map = new HashMap<>();

		static {
			for (ResponseStatus status : ResponseStatus.values()) {
				map.put(status.value, status);
			}
		}

		private ResponseStatus(int value) { this.value = value; }

		/**
		 * Converts the ResponseStatus to an integer value.
		 * @return An integer value.
		 */
		public int getValue() { return value; }

		/**
		 * Converts the interger value to a ResponseStatus enumerated type.
		 * @param status An integer value representing a ResponseStatus enumerated type.
		 * @return A ResponseStatus enumerated type, or null if the conversion fails.
		 */
		public static ResponseStatus valueOf(int status) {
			return map.get(status);
		}
	}


	/**
	 * Constructor.
	 *
	 * @see MessageFactory
	 */
	public Message()
	{
		m_jniMessage = new JNIMessage();
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
     * Acknowledges the processing of the message that has been received from the
	 * GMSEC Bus.
     * Some middleware can retain a delivered message in a queue until said time
     * the message has been acknowledged as being processed. Should the message not be
     * acknowledged, the middleware can deliver the message to another subscriber client.
     */
    public void acknowledge()
	{
		m_jniMessage.acknowledge();
	}


	/**
	 * Destroys a Message object.
	 * This method should NOT be used to destroy a Message object received within a callback.
	 *
	 * @param msg The message to destroy.
	 *
	 * @see Connection#receive
	 * @see Connection#receive(int)
	 * @see Connection#request(Message, int, int)
	 */
	public static void destroy(Message msg)
	{
		JNIMessage.destroy(msg);
	}


	/**
	 * Returns a string that identifies the schema ID, if known, from which the Message is based.
	 * @return A string that identifies the schema ID (if known), otherwise an empty string.
	 */
	public String getSchemaID()
	{
		return m_jniMessage.getSchemaID();
	}


	/**
	 * Returns the specification version number that was used to create the message.
	 * @return The specification version number that was used to create the message.
	 */
	public int getVersion()
	{
		return m_jniMessage.getVersion();
	}


	/**
	 * Returns the specification schema level that was used to create the message.
	 * @return Specification schema level that was used to create the message.
	 */
    public Specification.SchemaLevel getSchemaLevel()
	{
		return m_jniMessage.getSchemaLevel();
	}


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
	public Status isCompliant()
	{
		return m_jniMessage.isCompliant();
	}


	/**
	 * Registers the given message validator that is used when the
	 * message is checked for compliance.
	 *
	 * @param validator Custom message validation object.
	 *
	 * @throws IllegalArgumentException Thrown if the validator object is null.
	 *
	 * @see MessageValidator
	 * @see MessageFactory#registerMessageValidator
	 */
	public void registerMessageValidator(MessageValidator validator)
		throws IllegalArgumentException
	{
		if (validator == null)
		{
			throw new IllegalArgumentException("MessageValidator cannot be null");
		}

		m_jniMessage.registerMessageValidator(validator);
	}


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
	 * @param value The value of the field. The value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required
	 * by the field, or if the string is too big when converting to char.
	 */
	public boolean setFieldValue(String fieldName, String value)
		throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name cannot be null, nor be an empty string");
		}

		return m_jniMessage.setFieldValue(fieldName, value);
	}


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
	 * @param value The value of the field. The value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required by the field.
	 */
	public boolean setFieldValue(String fieldName, int value)
		throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name cannot be null, nor be an empty string");
		}

		return m_jniMessage.setFieldValue(fieldName, value);
	}


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
	 * @param value The value of the field. The value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required by the field.
	 */
	public boolean setFieldValue(String fieldName, long value)
		throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name cannot be null, nor be an empty string");
		}

		return m_jniMessage.setFieldValue(fieldName, value);
	}


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
	 * @param value The value of the field. The value's type is automatically determined based on the message schema.
	 *
	 * @return Returns true if the field was replaced; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the input value's type cannot be converted into the type required by the field.
	 */
	public boolean setFieldValue(String fieldName, double value)
		throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name cannot be null, nor be an empty string");
		}

		return m_jniMessage.setFieldValue(fieldName, value);
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
	public void setConfig(Config config) throws IllegalArgumentException
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniMessage.setConfig(config);
	}


	/**
	 * Returns the configuration associcated with the message.
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
	 * @throws IllegalArgumentException Thrown if the subject is null or contains an empty string.
	 * @throws GmsecException Thrown if the subject contains illegal content.
	 */
	public void setSubject(String subject)
		throws IllegalArgumentException, GmsecException
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject is null or contains an empty string");
		}

		m_jniMessage.setSubject(subject);
	}


	/**
	 * This method sets the message kind. The API can auto-deduce the message kind, but there
	 * may be cases where this method may be useful. For most common Use Cases, this method
	 * should not need to be called.
	 *
	 * @param kind - the message kind
	 */
	public void setKind(Kind kind)
	{
		m_jniMessage.setKind(kind);
	}


	/**
	 * Returns an enumerated value representing the message kind associated with the message.
	 *
	 * @return The message kind.
	 */
	public Kind getKind()
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
	 * Attempts to convert the field value into string representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a string value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a string value.
	 */
	public String getStringValue(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getStringValue(fieldName);
	}


	/**
	 * Attempts to convert the field value into boolean representation.
	 * Any non-zero value will be interpreted as true, otherwise as false.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a boolean value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a boolean value.
	 */
	public boolean getBooleanValue(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getBooleanValue(fieldName);
	}


	/**
	 * Attempts to convert the field value into a 16-bit integer number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an integer value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an integer.
	 */
	public short getI16Value(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getI16Value(fieldName);
	}


	/**
	 * Attempts to convert the field value into a 32-bit integer number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an integer value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an integer.
	 */
	public int getI32Value(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getI32Value(fieldName);
	}


	/**
	 * Attempts to convert the field value into a 64-bit integer number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an integer value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an integer.
	 */
	public long getI64Value(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getI64Value(fieldName);
	}


	/**
	 * Attempts to convert the field value into a U16 number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a U16 value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a U16.
	 */
	public U16 getU16Value(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getU16Value(fieldName);
	}


	/**
	 * Attempts to convert the field value into a U32 number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a U32 value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a U32.
	 */
	public U32 getU32Value(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getU32Value(fieldName);
	}


	/**
	 * Attempts to convert the field value into a U64 number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a U64 value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a U64.
	 */
	public U64 getU64Value(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getU64Value(fieldName);
	}


	/**
	 * Attempts to convert the field value into an 64-bit floating-point number representation.
	 *
	 * @param fieldName The name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a floating-point value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a floating-point value.
	 */
	public double getF64Value(String fieldName) throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniMessage.getF64Value(fieldName);
	}


    /**
     * Reports whether the field, identified by name, is present in the Message.
     *
     * @param name The name of field to find
     *
     * @return True if the field is found; false otherwise.
     */
    public boolean hasField(String name)
	{
		return m_jniMessage.hasField(name);
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
	 * @return A Field.Type value.
	 *
	 * @throws IllegalArgumentException Thrown if the given field name is null or contains an empty string.
	 * @throws GmsecException Thrown if the field cannot be found.
	 */
	public Field.Type getFieldType(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a BinaryField.
	 */
	public BinaryField getBinaryField(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a BooleanField.
	 */
	public BooleanField getBooleanField(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a CharField.
	 */
	public CharField getCharField(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not an F32Field.
	 */
	public F32Field getF32Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not an F32Field.
	 */
	public F64Field getF64Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not an I8Field.
	 */
	public I8Field getI8Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not an I16Field.
	 */
	public I16Field getI16Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not an I32Field.
	 */
	public I32Field getI32Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not an I64Field.
	 */
	public I64Field getI64Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a U8Field.
	 */
	public U8Field getU8Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a U16Field.
	 */
	public U16Field getU16Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a U16Field.
	 */
	public U32Field getU32Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a U16Field.
	 */
	public U64Field getU64Field(String name) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if the field cannot be found or is not a StringField.
	 */
	public StringField getStringField(String name) throws IllegalArgumentException, GmsecException
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
