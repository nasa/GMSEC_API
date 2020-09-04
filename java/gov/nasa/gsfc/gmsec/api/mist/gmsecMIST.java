/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file gmsecMIST.java
 * 
 *  @brief This file just contains a public MIST-related enumerations, etc.
**/

package gov.nasa.gsfc.gmsec.api.mist;


/**
 * @class gmsecMIST
 *
 * @brief Class that holds common MIST related enumerated values.
 */
public class gmsecMIST
{
	/// @brief 2014 version of the GMSEC Interface Specification Document (ISD)
	/// for use with ConnectionManager.
	public final static int GMSEC_ISD_2014_00 = 201400;
	
	/// @brief 2016 version of the GMSEC Interface Specification Document (ISD)
	/// for use with ConnectionManager.
	public final static int GMSEC_ISD_2016_00 = 201600;

	/// @brief Default version of the GMSEC Interface Specification Document (ISD)
	/// for use with ConnectionManager.
	public final static int GMSEC_ISD_CURRENT = GMSEC_ISD_2016_00;


	/**
	 * @enum ResponseStatus
	 *
	 * @desc Enumerated values that can be used to indicate the status of a request.
	 */
	public enum ResponseStatus
	{
		ACKNOWLEDGEMENT(1),         ///< Acknowledgement
		WORKING_KEEP_ALIVE(2),      ///< Working Keep Alive
		SUCCESSFUL_COMPLETION(3),   ///< Successful Completion
		FAILED_COMPLETION(4),       ///< Failed Completion
		INVALID_REQUEST(5),         ///< Invalid Request
		FINAL_MESSAGE(6);           ///< Final %Message


		private int value;
		private ResponseStatus(int value) { this.value = value; }

		public int getValue() { return value; }

		public static ResponseStatus getUsingValue(int value)
		{
			ResponseStatus rs = ACKNOWLEDGEMENT;

			for (ResponseStatus n : ResponseStatus.values())
			{
				if (n.value == value)
				{
					rs = n;
					break;
				}
			}

			return rs;
		}
	}
}
