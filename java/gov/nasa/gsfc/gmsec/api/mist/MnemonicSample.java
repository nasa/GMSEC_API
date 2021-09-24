/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicSample.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonicSample;


/**
 * This class is a lightweight container for holding information
 * on a Mnemonic Sample, and used to generate GMSEC Mnemonic messages by the 
 * ConnectionManager and Device classes.
 *
 * @see Mnemonic
 * @see ConnectionManager
*/
public class MnemonicSample
{
	private JNIMnemonicSample m_jniMnemonicSample = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param sample Object to reference for acquiring internal JNIMnemonicSample.
	 * @return Internal JNIMnemonicSample object.
	 */
	public static JNIMnemonicSample getInternal(MnemonicSample sample)
	{
		return (sample == null ? null : sample.m_jniMnemonicSample);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jSample Internal JNIMnemonicSample object.
	 */
	public MnemonicSample(JNIMnemonicSample jSample)
	{
		m_jniMnemonicSample = jSample;
	}


	protected MnemonicSample()
	{
	}


	/**
	 * LimitFlag enumeration of the Mnemonic object.
	 */
	public enum LimitFlag
	{
		/** Unspecified limit. */
		UNSPECIFIED,

		/** No limit violation occurred. */
		NO_LIMIT_VIOLATION,

		/** Low red limit violation occurred. */
		RED_LOW,

		/** Low yellow limit violation occurred. */
		YELLOW_LOW,

		/** High yellow limit violation occurred. */
		YELLOW_HIGH,

		/** High red limit violation occurred. */
		RED_HIGH
	}


	/**
	 * Constructor - Initializes the MnemonicSample object with a timestamp and Raw Value Field.
	 *
	 * @param timestamp The time at which the Mnemonic Sample Raw Value was taken.
	 * @param rawValue The Raw Value Field of the Mnemonic Sample.
	 *
	 * @throws IllegalArgumentException Thrown if the given timestamp string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the given Field object is null.
	 */
	public MnemonicSample(String timestamp, Field rawValue) throws IllegalArgumentException
	{
		if (timestamp == null || timestamp.isEmpty())
		{
			throw new IllegalArgumentException("MnemonicSample timestamp is null or contains an empty string");
		}
		if (rawValue == null)
		{
			throw new IllegalArgumentException("MnemonicSample raw value Field is null");
		}

		m_jniMnemonicSample = new JNIMnemonicSample(timestamp, rawValue);
	}


	/**
	 * Copy-Constructor - Initializes the MnemonicSample object from another MnemonicSample.
	 *
	 * @param other The other MnemonicSample to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given MnemonicSample object is null.
	 */
	public MnemonicSample(MnemonicSample other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("MnemonicSample is null");
		}

		m_jniMnemonicSample = new JNIMnemonicSample(MnemonicSample.getInternal(other));
	}


	/**
	 * Retrieves the timestamp of the MnemonicSample.
	 *
	 * @return The timestamp of the MnemonicSample.
	 */
	public String getTimestamp()
	{
		return m_jniMnemonicSample.getTimestamp();
	}


	/**
	 * Retrieves the Raw Value Field of the MnemonicSample.
	 *
	 * @return A reference to the Raw Value Field of the MnemonicSample.
	 */
	public Field getRawValue()
	{
		return m_jniMnemonicSample.getRawValue();
	}


	/**
	 * Returns the availability of a MnemonicSample value in engineering units.
	 *
	 * @return True if MnemonicSample EU value has been set, false otherwise.
	 */
	public boolean hasEUValue()
	{
		return m_jniMnemonicSample.hasEUValue();
	}


	/**
	 * Accessor for getting the EU Value.
	 *
	 * @return A reference to the MnemonicSample EU value field.
	 *
	 * @throws GMSEC_Exception Thrown if the EU value field has not been set.
	 */
	public Field getEUValue() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getEUValue();
	}


	/**
	 * Can be used to set the EU Value Field of the MnemonicSample.
	 *
	 * @param value The EU Value Field object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Field value is null.
	 */
	public void setEUValue(Field value) throws IllegalArgumentException
	{
		if (value == null)
		{
			throw new IllegalArgumentException("EU Field value is null");
		}

		m_jniMnemonicSample.setEUValue(value);
	}


	/**
	 * Returns the availability of a MnemonicSample Text Value.
	 *
	 * @return True if MnemonicSample text value has been set, false otherwise.
	 */
	public boolean hasTextValue()
	{
		return m_jniMnemonicSample.hasTextValue();
	}


	/**
	 * Accessor for getting the Text Value of the Mnemonic Sample.
	 *
	 * @return The Text Value string.
	 *
	 * @throws GMSEC_Exception Thrown if the Text Value has not been set.
	 */
	public String getTextValue() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getTextValue();
	}


	/**
	 * Can be used to set the Text Value of the MnemonicSample.
	 *
	 * @param value The Text Value string.
	 *
	 * @throws IllegalArgumentException Thrown if the value string is null or contains an empty string.
	 */
	public void setTextValue(String value) throws IllegalArgumentException
	{
		if (value == null || value.isEmpty())
		{
			throw new IllegalArgumentException("Text value is null or contains an empty string");
		}

		m_jniMnemonicSample.setTextValue(value);
	}


	/**
	 * Returns the availability of a MnemonicSample flags field.
	 *
	 * @return True if MnemonicSample flags value has been set, false otherwise.
	 */
	public boolean hasFlags()
	{
		return m_jniMnemonicSample.hasFlags();
	}


	/**
	 * Accessor for getting the Flags value of the MnemonicSample.
	 *
	 * @return The Flags value.
	 *
	 * @throws GMSEC_Exception Thrown if the flags field has not been set.
	 */
	public int getFlags() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getFlags();
	}


	/**
	 * Can be used to set the Flags value of the MnemonicSample.
	 *
	 * @param flags The Flags value.
	 */
	public void setFlags(int flags)
	{
		m_jniMnemonicSample.setFlags(flags);
	}


	/**
	 * Returns the availability of a MnemonicSample Limit Enable/Disable flag.
	 *
	 * @return True if MnemonicSample Limit Enable/Disable flag has been set, false otherwise.
	 */
	public boolean hasLimitEnableDisable()
	{
		return m_jniMnemonicSample.hasLimitEnableDisable();
	}


	/**
	 * Accessor for getting the Limit Enable/Disable flag of the MnemonicSample.
	 *
	 * @return The MnemonicSample Limit Enable/Disable flag.
	 *
	 * @throws GMSEC_Exception Thrown if the Limit Enable/Disable flag has not been set.
	 */
	public boolean getLimitEnableDisable() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getLimitEnableDisable();
	}


	/**
	 * Can be used to set the Limit Enable/Disable flag of the MnemonicSample.
	 *
	 * @param value The Limit Enable/Disable flag.
	 */
	public void setLimitEnableDisable(boolean value)
	{
		m_jniMnemonicSample.setLimitEnableDisable(value);
	}


	/**
	 * Returns the availability of a MnemonicSample Limit Flag.
	 *
	 * @return True if MnemonicSample Limit Flag has been set, false otherwise.
	 */
	public boolean hasLimit()
	{
		return m_jniMnemonicSample.hasLimit();
	}


	/**
	 * Accessor to get the Limit Flag of the MnemonicSample.
	 *
	 * @return The MnemonicSample Limit Flag.
	 *
	 * @throws GMSEC_Exception Thrown if the Limit Flag has not been set.
	 */
	public LimitFlag getLimit() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getLimit();
	}


	/**
	 * Can be used to set the Limit Flag of the MnemonicSample.
	 *
	 * @param flag The Limit Flag.
	 */
	public void setLimit(LimitFlag flag)
	{
		m_jniMnemonicSample.setLimit(flag);
	}


	/**
	 * Returns the availability of a MnemonicSample Staleness Status.
	 *
	 * @return True if MnemonicSample Staleness Status has been set, false otherwise.
	 */
	public boolean hasStalenessStatus()
	{
		return m_jniMnemonicSample.hasStalenessStatus();
	}


	/**
	 * Accessor to get the Staleness Status flag of the MnemonicSample.
	 *
	 * @return The MnemonicSample Staleness Status flag.
	 *
	 * @throws GMSEC_Exception Thrown if the Staleness Status flag has not been set.
	 */
	public boolean getStalenessStatus() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getStalenessStatus();
	}


	/**
	 * Can be used to set the Staleness Status flag of the MnemonicSample.
	 *
	 * @param staleness The Staleness Status flag.
	 */
	public void setStalenessStatus(boolean staleness)
	{
		m_jniMnemonicSample.setStalenessStatus(staleness);
	}


	/**
	 * Returns the availability of a MnemonicSample Quality flag.
	 *
	 * @return True if MnemonicSample Quality has been set, false otherwise.
	 */
	public boolean hasQuality()
	{
		return m_jniMnemonicSample.hasQuality();
	}


	/**
	 * Accessor for getting the Quality flag.
	 *
	 * @return The MnemonicSample Quality flag.
	 *
	 * @throws GMSEC_Exception Thrown if the Quality flag has not been set.
	 */
	public boolean getQuality() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getQuality();
	}


	/**
	 * Can be used to set the Quality flag of the MnemonicSample.
	 *
	 * @param quality The Quality flag.
	 */
	public void setQuality(boolean quality)
	{
		m_jniMnemonicSample.setQuality(quality);
	}
}
