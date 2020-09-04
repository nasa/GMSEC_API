/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFieldIterator.java
 *
 * @brief A class that can be used to iterate over the Fields contained within a Message.
 */

package gov.nasa.gsfc.gmsec.api;

import gov.nasa.gsfc.gmsec.api.field.*;
import gov.nasa.gsfc.gmsec.api.jni.JNIMessageFieldIterator;


/**
 * @class MessageFieldIterator
 *
 * @brief A class that can be used to iterate over the Fields contained within a Message.
 *
 * An example usage is:
 * @code
 * MessageFieldIterator iter = msg.getFieldIterator()
 *
 * while (iter.hasNext())
 * {
 *     Field field = iter.next();
 *
 *     System.out.println(field.toXML() + "\n");
 * }
 * @endcode
 */
public class MessageFieldIterator
{
	private JNIMessageFieldIterator m_jniIter;


	private MessageFieldIterator()
	{
	}


	public static JNIMessageFieldIterator getInternal(MessageFieldIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	public MessageFieldIterator(JNIMessageFieldIterator jIter)
	{
		m_jniIter = jIter;
	}


	/**
	 * @enum Selector
	 *
	 * @desc The type of MessageFieldIterator to employ.
	 */
	public enum Selector
	{
		ALL_FIELDS,          ///< Used for iterating over all available Fields in a Message.
		HEADER_FIELDS,       ///< Used for iterating only over header Fields.
		NON_HEADER_FIELDS    ///< Used for iterating only over non-header Fields.
	}


	/**
	 * @fn boolean hasNext()
	 *
	 * @brief Indicates whether there are additional Fields that can be accessed
	 * using this iterator.
	 *
	 * @return Returns true if more fields are accessible; otherwise returns false.
	 */
	public boolean hasNext()
	{
		return m_jniIter.hasNext();
	}


	/**
	 * @fn Field next()
	 *
	 * @brief Returns the next Field accessible using this iterator.
	 *
	 * @throw A GMSEC_Exception is thrown if this method is called and there are no
	 * more Fields accessible using this iterator.
	 */
	public Field next() throws GMSEC_Exception
	{
		return m_jniIter.next();
	}


	/**
	 * @fn void reset()
	 *
	 * @desc Resets this iterator so that it can be used again to iterate over the
	 * fields.
	 */
	public void reset()
	{
		m_jniIter.reset();
	}
}
