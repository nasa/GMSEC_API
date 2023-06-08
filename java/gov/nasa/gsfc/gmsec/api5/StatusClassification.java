/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file StatusClassification.java
 */

package gov.nasa.gsfc.gmsec.api5;


/**
 * Error/Status Class codes (used with Status and GmsecException classes)
 */
public class StatusClassification
{
	/** No error */
	public final static int NO_ERROR = 0;

	/** Error creating Connection object */
	public final static int FACTORY_ERROR = 1;

	/** Error within Connection class */
	public final static int CONNECTION_ERROR = 2;

	/** Error within Config class */
	public final static int CONFIG_ERROR = 3;

	/** Middleware specific error */
	public final static int MIDDLEWARE_ERROR = 4;

	/** Error within Message class */
	public final static int MSG_ERROR = 5;

	/** Error within Field class */
	public final static int FIELD_ERROR = 6;

	/** Error using a callback */
	public final static int CALLBACK_ERROR = 7;

	/** Error referencing a callback object */
	public final static int CALLBACK_LOOKUP_ERROR = 8;

	/** Error within ConfigFile class */
	public final static int CONFIGFILE_ERROR = 9;

	/** Error using iterrator */
	public final static int ITERATOR_ERROR = 10;

	/** Error with policy */
	public final static int POLICY_ERROR = 11;

	/** Error with AutoDispatcher */
	public final static int DISPATCHER_ERROR = 12;

	/** ResourceInfoGenerator Error */
	public final static int RSRC_INFO_GENERATOR_ERROR = 13;

	/** HeartbeatGenerator Error */
	public final static int HEARTBEAT_GENERATOR_ERROR = 14;

	/** Message Factory Error */
	public final static int MSG_FACTORY_ERROR = 15;

	/** Specification Error */
	public final static int SPECIFICATION_ERROR = 16;

	/** Message Template Error */
	public final static int MSG_TEMPLATE_ERROR = 17;

	/** Field Template Error */
	public final static int FIELD_TEMPLATE_ERROR = 18;

	/** Custom error */
	public final static int CUSTOM_ERROR = 49;

	/** Other error */
	public final static int OTHER_ERROR = 50;
}
