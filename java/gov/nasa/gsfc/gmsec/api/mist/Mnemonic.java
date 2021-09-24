/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Mnemonic.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonic;


/**
 * This class is a lightweight container for holding information
 * on a Mnemonic, and used to generate GMSEC Mnemonic messages by the 
 * ConnectionManager class.
 *
 * @see ConnectionManager
 */
public class Mnemonic
{
	private JNIMnemonic m_jniMnemonic = null;


	/**
	 * This method is for internal GMSEC API use only.
	 * @param obj Mnemonic object to reference for acquiring internal JNIMnemonic.
	 * @return Internal JNIMnemonic object.
	 */
	public static JNIMnemonic getInternal(Mnemonic obj)
	{
		return (obj == null ? null : obj.m_jniMnemonic);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jMnemonic Internal JNIMnemonic object.
	 */
	public Mnemonic(JNIMnemonic jMnemonic)
	{
		m_jniMnemonic = jMnemonic;
	}


	/**
	 * Default constructor.
	 */
	protected Mnemonic()
	{
	}


	/**
	 * Constructor - Initializes the Mnemonic object with a set of MnemonicSample information.
	 *
	 * @param name The name of the Mnemonic.
	 * @param samples The list of MnemonicSample objects.
	 *
	 * @throws IllegalArgumentException Thrown if the name is null, or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the MnemonicSample list is null.
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
	 * Constructor - Initializes a Mnemonic object with only its name.
	 *
	 * @param name The name of the Mnemonic.
	 *
	 * @throws IllegalArgumentException Thrown if the name is null, or contains an empty string.
	 */
	public Mnemonic(String name) throws IllegalArgumentException
	{
	    this(name, new java.util.ArrayList<MnemonicSample>());
    }


	/**
	 * Copy-Constructor - Makes a copy of the given Mnemonic object.
	 *
	 * @param other The Mnemonic to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the Mnemonic object is null.
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
	 * Retrieves the name of the Mnemonic.
	 *
	 * @return The name of the Mnemonic.
	 */
	public String getName()
	{
		return m_jniMnemonic.getName();
	}


	/**
	 * Returns the availability of a Mnemonic status.
	 *
	 * @return True if a Mnemonic status has been set, false otherwise.
	 */
	public boolean statusAvailable()
	{
		return m_jniMnemonic.statusAvailable();
	}


	/**
	 * Accessor for the Mnemonic status.
	 *
	 * @return A reference to the Mnemonic status field.
	 *
	 * @throws GMSEC_Exception Thrown if the status field has not been set.
	 */
	public Field getStatus() throws GMSEC_Exception
	{
		return m_jniMnemonic.getStatus();
	}


	/**
	 * Sets the given Status Field for the Mnemonic.
	 *
	 * @param status The status Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field object is null.
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
	 * Returns the availability of Mnemonic units.
	 *
	 * @return True if Mnemonic units have been set, false otherwise.
	 */
	public boolean unitsAvailable()
	{
		return m_jniMnemonic.unitsAvailable();
	}


	/**
	 * Accessor for the Mnemonic units.
	 *
	 * @return A reference to the Mnemonic units string.
	 *
	 * @throws GMSEC_Exception Thrown if the units string has not been set.
	 */
	public String getUnits() throws GMSEC_Exception
	{
		return m_jniMnemonic.getUnits();
	}


	/**
	 * Sets the given units string for the Mnemonic.
	 *
	 * @param units The units string.
	 *
	 * @throws IllegalArgumentException Thrown if the given units string is null, or contains an empty string.
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
	 * Returns the number of samples for this Mnemonic.
	 *
	 * @return the number of mnemonic samples associated with the object.
	 */
	public long getSampleCount()
	{
		return m_jniMnemonic.getSampleCount();
	}


	/**
	 * Returns a reference to a MnemonicSample held in this object.
	 *
	 * @param idx A zero-based index of the MnemonicSample object to access.
	 *
	 * @return A reference to the requested MnemonicSample object.
	 *
	 * @throws GMSEC_Exception Thrown if the index supplied is out of bounds.
	 */
	public MnemonicSample getSample(long idx) throws GMSEC_Exception
	{
		return m_jniMnemonic.getSample(idx);
	}

	                              
	/**
	 * Adds a single MnemonicSample to the current list of samples.
	 *
	 * @param sample The MnemonicSample object
	 *
	 * @throws IllegalArgumentException Thrown if the given MnemonicSample is null.
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
