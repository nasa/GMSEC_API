/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api;


/** 
 * The constants needed for the GMSEC API are in this file.
 *
 */
public class gmsecAPI
{
	/**
	 * Timeout period to indicate not to block, but to return immediately with
	 * error if condition not met
	 */
	public final static int NO_WAIT = 0;


	/**
	 * Timeout period to indicate to block indefinately, and not to return
	 * unless condition is met or an error has occured
	 */
	public final static int WAIT_FOREVER = -1;


	/**
	 * Value to indicate that a request message should only be issued once
	 * should a response message not be received.
	 */
	public final static int REQUEST_REPUBLISH_NEVER = -1;
}
