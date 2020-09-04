/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni;

import gov.nasa.gsfc.gmsec.api.mist.Mnemonic;
import gov.nasa.gsfc.gmsec.api.mist.MnemonicIterator;
import gov.nasa.gsfc.gmsec.api.mist.MnemonicMessage;


public class JNIMnemonicMessage extends JNIMessage
{
	protected JNIMnemonicMessage(long cPtr, boolean cMemoryOwn)
	{
		super(cPtr, cMemoryOwn);
	}


	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_MnemonicMessage(swigCPtr, this);
			swigCMemOwn = false;
		}

		swigCPtr = 0;
	}


	protected static long getCPtr(JNIMnemonicMessage obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIMnemonicMessage(String subject, int version)
	{
		this(gmsecJNI.new_MnemonicMessage(subject, version), true);
	}


	public JNIMnemonicMessage(String subject, JNIConfig jConfig, int version)
	{
		this(gmsecJNI.new_MnemonicMessage(subject, JNIConfig.getCPtr(jConfig), jConfig, version), true);
	}


	public JNIMnemonicMessage(String data)
	{
		this(gmsecJNI.new_MnemonicMessage(data), true);
	}


	public JNIMnemonicMessage(JNIMnemonicMessage other)
	{
		this(gmsecJNI.new_MnemonicMessage_Copy(JNIMnemonicMessage.getCPtr(other), other), true);
	}


	public void addMnemonic(JNIMnemonic jMnemonic)
	{
		gmsecJNI.MnemonicMessage_AddMnemonic(swigCPtr, this, JNIMnemonic.getCPtr(jMnemonic), jMnemonic);
	}


	public int getNumMnemonics()
	{
		return gmsecJNI.MnemonicMessage_GetNumMnemonics(swigCPtr, this);
	}


	public Mnemonic getMnemonic(int index)
	{
		long cPtr = gmsecJNI.MnemonicMessage_GetMnemonic(swigCPtr, this, index);

		return new Mnemonic(new JNIMnemonic(cPtr, true));
	}


	public MnemonicIterator getMnemonicIterator()
	{
		long cPtr = gmsecJNI.MnemonicMessage_GetMnemonicIterator(swigCPtr, this);

		return new MnemonicIterator(new JNIMnemonicIterator(cPtr, false));
	}


	public static MnemonicMessage convertMessage(JNIMessage jMsg)
	{
		long cPtr = gmsecJNI.MnemonicMessage_ConvertMessage(JNIMessage.getCPtr(jMsg), jMsg);

		return new MnemonicMessage(new JNIMnemonicMessage(cPtr, true));
	}
}
