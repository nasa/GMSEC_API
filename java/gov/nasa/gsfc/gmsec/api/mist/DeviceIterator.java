/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceIterator.java
 *
 * @brief A class that can be used to iterate over the Fields contained within a Message.
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDeviceIterator;


/**
 * @class DeviceIterator
 *
 * @brief A class that can be used to iterate over the Device object(s) contained within a DeviceMessage.
 *
 * An example usage is:
 * @code
 * DeviceIterator iter = devMessage.getDeviceIterator()
 *
 * while (iter.hasNext())
 * {
 *     Device device = iter.next();
 *
 *     ...
 * }
 * @endcode
 */
public class DeviceIterator
{
	private JNIDeviceIterator m_jniIter;


	private DeviceIterator()
	{
	}


	public static JNIDeviceIterator getInternal(DeviceIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	public DeviceIterator(JNIDeviceIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * @fn boolean hasNext()
	 *
	 * @brief Indicates whether there are additional Device objects that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more devices are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * @fn Device next()
	 *
	 * @brief Returns the next Device accessible using this iterator.
	 *
	 * @throw A GMSEC_Exception is thrown if this method is called and there are no
	 * more Devices accessible using this iterator.
	 */
	public Device next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * @fn void reset()
	 *
	 * @desc Resets this iterator so that it can be used again to iterate over the
	 * devices.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
