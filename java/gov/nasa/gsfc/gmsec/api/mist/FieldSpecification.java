/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file FieldSpecification.java
 */

package gov.nasa.gsfc.gmsec.api.mist;


/**
 * The FieldSpecification class represents a single field definition from a message
 * schema.
 *
 * @see MessageSpecification
 * @see Specification
 */
public class FieldSpecification
{
	private String m_name;
	private String m_type;
	private String m_mode;
	private String m_class;
	private String m_value;
	private String m_desc;


	private FieldSpecification()
	{
	}


	private FieldSpecification(String name, String type, String mode, String clazz, String value, String desc)
	{
		m_name  = name;
		m_type  = type;
		m_mode  = mode;
		m_class = clazz;
		m_value = value;
		m_desc  = desc;
	}


	/**
	 * Returns the name for the Field Specification.
	 *
	 * @return The name associated with the Field Specification.
	 */
	public String getName()
	{
		return m_name;
	}


	/**
	 * Returns the type (e.g. STRING, I16, etc.) for the Field Specification.
	 *
	 * @return The field type associated with the Field Specification.
	 */
	public String getType()
	{
		return m_type;
	}


	/**
	 * Returns the mode (e.g. REQUIRED, OPTIONAL, etc.) for the Field Specification.
	 *
	 * @return The mode associated with the Field Specification.
	 */
	public String getMode()
	{
		return m_mode;
	}


	/**
	 * Returns the field class type for the Field Specification.
	 *
	 * @return The class type associated with the Field Specification.
	 */
	public String getClassification()
	{
		return m_class;
	}


	/**
	 * Returns the default value, if any, for the Field Specification. If a
	 * value is not available, then an empty string is returned.
	 *
	 * @return The default value associated with the Field Specification.
	 */
	public String getValue()
	{
		return m_value;
	}


	/**
	 * Returns the description for the Field Specification.
	 *
	 * @return The description associated with the Field Specification.
	 */
	public String getDescription()
	{
		return m_desc;
	}
}
