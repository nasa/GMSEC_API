/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicMessage.java
 *
 * @brief The MnemonicMessage object is an extension of Message, and serves as
 * a container for a GMSEC MSG MVAL %Message.
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.jni.JNIMnemonicMessage;


/**
 * @class MnemonicMessage
 *
 * @brief The MnemonicMessage object is an extension of Message, and serves as a container for a GMSEC MSG MVAL %Message.
 * The methods of this class allow for the construction and manipulation of the data within the message.
 */
public class MnemonicMessage extends Message
{
	private JNIMnemonicMessage m_jniMnemonicMessage = null;


	public static JNIMnemonicMessage getInternal(MnemonicMessage msg)
	{
		return (msg == null ? null : msg.m_jniMnemonicMessage);
	}


	public MnemonicMessage(JNIMnemonicMessage jMnemMsg)
	{
		super(jMnemMsg);

		m_jniMnemonicMessage = jMnemMsg;
	}


	/**
	 * @fn MnemonicMessage(String subject, int version)
	 *
	 * @brief Specialized constructor where a GMSEC Interface Specification Document version number can be specified.
	 *
	 * @param subject - the GMSEC subject to associate with the message.
	 * @param version - the version of the GMSEC ISD to be used.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty string.
	 */
	public MnemonicMessage(String subject, int version) throws IllegalArgumentException
	{
		super(subject, Message.MessageKind.PUBLISH);

		m_jniMnemonicMessage = new JNIMnemonicMessage(subject, version);

		registerChild(m_jniMnemonicMessage);
	}


	/**
	 * @fn MnemonicMessage(String subject, Config config, int version)
	 *
	 * @brief Specialized constructor that accepts a Config object, and where a GMSEC Interface Specification Document
	 * version number can be speficied.
	 *
	 * @param subject - the GMSEC subject to associate with the message.
	 * @param config  - a configuration object that contains message-specific options.
	 * @param version - the version of the GMSEC ISD to be used.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the config object is null.
	 */
	public MnemonicMessage(String subject, Config config, int version) throws IllegalArgumentException
	{
		super(subject, Message.MessageKind.PUBLISH);

		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}

		m_jniMnemonicMessage = new JNIMnemonicMessage(subject, Config.getInternal(config), version);

		registerChild(m_jniMnemonicMessage);
	}


	/**
	 * @fn MnemonicMessage(String data)
	 *
	 * @brief Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data - the XML or JSON string that represents a GMSEC message.
	 *
	 * @throws An IllegalArgumentException is thrown if the data string is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the data string cannot be parsed.
	 */
	public MnemonicMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super(data);

		m_jniMnemonicMessage = new JNIMnemonicMessage(data);

		registerChild(m_jniMnemonicMessage);
	}


	/**
	 * @fn MnemonicMessage(MnemonicMessage other)
	 *
	 * @brief Copy constructor that initializes the message instance using another MnemonicMessage object.
	 *
	 * @param other - the other MnemonicMessage object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given MnemonicMessage object is null.
	 */
	public MnemonicMessage(MnemonicMessage other) throws IllegalArgumentException
	{
		super(other);

		m_jniMnemonicMessage = new JNIMnemonicMessage(MnemonicMessage.getInternal(other));

		registerChild(m_jniMnemonicMessage);
	}


	/**
	 * @fn void addMnemonic(Mnemonic mnemonic)
	 *
	 * @brief Adds the given Mnemonic object to the internal list of devices maintained by the message.
	 *
	 * @param mnemonic - the Mnemonic object to add to the message.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Mnemonic object is null.
	 */
	public void addMnemonic(Mnemonic mnemonic) throws IllegalArgumentException
	{
		if (mnemonic == null)
		{
			throw new IllegalArgumentException("Mnemonic is null");
		}

		m_jniMnemonicMessage.addMnemonic(Mnemonic.getInternal(mnemonic));
	}


	/**
	 * @fn int getNumMnemonics()
	 *
	 * @brief Returns the number of Mnemonic objects associated with this MnemonicMessage.
	 *
	 * @return The number of device objects.
	 *
	 * @sa getMnemonic()
	 */
	public int getNumMnemonics()
	{
		return m_jniMnemonicMessage.getNumMnemonics();
	}


	/**
	 * @fn Mnemonic getMnemonic(int index)
	 *
	 * @brief Returns Mnemonic object at specified index.
	 *
	 * @return A Mnemonic object.
	 *
	 * @throws A GMSEC_Exception is thrown if the specified index is outside the number of devices in the message.
	 *
	 * @sa getNumMnemonics()
	 */
	public Mnemonic getMnemonic(int index) throws GMSEC_Exception
	{
		return m_jniMnemonicMessage.getMnemonic(index);
	}


	/**
	 * @fn MnemonicIterator getMnemonicIterator()
	 *
	 * @brief Method that allows the callee to get the MnemonicIterator associated with the %MnemonicMessage.
	 * This iterator will allow for applications to iterate over the Mnemonic objects stored within the %MnemonicMessage.
	 * The iterator is reset each time getMnemonicIterator() is called.  The iterator itself is destroyed when the
	 * %MnemonicMessage object is destroyed.
	 *
	 * @note Only one MnemonicIterator object is associated with a %MnemonicMessage object; multiple calls to
	 * getMnemonicIterator() will return a reference to the same MnemonicIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a MnemonicIterator object.
	 */
	public MnemonicIterator getMnemonicIterator()
	{
		return m_jniMnemonicMessage.getMnemonicIterator();
	}


	/**
	 * @fn MnemonicMessage convertMessage(Message msg)
	 *
	 * @brief Constructs and returns a MnemonicMessage that is created using the given Message.
	 *
	 * @return A MnemonicMessage object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 */
	public static MnemonicMessage convertMessage(Message msg) throws IllegalArgumentException
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		return JNIMnemonicMessage.convertMessage(Message.getInternal(msg));
	}
}
