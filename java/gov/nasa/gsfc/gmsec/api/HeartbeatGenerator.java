/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file HeartbeatGenerator.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.jni.JNIHeartbeatGenerator;

import java.util.Collection;


/**
 * This class can be used to start a thread that will continuously publish C2CX HB messages.
 * <p>
 * The Heartbeat Generator creates a C2CX HB message using MistMessage. It then uses a
 * ConnectionManager to publish this message on a periodic basis.
 *
 * @see Config
 * @see gov.nasa.gsfc.gmsec.api.mist.ConnectionManager
 * @see gov.nasa.gsfc.gmsec.api.mist.message.MistMessage
 */
public class HeartbeatGenerator
{
	private JNIHeartbeatGenerator m_jniHeartbeatGenerator = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param hbgen Object to reference for acquiring internal JNIHeartbeatGenerator
	 * @return Internal JNIHeartbeatGenerator object
	 */
	public static JNIHeartbeatGenerator getInternal(HeartbeatGenerator hbgen)
	{
		return (hbgen == null ? null : hbgen.m_jniHeartbeatGenerator);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jhbgen Internal JNIHeartbeatGenerator object
	 */
	public HeartbeatGenerator(JNIHeartbeatGenerator jhbgen)
	{
		m_jniHeartbeatGenerator = jhbgen;
	}


	/**
	 * Basic class constructor.
	 *
	 * @param config the Config object to configure the ConnectionManager that is used to publish messages
	 * @param hbMsgSubject the subject to include with the published heartbeat messages
	 * @param hbPubRate the message publish rate (in seconds)
	 *
	 * @throws IllegalArgumentException Thrown if the Config object is null 
	 * @throws IllegalArgumentException Thrown if the message subject is null or contains an empty string
	 * @throws IllegalArgumentException Thrown if the publish rate is less than 0 (zero)
	 * @throws GMSEC_Exception Thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus
	 * @throws GMSEC_Exception Thrown if message validation is enabled and a valid C2CX HB message cannot be created
	 *
	 * @see setField
	 * @see gov.nasa.gsfc.gmsec.api.mist.message.MistMessage#setStandardFields
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 * </p>
	 */
	public HeartbeatGenerator(Config config, String hbMsgSubject, int hbPubRate)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}
		if (hbMsgSubject == null || hbMsgSubject.isEmpty())
		{
			throw new IllegalArgumentException("Heartbeat message subject is null or contains an empty string");
		}
		if (hbPubRate < 0)
		{
			throw new IllegalArgumentException("Publish rate cannot be less than 0 (zero)");
		}

		m_jniHeartbeatGenerator = new JNIHeartbeatGenerator(config, hbMsgSubject, hbPubRate);
	}


	/**
	 * Class constructor that accepts collection of standard fields.
	 *
	 * @param config the Config object to configure the ConnectionManager that is used to publish messages
	 * @param hbMsgSubject the subject to include with the published heartbeat messages
	 * @param hbPubRate the message publish rate (in seconds)
	 * @param fields a list of fields to be included within the heartbeat message
	 *
	 * @throws IllegalArgumentException Thrown if the Config object is null 
	 * @throws IllegalArgumentException Thrown if the message subject is null or contains an empty string
	 * @throws IllegalArgumentException Thrown if the publish rate is less than 0 (zero)
	 * @throws IllegalArgumentException Thrown if the Collection of fields is null
	 * @throws GMSEC_Exception Thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus
	 * @throws GMSEC_Exception Thrown if message validation is enabled and a valid C2CX HB message cannot be created
	 *
	 * @see setField
	 * @see gov.nasa.gsfc.gmsec.api.mist.message.MistMessage#setStandardFields
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 * </p>
	 */
	public HeartbeatGenerator(Config config, String hbMsgSubject, int hbPubRate, Collection<Field> fields)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (config == null)
		{
			throw new IllegalArgumentException("Config object is null");
		}
		if (hbMsgSubject == null || hbMsgSubject.isEmpty())
		{
			throw new IllegalArgumentException("Heartbeat message subject is null or contains an empty string");
		}
		if (hbPubRate < 0)
		{
			throw new IllegalArgumentException("Publish rate cannot be less than 0 (zero)");
		}
		if (fields == null)
		{
			throw new IllegalArgumentException("Collection object of fields is null");
		}

		m_jniHeartbeatGenerator = new JNIHeartbeatGenerator(config, hbMsgSubject, hbPubRate, fields);
	}


	/**
	 * Starts the heartbeat generator thread.
	 *
	 * @return True if the heartbeat generator thread has started; false otherwise.
	 *
	 * @see stop
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), only one heartbeat message will be published; however
	 * the heartbeat generator thread will continue to run.
	 * </p>
	 */ 
	public boolean start()
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
	 * Destroys the heartbeat generator.
	 *
	 * The calling of this method is typically not needed, however when operating
	 * with certain middleware (e.g. generic_jms), it may be necessary to call.
	 */
	public void destroy()
	{
		m_jniHeartbeatGenerator.destroy();
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
	 *
	 * @param pubRate the new publish rate for the heartbeat message generator
	 *
	 * @throws IllegalArgumentException Thrown if the given publish rate is less than 0 (zero).
	 * <p>
	 * Note: If the publish rate is set to 0 (zero), then only one additional heartbeat message will be published.
	 * </p>
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
	 *
	 * @param field The field to add to the message
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field object is null.
	 * @throws GMSEC_Exception Thrown if for whatever reason the field cannot be added to the message.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 * </p>
	 */
	public boolean setField(Field field)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (field == null)
		{
			throw new IllegalArgumentException("Field object is null");
		}

		return m_jniHeartbeatGenerator.setField(field);
	} 


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 *
	 * @param fieldName The name of the field
	 * @param fieldValue The value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if for whatever reason the field cannot be added to the message.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 * </p>
	 */
	public boolean setField(String fieldName, long fieldValue)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniHeartbeatGenerator.setField(fieldName, fieldValue);
	}


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 *
	 * @param fieldName The name of the field
	 * @param fieldValue The value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if for whatever reason the field cannot be added to the message.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 * </p>
	 */
	public boolean setField(String fieldName, double fieldValue)
		throws IllegalArgumentException, GMSEC_Exception
	{
		if (fieldName == null || fieldName.isEmpty())
		{
			throw new IllegalArgumentException("Field name is null or contains an empty string");
		}

		return m_jniHeartbeatGenerator.setField(fieldName, fieldValue);
	}


	/**
	 * Accessor that will add the given field to the heartbeat message being published.
	 *
	 * @param fieldName The name of the field
	 * @param fieldValue The value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throws IllegalArgumentException Thrown if the field name is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the field value is null.
	 * @throws GMSEC_Exception Thrown if for whatever reason the field cannot be added to the message.
	 * <p>
	 * Note: If the PUB-RATE field is provided, then then publish rate for the heartbeat generator
	 * will be updated to publish at the (new) rate.
	 * </p>
	 */
	public boolean setField(String fieldName, String fieldValue)
		throws IllegalArgumentException, GMSEC_Exception
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
