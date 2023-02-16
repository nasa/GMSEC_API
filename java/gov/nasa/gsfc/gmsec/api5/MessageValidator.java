/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MessageValidator.java
 *
 *  @brief This file contains the abstract base class for received message callbacks
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.jni.JNIMessageValidator;


/**
 * This abstract class is the base class for performing custom message validation.
 * A user created class, derived from this class, can be passed into
 * {@link MessageFactory#registerMessageValidator(MessageValidator)} to have user code
 * executed when a Message is being validated.
 * <p>
 * This custom message validator will be used to supplement the already built-in message validator
 * offered by the Specification object. The Specification object will first
 * call on the custom message validator, and if the returned status is nominal, then the API's
 * message validator will be called.
 * <p>
 * Note that because users are able to create their own MessageValidator class,
 * reentrancy is not guaranteed unless if reentrancy rules are specified.
 * <p>
 * Example MessageValidator class:
   <pre>{@code
   class CustomMessageValidator extends MessageValidator
   {
       public Status validateMessage(Message msg)
       {
           Status status = new Status();

           // Validate message

           return status;
       }
   }
   }</pre>
 * <p>
 * Example callback registration:
   <pre>{@code
   CustomMessageValidator validator = new CustomMessageValidator();
   try {
       Config config = new Config(args);
       config.addValue("gmsec-msg-content-validate-all", "true");
       MessageFactory factory = new MessageFactory(config);
       factory.registerMessageValidator(validator);
       ...
       Message msg = factory.createMessage("HB");
       ...
       connMgr.publish(msg);
   }
   catch (GmsecException e) {
       // handle error
   }
   }</pre>
 *
 * @see MessageFactory#registerMessageValidator
 * @see Message#registerMessageValidator
 */


public abstract class MessageValidator
{
	private JNIMessageValidator m_jniMessageValidator = null;

	/**
	 * This method is for internal GMSEC API use only.
	 * @param validator MessageValidator object to reference for acquiring internal JNIMessageValidator.
	 * @return Internal JNIMessageValidator object.
	 */
	public static JNIMessageValidator getInternal(MessageValidator validator)
	{
		return (validator == null ? null : validator.m_jniMessageValidator);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 */
	protected MessageValidator()
	{
		m_jniMessageValidator = new JNIMessageValidator(this);
	}


	/**
	 * This method is called by the API to perform a user-defined message validation.
	 * <p>
	 * If a MessageValidator is registered to multiple connections, validateMessage() can be
	 * invoked concurrently from different connection threads.
	 * <p>
	 * This custom message validator will be used to supplement the already built-in message validator
	 * offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
	 * call on the custom message validator, and if the returned status is nominal, then the API's
	 * message validator will be called.
	 * <p>
	 * <b>DO NOT STORE</b> the Message object for use beyond the scope of the method.
	 * @param msg The message to validate.
	 * @return The Status of the operation.
	 *
	 * @see MessageFactory#registerMessageValidator
	 * @see Message#registerMessageValidator
	 */
	public abstract Status validateMessage(Message msg);
}
