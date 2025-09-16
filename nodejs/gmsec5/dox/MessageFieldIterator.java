/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFieldIterator.java
 */

package gmsec;


/**
 * A class that can be used to iterate over the Fields contained within a Message.
 *
 * @note MessageFieldIterator is not thread safe.
 * <p>
 * An example usage is:
 * <pre>{@code
 * let iter = msg.getFieldIterator();
 * while (iter.hasNext()) {
 *     const field = iter.next();
 *     console.log(field.toXML() + "\n");
 * }
 * }</pre>
 */
public class MessageFieldIterator
{
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
	public hasNext();


	/**
	 * Returns the next Field accessible using this iterator.
	 *
	 * @return A Field object.
	 *
	 * @throws GmsecException Thrown if the iteration has no more elements
	 */
	public next();


	/**
	 * Resets this iterator so that it can be used again to iterate over the
	 * fields.
	 */
	public reset();
}
