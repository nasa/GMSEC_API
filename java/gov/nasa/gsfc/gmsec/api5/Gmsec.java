/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5;

import java.util.HashMap;
import java.util.Map;


/** 
 * Various useful constants for the GMSEC API.
 *
 */
public class Gmsec
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


	/**
	 * 2014 version of the GMSEC Interface Specification Document (ISD)
	 */
	public final static int MSG_SPEC_2014_00 = 201400;


	/**
	 * 2016 version of the GMSEC Interface Specification Document (ISD)
	 */
	public final static int MSG_SPEC_2016_00 = 201600;


	/**
	 * 2018 version of the Command and Control Message Specification (C2MS)
	 */
	public final static int MSG_SPEC_2018_00 = 201800;


	/**
	 * 2019 version of the Command and Control Message Specification (C2MS)
	 */
	public final static int MSG_SPEC_2019_00 = 201900;


	/**
	 * Default version of the Command and Control Message Specification (C2MS)
	 */
	public final static int MSG_SPEC_CURRENT = MSG_SPEC_2019_00;


	/**
	 * The type of data to be parsed/ingested.
	 */
	public enum DataType
	{
		/** XML formatted data string. */
		XML_DATA(0),

		/** JSON formatted data string. */
		JSON_DATA(1),

		/** Key=Value formatted data string. */
		KEY_VALUE_DATA(2);

		private int type;

		private static Map<Integer, DataType> map = new HashMap<>();

		private DataType(int type) {
			this.type = type;
		}

		static {
			for (DataType dt : DataType.values()) {
				map.put(dt.type, dt);
			}
		}

		/**
		 * Converts the enumerated DataType to an integer.
		 * @return An integer value.
		 */
		public int getValue() {
			return type;
		}

		/**
		 * Converts the integer to an enumerated DataType value.
		 * @param type An integer value representing a DataType enumerated type.
		 * @return A DataType enumerated value.
		 */
		public static DataType valueOf(int type) {
			return map.get(type);
		}
	}
}
