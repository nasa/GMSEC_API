/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ServiceParam.java
 *
 *  @brief This file contains declarations for Simple Service containers
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIServiceParam;


/**
 * @class ServiceParam
 *
 * @brief This class is a container for holding information
 * on a Simple Service parameter, and used to generate GMSEC Simple Service
 * messages by the ConnectionManager class
 *
 * @sa ConnectionManager @n
 *		
*/
public class ServiceParam
{
	private JNIServiceParam m_jniParam = null;


	public static JNIServiceParam getInternal(ServiceParam param)
	{
		return (param == null ? null : param.m_jniParam);
	}


	public ServiceParam(JNIServiceParam jParam)
	{
		m_jniParam = jParam;
	}


	protected ServiceParam()
	{
	}


	/**
	 * @fn ServiceParam(String name, Field field)
	 *
	 * @brief Constructor - Initializes the ServiceParam object with 
	 * a name and a Field object
	 *
	 * @param name  - The name of the ServiceParam
	 * @param field - The field to store within the ServiceParam
	 *
	 * @throws An IllegalArgumentException is thrown if the Service Parameter name is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the Field object is null.
	 */
	public ServiceParam(String name, Field field) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("ServiceParam name is null or contains an empty string");
		}
		if (field == null)
		{
			throw new IllegalArgumentException("Field is null");
		}

		m_jniParam = new JNIServiceParam(name, field);
	}


	/**
	 * @fn ServiceParam(ServiceParam other)
	 *
	 * @brief Constructor - Initializes the ServiceParam object from another
	 * ServiceParam
	 *
	 * @param other - The other ServiceParam to inherit values from
	 *
	 * @throws An IllegalArgumentException is thrown if the given ServiceParam object is null.
	 */
	public ServiceParam(ServiceParam other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("ServiceParam is null");
		}

		m_jniParam = new JNIServiceParam(other.m_jniParam);
	}


	/**
	 * @fn String getName()
	 *
	 * @brief Retrieves the name of the ServiceParam
	 *
	 * @return The name of the ServiceParam
	 */
	public String getName()
	{
		return m_jniParam.getName();
	}


	/**
	 * @fn Field getValue()
	 *
	 * @brief Retrieves the Field value associated with the ServiceParam
	 *
	 * @return The Field value of the ServiceParam
	 */
	public Field getValue()
	{
		return m_jniParam.getValue();
	}
}
