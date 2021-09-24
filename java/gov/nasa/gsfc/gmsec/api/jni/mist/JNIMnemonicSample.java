/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.MnemonicSample;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;
import gov.nasa.gsfc.gmsec.api.jni.field.JNIFieldConverter;


public class JNIMnemonicSample
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	protected JNIMnemonicSample(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_MnemonicSample(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	protected static long getCPtr(JNIMnemonicSample obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIMnemonicSample(String timestamp, Field rawValue)
	{
		this(gmsecJNI.new_MnemonicSample(timestamp, JNIField.getCPtr(Field.getInternal(rawValue)), Field.getInternal(rawValue)), true);
	}


	public JNIMnemonicSample(JNIMnemonicSample other)
	{
		this(gmsecJNI.new_MnemonicSample_Copy(JNIMnemonicSample.getCPtr(other), other), true);
	}


	public String getTimestamp()
	{
		return gmsecJNI.MnemonicSample_GetTimestamp(swigCPtr, this);
	}


	public Field getRawValue()
	{
		long cPtr = gmsecJNI.MnemonicSample_GetRawValue(swigCPtr, this);

		return JNIFieldConverter.cloneField(cPtr);
	}


	public boolean hasEUValue()
	{
		return gmsecJNI.MnemonicSample_HasEUValue(swigCPtr, this);
	}


	public Field getEUValue() throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.MnemonicSample_GetEUValue(swigCPtr, this);

		return JNIFieldConverter.cloneField(cPtr);
	}


	public void setEUValue(Field value)
	{
		gmsecJNI.MnemonicSample_SetEUValue(swigCPtr, this, JNIField.getCPtr(Field.getInternal(value)), Field.getInternal(value));
	}


	public boolean hasTextValue()
	{
		return gmsecJNI.MnemonicSample_HasTextValue(swigCPtr, this);
	}


	public String getTextValue() throws GMSEC_Exception
	{
		return gmsecJNI.MnemonicSample_GetTextValue(swigCPtr, this);
	}


	public void setTextValue(String value)
	{
		gmsecJNI.MnemonicSample_SetTextValue(swigCPtr, this, value);
	}


	public boolean hasFlags()
	{
		return gmsecJNI.MnemonicSample_HasFlags(swigCPtr, this);
	}


	public int getFlags() throws GMSEC_Exception
	{
		return gmsecJNI.MnemonicSample_GetFlags(swigCPtr, this);
	}


	public void setFlags(int flags)
	{
		gmsecJNI.MnemonicSample_SetFlags(swigCPtr, this, flags);
	}


	public boolean hasLimitEnableDisable()
	{
		return gmsecJNI.MnemonicSample_HasLimitEnableDisable(swigCPtr, this);
	}


	public boolean getLimitEnableDisable() throws GMSEC_Exception
	{
		return gmsecJNI.MnemonicSample_GetLimitEnableDisable(swigCPtr, this);
	}


	public void setLimitEnableDisable(boolean value)
	{
		gmsecJNI.MnemonicSample_SetLimitEnableDisable(swigCPtr, this, value);
	}


	public boolean hasLimit()
	{
		return gmsecJNI.MnemonicSample_HasLimit(swigCPtr, this);
	}


	public MnemonicSample.LimitFlag getLimit() throws GMSEC_Exception
	{
		int flag = gmsecJNI.MnemonicSample_GetLimit(swigCPtr, this);

		return MnemonicSample.LimitFlag.values()[flag];
	}


	public void setLimit(MnemonicSample.LimitFlag flag)
	{
		gmsecJNI.MnemonicSample_SetLimit(swigCPtr, this, flag.ordinal());
	}


	public boolean hasStalenessStatus()
	{
		return gmsecJNI.MnemonicSample_HasStalenessStatus(swigCPtr, this);
	}


	public boolean getStalenessStatus() throws GMSEC_Exception
	{
		return gmsecJNI.MnemonicSample_GetStalenessStatus(swigCPtr, this);
	}


	public void setStalenessStatus(boolean staleness)
	{
		gmsecJNI.MnemonicSample_SetStalenessStatus(swigCPtr, this, staleness);
	}


	public boolean hasQuality()
	{
		return gmsecJNI.MnemonicSample_HasQuality(swigCPtr, this);
	}


	public boolean getQuality() throws GMSEC_Exception
	{
		return gmsecJNI.MnemonicSample_GetQuality(swigCPtr, this);
	}


	public void setQuality(boolean quality)
	{
		gmsecJNI.MnemonicSample_SetQuality(swigCPtr, this, quality);
	}
}
