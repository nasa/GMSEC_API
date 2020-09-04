/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileIterator.java
 *
 * @brief A class that can be used to iterate over the Fields contained within a Message.
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIProductFileIterator;


/**
 * @class ProductFileIterator
 *
 * @brief A class that can be used to iterate over the ProductFile object(s) contained within a ProductFileMessage.
 *
 * An example usage is:
 * @code
 * ProductFileIterator iter = prodFileMessage.getProductFileIterator()
 *
 * while (iter.hasNext())
 * {
 *     ProductFile prodFile = iter.next();
 *
 *     ...
 * }
 * @endcode
 */
public class ProductFileIterator
{
	private JNIProductFileIterator m_jniIter;


	private ProductFileIterator()
	{
	}


	public static JNIProductFileIterator getInternal(ProductFileIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	public ProductFileIterator(JNIProductFileIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * @fn boolean hasNext()
	 *
	 * @brief Indicates whether there are additional ProductFile objects that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more product files are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * @fn ProductFile next()
	 *
	 * @brief Returns the next ProductFile accessible using this iterator.
	 *
	 * @throw A GMSEC_Exception is thrown if this method is called and there are no
	 * more ProductFiles accessible using this iterator.
	 */
	public ProductFile next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * @fn void reset()
	 *
	 * @desc Resets this iterator so that it can be used again to iterate over the
	 * product files.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
