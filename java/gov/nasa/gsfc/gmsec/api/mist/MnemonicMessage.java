/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicMessage.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.util.Log;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMistMessage;
import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMnemonicMessage;


/**
 * The MnemonicMessage object is an extension of Message, and serves as a container for a GMSEC MSG MVAL Message.
 * The methods of this class allow for the construction and manipulation of the data within the message.
 *
 * @deprecated This class has been deprecated; use {@link gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage} instead.
 */
@Deprecated
public class MnemonicMessage extends Message
{
	/**
	 * Specialized constructor where a GMSEC Interface Specification Document version number can be specified.
	 *
	 * @param subject The GMSEC subject to associate with the message.
	 * @param version The version of the GMSEC ISD to be used.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty string.
	 */
	public MnemonicMessage(String subject, int version) throws IllegalArgumentException
	{
		super((JNIMnemonicMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage instead.");

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}

		registerChild(new JNIMnemonicMessage(subject,
		                                     JNIMistMessage.buildSchemaID(Message.MessageKind.PUBLISH, "MVAL", null, null, version),
		                                     JNIMistMessage.buildSpecification(version)));
	}


	/**
	 * Specialized constructor that accepts a Config object, and where a GMSEC Interface Specification Document
	 * version number can be speficied.
	 * <p>
	 * Note: This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage instead.
	 *
	 * @param subject The GMSEC subject to associate with the message.
	 * @param config  A configuration object that contains message-specific options.
	 * @param version The version of the GMSEC ISD to be used.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null, or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the config object is null.
	 */
	public MnemonicMessage(String subject, Config config, int version) throws IllegalArgumentException
	{
		super((JNIMnemonicMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage instead.");

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}

		registerChild(new JNIMnemonicMessage(subject,
		                                     JNIMistMessage.buildSchemaID(Message.MessageKind.PUBLISH, "MVAL", null, null, version),
		                                     config,
		                                     JNIMistMessage.buildSpecification(version)));
	}


	/**
	 * Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data The XML or JSON string that represents a GMSEC message.
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the data string cannot be parsed.
	 */
	public MnemonicMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMnemonicMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage instead.");

		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIMnemonicMessage(data));
	}


	/**
	 * Copy constructor that initializes the message instance using another MnemonicMessage object.
	 *
	 * @param other The other MnemonicMessage object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given MnemonicMessage object is null.
	 */
	public MnemonicMessage(MnemonicMessage other) throws IllegalArgumentException
	{
		super((JNIMnemonicMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage instead.");

		if (other == null)
		{
			throw new IllegalArgumentException("MnemonicMessage cannot be null");
		}

		registerChild(new JNIMnemonicMessage((JNIMnemonicMessage) getInternal(other)));
	}


	/**
	 * Adds the given Mnemonic object to the internal list of devices maintained by the message.
	 *
	 * @param mnemonic The Mnemonic object to add to the message.
	 *
	 * @throws IllegalArgumentException Thrown if the given Mnemonic object is null.
	 */
	public void addMnemonic(Mnemonic mnemonic) throws IllegalArgumentException
	{
		if (mnemonic == null)
		{
			throw new IllegalArgumentException("Mnemonic is null");
		}

		((JNIMnemonicMessage) getInternal(this)).addMnemonic(Mnemonic.getInternal(mnemonic));
	}


	/**
	 * Returns the number of Mnemonic objects associated with this MnemonicMessage.
	 *
	 * @return The number of device objects.
	 *
	 * @see MnemonicMessage#getMnemonic(int)
	 */
	public int getNumMnemonics()
	{
		return ((JNIMnemonicMessage) getInternal(this)).getNumMnemonics();
	}


	/**
	 * Returns Mnemonic object at specified index.
	 *
	 * @param index The zero-based index of the Mnemonic object to retrieve.
	 *
	 * @return A Mnemonic object.
	 *
	 * @throws GMSEC_Exception Thrown if the specified index is outside the number of devices in the message.
	 *
	 * @see MnemonicMessage#getNumMnemonics()
	 */
	public Mnemonic getMnemonic(int index) throws GMSEC_Exception
	{
		return ((JNIMnemonicMessage) getInternal(this)).getMnemonic(index);
	}


	/**
	 * Method that allows the callee to get the MnemonicIterator associated with the MnemonicMessage.
	 * This iterator will allow for applications to iterate over the Mnemonic objects stored within the MnemonicMessage.
	 * The iterator is reset each time getMnemonicIterator() is called. The iterator itself is destroyed when the
	 * MnemonicMessage object is destroyed.
	 * <p>
	 * Note: Only one MnemonicIterator object is associated with a MnemonicMessage object; multiple calls to
	 * getMnemonicIterator() will return a reference to the same MnemonicIterator object. Each call will reset the iterator.
	 *
	 * @return A reference to a MnemonicIterator object.
	 */
	public MnemonicIterator getMnemonicIterator()
	{
		return ((JNIMnemonicMessage) getInternal(this)).getMnemonicIterator();
	}


	/**
	 * Constructs and returns a MnemonicMessage that is created using the given Message.
	 *
	 * @param msg The message to convert.
	 *
	 * @return A MnemonicMessage object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
	 */
	public static MnemonicMessage convertMessage(Message msg) throws IllegalArgumentException
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		MnemonicMessage mnemMsg = null;

		try
		{
			mnemMsg = new MnemonicMessage(msg.toXML());
		}
		catch (GMSEC_Exception e)
		{
				throw new IllegalArgumentException(e.toString());
		}

		return mnemMsg;
	}
}
