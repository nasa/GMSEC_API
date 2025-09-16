/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SchemaIDIterator.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.GmsecException;

import gov.nasa.gsfc.gmsec.api5.jni.JNISchemaIDIterator;


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


	//! @cond
	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param iter Object to reference for acquiring internal JNISchemaIDIterator.
	 * @return Internal JNISchemaIDIterator object.
	 */
	public static JNISchemaIDIterator getInternal(SchemaIDIterator iter)
	{
		return (iter == null ? null : iter.m_jniSchemaIDIterator);
	}


	/**
	 * @hidden
	 * This constructor is for internal GMSEC API use only.
	 * @param jIter Internal JNISchemaIDIterator object.
	 */
	public SchemaIDIterator(JNISchemaIDIterator jIter)
	{
		m_jniSchemaIDIterator = jIter;
	}
	//! @endcond


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
	 * @throws GmsecException Thrown if the iterator has reached the end (i.e. there are no more IDs).
	 */
	public String next() throws GmsecException
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
