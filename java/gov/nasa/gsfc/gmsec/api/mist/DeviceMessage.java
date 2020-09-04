/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceMessage.java
 *
 * @brief The DeviceMessage object is an extension of Message, and serves as a container
 * for a GMSEC C2CX %Device %Message.
 *
 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.util.Log;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIDeviceMessage;
import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMistMessage;


/**
 * @class DeviceMessage
 *
 * @brief The DeviceMessage object is an extension of Message, and serves as a container for a GMSEC C2CX %Device %Message.
 * The methods of this class allow for the construction and manipulation of the data within the message.
 *
 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.
 */
public class DeviceMessage extends Message
{
	/**
	 * @fn DeviceMessage(String subject, int version)
	 *
	 * @brief Specialized constructor where a GMSEC Interface Specification Document version number can be specified.
	 *
	 * @param subject - the GMSEC subject to associate with the message.
	 * @param version - the version of the GMSEC ISD to be used.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.
	 */
	public DeviceMessage(String subject, int version) throws IllegalArgumentException
	{
		super((JNIDeviceMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.");

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}

		registerChild(new JNIDeviceMessage(subject, JNIMistMessage.buildSpecification(version)));
	}


	/**
	 * @fn DeviceMessage(String subject, Config config, int version)
	 *
	 * @brief Specialized constructor that accepts a Config object, and where a GMSEC Interface Specification Document
	 * version number can be speficied.
	 *
	 * @param subject - the GMSEC subject to associate with the message.
	 * @param config  - a configuration object that contains message-specific options.
	 * @param version - the version of the GMSEC ISD to be used.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the config object is null.
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.
	 */
	public DeviceMessage(String subject, Config config, int version) throws IllegalArgumentException
	{
		super((JNIDeviceMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.");

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}

		registerChild(new JNIDeviceMessage(subject, config, JNIMistMessage.buildSpecification(version)));
	}


	/**
	 * @fn DeviceMessage(String data)
	 *
	 * @brief Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data - the XML or JSON string that represents a GMSEC message.
	 *
	 * @throws An IllegalArgumentException is thrown if the data string is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the data string cannot be parsed.
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.
	 */
	public DeviceMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIDeviceMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.");

		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIDeviceMessage(data));
	}


	/**
	 * @fn DeviceMessage(DeviceMessage other)
	 *
	 * @brief Copy constructor that initializes the message instance using another DeviceMessage object.
	 *
	 * @param other - the other DeviceMessage object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the device message object is null.
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.
	 */
	public DeviceMessage(DeviceMessage other) throws IllegalArgumentException
	{
		super((JNIDeviceMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.DeviceMessage instead.");

		if (other == null)
		{
			throw new IllegalArgumentException("DeviceMessage cannot be null");
		}

		registerChild(new JNIDeviceMessage((JNIDeviceMessage) getInternal(other)));
	}


	/**
	 * @fn void addDevice(Device device)
	 *
	 * @brief Adds the given Device object to the internal list of devices maintained by the message.
	 *
	 * @param device - the Device object to add to the message.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Device object is null.
	 */
	public void addDevice(Device device) throws IllegalArgumentException
	{
		if (device == null)
		{
			throw new IllegalArgumentException("Device is null");
		}

		((JNIDeviceMessage) getInternal(this)).addDevice(Device.getInternal(device));
	}


	/**
	 * @fn int getNumDevices()
	 *
	 * @brief Returns the number of Device objects associated with this DeviceMessage.
	 *
	 * @return The number of device objects.
	 *
	 * @sa getDevice()
	 */
	public int getNumDevices()
	{
		return ((JNIDeviceMessage) getInternal(this)).getNumDevices();
	}


	/**
	 * @fn Device getDevice(int index)
	 *
	 * @brief Returns Device object at specified index.
	 *
	 * @return A Device object.
	 *
	 * @throws A GMSEC_Exception is thrown if the specified index is outside the number of devices in the message.
	 *
	 * @sa getNumDevices()
	 */
	public Device getDevice(int index) throws GMSEC_Exception
	{
		return ((JNIDeviceMessage) getInternal(this)).getDevice(index);
	}


	/**
	 * @fn DeviceIterator getDeviceIterator()
	 *
	 * @brief Method that allows the callee to get the DeviceIterator associated with the %DeviceMessage.
	 * This iterator will allow for applications to iterate over the Device objects stored within the %DeviceMessage.
	 * The iterator is reset each time getDeviceIterator() is called.  The iterator itself is destroyed when the
	 * %DeviceMessage object is destroyed.
	 *
	 * @note Only one DeviceIterator object is associated with a %DeviceMessage object; multiple calls to
	 * getDeviceIterator() will return a reference to the same DeviceIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a DeviceIterator object.
	 */
	public DeviceIterator getDeviceIterator()
	{
		return ((JNIDeviceMessage) getInternal(this)).getDeviceIterator();
	}


	/**
	 * @fn DeviceMessage convertMessage(Message msg)
	 *
	 * @brief Constructs and returns a DeviceMessage that is created using the given Message.
	 *
	 * @return A DeviceMessage object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 */
	public static DeviceMessage convertMessage(Message msg) throws IllegalArgumentException
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		DeviceMessage devMsg = null;

		try
		{
			devMsg = new DeviceMessage(msg.toXML());
		}
		catch (GMSEC_Exception e)
		{
			throw new IllegalArgumentException(e.toString());
		}

		return devMsg;
	}
}
