/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file HeartbeatGenerator.java
 */

package gmsec;


/**
 * This class can be used to start a thread that will continuously publish HB messages.
 * <p>
 * The Heartbeat Generator creates a HB message. It then uses a
 * Connection to publish this message on a periodic basis.
 *
 * @see Config
 */
public class HeartbeatGenerator
{
	/**
	 * Creates an instance of a HeartbeatGenerator object.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 *
	 * @param config    The Config object to configure the Connection that is used to publish messages
	 * @param hbPubRate The message publish rate (in seconds)
	 *
	 * @return A instance of a HeartbeatGenerator object.
	 *
	 * @throws SWIG_Exception Thrown if the Config object is null 
	 * @throws GmsecException Thrown if the publish rate is less than 0 (zero)
	 * @throws GmsecException Thrown if a Connection object cannot be created
	 */
	public HeartbeatGenerator(config, hbPubRate);


	/**
	 * Creates an instance of a HeartbeatGenerator object.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 *
	 * @param config    The Config object to configure the Connection that is used to publish messages
	 * @param hbPubRate The message publish rate (in seconds)
	 * @param fields    A FieldArray of fields to be included within the heartbeat message
	 *
	 * @return A instance of a HeartbeatGenerator object.
	 *
	 * @throws SWIG_Exception Thrown if the Config object is null 
	 * @throws SWIG_Exception Thrown if the FieldArray object is null 
	 * @throws GmsecException Thrown if the publish rate is less than 0 (zero)
	 * @throws GmsecException Thrown if a Connection object cannot be created
	 */
	public HeartbeatGenerator(config, hbPubRate, fields);


	/**
	 * Starts the heartbeat generator thread.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), only one heartbeat message will be published; however
	 * the heartbeat generator thread will continue to run.
	 *
	 * @return True if the heartbeat generator thread has started; false otherwise.
	 *
	 * @throws GmsecException Thrown if a connection cannot be established with the GMSEC Bus
	 * @throws GmsecException Thrown if message validation is enabled and a valid HB message cannot be created
	 *
	 * @see stop
	 */ 
	public start();


	/**
	 * Stops the heartbeat generator thread.
	 *
	 * @return True if the heartbeat generator thread has stopped; false otherwise.
	 *
	 * @see start 
	 */ 
	public stop();


	/**
	 * Accessor that can be used to query whether the heartbeat generator thread is running.
	 *
	 * @return True if the thread is running; false otherwise.
	 */
	public isRunning();


	/**
	 * Accessor for changing the publish rate of the heartbeat message generator.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), then only one additional heartbeat message will be published.
	 *
	 * @param pubRate the new publish rate for the heartbeat message generator
	 *
	 * @throws GmsecException Thrown if the given publish rate is less than 0 (zero).
	 */
	public changePublishRate(pubRate);


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param field The Field object to add to the message
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field object is null.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public setField(field);


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param fieldName      The name of the field
	 * @param longFieldValue The long integer value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public setField(fieldName, longFieldValue);


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param fieldName        The name of the field
	 * @param doubleFieldValue The double value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public setField(fieldName, doubleFieldValue);


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param fieldName     The name of the field
	 * @param strFieldValue The string value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field name is null.
	 * @throws SWIG_Exception Thrown if the field value is null.
	 * @throws GmsecException Thrown if the field name contains an empty string.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public setField(fieldName, strFieldValue);
}
