/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.ConfigFile;
import gov.nasa.gsfc.gmsec.api.ConfigFileIterator;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;


public class JNIConfigFile
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected JNIConfigFile(long cPtr, boolean cMemoryOwn)
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
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_ConfigFile(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIConfigFile obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConfigFile()
	{
		this(gmsecJNI.new_ConfigFile(), true);
	}


	public void load(String filePath)
	{
		gmsecJNI.ConfigFile_Load(swigCPtr, this, filePath);
	}


	public void save(String filePath, boolean compact)
	{
		gmsecJNI.ConfigFile_Save(swigCPtr, this, filePath, compact);
	}


	public void fromXML(String xml)
	{
		gmsecJNI.ConfigFile_FromXML(swigCPtr, this, xml);
	}


	public String toXML()
	{
		return gmsecJNI.ConfigFile_ToXML(swigCPtr, this);
	}


	public boolean isLoaded()
	{
		return gmsecJNI.ConfigFile_IsLoaded(swigCPtr, this);
	}


	public Config lookupConfig(String name)
	{
		long cPtr = gmsecJNI.ConfigFile_LookupConfig(swigCPtr, this, name);

		if (cPtr != 0)
		{
			return new Config(new JNIConfig(cPtr, true));
		}

		return null;
	}


	public Message lookupMessage(String name)
	{
		long cPtr = gmsecJNI.ConfigFile_LookupMessage(swigCPtr, this, name);

		if (cPtr != 0)
		{
			return new Message(new JNIMessage(cPtr, true));
		}

		return null;
	}


	public String lookupSubscription(String name)
	{
		return gmsecJNI.ConfigFile_LookupSubscription(swigCPtr, this, name);
	}


	public ConfigFile.SubscriptionEntry lookupSubscriptionEntry(String name)
	{
		long cPtr = gmsecJNI.ConfigFile_LookupSubscriptionEntry(swigCPtr, this, name);

		return new ConfigFile.SubscriptionEntry(new JNISubscriptionEntry(cPtr, false));
	}


	public void addSubscription(String name, String subject)
	{
		gmsecJNI.ConfigFile_AddSubscription(swigCPtr, this, name, subject);
	}


	public void addConfig(String name, Config config)
	{
		JNIConfig jCfg = Config.getInternal(config);

		gmsecJNI.ConfigFile_AddConfig(swigCPtr, this, name, JNIConfig.getCPtr(jCfg), jCfg);
	}


	public void addMessage(String name, Message message)
	{
		JNIMessage jMsg = Message.getInternal(message);

		gmsecJNI.ConfigFile_AddMessage(swigCPtr, this, name, JNIMessage.getCPtr(jMsg), jMsg);
	}


	public void addCustomXML(String xml)
	{
		gmsecJNI.ConfigFile_AddCustomXML(swigCPtr, this, xml);
	}


	public boolean removeSubscription(String name)
	{
		return gmsecJNI.ConfigFile_RemoveSubscription(swigCPtr, this, name);
	}


	public boolean removeConfig(String name)
	{
		return gmsecJNI.ConfigFile_RemoveConfig(swigCPtr, this, name);
	}


	public boolean removeMessage(String name)
	{
		return gmsecJNI.ConfigFile_RemoveMessage(swigCPtr, this, name);
	}


	public boolean removeCustomXML(String xml)
	{
		return gmsecJNI.ConfigFile_RemoveCustomXML(swigCPtr, this, xml);
	}


	public ConfigFileIterator getIterator()
	{
		long cPtr = gmsecJNI.ConfigFile_GetIterator(swigCPtr, this);

		return new ConfigFileIterator(new JNIConfigFileIterator(cPtr, false));
	}
}
