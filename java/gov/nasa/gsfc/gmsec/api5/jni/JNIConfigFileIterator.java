/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.ConfigFile;
import gov.nasa.gsfc.gmsec.api5.ConfigFile.ConfigEntry;
import gov.nasa.gsfc.gmsec.api5.ConfigFile.MessageEntry;
import gov.nasa.gsfc.gmsec.api5.ConfigFile.SubscriptionEntry;
import gov.nasa.gsfc.gmsec.api5.ConfigFileIterator;
import gov.nasa.gsfc.gmsec.api5.Message;


public class JNIConfigFileIterator
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIConfigFileIterator(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	protected void finalize() throws Throwable
	{
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIConfigFileIterator iter)
	{
		return (iter == null) ? 0 : iter.swigCPtr;
	}


	public boolean hasNextConfig()
	{
		return gmsecJNI.ConfigFileIterator_HasNextConfig(swigCPtr, this);
	}


	public boolean hasNextMessage()
	{
		return gmsecJNI.ConfigFileIterator_HasNextMessage(swigCPtr, this);
	}


	public boolean hasNextSubscription()
	{
		return gmsecJNI.ConfigFileIterator_HasNextSubscription(swigCPtr, this);
	}


	public ConfigFile.ConfigEntry nextConfig()
	{
		long cPtr = gmsecJNI.ConfigFileIterator_NextConfig(swigCPtr, this);

		return new ConfigFile.ConfigEntry(new JNIConfigEntry(cPtr, true));
	}


	public ConfigFile.MessageEntry nextMessage()
	{
		long cPtr = gmsecJNI.ConfigFileIterator_NextMessage(swigCPtr, this);

		return new ConfigFile.MessageEntry(new JNIMessageEntry(cPtr, true));
	}


	public ConfigFile.SubscriptionEntry nextSubscription()
	{
		long cPtr = gmsecJNI.ConfigFileIterator_NextSubscription(swigCPtr, this);

		return new ConfigFile.SubscriptionEntry(new JNISubscriptionEntry(cPtr, true));
	}


	public void reset()
	{
		gmsecJNI.ConfigFileIterator_Reset(swigCPtr, this);
	}
}
