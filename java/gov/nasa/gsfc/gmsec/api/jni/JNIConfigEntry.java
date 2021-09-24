/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.Config;


public class JNIConfigEntry
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIConfigEntry(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_ConfigEntry(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIConfigEntry entry)
	{
		return (entry == null) ? 0 : entry.swigCPtr;
	}


	public String getName()
	{
		return gmsecJNI.ConfigEntry_GetName(swigCPtr, this);
	}


	public Config getConfig()
	{
		long cPtr = gmsecJNI.ConfigEntry_GetConfig(swigCPtr, this);

		return new Config(new JNIConfig(cPtr, false));
	}
}
