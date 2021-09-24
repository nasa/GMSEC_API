/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFieldIterator.java
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessageFieldIterator;


/**
 * A class that can be used to iterate over the Fields contained within a Message.
 * <p>
 * An example usage is:
 * <pre>{@code
 * MessageFieldIterator iter = msg.getFieldIterator()
 *
 * while (iter.hasNext())
 * {
 *     Field field = iter.next();
 *
 *     System.out.println(field.toXML() + "\n");
 * }
 * }</pre>
 */
public class MessageFieldIterator
{
	private JNIMessageFieldIterator m_jniIter;


	private MessageFieldIterator()
	{
	}


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param iter Object to reference for acquiring internal JNIMessageFieldIterator
	 * @return Internal JNIMessageFieldIterator object
	 */
	public static JNIMessageFieldIterator getInternal(MessageFieldIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	/**
	 * This method is for internal GMSEC API use only.
	 * @param jIter Internal JNIMessageFieldIterator object
	 */
	public MessageFieldIterator(JNIMessageFieldIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * The type of MessageFieldIterator to employ.
	 */
	public enum Selector
	{
		/**
		 * Used for iterating over all available Fields in a Message.
		 */
		ALL_FIELDS,

		/**
		 * Used for iterating only over header Fields.
		 */
		HEADER_FIELDS,

		/**
		 * Used for iterating only over non-header Fields.
		 */
		NON_HEADER_FIELDS
	}


	/**
	 * Indicates whether there are additional Fields that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more fields are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * Returns the next Field accessible using this iterator.
	 *
	 * @return A Field object.
	 *
	 * @throws GMSEC_Exception Thrown if this method is called and there are no
	 * more Fields accessible using this iterator.
	 */
	public Field next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * Resets this iterator so that it can be used again to iterate over the
	 * fields.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
