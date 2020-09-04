/*
 * Copyright 2007-2015 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17; U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file StatusClassification.java
 *
 * @brief Types and constants for the gmsec API related to error classes.
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * @enum StatusClassification
 * @desc Error/Status Class codes (used with Status and Exception classes)
 */
public enum StatusClassification
{
	NO_ERROR(0),                ///< No error
	FACTORY_ERROR(1),           ///< Error creating Connection object
	CONNECTION_ERROR(2),        ///< Error within Connection class
	CONFIG_ERROR(3),            ///< Error within Config class
	MIDDLEWARE_ERROR(4),        ///< Middleware specific error
	MSG_ERROR(5),               ///< Error within Message class
	FIELD_ERROR(6),             ///< Error within Field class
	CALLBACK_ERROR(7),          ///< Error using a callback
	CALLBACK_LOOKUP_ERROR(8),   ///< Error referencing a callback object
	CONFIGFILE_ERROR(9),        ///< Error within ConfigFile class
	ITERATOR_ERROR(10),         ///< Error using iterrator
	POLICY_ERROR(11),           ///< Error with policy
	DISPATCHER_ERROR(12),       ///< Error with AutoDispatcher
	MIST_ERROR(13),             ///< MIST error
	CUSTOM_ERROR(49),           ///< Custom error
	OTHER_ERROR(50);            ///< Other error

	private int value;
	private StatusClassification(int value) { this.value = value; }

	public int getValue() { return value; };

	public static StatusClassification getUsingValue(int value)
	{
		StatusClassification sc = OTHER_ERROR;

		for (StatusClassification n : StatusClassification.values())
		{
			if (n.value == value)
			{
				sc = n;
				break;
			}
		}

		return sc;
	}
}
