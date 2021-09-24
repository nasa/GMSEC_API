/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.jni.util.JNILog;

import java.util.AbstractMap;
import java.util.AbstractMap.SimpleImmutableEntry;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Map;
import java.util.Map.Entry;


public class JNIConfig
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIConfig(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;

		initLogging(gmsecJNI.Config_GetValue(swigCPtr, this, "logfile", null));
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
			gmsecJNI.delete_Config(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIConfig obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIConfig()
	{
		this(gmsecJNI.new_Config(), true);
	}


	public JNIConfig(String[] args)
	{
		this(gmsecJNI.new_Config_Args(args), true);
	}


	public JNIConfig(String data)
	{
		this(gmsecJNI.new_Config_Data(data), true);
	}


	public JNIConfig(Config other)
	{
		this(gmsecJNI.new_Config_Copy(JNIConfig.getCPtr(Config.getInternal(other)), Config.getInternal(other)), true);
	}


	public JNIConfig(JNIConfig other)
	{
		this(gmsecJNI.new_Config_Copy(JNIConfig.getCPtr(other), other), true);
	}


	public void addValue(String name, String value)
	{
		gmsecJNI.Config_AddValue(swigCPtr, this, name, value);

		if (name.equalsIgnoreCase("logfile"))
		{
			initLogging(value);
		}
	}


	public boolean clearValue(String name)
	{
		return gmsecJNI.Config_ClearValue(swigCPtr, this, name);
	}


	public void clear()
	{
		gmsecJNI.Config_Clear(swigCPtr, this);
	}


	public Collection<Map.Entry<String, String>> entrySet()
	{
		Collection<Map.Entry<String, String>> pairs = new HashSet<Map.Entry<String, String>>();

		String[] keys   = gmsecJNI.Config_KeySet(swigCPtr, this);
		String[] values = gmsecJNI.Config_Values(swigCPtr, this);

		int i = 0;
		for (String key : keys)
		{
			Map.Entry<String, String> entry = new AbstractMap.SimpleImmutableEntry<String, String>(key, values[i++]);

			pairs.add(entry);
		}

		return pairs;
	}


	public Collection<String> keySet()
	{
		Collection<String> keySet = new ArrayList<String>();

		String[] keys = gmsecJNI.Config_KeySet(swigCPtr, this);

		for (String key : keys)
		{
			keySet.add(key);
		}

		return keySet;
	}


	public String getValue(String name, String defaultValue)
	{
		return gmsecJNI.Config_GetValue(swigCPtr, this, name, defaultValue);
	}	
	

	public boolean getBooleanValue(String name) throws GMSEC_Exception
	{
		boolean result = false;

		result = gmsecJNI.Config_GetBooleanValue(swigCPtr, this, name);

		return result;
	}


	public boolean getBooleanValue(String name, boolean defaultValue)
	{
		return gmsecJNI.Config_GetBooleanValue(swigCPtr, this, name, defaultValue);
	}


	public int getIntegerValue(String name)
	{
		return gmsecJNI.Config_GetIntegerValue(swigCPtr, this, name);
	}


	public int getIntegerValue(String name, int defaultValue)
	{
		return gmsecJNI.Config_GetIntegerValue(swigCPtr, this, name, defaultValue);
	}


	public double getDoubleValue(String name)
	{
		return gmsecJNI.Config_GetDoubleValue(swigCPtr, this, name);
	}


	public double getDoubleValue(String name, double defaultValue)
	{
		return gmsecJNI.Config_GetDoubleValue(swigCPtr, this, name, defaultValue);
	}


	public void merge(Config other, boolean overwriteExisting)
	{
		JNIConfig jOtherCfg = Config.getInternal(other);

		gmsecJNI.Config_Merge(swigCPtr, this, JNIConfig.getCPtr(jOtherCfg), jOtherCfg, overwriteExisting);
	}


	public String toXML()
	{
		return gmsecJNI.Config_ToXML(swigCPtr, this);
	}


	public void fromXML(String xml)
	{
		gmsecJNI.Config_FromXML(swigCPtr, this, xml);
	}


	public String toJSON()
	{
		return gmsecJNI.Config_ToJSON(swigCPtr, this);
	}


	private void initLogging(String stream)
	{
		if (stream != null)
		{
			JNILog.getDefaultLogHandler().setStream(stream);
		}
	}
}
