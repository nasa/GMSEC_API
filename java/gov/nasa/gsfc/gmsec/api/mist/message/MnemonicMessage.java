/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicMessage.java
 *
 * @brief The MnemonicMessage object is an extension of MistMessage, and serves as a container
 * for mnemonic messages.
 */

package gov.nasa.gsfc.gmsec.api.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.mist.Mnemonic;
import gov.nasa.gsfc.gmsec.api.mist.MnemonicIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonic;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMnemonicMessage;


/**
 * @class MnemonicMessage
 *
 * @brief The Message object contains a mnemonic value message with one of 3 schema definitions:
 * GMSEC.MSG.MVAL, GMSEC.REQ.MVAL, GMSEC.RESP.MVAL
 *
 * @sa Message
 * @sa MistMessage
 * @sa Config
 * @sa Specification 
 * @sa Field
 * @sa MessageFieldIterator
 * @sa MnemonicIterator
 */
public class MnemonicMessage extends MistMessage
{
	public MnemonicMessage(JNIMnemonicMessage jMnemMsg)
	{
		super(jMnemMsg);
	}


	/**
	 * @fn MnemonicMessage(String subject, String schemaID, Specification spec)
	 *
	 * @brief Default constructor - Initializes the message instance
	 *
	 * @param subject - The subject string for the message.
	 * @param spec    - A reference to the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the schema ID string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the %Specification object is null.
	 */
	public MnemonicMessage(String subject, String schemaID, Specification spec) throws IllegalArgumentException
	{
		super((JNIMnemonicMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("Schema ID cannot be null, nor contain an empty string");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIMnemonicMessage(subject, schemaID, spec));
	}


   /**
     * @fn MnemonicMessage(String subject, String schemaID, Config config, Specification spec)
     *
     * @brief constructor - Initializes the message instance and associates a Configuration object
     *
     * @param subject - The subject string for the message.
     * @param config  - A configuration to associate with the message.
     * @param spec    - A reference to the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the schema ID string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the %Config object is null.
	 * @throws An IllegalArgumentException is thrown if the %Specification object is null.
	 */
	public MnemonicMessage(String subject, String schemaID, Config config, Specification spec) throws IllegalArgumentException
	{
		super((JNIMnemonicMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("Schema ID cannot be null, nor contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIMnemonicMessage(subject, schemaID, config, spec));
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
		super((JNIMnemonicMessage) null);

		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIMnemonicMessage(data));
	}


	/**
	 * @fn MnemonicMessage(MnemonicMessage other)
	 *
	 * @brief Copy constructor that initializes the message instance using another MnemonicMessage object.
	 *
	 * @param other - the other MnemonicMessage object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the mnemonic message object is null.
	 */
	public MnemonicMessage(MnemonicMessage other) throws IllegalArgumentException
	{
		super((JNIMnemonicMessage) null);

		if (other == null)
		{
			throw new IllegalArgumentException("MnemonicMessage cannot be null");
		}

		registerChild(new JNIMnemonicMessage((JNIMnemonicMessage) MistMessage.getInternal(other)));
	}


	/**
	 * @fn void addMnemonic(Mnemonic mnemonic)
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

		((JNIMnemonicMessage) MistMessage.getInternal(this)).addMnemonic(Mnemonic.getInternal(mnemonic));
	}


	/**
	 * @fn int getNumMnemonics()
	 *
	 * @brief Returns the number of Mnemonic objects associated with this MnemonicMessage.
	 *
	 * @return The number of mnemonic objects.
	 *
	 * @sa getMnemonic()
	 */
	public int getNumMnemonics()
	{
		return ((JNIMnemonicMessage) MistMessage.getInternal(this)).getNumMnemonics();
	}


	/**
	 * @fn Mnemonic getMnemonic(int index)
	 *
	 * @brief Returns Mnemonic object at specified index.
	 *
	 * @return A Mnemonic object.
	 *
	 * @throws A GMSEC_Exception is thrown if the specified index is outside the number of mnemonics in the message.
	 *
	 * @sa getNumMnemonics()
	 */
	public Mnemonic getMnemonic(int index) throws GMSEC_Exception
	{
		return ((JNIMnemonicMessage) MistMessage.getInternal(this)).getMnemonic(index);
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
		return ((JNIMnemonicMessage) MistMessage.getInternal(this)).getMnemonicIterator();
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
