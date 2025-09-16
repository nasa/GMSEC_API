/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Specification.java
 */

package gmsec;


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
	}


	/**
	 * Basic constructor that initializes the %Specification instance with the
	 * default message specification.
	 *
	 * @throws GmsecException Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification();


	/**
	 * Initializes the %Specification instance with the given configuration.
	 *
	 * @param config The configuration for the Specification object.
	 *
	 * @throws SWIG_Exception Thrown if the %Config object is null.
	 * @throws GmsecException Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification(config);


	/**
	 * Copy constructor - initializes the Specification instance based on another instance.
	 *
	 * @param other The other Specification object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the Specification object is null.
	 * @throws GmsecException Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification(other);


	/**
	 * Returns a SchemaIDIterator which can be used to iterate over the IDs of the loaded
	 * templates.
	 *
	 * @return A handle to a SchemaIDIterator object.
	 */
	public getSchemaIDIterator();


	/**
	 * Returns the version of the message specification that is referenced by the Specification.
	 *
	 * @return The version of the templates in use.
	 */
	public getVersion();


	/**
	 * Returns the operating schema level for the specification.
	 *
	 * @return A SchemaLevel enumerated value.
	 */
	public getSchemaLevel();


	/**
	 * Returns a MsgSpecArray of Message Specification objects associated with the Specification.
	 *
	 * @return A MsgSpecArray of MessageSpecifications.
	 */
	public getMessageSpecifications();


	/**
	* Returns a StringArray of the field names that are identified as header fields.
	 * 
	 * @return A list of strings that correspond to the names of the header fields.
	 */
	public getHeaderFields();


	/**
	* Returns a StringArray of the field names that are identified as header fields.
	*
	* @param schemaName The name of the specified schema level (e.g. C2MS, C2MS-PIPE, GMSEC, DEFAULT). An empty or NULL string refers to DEFAULT.
	*
	* @return a list of strings that correspond to the names of the header fields.
	*/
	public getHeaderFields(schemaName);
}
