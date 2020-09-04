/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SchemaIDIterator.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNISchemaIDIterator;


/**
 * @class SchemaIDIterator
 *
 * @brief Supports the iteration over the list of schema templates maintained by a Specification object.
 *
 * @sa Specification
 */
public class SchemaIDIterator
{
	private JNISchemaIDIterator m_jniSchemaIDIterator = null;


	private SchemaIDIterator()
	{
	}


	public static JNISchemaIDIterator getInternal(SchemaIDIterator iter)
	{
		return (iter == null ? null : iter.m_jniSchemaIDIterator);
	}


	public SchemaIDIterator(JNISchemaIDIterator jIter)
	{
		m_jniSchemaIDIterator = jIter;
	}


	/**
	 * @fn boolean hasNext()
	 *
	 * @desc Indicates whether there are additional Schema IDs that can be referenced using next().
	 *
	 * @return True is returned if additional fields are available, false otherwise.
	 */
	public boolean hasNext()
	{
		return m_jniSchemaIDIterator.hasNext();
	}


	/**
	 * @fn String next()
	 *
	 * @desc Returns the next available Schema ID.
	 *
	 * @throws A GMSECC_Exception is thrown if the iterator has reached the end (i.e. there are no more IDs).
	 */
	public String next() throws GMSEC_Exception
	{
		return m_jniSchemaIDIterator.next();
	}


	/**
	 * @fn void reset()
	 * @desc Resets the iterator to the beginning of the schema list that is maintained by the Specification object.
	 */
	public void reset()
	{
		m_jniSchemaIDIterator.reset();
	}
}
