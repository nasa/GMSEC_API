/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFactory.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageValidator;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.field.*;

import gov.nasa.gsfc.gmsec.api5.jni.JNIMessageFactory;

import java.util.Collection;


/**
 * The MessageFactory is used to create Message objects that are based on C2MS message templates.
 */
public class MessageFactory
{
	private JNIMessageFactory m_jniMessageFactory = null;


	//! @cond
	/**
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param msgFactory A MessageFactory object.
	 * @return A JNIMessageFactory object.
	 */
	public static JNIMessageFactory getInternal(MessageFactory msgFactory)
	{
		return (msgFactory == null ? null : msgFactory.m_jniMessageFactory);
	}


	/**
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param factory MessageFactory object.
	 */
	private static void resetInternal(MessageFactory factory)
	{
		if (factory != null)
		{
			factory.m_jniMessageFactory = null;
		}
	}


	/**
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param jFactory A JNIMessageFactory object.
	 */
	public MessageFactory(JNIMessageFactory jFactory)
	{
		m_jniMessageFactory = jFactory;
	}
	//! @endcond


	/**
	 * Do not allow instantiation of this class.
	 * Users must use MessageFactory.create().
	 */
	private MessageFactory()
	{
	}


	/**
	 * Creates %MessageFactory instance using the latest/current message specification.
	 *
	 * @return A new MessageFactory object.
	 *
	 * @throws GmsecException Thrown if the latest message templates cannot be loaded.
	 *
	 * @see MessageFactory#destroy(MessageFactory)
	 */
	public static MessageFactory create()
		throws GmsecException
	{
		return new MessageFactory(new JNIMessageFactory());
	}


	/**
	 * Creates %MessageFactory instance using the given configuration.
	 *
	 * @param config Config object to customize the %MessageFactory, typically for alternative message specification.
	 *
	 * @return A new MessageFactory object.
	 *
	 * @throws IllegalArgumentException Thrown if the Config is null.
	 * @throws GmsecException Thrown if the message templates cannot be loaded.
	 *
	 * @see MessageFactory#destroy(MessageFactory)
	 */
	public static MessageFactory create(Config config)
		throws IllegalArgumentException, GmsecException
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}

		return new MessageFactory(new JNIMessageFactory(config));
	}


	/**
	 * Destroys the given MessageFactory object.
	 *
	 * @param factory The MessageFactory object to destroy
	 *
	 * @throws IllegalArgumentException Thrown if the given MessageFactory object is null.
	 *
	 * @see MessageFactory#create
	 * @see MessageFactory#create(Config)
	 */
	public static void destroy(MessageFactory factory)
	{
		if (factory == null)
		{
			throw new IllegalArgumentException("MessageFactory is null");
		}

		JNIMessageFactory.destroy(factory);

		MessageFactory.resetInternal(factory);
	}


	/**
	 * Sets the internal list of fields which are to be automatically placed in all
	 * Message objects that are created using the %MessageFactory. Internal copies of
	 * the Fields are made, and thus ownership of the fields that are provided are not
	 * retained by %MessageFactory.
	 *
	 * @param standardFields The array of fields to be copied to the internal set of fields,
	 * which will in turn be included with all Message objects that are created.
	 */
	public void setStandardFields(java.util.List<Field> standardFields)
	{
		if (standardFields != null && standardFields.size() > 0)
		{
			m_jniMessageFactory.setStandardFields(standardFields);
		}
	}


	/**
	 * Destroys the lists of standard fields that are included with Message objects
	 * created using the MessageFactory.
	 */
	public void clearStandardFields()
	{
		m_jniMessageFactory.clearStandardFields();
	}


	/**
	 * Allows for the setting of a common message configuration that should be
	 * applied to all messages created by the %MessageFactory.
	 * <p>
	 * Note: Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 *
	 * @param msgConfig The message configuration object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Config object is null.
	 */
	public void setMessageConfig(Config msgConfig) throws IllegalArgumentException
	{
		if (msgConfig == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniMessageFactory.setMessageConfig(msgConfig);
	}


	/**
	 * Creates and returns a %Message object. The %Message will include any user-supplied
	 * message attributes (such as a message configuration, standard fields, and 
	 * custom message validator). It is recommended to destroy the %Message object when it
	 * is no longer needed.
	 *
	 * @return A Message object is returned.
	 *
	 * @see setMessageConfig
	 * @see setStandardFields
	 * @see registerMessageValidator
	 * @see Message#destroy
	 */
	public Message createMessage()
	{
		return m_jniMessageFactory.createMessage();
	}


	/**
	 * Creates and returns a %Message object that is populated with fields derived
	 * from the working message specification and schema ID. In addition, the
	 * %Message object will include any user-supplied message attributes (such as a
	 * message configuration, standard fields, and custom message validator). It is
	 * recommended to destroy the %Message object when it is no longer needed.
	 *
	 * @param schemaID The string identifying which message to create (e.g. HB).
	 *
	 * @return A Message object is returned.
	 *
	 * @throws IllegalArgumentException Thrown if the schemaID is null or is an empty string.
	 * @throws GmsecException Thrown if the schemaID references an unknown/illegal schema ID.
	 *
	 * @see setMessageConfig
	 * @see setStandardFields
	 * @see registerMessageValidator
	 * @see Message#destroy
	 */
	public Message createMessage(String schemaID)
		throws IllegalArgumentException, GmsecException
	{
		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("SchemaID cannot be null, nor be an empty string");
		}

		return m_jniMessageFactory.createMessage(schemaID);
	}


	/**
	 * Creates and returns a Message object based on the given XML or JSON data.
	 *
	 * @param data An XML or JSON data string that represents a GMSEC-style message.
	 * @param type the type of data being provided
	 *
	 * @return A Message object that is populated using the given data is returned.
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null, or is an empty string.
	 * @throws IllegalArgumentException Thrown if the given data type is not XML_DATA or JSON_DATA.
	 * @throws GmsecException Thrown if the given data string is not parsable as a GMSEC-style message in either XML or JSON format.
	 */
	public Message fromData(String data, Gmsec.DataType type)
		throws IllegalArgumentException, GmsecException
	{
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Message data is null or contains an empty string");
		}
		if (type == Gmsec.DataType.KEY_VALUE_DATA)
		{
			throw new IllegalArgumentException("DataType " + type + " is not supported");
		}

		return m_jniMessageFactory.fromData(data, type);
	}


	/**
	 * Returns the Specification object associated with the %MessageFactory.
	 *
	 * @return A Specification object.
	 */
	public Specification getSpecification()
	{
		return m_jniMessageFactory.getSpecification();
	}


	/**
	 * Registers the given message validator with each message
	 * created using the Message Factory.
	 *
	 * @param validator A custom message validation object.
	 *
	 * @throws IllegalArgumentException Thrown if the validator object is null.
	 *
	 * @see MessageValidator
	 * @see Message#registerMessageValidator
	 */
	public void registerMessageValidator(MessageValidator validator)
		throws IllegalArgumentException
	{
		if (validator == null)
		{
			throw new IllegalArgumentException("MessageValidator cannot be null");
		}

		m_jniMessageFactory.registerMessageValidator(validator);
	}
}
