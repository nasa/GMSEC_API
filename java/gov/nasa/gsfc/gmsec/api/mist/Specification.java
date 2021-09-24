/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Specification.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;

import java.util.Collection;
import java.util.Map;
import java.util.HashMap;


/**
 * The Specification class loads a list of templates from a designated directory
 * that are used to create and validate message schemas. Each template has an associated ID
 * that is used to identify the kind of message schema the template will be defining.
 *
 * @see Config
 * @see SchemaIDIterator
 */
public class Specification
{
	private JNISpecification m_jniSpecification = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param spec Object to reference for acquiring internal JNISpecification.
	 * @return Internal JNISpecification object.
	 */
	public static JNISpecification getInternal(Specification spec)
	{
		return (spec == null ? null : spec.m_jniSpecification);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jSpec Internal JNISpecification object.
	 */
	public Specification(JNISpecification jSpec)
	{
		m_jniSpecification = jSpec;
	}


	/**
	 * Schema Levels for representing message specification.
	 */
	public enum SchemaLevel
	{
		/** C2MS */
		LEVEL_0(0),

		/** C2MS Extensions, or optionally user-defined */
		LEVEL_1(1),

		/** NASA/GMSEC Addendum, or optionally user-defined */
		LEVEL_2(2),

		/** User-defined */
		LEVEL_3(3),

		/** User-defined */
		LEVEL_4(4),

		/** User-defined */
		LEVEL_5(5),

		/** User-defined */
		LEVEL_6(6);

		private int level;
		private static Map<Integer, SchemaLevel> map = new HashMap<>();

		private SchemaLevel(int level) {
			this.level = level;
		}

		static {
			for (SchemaLevel schemaLevel : SchemaLevel.values()) {
				map.put(schemaLevel.level, schemaLevel);
			}
		}

		public static SchemaLevel valueOf(int level) {
			return (SchemaLevel) map.get(level);
		}

		public int getValue() {
			return level;
		}
	}


	/**
	 * Basic constructor that initializes the %Specification instance with the
	 * default message specification (NASA/GMSEC Addendum)
	 *
	 * @throws GMSEC_Exception Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification() throws GMSEC_Exception
	{
		Config config = new Config();

		m_jniSpecification = new JNISpecification(Config.getInternal(config));
	}


	/**
	 * Initializes the %Specification instance with the given configuration.
	 *
	 * @param config The configuration for the Specification object.
	 *
	 * @throws IllegalArgumentException Thrown if the %Config object is null.
	 * @throws GMSEC_Exception Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification(Config config) throws IllegalArgumentException, GMSEC_Exception
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}

		m_jniSpecification = new JNISpecification(Config.getInternal(config));
	}


	/**
	 * Copy constructor - initializes the Specification instance based on another instance.
	 *
	 * @param other The other Specification object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the Specification object is null.
	 * @throws GMSEC_Exception Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification(Specification other) throws IllegalArgumentException, GMSEC_Exception
	{
		if (other == null)
		{
			throw new IllegalArgumentException("Specification object is null");
		}

		m_jniSpecification = new JNISpecification(Specification.getInternal(other));
	}


	/**
	 * Using the given message's subject, the appropriate template from the message registry
	 * is referenced. The contents of the message are then compared to the template to ensure the
	 * message complies with the GMSEC Interface Specification Document (ISD). If a template cannot
	 * be found for the message, then a new template based on the message will be added to the registry.
	 *
	 * @param msg The message to be validated.
	 *
	 * @throws IllegalArgumentException Thrown if the Message object is null.
	 * @throws GMSEC_Exception Thrown if the Message is invalid.
	 */
	public void validateMessage(Message msg) throws IllegalArgumentException, GMSEC_Exception
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message object is null");
		}

		m_jniSpecification.validateMessage(Message.getInternal(msg));
	}


	/**
	 * Returns a SchemaIDIterator which can be used to iterate over the IDs of the loaded
	 * templates.
	 *
	 * @return A handle to a SchemaIDIterator object.
	 */
	public SchemaIDIterator getSchemaIDIterator()
	{
		return m_jniSpecification.getSchemaIDIterator();
	}


	/**
	 * Returns the version of the GMSEC Interface Specification Document (ISD) that is referenced
	 * by the Specification object.
	 *
	 * @return The version of the ISD in use.
	 *
	 * @see gmsecMIST
	 */
	public int getVersion()
	{
		return m_jniSpecification.getVersion();
	}


	/**
	 * Returns the operating schema level for the specification.
	 *
	 * @return A SchemaLevel enumerated value.
	 */
	public SchemaLevel getSchemaLevel()
	{
		return m_jniSpecification.getSchemaLevel();
	}


	/**
	 * Returns a collection of Message Specification objects associated with the Specification.
	 *
	 * @return A collection of MessageSpecifications.
	 */
	public Collection<MessageSpecification> getMessageSpecifications()
	{
		return m_jniSpecification.getMessageSpecifications();
	}


	/**
	 * Registers the given message validator to be used when message validation takes place.
	 *
	 * @param validator The custom message validator
	 *
	 * @throws IllegalArgumentException Thrown if the given validator object is null.
	 */
	public void registerMessageValidator(MessageValidator validator)
	{
		if (validator == null)
		{
			throw new IllegalArgumentException("MessageValidator is null");
		}

		m_jniSpecification.registerMessageValidator(validator);
	}


	/**
	 * Returns a message schema, as XML, from the template with the matching schema ID. This
	 * XML data can be used to construct a Message object.
	 *
	 * @param subject The desired subject for the message.
	 * @param schemaID The schema ID for the desired template.
	 *
	 * @return XML string representation of the message schema, or null if a schema cannot be referenced.
	 *
	 * @throws IllegalArgumentException Thrown if either the subject or the schemaID are null, or
	 * contain empty strings.
	 */
	@Deprecated
	public String getTemplateXML(String subject, String schemaID)
	{
		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject object is null or contains empty string");
		}
		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("Message object is null or contains empty string");
		}

		return m_jniSpecification.getTemplateXML(subject, schemaID);
	}
}
