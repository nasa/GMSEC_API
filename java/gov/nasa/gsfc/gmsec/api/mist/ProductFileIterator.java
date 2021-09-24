/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileIterator.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNIProductFileIterator;


/**
 * A class that can be used to iterate over the ProductFile object(s) contained within a ProductFileMessage.
 * <p>
 * An example usage is:
 * <pre>{@code
 * ProductFileIterator iter = prodFileMessage.getProductFileIterator()
 *
 * while (iter.hasNext())
 * {
 *     ProductFile prodFile = iter.next();
 *
 *     ...
 * }
 * }</pre>
 */
public class ProductFileIterator
{
	private JNIProductFileIterator m_jniIter;


	private ProductFileIterator()
	{
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param iter Object to reference for acquiring internal JNIProductFileIterator.
	 * @return Internal JNIProductFileIterator object.
	 */
	public static JNIProductFileIterator getInternal(ProductFileIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jIter Internal JNIProductFileIterator object.
	 */
	public ProductFileIterator(JNIProductFileIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * Indicates whether there are additional ProductFile objects that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more product files are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * Returns the next ProductFile accessible using this iterator.
	 *
	 * @return The next available ProductFile.
	 *
	 * @throws GMSEC_Exception Thrown if this method is called and there are no
	 * more ProductFiles accessible using this iterator.
	 */
	public ProductFile next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * Resets this iterator so that it can be used again to iterate over the
	 * product files.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
