/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file MessageSpecification.java
 */

package gov.nasa.gsfc.gmsec.api5;

import java.util.ArrayList;
import java.util.Collection;


/**
 * The MessageSpecification class represents a message defintion derived
 * from a message schema.
 *  
 * @see FieldSpecification
 * @see Specification
 * @see MessageFactory#getSpecification()
 */
public class MessageSpecification
{
	private String                        m_schemaID;
	private String                        m_subjectTemplate;
	private ArrayList<FieldSpecification> m_fieldSpecs;


	private MessageSpecification()
	{
	}


	private MessageSpecification(String schemaID, String subjectTemplate, ArrayList<FieldSpecification> fieldSpecs)
	{
		m_schemaID        = schemaID;
		m_subjectTemplate = subjectTemplate;
		m_fieldSpecs      = fieldSpecs;
	}


	/**
	 * Returns the schema (template) ID for this Message Specification.
	 *
	 * @return The schema ID for this Message Specification.
	 */
	public String getSchemaID()
	{
		return m_schemaID;
	}


	/**
	 * Returns the subject template for this Message Specification.
	 *
	 * @return The subject template for this Message Specification.
	 */
	public String getSubjectTemplate()
	{
		return m_subjectTemplate;
	}


	/**
	 * Returns the collection of FieldSpecification objects for this Message Specification.
	 *
	 * @return The collection of FieldSpecification objects.
	 */
	public Collection<FieldSpecification> getFieldSpecifications()
	{
		return m_fieldSpecs;
	}
}
