/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Mnemonic.java
 *
 *  @brief This file contains declarations for Mnemonic containers
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonic;


/**
 * @class Mnemonic
 *
 * @brief This class is a lightweight container for holding information
 * on a Mnemonic, and used to generate GMSEC Mnemonic messages by the 
 * ConnectionManager class
 *
 * @sa ConnectionManager @n
 */
public class Mnemonic
{
	private JNIMnemonic m_jniMnemonic = null;


	public static JNIMnemonic getInternal(Mnemonic obj)
	{
		return (obj == null ? null : obj.m_jniMnemonic);
	}


	public Mnemonic(JNIMnemonic jMnemonic)
	{
		m_jniMnemonic = jMnemonic;
	}


	protected Mnemonic()
	{
	}


	/**
	 * @fn Mnemonic(String name, java.util.List<MnemonicSample> samples)
	 *
	 * @brief Constructor - Initializes the Mnemonic object with a set of
	 * MnemonicSample information
	 *
	 * @param name - The name of the Mnemonic
	 * @param samples - The list of MnemonicSample objects
	 *
	 * @throw An IllegalArgumentException is thrown if the name is null, or contains an empty string.
	 * @throw An IllegalArgumentException is thrown if the MnemonicSample list is null.
	 */
	public Mnemonic(String name, java.util.List<MnemonicSample> samples) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Mnemonic name is null or contains an empty string");
		}
		if (samples == null)
		{
			throw new IllegalArgumentException("MnemonicSample list is null");
		}

		m_jniMnemonic = new JNIMnemonic(name, samples);
	}


	/**
	 * @fn Mnemonic(Mnemonic other)
	 *
	 * @brief Copy-Constructor - Makes a copy of the given Mnemonic object.
	 *
	 * @param other - The Mnemonic to copy.
	 *
	 * @throw An IllegalArgumentException is thrown if the Mnemonic object is null.
	 */
	public Mnemonic(Mnemonic other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("Mnemonic is null");
		}

		m_jniMnemonic = new JNIMnemonic(Mnemonic.getInternal(other));
	}


	/**
	 * @fn String getName()
	 *
	 * @brief Retrieves the name of the Mnemonic
	 *
	 * @return The name of the Mnemonic
	 */
	public String getName()
	{
		return m_jniMnemonic.getName();
	}


	/**
	 * @fn boolean statusAvailable()
	 *
	 * @brief Returns the availability of a Mnemonic status
	 *
	 * @return True if a Mnemonic status has been set, false otherwise
	 */
	public boolean statusAvailable()
	{
		return m_jniMnemonic.statusAvailable();
	}


	/**
	 * @fn Field getStatus()
	 *
	 * @brief Accessor for the Mnemonic status
	 *
	 * @return A reference to the Mnemonic status field
	 *
	 * @throw A GMSEC_Exception is thrown if the status field has not been set
	 */
	public Field getStatus() throws GMSEC_Exception
	{
		return m_jniMnemonic.getStatus();
	}


	/**
	 * @fn void setStatus(Field status)
	 *
	 * @brief Supplies a Status for the Mnemonic and makes a copy of the field
	 *
	 * @throws An IllegalArgumentException is thrown if the given Field object is null.
	 */
	public void setStatus(Field status) throws IllegalArgumentException
	{
		if (status == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		m_jniMnemonic.setStatus(status);
	}


	/**
	 * @fn boolean unitsAvailable()
	 *
	 * @brief Returns the availability of Mnemonic units
	 *
	 * @return True if Mnemonic units have been set, false otherwise
	 */
	public boolean unitsAvailable()
	{
		return m_jniMnemonic.unitsAvailable();
	}


	/**
	 * @fn String getUnits()
	 *
	 * @brief Accessor for the Mnemonic units
	 *
	 * @return A reference to the Mnemonic units string
	 *
	 * @throw A GMSEC_Exception is thrown if the units string has not been set
	 */
	public String getUnits() throws GMSEC_Exception
	{
		return m_jniMnemonic.getUnits();
	}


	/**
	 * @fn void setUnits(String units)
	 *
	 * @brief Supplies a units string for the Mnemonic and makes a copy of the string
	 *
	 * @throws An IllegalArgumentException is thrown if the given units string is null, or contains an empty string.
	 */
	public void setUnits(String units) throws IllegalArgumentException
	{
		if (units == null || units.isEmpty())
		{
			throw new IllegalArgumentException("Units is null or contains an empty string");
		}

		m_jniMnemonic.setUnits(units);
	}


	/**
	 * @fn long getSampleCount()
	 *
	 * @brief Returns the number of samples for this Mnemonic
	 *
	 * @return the number of mnemonic samples associated with the object
	 */
	public long getSampleCount()
	{
		return m_jniMnemonic.getSampleCount();
	}


	/**
	 * @fn MnemonicSample getSample(long idx)
	 *
	 * @brief Returns a reference to a MnemonicSample held in this object
	 *
	 * @param idx - the zero-based index of the MnemonicSample object to access
	 *
	 * @return A reference to the requested MnemonicSample object
	 *
	 * @throw A GMSEC_Exception is thrown if the index supplied is out of bounds
	 */
	public MnemonicSample getSample(long idx) throws GMSEC_Exception
	{
		return m_jniMnemonic.getSample(idx);
	}

	                              
	/**
	 * @fn void addSample(MnemonicSample sample)
	 *
	 * @brief Adds a single MnemonicSample to the current list of samples.
	 *
	 * @param param - the MnemonicSample object
	 *
	 * @throws An IllegalArgumentException is thrown if the given MnemonicSample is null.
	 */
	public void addSample(MnemonicSample sample) throws IllegalArgumentException
	{
		if (sample == null)
		{
			throw new IllegalArgumentException("MnemonicSample is null");
		}

		m_jniMnemonic.addSample(sample);
	}
}
