/*
 * Copyright 2007-2021 United States Government as represented by the
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
 * Supports the iteration over the list of schema templates maintained by a Specification object.
 *
 * @see Specification
 */
public class SchemaIDIterator
{
	private JNISchemaIDIterator m_jniSchemaIDIterator = null;


	private SchemaIDIterator()
	{
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param iter Object to reference for acquiring internal JNISchemaIDIterator.
	 * @return Internal JNISchemaIDIterator object.
	 */
	public static JNISchemaIDIterator getInternal(SchemaIDIterator iter)
	{
		return (iter == null ? null : iter.m_jniSchemaIDIterator);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jIter Internal JNISchemaIDIterator object.
	 */
	public SchemaIDIterator(JNISchemaIDIterator jIter)
	{
		m_jniSchemaIDIterator = jIter;
	}


	/**
	 * Indicates whether there are additional Schema IDs that can be referenced using next().
	 *
	 * @return True is returned if additional fields are available, false otherwise.
	 */
	public boolean hasNext()
	{
		return m_jniSchemaIDIterator.hasNext();
	}


	/**
	 * Returns the next available Schema ID.
	 *
	 * @return A Schema ID string.
	 *
	 * @throws GMSEC_Exception Thrown if the iterator has reached the end (i.e. there are no more IDs).
	 */
	public String next() throws GMSEC_Exception
	{
		return m_jniSchemaIDIterator.next();
	}


	/**
	 * Resets the iterator to the beginning of the schema list that is maintained by the Specification object.
	 */
	public void reset()
	{
		m_jniSchemaIDIterator.reset();
	}
}
