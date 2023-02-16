/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ResourceGenerator.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.field.Field;

import gov.nasa.gsfc.gmsec.api5.jni.JNIResourceGenerator;

import java.util.Collection;


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
	private JNIResourceGenerator m_jniResourceGenerator = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param rsrcgen Object to reference for acquiring internal JNIResourceGenerator
	 * @return Internal JNIResourceGenerator object
	 */
	public static JNIResourceGenerator getInternal(ResourceGenerator rsrcgen)
	{
		return (rsrcgen == null ? null : rsrcgen.m_jniResourceGenerator);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jrsrcgen Internal JNIResourceGenerator object
	 */
	public ResourceGenerator(JNIResourceGenerator jrsrcgen)
	{
		m_jniResourceGenerator = jrsrcgen;
	}


	/**
	 * Do not allow instantiation of this class.
	 * Users must use ResourceGenerator.create().
	 */
	private ResourceGenerator()
	{
	}


	/**
	 * Creates an instance of a ResourceGenerator object.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the resource generator will only publish one message.
	 *
	 * @param config          The Config object to configure the Connection that is used to publish messages
	 * @param pubRate         The message publish rate (in seconds)
	 * @param sampleInterval  The time distance between sample collection (in seconds)
	 * @param averageInterval Time window over which to make a moving average of samples (in seconds)
	 * @param fields          A list of fields to be included within the resource message; value can be null.
	 *
	 * @return An instance of a ResourceGenerator object.
	 *
	 * @throws IllegalArgumentException Thrown if the Config object is null 
	 * @throws IllegalArgumentException Thrown if the publish rate is less than 0 (zero)
	 * @throws IllegalArgumentException Thrown if the sample rate is less than 1 (one), or if averageInterval is less than sampleInterval
	 * @throws GmsecException Thrown if a Connection object cannot be created
	 *
	 * @see ResourceGenerator#destroy(ResourceGenerator)
	 */
	public static ResourceGenerator create(Config config, int pubRate, int sampleInterval, int averageInterval, Collection<Field> fields)
		throws IllegalArgumentException, GmsecException
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}
		if (pubRate < 0)
		{
			throw new IllegalArgumentException("Publish rate cannot be less than 0 (zero)");
		}
		if (sampleInterval < 1)
		{
			throw new IllegalArgumentException("Sample interval cannot be less than 0 (zero)");
		}
		if (averageInterval < sampleInterval)
		{
			throw new IllegalArgumentException("Moving average interval is less than the sampling interval");
		}

		return JNIResourceGenerator.create(config, pubRate, sampleInterval, averageInterval, fields);
	}


	/**
	 * This static method is used to destroy the ResourceGenerator object.
	 *
	 * @param rsrcGen The ResourceGenerator object to destroy
	 *
	 * @throws IllegalArgumentException Thrown if the given ResourceGenerator object is null.
	 */
	public static void destroy(ResourceGenerator rsrcGen)
		throws IllegalArgumentException
	{
		if (rsrcGen == null)
		{
			throw new IllegalArgumentException("ResourceGenerator is null");
		}

		JNIResourceGenerator.destroy(rsrcGen);
	}


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
	public boolean start()
		throws GmsecException
	{
		return m_jniResourceGenerator.start();
	}


	/**
	 * Stops the resource generator thread.
	 *
	 * @return True if the resource generator thread has stopped; false otherwise.
	 *
	 * @see start 
	 */ 
	public boolean stop()
	{
		return m_jniResourceGenerator.stop();
	}


	/**
	 * Accessor that can be used to query whether the resource generator thread is running.
	 *
	 * @return True if the thread is running; false otherwise.
	 */
	public boolean isRunning()
	{
		return m_jniResourceGenerator.isRunning();
	}


	/**
	 * Accessor that will add the given field to the resource message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the resource generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param field The field to add to the message
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field object is null.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public boolean setField(Field field)
		throws IllegalArgumentException, GmsecException
	{
		if (field == null)
		{
			throw new IllegalArgumentException("Field object is null");
		}

		return m_jniResourceGenerator.setField(field);
	} 


	/**
	 * Creates/returns a Resource Message using the given MessageFactory.
	 *
	 * @param factory         The MessageFactory to reference when creating the resource message
	 * @param sampleInterval  The time distance between sample collection (in seconds)
	 * @param averageInterval Time window over which to make a moving average of samples (in seconds)
	 *
	 * @return A Message object representing a Resource Message.
	 *
	 * @throws IllegalArgumentException Thrown if the message factory object is null.
	 * @throws GmsecException Thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval.
	 *
	 * @see Connection#getMessageFactory
	 */
	public static Message createResourceMessage(MessageFactory factory, int sampleInterval, int averageInterval)
		throws IllegalArgumentException, GmsecException
	{
		if (factory == null)
		{
			throw new IllegalArgumentException("MessageFactory object is null");
		}

		return JNIResourceGenerator.createResourceMessage(factory, sampleInterval, averageInterval);
	}
}
