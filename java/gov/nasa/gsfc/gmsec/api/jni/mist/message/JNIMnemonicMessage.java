/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.mist.Mnemonic;
import gov.nasa.gsfc.gmsec.api.mist.MnemonicIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.mist.message.MnemonicMessage;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;
import gov.nasa.gsfc.gmsec.api.jni.JNIConfig;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonic;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonicIterator;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;


public class JNIMnemonicMessage extends JNIMistMessage
{
	private JNIMnemonicMessage(long cPtr, boolean own)
	{
		super(cPtr, own);
	}


	public JNIMnemonicMessage(String subject, String schemaID, Specification spec)
	{
		super(gmsecJNI.new_MnemonicMessage(subject,
		                                   schemaID,
		                                   JNISpecification.getCPtr(Specification.getInternal(spec)),
		                                   Specification.getInternal(spec)),
		                                   true);
	}


	public JNIMnemonicMessage(String subject, String schemaID, Config config, Specification spec)
	{
		super(gmsecJNI.new_MnemonicMessage(subject,
		                                   schemaID,
		                                   JNIConfig.getCPtr(Config.getInternal(config)),
		                                   Config.getInternal(config),
		                                   JNISpecification.getCPtr(Specification.getInternal(spec)),
		                                   Specification.getInternal(spec)),
		                                   true);
	}


	public JNIMnemonicMessage(String data) throws GMSEC_Exception
	{
		super(gmsecJNI.new_MnemonicMessage(data), true);
	}


	public JNIMnemonicMessage(JNISpecification jSpec, String data) throws GMSEC_Exception
	{
		super(gmsecJNI.new_MnemonicMessage(JNISpecification.getCPtr(jSpec), jSpec, data), true);
	}


	public JNIMnemonicMessage(JNIMnemonicMessage jOther) throws IllegalArgumentException
	{
		super(gmsecJNI.new_MnemonicMessage_Copy(JNIMnemonicMessage.getCPtr(jOther), jOther), true);
	}


	public void addMnemonic(JNIMnemonic jMnemonic) throws IllegalArgumentException
	{
		gmsecJNI.MnemonicMessage_AddMnemonic(swigCPtr, this, JNIMnemonic.getCPtr(jMnemonic), jMnemonic);
	}


	public int getNumMnemonics()
	{
		return gmsecJNI.MnemonicMessage_GetNumMnemonics(swigCPtr, this);
	}


	public Mnemonic getMnemonic(int index) throws GMSEC_Exception
	{
		long cPtr = gmsecJNI.MnemonicMessage_GetMnemonic(swigCPtr, this, index);

		return new Mnemonic(new JNIMnemonic(cPtr, false));
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
