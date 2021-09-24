/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceIterator.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDeviceIterator;


/**
 * A class that can be used to iterate over the Device object(s) contained within a DeviceMessage.
 * <p>
 * An example usage is:
 * <pre>{@code
 * DeviceIterator iter = devMessage.getDeviceIterator()
 *
 * while (iter.hasNext())
 * {
 *     Device device = iter.next();
 *
 *     ...
 * }
 * }</pre>
 */
public class DeviceIterator
{
	private JNIDeviceIterator m_jniIter;


	private DeviceIterator()
	{
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param iter Object to reference for acquiring internal JNIDeviceIterator.
	 * @return Internal JNIDeviceIterator object.
	 */
	public static JNIDeviceIterator getInternal(DeviceIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jIter Internal JNIDeviceIterator object.
	 */
	public DeviceIterator(JNIDeviceIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * Indicates whether there are additional Device objects that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more devices are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * Returns the next Device accessible using this iterator.
	 *
	 * @return The next available Device object.
	 *
	 * @throws GMSEC_Exception Thrown if this method is called and there are no
	 * more Devices accessible using this iterator.
	 */
	public Device next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * Resets this iterator so that it can be used again to iterate over the devices.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
