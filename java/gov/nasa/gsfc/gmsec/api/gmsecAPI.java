/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api;


/** 
 * @class gmsecAPI
 *
 * @brief The constants needed for the GMSEC API are in this file.
 *
 */
public class gmsecAPI
{
	/**
	 * @brief Timeout period to indicate not to block, but to return immediately with
	 * error if condition not met
	 *
	 * @sa Connection.receive()
	 * @sa Connection.request()
	 * @sa ConnectionManager.receive()
	 * @sa ConnectionManager.request()
	 */
	public final static int NO_WAIT = 0;


	/**
	 * @brief Timeout period to indicate to block indefinately, and not to return
	 * unless condition is met or an error has occured
	 *
	 * @sa Connection.receive()
	 * @sa Connection.request()
	 * @sa ConnectionManager.receive()
	 * @sa ConnectionManager.request()
	 */
	public final static int WAIT_FOREVER = -1;
}
