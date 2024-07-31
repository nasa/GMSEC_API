/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file FieldSpecification.java
 */

package gmsec;


/**
 * The FieldSpecification class represents a single field definition from a message
 * schema.
 *
 * @see MessageSpecification
 * @see Specification
 */
public class FieldSpecification
{
	private FieldSpecification();


	private FieldSpecification(name, type, mode, clazz, value, desc);


	/**
	 * Returns the name for the Field Specification.
	 *
	 * @return The name associated with the Field Specification.
	 */
	public getName();


	/**
	 * Returns the type (e.g. STRING, I16, etc.) for the Field Specification.
	 *
	 * @return The field type associated with the Field Specification.
	 */
	public getType();


	/**
	 * Returns the mode (e.g. REQUIRED, OPTIONAL, etc.) for the Field Specification.
	 *
	 * @return The mode associated with the Field Specification.
	 */
	public getMode();


	/**
	 * Returns the field class type for the Field Specification.
	 *
	 * @return The class type associated with the Field Specification.
	 */
	public getClassification();


	/**
	 * Returns the default value, if any, for the Field Specification. If a
	 * value is not available, then an empty string is returned.
	 *
	 * @return The default value associated with the Field Specification.
	 */
	public getValue();


	/**
	 * Returns the description for the Field Specification.
	 *
	 * @return The description associated with the Field Specification.
	 */
	public getDescription();
}
