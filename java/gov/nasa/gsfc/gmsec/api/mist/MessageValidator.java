/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MessageValidator.java
 *
 *  @brief This file contains the abstract base class for received message callbacks
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Message;
import gov.nasa.gsfc.gmsec.api.Status;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMessageValidator;


/**
 * This abstract class is the base class for performing custom message validation.
 * A user created class, derived from this class, can be passed into
 * {@link ConnectionManager#registerMessageValidator(MessageValidator)} to have user code
 * executed when a message is being validated.
 * <p>
 * This custom message validator will be used to supplement the already built-in message validator
 * offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
 * call on the custom message validator, and if the returned status is nominal, then the API's
 * message validator will be called.
 * <p>
 * Note that because users are able to create their own MessageValidator class,
 * reentrancy is not guaranteed unless if reentrancy rules are specified.
 * <p>
 * In addition, if a MessageValidator is registered to multiple ConnectionManager objects,
 * validateMessage() can be invoked concurrently from different threads. Use of a gmsec::util::AutoMutex
 * is suggested to enforce thread safety.
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
       ConnectionManager connMgr = new ConnectionManager(config);
       connMgr.registerMessageValidator(validator);
       ...
       connMgr.publish(msg);
   }
   catch (GMSEC_Exception e) {
       // handle error
   }
   }</pre>
 *
 * @see ConnectionManager#registerMessageValidator(MessageValidator)
 * @see Specification#registerMessageValidator(MessageValidator)
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
	 * <p>
	 * @param msg The message to validate.
	 * @return The Status of the operation.
	 *
	 * @see ConnectionManager#registerMessageValidator(MessageValidator)
	 * @see Specification#registerMessageValidator(MessageValidator)
	 */
	public abstract Status validateMessage(Message msg);
}
