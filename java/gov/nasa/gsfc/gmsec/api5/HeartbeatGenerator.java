/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file HeartbeatGenerator.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.field.Field;

import gov.nasa.gsfc.gmsec.api5.jni.JNIHeartbeatGenerator;

import java.util.Collection;


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
	private JNIHeartbeatGenerator m_jniHeartbeatGenerator = null;


	//! @cond
	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param hbgen Object to reference for acquiring internal JNIHeartbeatGenerator
	 * @return Internal JNIHeartbeatGenerator object
	 */
	public static JNIHeartbeatGenerator getInternal(HeartbeatGenerator hbgen)
	{
		return (hbgen == null ? null : hbgen.m_jniHeartbeatGenerator);
	}


	/**
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param jhbgen Internal JNIHeartbeatGenerator object
	 */
	public HeartbeatGenerator(JNIHeartbeatGenerator jhbgen)
	{
		m_jniHeartbeatGenerator = jhbgen;
	}
	//! @endcond


	/**
	 * Do not allow instantiation of this class.
	 * Users must use HeartbeatGenerator.create().
	 */
	private HeartbeatGenerator()
	{
	}


	/**
	 * Creates an instance of a HeartbeatGeneraror object.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 *
	 * @param config the Config object to configure the Connection that is used to publish messages.
	 * @param hbPubRate the message publish rate (in seconds).
	 * @param fields a list of fields to be included within the heartbeat message; value can be null.
	 *
	 * @return A instance of a HeartbeatGenerator object.
	 *
	 * @throws IllegalArgumentException Thrown if the Config object is null.
	 * @throws IllegalArgumentException Thrown if the publish rate is less than 0 (zero).
	 * @throws GmsecException Thrown if a Connection object cannot be created.
	 *
	 * @see HeartbeatGenerator#destroy(HeartbeatGenerator)
	 */
	public static HeartbeatGenerator create(Config config, int hbPubRate, Collection<Field> fields)
		throws IllegalArgumentException, GmsecException
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}
		if (hbPubRate < 0)
		{
			throw new IllegalArgumentException("Publish rate cannot be less than 0 (zero)");
		}

		return JNIHeartbeatGenerator.create(config, hbPubRate, fields);
	}


	/**
	 * This static method is used to destroy the HeartbeatGenerator object.
	 *
	 * @param hbGen The HeartbeatGenerator object to destroy.
	 *
	 * @throws IllegalArgumentException Thrown if the given HeartbeatGenerator object is null.
	 */
	public static void destroy(HeartbeatGenerator hbGen)
		throws IllegalArgumentException
	{
		if (hbGen == null)
		{
			throw new IllegalArgumentException("HeartbeatGenerator is null");
		}

		JNIHeartbeatGenerator.destroy(hbGen);
	}


	/**
	 * Starts the heartbeat generator thread.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), only one heartbeat message will be published; however
	 * the heartbeat generator thread will continue to run.
	 *
	 * @return True if the heartbeat generator thread has started; false otherwise.
	 *
	 * @throws GmsecException Thrown if a connection cannot be established with the GMSEC Bus.
	 * @throws GmsecException Thrown if message validation is enabled and a valid HB message cannot be created.
	 *
	 * @see stop
	 */ 
	public boolean start()
		throws GmsecException
	{
		return m_jniHeartbeatGenerator.start();
	}


	/**
	 * Stops the heartbeat generator thread.
	 *
	 * @return True if the heartbeat generator thread has stopped; false otherwise.
	 *
	 * @see start 
	 */ 
	public boolean stop()
	{
		return m_jniHeartbeatGenerator.stop();
	}


	/**
	 * Accessor that can be used to query whether the heartbeat generator thread is running.
	 *
	 * @return True if the thread is running; false otherwise.
	 */
	public boolean isRunning()
	{
		return m_jniHeartbeatGenerator.isRunning();
	}


	/**
	 * Accessor for changing the publish rate of the heartbeat message generator.
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), then only one additional heartbeat message will be published.
	 *
	 * @param pubRate the new publish rate for the heartbeat message generator.
	 *
	 * @throws IllegalArgumentException Thrown if the given publish rate is less than 0 (zero).
	 */
	public void changePublishRate(int pubRate)
		throws IllegalArgumentException
	{
		if (pubRate < 0)
		{
			throw new IllegalArgumentException("Publish rate cannot be less than 0 (zero)");
		}

		m_jniHeartbeatGenerator.changePublishRate(pubRate);
	}


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param field The field to add to the message.
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

		return m_jniHeartbeatGenerator.setField(field);
	} 


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param fieldName The name of the field.
	 * @param fieldValue The value of the field.
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public boolean setField(String fieldName, long fieldValue)
		throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniHeartbeatGenerator.setField(fieldName, fieldValue);
	}


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param fieldName The name of the field.
	 * @param fieldValue The value of the field.
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public boolean setField(String fieldName, double fieldValue)
		throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniHeartbeatGenerator.setField(fieldName, fieldValue);
	}


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 *
	 * @param fieldName The name of the field.
	 * @param fieldValue The value of the field.
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the field value is null.
	 * @throws GmsecException Thrown if for whatever reason the field cannot be added to the message.
	 */
	public boolean setField(String fieldName, String fieldValue)
		throws IllegalArgumentException, GmsecException
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}
		if (fieldValue == null)
		{
			throw new IllegalArgumentException("Field value is null");
		}

		return m_jniHeartbeatGenerator.setField(fieldName, fieldValue);
	}
}
