/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

import java.lang.reflect.Array;
import java.nio.ByteBuffer; // The ByteBuffer class was new in SDK 1.4.0

/** @class GMSEC_Bin
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC binary field type.
 *
 */
@Deprecated
public class GMSEC_Bin
{
	public byte[] value = null;

	/**
	 * @brief create a binary field value
	 */
	public GMSEC_Bin()
	{
	}

	public GMSEC_Bin(int size)
	{
		value = new byte[size];
	}

	/**
	 * @brief create a binary field from a ByteBuffer
	 * @param arg - ByteBuffer to covert
	 */
	public GMSEC_Bin(ByteBuffer arg)
	{
		value = arg.array();
	}

	/**
	 * @brief create a binary field from a byte array
	 * @param arg - byte array to convert
	 */
	public GMSEC_Bin(byte[] arg)
	{
		value = arg;
	}

	/**
	 * @param b Bin value
	 */
	public GMSEC_Bin(GMSEC_Bin b)
	{
		Set(b.Get());
	}

	/**
	 * @brief set binary value from ByteBuffer
	 * @param arg
	 */
	public void SetBuffer(ByteBuffer arg)
	{
		value = arg.array();
	}

	/**
	 * @brief set binary value from byte array
	 * @param arg
	 */
	public void Set(byte[] arg)
	{
		value = arg;
	}

	/**
	 * @brief get value as byte buffer
	 * @return
	 */
	public byte[] Get()
	{
		return value;
	}

	/**
	 * @brief get size of current binary value
	 * @return
	 */
	public long size()
	{
		return Array.getLength(value);
	}

	/**
	 * @brief convert binary to HEX string
	 * @return hex string
	 */
	public String toString()
	{
		StringBuffer strbuffer = new StringBuffer(((int)this.size() * 2));

		byte[] array = Get();
		char[] raw = new char[] {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

		char high, low;

		long len = this.size();

		for (int i = 0; i < len; i++)
		{

			// Grab the high order bits and pull the raw value
			// shift the high right 4 then mask high
			high = raw[(array[i] >> 4) & 0x0F];

			// Now the low ones	(mask off the high)
			low = raw[array[i] & 0x0F];

			// Append to Stringbuffer
			strbuffer.append(high);
			strbuffer.append(low);
		}

		return strbuffer.toString();
	}
}
