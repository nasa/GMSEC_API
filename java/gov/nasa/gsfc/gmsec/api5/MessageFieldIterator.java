/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFieldIterator.java
 */

package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.jni.JNIMessageFieldIterator;

import java.util.NoSuchElementException;


/**
 * A class that can be used to iterate over the Fields contained within a Message.
 *
 * <p>
 * Note: MessageFieldIterator is not thread safe.
 * </p>
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


	//! @cond
	/** 
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param iter Object to reference for acquiring internal JNIMessageFieldIterator
	 * @return Internal JNIMessageFieldIterator object
	 */
	public static JNIMessageFieldIterator getInternal(MessageFieldIterator iter)
	{
		return (iter == null ? null : iter.m_jniIter);
	}


	/**
	 * @hidden
	 * This method is for internal GMSEC API use only.
	 * @param jIter Internal JNIMessageFieldIterator object
	 */
	public MessageFieldIterator(JNIMessageFieldIterator jIter)
	{
		m_jniIter = jIter;
	}
	//! @endcond


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
		NON_HEADER_FIELDS,

		/**
		 * Used for iterating only over tracking Fields.
		 */
		TRACKING_FIELDS
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
	 * @return A Field object, or null if no field is available.
	 *
	 * @throws NoSuchElementException Thrown if the iteration has no more elements
	 */
	public Field next()
		throws NoSuchElementException
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
