/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Specification.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;

import gov.nasa.gsfc.gmsec.api5.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api5.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api5.jni.JNISpecification;

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


	//! @cond
	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param spec Object to reference for acquiring internal JNISpecification.
	 * @return Internal JNISpecification object.
	 */
	public static JNISpecification getInternal(Specification spec)
	{
		return (spec == null ? null : spec.m_jniSpecification);
	}


	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param jSpec Internal JNISpecification object.
	 */
	public Specification(JNISpecification jSpec)
	{
		m_jniSpecification = jSpec;
	}
	//! @endcond


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

		/**
		 * Converts the SchemaLevel enumerated type to an integer.
		 * @return An integer value.
		 */
		public int getValue() {
			return level;
		}

		/**
		 * Converts an integer value into a SchemaLevel enumerated type.
		 * @param level An integer value representing a SchemaLevel enumerated type.
		 * @return A SchemaLevel, or null if the integer value cannot be converted.
		 */
		public static SchemaLevel valueOf(int level) {
			return (SchemaLevel) map.get(level);
		}
	}


	/**
	 * Basic constructor that initializes the %Specification instance with the
	 * default message specification.
	 *
	 * @throws GmsecException Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification() throws GmsecException
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
	 * @throws GmsecException Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification(Config config) throws IllegalArgumentException, GmsecException
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
	 * @throws GmsecException Thrown if schemas (templates) cannot be loaded.
	 */
	public Specification(Specification other) throws IllegalArgumentException, GmsecException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("Specification object is null");
		}

		m_jniSpecification = new JNISpecification(Specification.getInternal(other));
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
	 * Returns the version of the message specification that is referenced by the Specification.
	 *
	 * @return A version number.
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
	 * Returns a list of the field names for the default header (which is the topmost schema level currently configured)
	 * 
	 * @return a list of strings that correspond to the names of the header fields.
	 */
	public Collection<String> getHeaderFieldNames()
	{
		return m_jniSpecification.getHeaderFieldNames("DEFAULT");
	}

	/**
	* Returns a list of the field names for the specified header 
	*
	* @param schemaName The name of the specified schema level (e.g. C2MS, C2MS-PIPE, GMSEC, DEFAULT).  A blank or NULL string refers to DEFAULT.
	*
	* @return a list of strings that correspond to the names of the header fields.
	*/
	public Collection<String> getHeaderFieldNames(String schemaName)
	{
		return m_jniSpecification.getHeaderFieldNames(((schemaName == null || schemaName.isEmpty()) ? "DEFAULT" : schemaName));
	}
}
