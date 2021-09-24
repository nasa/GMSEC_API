/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicMessage.java
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
 * The Message object contains a mnemonic value message with one of 3 schema definitions:
 * MSG.MVAL, REQ.MVAL, RESP.MVAL
 *
 * @see Message
 * @see MistMessage
 * @see Config
 * @see Specification 
 * @see gov.nasa.gsfc.gmsec.api.field.Field
 * @see gov.nasa.gsfc.gmsec.api.MessageFieldIterator
 * @see MnemonicIterator
 */
public class MnemonicMessage extends MistMessage
{
	public MnemonicMessage(JNIMnemonicMessage jMnemMsg)
	{
		super(jMnemMsg);
	}


	/**
	 * Default constructor - Initializes the message instance
	 *
	 * @param subject The subject string for the message.
	 * @param schemaID the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevelName.messageKind.messageType (e.g. 2019.00.C2MS.MSG.MVAL). You may also
	 * use the shorthand notation of messageKind.messageType (e.g. MSG.MVAL).
	 * @param spec    A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the schema ID string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
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
     * Constructor - Initializes the message instance and associates a Configuration object
     *
     * @param subject The subject string for the message.
	 * @param schemaID the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevelName.messageKind.messageType (e.g. 2019.00.C2MS.MSG.MVAL). You may also
	 * use the shorthand notation of messageKind.messageType (e.g. MSG.MVAL).
     * @param config  A configuration to associate with the message.
     * @param spec    A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the schema ID string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Config object is null.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
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
	 * Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data The XML or JSON string that represents a GMSEC message.
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the data string cannot be parsed.
	 *
	 * @deprecated This constructor has been deprecated; use MnemonicMessage(Specification, String) instead.
	 */
	@Deprecated
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
	 * Initializes a MnemonicMessage using an XML or JSON string, and ensures the message adheres
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
	public MnemonicMessage(Specification spec, String data)
		throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMnemonicMessage) null);

		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIMnemonicMessage(Specification.getInternal(spec), data));
	}


	/**
	 * Copy constructor that initializes the message instance using another MnemonicMessage object.
	 *
	 * @param other The other MnemonicMessage object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the mnemonic message object is null.
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

		((JNIMnemonicMessage) MistMessage.getInternal(this)).addMnemonic(Mnemonic.getInternal(mnemonic));
	}


	/**
	 * Returns the number of Mnemonic objects associated with this MnemonicMessage.
	 *
	 * @return The number of mnemonic objects.
	 *
	 * @see MnemonicMessage#getMnemonic(int)
	 */
	public int getNumMnemonics()
	{
		return ((JNIMnemonicMessage) MistMessage.getInternal(this)).getNumMnemonics();
	}


	/**
	 * Returns Mnemonic object at specified index.
	 *
	 * @param index The index of the Mnemonic object to retrieve.
	 *
	 * @return A Mnemonic object.
	 *
	 * @throws GMSEC_Exception Thrown if the specified index is outside the number of mnemonics in the message.
	 *
	 * @see MnemonicMessage#getNumMnemonics()
	 *
	 * @deprecated This method has been deprecated; use MnemonicIterator instead.
	 */
	@Deprecated
	public Mnemonic getMnemonic(int index) throws GMSEC_Exception
	{
		return ((JNIMnemonicMessage) MistMessage.getInternal(this)).getMnemonic(index);
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
		return ((JNIMnemonicMessage) MistMessage.getInternal(this)).getMnemonicIterator();
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

		return JNIMnemonicMessage.convertMessage(Message.getInternal(msg));
	}
}
