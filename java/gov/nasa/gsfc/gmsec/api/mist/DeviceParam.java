/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file DeviceParam.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDeviceParam;


/**
 * This class is a container for holding information
 * on a Simple Device parameter, and used to generate GMSEC Simple Device
 * messages by the ConnectionManager class.
 *
 * @see ConnectionManager
 */
public class DeviceParam
{
	private JNIDeviceParam m_jniParam = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param param Object to reference for acquiring internal JNIDeviceParam.
	 * @return Internal JNIDeviceParam object.
	 */
	public static JNIDeviceParam getInternal(DeviceParam param)
	{
		return (param == null ? null : param.m_jniParam);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jParam Internal JNIDeviceParam object.
	 */
	public DeviceParam(JNIDeviceParam jParam)
	{
		m_jniParam = jParam;
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 */
	protected DeviceParam()
	{
	}


	/**
	 * Constructor - Initializes the DeviceParam object with a name, a timestamp and a Field (containing a sampled value).
	 *
	 * @param name  The name of the DeviceParam.
	 * @param timestamp The time at which the sampled value was taken.
	 * @param field The field containing sampled value.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the timestamp string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the Field object is null.
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
	 * Constructor - Initializes the DeviceParam object from another DeviceParam.
	 *
	 * @param other The other DeviceParam to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the DeviceParam object that is given is null.
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
	 * Retrieves the name of the DeviceParam.
	 *
	 * @return The name of the DeviceParam.
	 */
	public String getName()
	{
		return m_jniParam.getName();
	}


	/**
	 * Retrieves the timestamp of the DeviceParam.
	 *
	 * @return The timestamp of the DeviceParam.
	 */
	public String getTimestamp()
	{
		return m_jniParam.getTimestamp();
	}


	/**
	 * Retrieves the Field value associated with the DeviceParam.
	 *
	 * @return The Field value of the DeviceParam.
	 */
	public Field getValue()
	{
		return m_jniParam.getValue();
	}
}
