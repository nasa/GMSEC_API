/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFile.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIProductFile;


/**
 * This class is a lightweight container for holding information
 * on a Product File, and used to generate GMSEC Product File messages by the 
 * ConnectionManager class.
 *
 * @see ConnectionManager
 */
public class ProductFile
{
	private JNIProductFile m_jniProductFile = null;


	/**
	 * This method is for internal GMSEC API use only.
	 * @param obj ProductFile object to reference for acquiring internal JNIProductFile.
	 * @return Internal JNIProductFile object.
	 */
	public static JNIProductFile getInternal(ProductFile obj)
	{
		return (obj == null ? null : obj.m_jniProductFile);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jProductFile Internal JNIProductFile object.
	 */
	public ProductFile(JNIProductFile jProductFile)
	{
		m_jniProductFile = jProductFile;
	}


	protected ProductFile()
	{
	}


	/**
	 * Constructor - Initializes the ProductFile object with a URI to describe a commonly accessible resource.
	 *
	 * @param name The name of the file.
	 * @param description The description of the file.
	 * @param version The version of the file.
	 * @param format The descriptor for the file format.
	 * @param uri The URI for the file.
	 *
	 * @throws IllegalArgumentException Thrown if the ProductFile name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the ProductFile description is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the ProductFile version is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the ProductFile format is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the ProductFile URI is null.
	 */
	public ProductFile(String name, String description, String version, String format, String uri)
		throws IllegalArgumentException
	{
		validateParameters(name, description, version, format);

		if (uri == null)
		{
			throw new IllegalArgumentException("ProductFile URI cannot be null");
		}

		m_jniProductFile = new JNIProductFile(name, description, version, format, uri);
	}


	/**
	 * Constructor - Initializes the ProductFile object with the binary contents of the file.
	 *
	 * @param name The name of the file.
	 * @param description The description of the file.
	 * @param version The version of the file.
	 * @param format The descriptor for the file format.
	 * @param data The binary contents of the file.
	 *
	 * @throws IllegalArgumentException Thrown if the ProductFile name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the ProductFile description is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the ProductFile version is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the ProductFile format is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the data object is null.
	 */
	public ProductFile(String name, String description, String version, String format, byte[] data)
		throws IllegalArgumentException
	{
		validateParameters(name, description, version, format);

		if (data == null)
		{
			throw new IllegalArgumentException("ProductFile data byte array is null");
		}

		m_jniProductFile = new JNIProductFile(name, description, version, format, data);
	}


	/**
	 * Copy-Constructor - copies information from the given ProductFile.
	 *
	 * @param other The ProductFile to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given ProductFile is null.
	 */
	public ProductFile(ProductFile other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("ProductFile is null");
		}

		m_jniProductFile = new JNIProductFile(ProductFile.getInternal(other));
	}


	/**
	 * Retrieves the name of the ProductFile.
	 *
	 * @return The name of the ProductFile.
	 */
	public String getName()
	{
		return m_jniProductFile.getName();
	}


	/**
	 * Retrieves the description of the ProductFile.
	 *
	 * @return The description of the ProductFile.
	 */
	public String getDescription()
	{
		return m_jniProductFile.getDescription();
	}


	/**
	 * Retrieves the version of the ProductFile.
	 *
	 * @return The version of the ProductFile.
	 */
	public String getVersion()
	{
		return m_jniProductFile.getVersion();
	}


	/**
	 * Retrieves the format of the ProductFile.
	 *
	 * @return The format of the ProductFile.
	 */
	public String getFormat()
	{
		return m_jniProductFile.getFormat();
	}


	/**
	 * Returns the availability of a ProductFile URI.
	 *
	 * @return True if a ProductFile URI has been set, false otherwise.
	 */
	public boolean isURIAvailable()
	{
		return m_jniProductFile.isURIAvailable();
	}


	/**
	 * Accessor for the ProductFile URI.
	 *
	 * @return A reference to the ProductFile URI string.
	 *
	 * @throws GMSEC_Exception Thrown if the URI string has not been set.
	 */
	public String getURI() throws GMSEC_Exception
	{
		return m_jniProductFile.getURI();
	}


	/**
	 * Returns the availability of a ProductFile file content.
	 *
	 * @return True if a ProductFile file content has been set, false otherwise.
	 */
	public boolean contentsAvailable()
	{
		return m_jniProductFile.contentsAvailable();
	}


	/**
	 * Accessor for the ProductFile file content.
	 *
	 * @return A byte array containing the ProductFile's content.
	 *
	 * @throws GMSEC_Exception Thrown if the file content has not been set, or
	 * if the contents are unavailable.
	 */
	public byte[] getContents() throws GMSEC_Exception
	{
		return m_jniProductFile.getContents();
	}


	private void validateParameters(String name, String description, String version, String format)
		throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("ProductFile name is null or an contains an empty string");
		}
		if (description == null || description.isEmpty())
		{
			throw new IllegalArgumentException("ProductFile description is null or an contains an empty string");
		}
		if (version == null || version.isEmpty())
		{
			throw new IllegalArgumentException("ProductFile version is null or an contains an empty string");
		}
		if (format == null || format.isEmpty())
		{
			throw new IllegalArgumentException("ProductFile format is null or an contains an empty string");
		}
	}
}
