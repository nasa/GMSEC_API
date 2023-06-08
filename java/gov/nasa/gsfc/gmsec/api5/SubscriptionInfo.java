/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SubscriptionInfo.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.jni.JNISubscriptionInfo;


/**
 * A class that contains the information the user has supplied when
 * setting up a subscription.
 *
 * @see Connection#subscribe(String)
 * @see Connection#unsubscribe(SubscriptionInfo)
 */
public class SubscriptionInfo
{
	private JNISubscriptionInfo m_jniSubInfo;


	// Prevent users from instantiating their own objects.
	private SubscriptionInfo()
	{
	}


	/**
	 * This constructor for internal GMSEC API use only.
	 * @param jSubInfo A JNISubscriptionInfo object.
	 */
	public SubscriptionInfo(JNISubscriptionInfo jSubInfo)
	{
		m_jniSubInfo = jSubInfo;
	}


	/**
	 * This method for internal GMSEC API use only.
	 * @param obj A SubscriptionInfo object.
	 * @return A JNISubscriptionInfo object.
	 */
	public static JNISubscriptionInfo getInternal(SubscriptionInfo obj)
	{
		return (obj == null ? null : obj.m_jniSubInfo);
	}


	/**
	 * Returns the subject pattern string used when setting up the subscription.
	 *
	 * @return A subject pattern string.
	 */
	public String getSubjectPattern()
	{
		return m_jniSubInfo.getSubjectPattern();
	}
}
