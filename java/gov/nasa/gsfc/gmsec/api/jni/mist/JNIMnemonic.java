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


public class JNIMnemonic
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


        public JNIMnemonic(long cPtr, boolean cMemoryOwn)
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
			gmsecJNI.delete_Mnemonic(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIMnemonic obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIMnemonic(String name, java.util.List<MnemonicSample> samples)
	{
		this(gmsecJNI.new_Mnemonic(name,
			ArrayListConverter.listToMnemonicSamplePtrs(samples),
			ArrayListConverter.listToJNIMnemonicSamples(samples),
			(samples == null ? 0 : samples.size())), true);
	}


	public JNIMnemonic(String name)
        {
        	this(name, new java.util.ArrayList<MnemonicSample>());
        }


	public JNIMnemonic(JNIMnemonic other)
	{
		this(gmsecJNI.new_Mnemonic_Copy(other.swigCPtr, other), true);
	}


	public String getName()
	{
		return gmsecJNI.Mnemonic_GetName(swigCPtr, this);
	}


	public boolean statusAvailable()
	{
		return gmsecJNI.Mnemonic_StatusAvailable(swigCPtr, this);
	}


	public Field getStatus() throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.Mnemonic_GetStatus(swigCPtr, this);

		return JNIFieldConverter.cloneField(cPtr);
	}


	public void setStatus(Field status)
	{
		JNIField jField = Field.getInternal(status);

		gmsecJNI.Mnemonic_SetStatus(swigCPtr, this, JNIField.getCPtr(jField), jField);
	}


	public boolean unitsAvailable()
	{
		return gmsecJNI.Mnemonic_UnitsAvailable(swigCPtr, this);
	}


	public String getUnits() throws GMSEC_Exception
	{
		return gmsecJNI.Mnemonic_GetUnits(swigCPtr, this);
	}


	public void setUnits(String units)
	{
		gmsecJNI.Mnemonic_SetUnits(swigCPtr, this, units);
	}


	public long getSampleCount()
	{
		return gmsecJNI.Mnemonic_GetSampleCount(swigCPtr, this);
	}


	public MnemonicSample getSample(long index) throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.Mnemonic_GetSample(swigCPtr, this, index);

		return new MnemonicSample(new JNIMnemonicSample(cPtr, true));
	}

	                              
	public void addSample(MnemonicSample sample)
	{
		JNIMnemonicSample jSample = MnemonicSample.getInternal(sample);

		gmsecJNI.Mnemonic_AddSample(swigCPtr, this, JNIMnemonicSample.getCPtr(jSample), jSample);
	}
}
