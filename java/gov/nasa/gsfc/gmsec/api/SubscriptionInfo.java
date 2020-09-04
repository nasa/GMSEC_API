/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SubscriptionInfo.java
 *
 * @brief A class that contains the information the user has supplied when
 * setting up a subscription.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.jni.JNISubscriptionInfo;


/**
 * @class SubscriptionInfo
 *
 * @brief A class that contains the information the user has supplied when
 * setting up a subscription.
 *
 * @sa Connection.subscribe() @n
 *     Connection.unsubscribe()
 */
public class SubscriptionInfo
{
	private JNISubscriptionInfo m_jniSubInfo;


	// Prevent users from instantiating their own objects.
	//
	private SubscriptionInfo()
	{
	}


	public SubscriptionInfo(JNISubscriptionInfo jSubInfo)
	{
		m_jniSubInfo = jSubInfo;
	}


	public static JNISubscriptionInfo getInternal(SubscriptionInfo obj)
	{
		return (obj == null ? null : obj.m_jniSubInfo);
	}


	/**
	 * @fn String getSubject()
	 *
	 * @brief Returns the subject/topic string the user has supplied when setting
	 * up a subscription.
	 */
	public String getSubject()
	{
		return m_jniSubInfo.getSubject();
	}


	/**
	 * @fn Callback getCallback()
	 *
	 * @brief Returns the Callback object, if any, used when setting up the subscription.
	 */
	public Callback getCallback()
	{
		return m_jniSubInfo.getCallback();
	}
}
