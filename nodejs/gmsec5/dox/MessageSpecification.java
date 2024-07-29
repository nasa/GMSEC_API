/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageSpecification.java
 */

package gmsec;


/**
 * The MessageSpecification class represents a message defintion derived
 * from a message schema.
 *  
 * @see FieldSpecification
 * @see Specification
 * @see MessageFactory.getSpecification()
 */
public class MessageSpecification
{
	/**
	 * Returns the schema (template) ID for this Message Specification.
	 *
	 * @return The schema ID for this Message Specification.
	 */
	public getSchemaID();


	/**
	 * Returns the subject template for this Message Specification.
	 *
	 * @return The subject template for this Message Specification.
	 */
	public getSubjectTemplate();


	/**
	 * Returns a FieldSpecificationArray of FieldSpecification objects for this Message Specification.
	 *
	 * @return An array of FieldSpecification objects.
	 */
	public getFieldSpecifications();
}
