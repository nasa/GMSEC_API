/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceMessage.java
 */

package gov.nasa.gsfc.gmsec.api.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.mist.Device;
import gov.nasa.gsfc.gmsec.api.mist.DeviceIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDevice;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIDeviceMessage;
import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMistMessage;


/**
 * The DeviceMessage object is an extension of Message, and serves as a container for a C2CX Device Message.
 * The methods of this class allow for the construction and manipulation of the data within the message.
 *
 * @see Message
 * @see MistMessage
 * @see Config
 * @see Specification 
 * @see gov.nasa.gsfc.gmsec.api.field.Field
 * @see gov.nasa.gsfc.gmsec.api.MessageFieldIterator
 */
public class DeviceMessage extends MistMessage
{
	public DeviceMessage(JNIDeviceMessage jDevMsg)
	{
		super(jDevMsg);
	}


	/**
	 * Default constructor - Initializes the message instance
	 *
	 * @param subject The subject string for the message.
	 * @param spec    A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
	 */
	public DeviceMessage(String subject, Specification spec) throws IllegalArgumentException
	{
		super((JNIDeviceMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor an empty string");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIDeviceMessage(subject, Specification.getInternal(spec)));
	}


   /**
     * Constructor - Initializes the message instance and associates a Configuration object
     *
     * @param subject The subject string for the message.
     * @param config  A configuration to associate with the message.
     * @param spec    A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Config object is null.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
	 */
	public DeviceMessage(String subject, Config config, Specification spec) throws IllegalArgumentException
	{
		super((JNIDeviceMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIDeviceMessage(subject, Config.getInternal(config), Specification.getInternal(spec)));
	}


	/**
	 * Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data The XML or JSON string that represents a GMSEC message.
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the data string cannot be parsed.
	 *
	 * @deprecated This constructor has been deprecated; use DeviceMessage(Specification, String) instead.
	 */
	@Deprecated
	public DeviceMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIDeviceMessage) null);

		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor an empty string");
		}

		registerChild(new JNIDeviceMessage(data));
	}


	/**
	 * Initializes a DeviceMessage using an XML or JSON string, and ensures the message adheres
	 * to the provided specification.
	 *
	 * @param spec The Specification this message's schema will adhere to.
	 * @param data XML or JSON string used to initialize the message
	 *
	 * @throws IllegalArgumentException Thrown if the Specification is null.
	 * @throws IllegalArgumentException Thrown if the data string is null.
	 * @throws GMSEC_Exception Thrown if data string contains invalid an XML or JSON statement.
	 *
	 * @see Message#toXML()
	 * @see Message#toJSON()
	 */
	public DeviceMessage(Specification spec, String data)
		throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMistMessage) null);

		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIDeviceMessage(Specification.getInternal(spec), data));
	}


	/**
	 * Copy constructor that initializes the message instance using another DeviceMessage object.
	 *
	 * @param other The other DeviceMessage object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the device message object is null.
	 */
	public DeviceMessage(DeviceMessage other) throws IllegalArgumentException
	{
		super((JNIDeviceMessage) null);

		if (other == null)
		{
			throw new IllegalArgumentException("DeviceMessage cannot be null");
		}

		registerChild(new JNIDeviceMessage((JNIDeviceMessage) MistMessage.getInternal(other)));
	}


	/**
	 * Adds the given Device object to the internal list of devices maintained by the message.
	 *
	 * @param device The Device object to add to the message.
	 *
	 * @throws IllegalArgumentException Thrown if the given Device object is null.
	 */
	public void addDevice(Device device) throws IllegalArgumentException
	{
		if (device == null)
		{
			throw new IllegalArgumentException("Device is null");
		}

		((JNIDeviceMessage) MistMessage.getInternal(this)).addDevice(Device.getInternal(device));
	}


	/**
	 * Returns the number of Device objects associated with this DeviceMessage.
	 *
	 * @return The number of device objects.
	 *
	 * @see DeviceMessage#getDevice(int)
	 */
	public int getNumDevices()
	{
		return ((JNIDeviceMessage) MistMessage.getInternal(this)).getNumDevices();
	}


	/**
	 * Returns Device object at specified index.
	 *
	 * @param index The index of the Device object to retrieve.
	 *
	 * @return A Device object.
	 *
	 * @throws GMSEC_Exception Thrown if the specified index is outside the number of devices in the message.
	 *
	 * @see DeviceMessage#getNumDevices()
	 *
	 * @deprecated This method has been deprecated; use DeviceIterator instead.
	 */
	@Deprecated
	public Device getDevice(int index) throws GMSEC_Exception
	{
		return ((JNIDeviceMessage) MistMessage.getInternal(this)).getDevice(index);
	}


	/**
	 * Method that allows the callee to get the DeviceIterator associated with the DeviceMessage.
	 * This iterator will allow for applications to iterate over the Device objects stored within the DeviceMessage.
	 * The iterator is reset each time getDeviceIterator() is called. The iterator itself is destroyed when the
	 * DeviceMessage object is destroyed.
	 * <p>
	 * Note: Only one DeviceIterator object is associated with a DeviceMessage object; multiple calls to
	 * getDeviceIterator() will return a reference to the same DeviceIterator object. Each call will reset the iterator.
	 *
	 * @return A reference to a DeviceIterator object.
	 */
	public DeviceIterator getDeviceIterator()
	{
		return ((JNIDeviceMessage) MistMessage.getInternal(this)).getDeviceIterator();
	}


	/**
	 * Constructs and returns a DeviceMessage that is created using the given Message.
	 *
	 * @param msg The message to convert.
	 *
	 * @return A DeviceMessage object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 */
	public static DeviceMessage convertMessage(Message msg) throws IllegalArgumentException
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		return JNIDeviceMessage.convertMessage(Message.getInternal(msg));
	}
}
