/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SubscriptionInfo.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIConnMgrSubscriptionInfo;


/**
 * A class that contains the information the user has supplied when
 * setting up a subscription using any of the ConnectionManager subcribe() methods.
 *
 * @see ConnectionManager#subscribe(String)
 * @see ConnectionManager#subscribe(String, ConnectionManagerCallback)
 * @see ConnectionManager#unsubscribe(SubscriptionInfo)
 */
public class SubscriptionInfo
{
	private JNIConnMgrSubscriptionInfo m_jniSubInfo;


	// Prevent users from instantiating their own objects.
	private SubscriptionInfo()
	{
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param obj Object to reference for acquiring internal JNIConnMgrSubscriptionInfo.
	 * @return Internal JNIConnMgrSubscriptionInfo object.
	 */
	public static JNIConnMgrSubscriptionInfo getInternal(SubscriptionInfo obj)
	{
		return (obj == null ? null : obj.m_jniSubInfo);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jSubInfo Internal JNIConnMgrSubscriptionInfo object.
	 */
	public SubscriptionInfo(JNIConnMgrSubscriptionInfo jSubInfo)
	{
		m_jniSubInfo = jSubInfo;
	}


	/**
	 * Returns the subject/topic string the user has supplied when setting
	 * up a subscription.
	 *
	 * @return Subject string associated with the SubscriptionInfo.
	 */
	public String getSubject()
	{
		return m_jniSubInfo.getSubject();
	}


	/**
	 * Returns the Callback object, if any, used when setting up the subscription.
	 *
	 * @return The ConnectionManagerCallback, if any, associated with the SubscriptionInfo.
	 */
	public ConnectionManagerCallback getCallback()
	{
		return m_jniSubInfo.getCallback();
	}
}
