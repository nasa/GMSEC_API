/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicIterator.java
 *
 * @brief A class that can be used to iterate over the Fields contained within a Message.
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIMnemonicIterator;


/**
 * @class MnemonicIterator
 *
 * @brief A class that can be used to iterate over the Mnemonic object(s) contained within a MnemonicMessage.
 *
 * An example usage is:
 * @code
 * MnemonicIterator iter = mnemonicMessage.getMnemonicIterator()
 *
 * while (iter.hasNext())
 * {
 *     Mnemonic mnemonic = iter.next();
 *
 *     ...
 * }
 * @endcode
 */
public class MnemonicIterator
{
	private JNIMnemonicIterator m_jniIter;


	private MnemonicIterator()
	{
	}


	public static JNIMnemonicIterator getInternal(MnemonicIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	public MnemonicIterator(JNIMnemonicIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * @fn boolean hasNext()
	 *
	 * @brief Indicates whether there are additional Mnemonic objects that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more mnemonics are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * @fn Mnemonic next()
	 *
	 * @brief Returns the next Mnemonic accessible using this iterator.
	 *
	 * @throw A GMSEC_Exception is thrown if this method is called and there are no
	 * more Mnemonics accessible using this iterator.
	 */
	public Mnemonic next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * @fn void reset()
	 *
	 * @desc Resets this iterator so that it can be used again to iterate over the
	 * mnemonics.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
