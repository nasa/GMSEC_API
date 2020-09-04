/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file DeviceParam.java
 *
 *  @brief This file contains declarations for Simple Device containers
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDeviceParam;


/**
 * @class DeviceParam
 *
 * @brief This class is a container for holding information
 * on a Simple Device parameter, and used to generate GMSEC Simple Device
 * messages by the ConnectionManager class
 *
 * @sa ConnectionManager @n
 *		
*/
public class DeviceParam
{
	private JNIDeviceParam m_jniParam = null;


	public static JNIDeviceParam getInternal(DeviceParam param)
	{
		return (param == null ? null : param.m_jniParam);
	}


	public DeviceParam(JNIDeviceParam jParam)
	{
		m_jniParam = jParam;
	}


	protected DeviceParam()
	{
	}


	/**
	 * @fn DeviceParam(String name, String timestamp, Field field)
	 *
	 * @brief Constructor - Initializes the DeviceParam object with 
	 * a name, a timestamp and a Field (containing a sampled value).
	 *
	 * @param name  - The name of the DeviceParam
	 * @param timestamp - The time at which the sampled value was taken
	 * @param field - The field containing sampled value
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the timestamp string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the Field object is null.
	 */
	public DeviceParam(String name, String timestamp, Field field) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("DeviceParam name is null or contains an empty string");
		}
		if (timestamp == null || timestamp.isEmpty())
		{
			throw new IllegalArgumentException("DeviceParam timestamp is null or contains an empty string");
		}
		if (field == null)
		{
			throw new IllegalArgumentException("DeviceParam Field is null");
		}

		m_jniParam = new JNIDeviceParam(name, timestamp, field);
	}


	/**
	 * @fn DeviceParam(DeviceParam other)
	 *
	 * @brief Constructor - Initializes the DeviceParam object from another DeviceParam
	 *
	 * @param other - The other DeviceParam to inherit values from
	 *
	 * @throws An IllegalArgumentException is thrown if the DeviceParam object that is given is null.
	 */
	public DeviceParam(DeviceParam other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("DeviceParam is null");
		}

		m_jniParam = new JNIDeviceParam(other.m_jniParam);
	}


	/**
	 * @fn String getName()
	 *
	 * @brief Retrieves the name of the DeviceParam
	 *
	 * @return The name of the DeviceParam
	 */
	public String getName()
	{
		return m_jniParam.getName();
	}


	/**
	 * @fn String getTimestamp()
	 *
	 * @brief Retrieves the timestamp of the DeviceParam
	 *
	 * @return The timestamp of the DeviceParam
	 */
	public String getTimestamp()
	{
		return m_jniParam.getTimestamp();
	}


	/**
	 * @fn Field getValue()
	 *
	 * @brief Retrieves the Field value associated with the DeviceParam
	 *
	 * @return The Field value of the DeviceParam
	 */
	public Field getValue()
	{
		return m_jniParam.getValue();
	}
}
