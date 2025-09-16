/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ResourceGenerator.java
 */

package gmsec;


/**
 * This class can be used to start a thread that will continuously publish resource messages.
 * <p>
 * The Resource Generator creates a RSRC message. It then uses a
 * Connection to publish this message on a periodic basis.
 *
 * @see Config
 */
public class ResourceGenerator
{
	/**
	 * Creates an instance of a ResourceGenerator object.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the resource generator will only publish one message.
	 *
	 * @param config          The Config object to configure the Connection that is used to publish messages
	 * @param pubRate         The message publish rate (in seconds)
	 * @param sampleInterval  The time distance between sample collection (in seconds)
	 * @param averageInterval Time window over which to make a moving average of samples (in seconds)
	 *
	 * @throws SWIG_Exception Thrown if the Config object is null 
	 * @throws GmsecException Thrown if the publish rate is less than 0 (zero)
	 * @throws GmsecException Thrown if the sample rate is less than 1 (one), or if averageInterval is less than sampleInterval
	 * @throws GmsecException Thrown if a Connection object cannot be created
	 */
	public ResourceGenerator(config, pubRate, sampleInterval, averageInterval);


	/**
	 * Creates an instance of a ResourceGenerator object.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the resource generator will only publish one message.
	 *
	 * @param config          The Config object to configure the Connection that is used to publish messages
	 * @param pubRate         The message publish rate (in seconds)
	 * @param sampleInterval  The time distance between sample collection (in seconds)
	 * @param averageInterval Time window over which to make a moving average of samples (in seconds)
	 * @param fields          A FieldArray of Field object(s) to be included within the resource message
	 *
	 * @throws SWIG_Exception Thrown if the Config object is null 
	 * @throws GmsecException Thrown if the publish rate is less than 0 (zero)
	 * @throws GmsecException Thrown if the sample rate is less than 1 (one), or if averageInterval is less than sampleInterval
	 * @throws GmsecException Thrown if a Connection object cannot be created
	 */
	public ResourceGenerator(config, pubRate, sampleInterval, averageInterval, fields);


	/**
	 * Starts the resource generator thread.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), only one resource message will be published; however
	 * the resource generator thread will continue to run.
	 *
	 * @return True if the resource generator thread has started; false otherwise.
	 *
	 * @throws GmsecException Thrown if a connection cannot be established with the GMSEC Bus
	 * @throws GmsecException Thrown if message validation is enabled and a valid RSRC message cannot be created
	 *
	 * @see stop
	 */ 
	public start();


	/**
	 * Stops the resource generator thread.
	 *
	 * @return True if the resource generator thread has stopped; false otherwise.
	 *
	 * @see start 
	 */ 
	public stop();


	/**
	 * Accessor that can be used to query whether the resource generator thread is running.
	 *
	 * @return True if the thread is running; false otherwise.
	 */
	public isRunning();


	/**
	 * Accessor that will add the given field to the resource message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the resource generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param field The Field object to add to the message
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws SWIG_Exception Thrown if the field object is null.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public setField(Field field);


	/**
	 * Creates/returns a Resource Message using the given MessageFactory.
	 *
	 * @param factory         The MessageFactory to reference when creating the resource message
	 * @param sampleInterval  The time distance between sample collection (in seconds)
	 * @param averageInterval Time window over which to make a moving average of samples (in seconds)
	 *
	 * @return A Message object representing a Resource Message.
	 *
	 * @throws SWIG_Exception Thrown if the MessageFactory object is null.
	 * @throws GmsecException Thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval.
	 *
	 * @see Connection.getMessageFactory
	 */
	public static createResourceMessage(factory, sampleInterval, averageInterval);
}
