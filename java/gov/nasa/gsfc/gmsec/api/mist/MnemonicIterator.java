/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicIterator.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonicIterator;


/**
 * A class that can be used to iterate over the Mnemonic object(s) contained within a MnemonicMessage.
 * <p>
 * An example usage is:
 * <pre>{@code
 * MnemonicIterator iter = mnemonicMessage.getMnemonicIterator()
 *
 * while (iter.hasNext())
 * {
 *     Mnemonic mnemonic = iter.next();
 *
 *     ...
 * }
 * }</pre>
 */
public class MnemonicIterator
{
	private JNIMnemonicIterator m_jniIter;


	private MnemonicIterator()
	{
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param iter MnemonicIterator object to reference for acquiring internal JNIMnemonicIterator.
	 * @return Internal JNIMnemonicIterator object.
	 */
	public static JNIMnemonicIterator getInternal(MnemonicIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jIter JNIMnemonicIterator object.
	 */
	public MnemonicIterator(JNIMnemonicIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * Indicates whether there are additional Mnemonic objects that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more mnemonics are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * Returns the next Mnemonic accessible using this iterator.
	 *
	 * @return The next available Mnemonic object.
	 *
	 * @throws GMSEC_Exception Thrown if this method is called and there are no
	 * more Mnemonics accessible using this iterator.
	 */
	public Mnemonic next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * Resets this iterator so that it can be used again to iterate over the
	 * mnemonics.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
