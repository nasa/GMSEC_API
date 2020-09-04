/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MnemonicSample.java
 *
 *  @brief This file contains declarations for Mnemonic parameters
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonicSample;


/**
 * @class MnemonicSample
 *
 * @brief This class is a lightweight container for holding information
 * on a Mnemonic Sample, and used to generate GMSEC Mnemonic messages by the 
 * ConnectionManager and Device classes
 *
 * @sa Mnemonic
 * @sa ConnectionManager
*/
public class MnemonicSample
{
	private JNIMnemonicSample m_jniMnemonicSample = null;


	public static JNIMnemonicSample getInternal(MnemonicSample sample)
	{
		return (sample == null ? null : sample.m_jniMnemonicSample);
	}


	public MnemonicSample(JNIMnemonicSample jSample)
	{
		m_jniMnemonicSample = jSample;
	}


	protected MnemonicSample()
	{
	}


	/**
	 * @enum LimitFlag
	 *
	 * @desc LimitFlag enumeration for the Mnemonic object
	 */
	public enum LimitFlag
	{
		UNSPECIFIED,          ///< Unspecified
		NO_LIMIT_VIOLATION,   ///< No Limit Violation
		RED_LOW,              ///< Red Low
		YELLOW_LOW,           ///< Yellow Low
		YELLOW_HIGH,          ///< Yellow High
		RED_HIGH              ///< Red High
	}


	/**
	 * @fn MnemonicSample(String timestamp, Field rawValue)
	 *
	 * @brief Constructor - Initializes the MnemonicSample object with 
	 * a timestamp and value
	 *
	 * @param timestamp - The time at which the sampled value was taken
	 * @param rawValue - The raw value of the parameter
	 *
	 * @throws An IllegalArgumentException is thrown if the given timestamp string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given Field object is null.
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
	 * @fn MnemonicSample(MnemonicSample other)
	 *
	 * @brief Copy-Constructor - Initializes the MnemonicSample object from another MnemonicSample
	 *
	 * @param other - The other MnemonicSample to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given MnemonicSample object is null.
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
	 * @fn String getTimestamp()
	 *
	 * @brief Retrieves the timestamp of the MnemonicSample
	 *
	 * @return The timestamp of the MnemonicSample
	 */
	public String getTimestamp()
	{
		return m_jniMnemonicSample.getTimestamp();
	}


	/**
	 * @fn Field getRawValue() const
	 *
	 * @brief Retrieves the value of the MnemonicSample
	 *
	 * @return The value of the MnemonicSample
	 */
	public Field getRawValue()
	{
		return m_jniMnemonicSample.getRawValue();
	}


	/**
	 * @fn boolean hasEUValue()
	 *
	 * @brief Returns the availability of a MnemonicSample value in
	 * engineering units
	 *
	 * @return True if MnemonicSample EU value has been set, false otherwise
	 */
	public boolean hasEUValue()
	{
		return m_jniMnemonicSample.hasEUValue();
	}


	/**
	 * @fn Field getEUValue() const
	 *
	 * @brief Accessor for the EU Value
	 *
	 * @return A reference to the MnemonicSample EU value field
	 *
	 * @throws A GMSEC_Exception is thrown if the EU value field has not been set
	 */
	public Field getEUValue() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getEUValue();
	}


	/**
	 * @fn void setEUValue(Field value)
	 *
	 * @brief Supplies a field for the MnemonicSample EU value and makes a copy of the field
	 *
	 * @throws An IllegalArgumentException is thrown if the given Field value is null.
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
	 * @fn boolean hasTextValue()
	 *
	 * @brief Returns the availability of a MnemonicSample text value
	 *
	 * @return True if MnemonicSample text value has been set, false otherwise
	 */
	public boolean hasTextValue()
	{
		return m_jniMnemonicSample.hasTextValue();
	}


	/**
	 * @fn String getTextValue()
	 *
	 * @brief Accessor for the text value
	 *
	 * @return A reference to the MnemonicSample text value field
	 *
	 * @throw A GMSEC_Exception is thrown if the text value field has not been set
	 */
	public String getTextValue() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getTextValue();
	}


	/**
	 * @fn void setTextValue(String value)
	 *
	 * @brief Supplies a string for the MnemonicSample text value and makes a copy of the string
	 *
	 * @throws An IllegalArgumentException is thrown if the value string is null or contains an empty string.
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
	 * @fn boolean hasFlags()
	 *
	 * @brief Returns the availability of a MnemonicSample flags field
	 *
	 * @return True if MnemonicSample flags value has been set, false otherwise
	 */
	public boolean hasFlags()
	{
		return m_jniMnemonicSample.hasFlags();
	}


	/**
	 * @fn int getFlags() const
	 *
	 * @brief Accessor for the flags
	 *
	 * @return A reference to the MnemonicSample flags field
	 *
	 * @throw A GMSEC_Exception is thrown if the flags field has not been set
	 */
	public int getFlags() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getFlags();
	}


	/**
	 * @fn void setFlags(int flags)
	 *
	 * @brief Supplies a GMSEC_I32 for the MnemonicSample flags value
	 */
	public void setFlags(int flags)
	{
		m_jniMnemonicSample.setFlags(flags);
	}


	/**
	 * @fn boolean hasLimitEnableDisable()
	 *
	 * @brief Returns the availability of a MnemonicSample limit enable/disable flag
	 *
	 * @return True if MnemonicSample limit enable/disable flag has been set, false otherwise
	 */
	public boolean hasLimitEnableDisable()
	{
		return m_jniMnemonicSample.hasLimitEnableDisable();
	}


	/**
	 * @fn boolean getLimitEnableDisable()
	 *
	 * @brief Accessor for the limit enable/disable flag 
	 *
	 * @return The MnemonicSample limit enable/disable flag
	 *
	 * @throw A GMSEC_Exception is thrown if the limit enable/disable flag has not been set
	 */
	public boolean getLimitEnableDisable() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getLimitEnableDisable();
	}


	/**
	 * @fn void setLimitEnableDisable(boolean value)
	 *
	 * @brief Supplies a boolean flag for the MnemonicSample setLimitEnableDisable(bool value)
	 */
	public void setLimitEnableDisable(boolean value)
	{
		m_jniMnemonicSample.setLimitEnableDisable(value);
	}


	/**
	 * @fn boolean hasLimit()
	 *
	 * @brief Returns the availability of a MnemonicSample limit
	 *
	 * @return True if MnemonicSample limit has been set, false otherwise
	 */
	public boolean hasLimit()
	{
		return m_jniMnemonicSample.hasLimit();
	}


	/**
	 * @fn LimitFlag getLimit()
	 *
	 * @brief Accessor for the limit
	 *
	 * @return The MnemonicSample limit
	 *
	 * @throw A GMSEC_Exception is thrown if the limit has not been set
	 */
	public LimitFlag getLimit() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getLimit();
	}


	/**
	 * @fn void setLimit(LimitFlag flag)
	 *
	 * @brief Supplies a limit flag for the MnemonicSample limit
	 */
	public void setLimit(LimitFlag flag)
	{
		m_jniMnemonicSample.setLimit(flag);
	}


	/**
	 * @fn boolean hasStalenessStatus()
	 *
	 * @brief Returns the availability of a MnemonicSample staleness status
	 *
	 * @return True if MnemonicSample staleness has been set, false otherwise
	 */
	public boolean hasStalenessStatus()
	{
		return m_jniMnemonicSample.hasStalenessStatus();
	}


	/**
	 * @fn boolean getStalenessStatus()
	 *
	 * @brief Accessor for the staleness
	 *
	 * @return The MnemonicSample staleness
	 *
	 * @throw A GMSEC_Exception is thrown if the staleness flag has not been set
	 */
	public boolean getStalenessStatus() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getStalenessStatus();
	}


	/**
	 * @fn void setStalenessStatus(boolean staleness)
	 *
	 * @brief Supplies a staleness flag for the MnemonicSample
	 */
	public void setStalenessStatus(boolean staleness)
	{
		m_jniMnemonicSample.setStalenessStatus(staleness);
	}


	/**
	 * @fn boolean hasQuality()
	 *
	 * @brief Returns the availability of a MnemonicSample quality flag
	 *
	 * @return True if MnemonicSample quality has been set, false otherwise
	 */
	public boolean hasQuality()
	{
		return m_jniMnemonicSample.hasQuality();
	}


	/**
	 * @fn boolean getQuality()
	 *
	 * @brief Accessor for the quality
	 *
	 * @return The MnemonicSample quality
	 *
	 * @throw A GMSEC_Exception is thrown if the quality flag has not been set
	 */
	public boolean getQuality() throws GMSEC_Exception
	{
		return m_jniMnemonicSample.getQuality();
	}


	/**
	 * @fn void setQuality(boolean quality)
	 *
	 * @brief Supplies a quality flag for the MnemonicSample
	 */
	public void setQuality(boolean quality)
	{
		m_jniMnemonicSample.setQuality(quality);
	}
}
