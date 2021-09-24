/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file gmsecMIST.java
 */

package gov.nasa.gsfc.gmsec.api.mist;


/**
 * Class that holds common MIST related enumerated values.
 */
public class gmsecMIST
{
	/**
	 * Constructor.
	 */
	private gmsecMIST()
	{
	}


	/**
	 * 2014 version of the GMSEC Interface Specification Document (ISD)
	 * for use with ConnectionManager.
	 */
	public final static int GMSEC_ISD_2014_00 = 201400;
	
	/**
	 * 2016 version of the GMSEC Interface Specification Document (ISD)
	 * for use with ConnectionManager.
	 */
	public final static int GMSEC_ISD_2016_00 = 201600;

	/**
	 * 2018 version of the Command and Control Message Specification (C2MS)
	 * for use with ConnectionManager.
	 */
	public final static int GMSEC_ISD_2018_00 = 201800;

	/**
	 * 2019 version of the Command and Control Message Specification (C2MS)
	 * for use with ConnectionManager.
	 */
	public final static int GMSEC_ISD_2019_00 = 201900;

	/**
	 * Default version
	 * for use with ConnectionManager.
	 */
	public final static int GMSEC_ISD_CURRENT = GMSEC_ISD_2019_00;


	/**
	 * Enumerated values that can be used to indicate the status of a request.
	 */
	public enum ResponseStatus
	{
		/** Acknowledge request. */
		ACKNOWLEDGEMENT(1),

		/** Still processing request. */
		WORKING_KEEP_ALIVE(2),

		/** Successful completion of request. */
		SUCCESSFUL_COMPLETION(3),

		/** Failed to complete request. */
		FAILED_COMPLETION(4),

		/** Invalid request. */
		INVALID_REQUEST(5),

		/** Final reply message to request. */
		FINAL_MESSAGE(6);


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
