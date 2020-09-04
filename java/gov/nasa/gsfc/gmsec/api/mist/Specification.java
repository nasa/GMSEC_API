/*
 * Copyright 2007-2016 United States Government as represented by the
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


/**
 * @class Specification
 *
 * @brief The Specification class loads a list of templates from a designated directory
 * that are used to create and validate message schemas.  Each template has an associated ID
 * that is used to identify the kind of message schema the template will be defining.
 *
 * @sa Config
 * @sa SchemaIDIterator
 */
public class Specification
{
	private JNISpecification m_jniSpecification = null;


	public static JNISpecification getInternal(Specification spec)
	{
		return (spec == null ? null : spec.m_jniSpecification);
	}


	public Specification(JNISpecification jSpec)
	{
		m_jniSpecification = jSpec;
	}


	/**
	 * @fn Specification(Config config)
	 *
	 * @brief Default constructor - initializes the %Specification instance.
	 *
	 * @param config - the configuration for the %Specification object.
	 *
	 * @throws An IllegalArgumentException is thrown if the %Config object is null.
	 * @throws A GMSEC_Exception is thrown if schemas (templates) cannot be loaded.
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
	 * @fn Specification(Specification other)
	 *
	 * @brief Copy constructor - initializes the %Specification instance based on another instance.
	 *
	 * @param other - the %Specification object to be copied.
	 *
	 * @throws An IllegalArgumentException is thrown if the %Specification object is null.
	 * @throws A GMSEC_Exception is thrown if schemas (templates) cannot be loaded.
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
	 * @fn void validateMessage(Message msg)
	 *
	 * @brief Using the given message's subject, the appropriate template from the message registry
	 * is referenced.  The contents of the message are then compared to the template to ensure the
	 * message complies with the GMSEC Interface Specification Document (ISD).  If a template cannot
	 * be found for the message, then a new template based on the message will be added to the registry.
	 *
	 * @param message - the message to be validated.
	 *
	 * @throws An IllegalArgumentException is thrown if the %Message object is null.
	 * @throws A GMSEC_Exception is thrown if the %Message is invalid.
	 */
	public void validateMessage(Message msg)
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message object is null");
		}

		m_jniSpecification.validateMessage(Message.getInternal(msg));
	}


	/**
	 * @fn SchemaIDIterator getSchemaIDIterator()
	 *
	 * @brief Returns a SchemaIDIterator which can be used to iterate over the IDs of the loaded
	 * templates.
	 *
	 * @return A handle to a SchemaIDIterator object.
	 */
	public SchemaIDIterator getSchemaIDIterator()
	{
		return m_jniSpecification.getSchemaIDIterator();
	}


	/**
	 * @fn int getVersion()
	 * @desc Returns the version of the GMSEC Interface Specification Document (ISD) that is referenced
	 * by the %Specification object.
	 */
	public int getVersion()
	{
		return m_jniSpecification.getVersion();
	}


	/**
	 * @fn String getTemplateXML(String subject, String schemaID)
	 *
	 * @brief Returns a message schema, as XML, from the template with the matching schema ID.  This
	 * XML data can be used to construct a Message object.
	 *
	 * @param subject - the desired subject for the message.
	 * @param schemaID - the schema ID for the desired template.
	 *
	 * @return XML string representation of the message schema, or null if a schema cannot be referenced.
	 *
	 * @throws An IllegalArgumentException is thrown if either the subject or the schemaID are null, or
	 * contain empty strings.
	 */
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
