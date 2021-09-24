/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.gmsecAPI;


/** @class ConfigFile
 *
 * @brief The Message object is a container for GMSEC Messages.  The methods of this
 * class allow the construction and manipulation of the data in a message.
 *
 */
public class JNIConfigFile
{
	private long    swigCPtr;
	private boolean swigCMemOwn;


	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}


	protected synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_ConfigFile(swigCPtr, this);
			swigCMemOwn = false;
		}
		swigCPtr = 0;
	}


	protected static long getCPtr(JNIConfigFile obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConfigFile()
	{
		swigCPtr = gmsecJNI.new_ConfigFile(this);

		if (swigCPtr != 0)
		{
			swigCMemOwn = true;
		}
	}


	public JNIConfigFile(String file)
	{
		swigCPtr = gmsecJNI.new_ConfigFile(this, file);

		if (swigCPtr != 0)
		{
			swigCMemOwn = true;
		}
	}


	public Status Load()
	{
		long cPtr = gmsecJNI.ConfigFile_Load(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	public Status Load(String filePath)
	{
		long cPtr = gmsecJNI.ConfigFile_Load(swigCPtr, this, filePath);
		return new JNIStatus(cPtr, true);
	}


	public Status Save(String filePath, boolean compact)
	{
		long cPtr = gmsecJNI.ConfigFile_Save(swigCPtr, this, filePath, compact);
		return new JNIStatus(cPtr, true);
	}


	public Status FromXML(String xml)
	{
		long cPtr = gmsecJNI.ConfigFile_FromXML(swigCPtr, this, xml);
		return new JNIStatus(cPtr, true);
	}


	public Status ToXML(GMSEC_String xml)
	{
		long cPtr = gmsecJNI.ConfigFile_ToXML(swigCPtr, this, xml);
		return new JNIStatus(cPtr, true);
	}


	public boolean IsLoaded()
	{
		return gmsecJNI.ConfigFile_IsLoaded(swigCPtr, this);
	}


	public Status LookupConfig(String name, JNIConfig cfg)
	{
		JNIConfig jcfg = new JNIConfig();

		long cPtr = gmsecJNI.ConfigFile_LookupConfig(swigCPtr, this, name, JNIConfig.getCPtr(jcfg), jcfg);

		cfg.Set(jcfg);

		return new JNIStatus(cPtr, true);
	}


	public Status LookupMessage(String name, Message msg)
	{
		long cPtr;
		JNIMessage jmsg = (JNIMessage) msg.GetImpl();

		cPtr = gmsecJNI.ConfigFile_LookupMessage(swigCPtr, this, name, JNIMessage.getCPtr(jmsg), jmsg);

		msg.SetImpl(jmsg);

		return new JNIStatus(cPtr, true);
	}


	public Status LookupSubscription(String name, GMSEC_String pattern)
	{
		long cPtr = gmsecJNI.ConfigFile_LookupSubscription(swigCPtr, this, name, pattern);
		return new JNIStatus(cPtr, true);
	}


	public Status AddSubscription(String name, String subscription)
	{
		long cPtr = gmsecJNI.ConfigFile_AddSubscription(swigCPtr, this, name, subscription);
		return new JNIStatus(cPtr, true);
	}


	public Status AddConfig(String name, Config config)
	{
		JNIConfig jcfg = Config.getInternal(config);

		long cPtr = gmsecJNI.ConfigFile_AddConfig(swigCPtr, this, JNIConfig.getCPtr(jcfg), name, config);
		return new JNIStatus(cPtr, true);
	}


	public Status AddMessage(String name, Message message)
	{
		JNIMessage jmsg = (JNIMessage) message.GetImpl();

		long cPtr = gmsecJNI.ConfigFile_AddMessage(swigCPtr, this, JNIMessage.getCPtr(jmsg), name, message);
		return new JNIStatus(cPtr, true);
	}


	public Status AddCustomXML(String xml)
	{
		long cPtr = gmsecJNI.ConfigFile_AddCustomXML(swigCPtr, this, xml);
		return new JNIStatus(cPtr, true);
	}


	public Status RemoveSubscription(String name)
	{
		long cPtr = gmsecJNI.ConfigFile_RemoveSubscription(swigCPtr, this, name);
		return new JNIStatus(cPtr, true);
	}


	public Status RemoveConfig(String name)
	{
		long cPtr = gmsecJNI.ConfigFile_RemoveConfig(swigCPtr, this, name);
		return new JNIStatus(cPtr, true);
	}


	public Status RemoveMessage(String name)
	{
		long cPtr = gmsecJNI.ConfigFile_RemoveMessage(swigCPtr, this, name);
		return new JNIStatus(cPtr, true);
	}


	public Status RemoveCustomXML(String xml)
	{
		long cPtr = gmsecJNI.ConfigFile_RemoveCustomXML(swigCPtr, this, xml);
		return new JNIStatus(cPtr, true);
	}
}
