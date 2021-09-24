/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file StatusClassification.java
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * Error/Status Class codes (used with Status and Exception classes)
 */
public enum StatusClassification
{
	/** No error */
	NO_ERROR(0),

	/** Error creating Connection object */
	FACTORY_ERROR(1),

	/** Error within Connection class */
	CONNECTION_ERROR(2),

	/** Error within Config class */
	CONFIG_ERROR(3),

	/** Middleware specific error */
	MIDDLEWARE_ERROR(4),

	/** Error within Message class */
	MSG_ERROR(5),

	/** Error within Field class */
	FIELD_ERROR(6),

	/** Error using a callback */
	CALLBACK_ERROR(7),

	/** Error referencing a callback object */
	CALLBACK_LOOKUP_ERROR(8),

	/** Error within ConfigFile class */
	CONFIGFILE_ERROR(9),

	/** Error using iterrator */
	ITERATOR_ERROR(10),

	/** Error with policy */
	POLICY_ERROR(11),

	/** Error with AutoDispatcher */
	DISPATCHER_ERROR(12),

	/** MIST Error */
	MIST_ERROR(13),

	/** Custom error */
	CUSTOM_ERROR(49),

	/** Other error */
	OTHER_ERROR(50);

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
