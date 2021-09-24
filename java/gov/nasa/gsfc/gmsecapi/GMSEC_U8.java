/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsecapi;

/** @class GMSEC_U8
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is a container class for the GMSEC unsigned 8-bit integer field type.
 *
 */
@Deprecated
public class GMSEC_U8
{
	public int value;


	/**
	 * @brief Default constructor that sets value to zero.
	 */
	public GMSEC_U8()
	{
		this.value = 0;
	}


	/**
	 * @brief Constructor that can be used to assign an unsigned 8-bit value.
	 * @param value - the value to associate with the GMSEC_U8 object.
	 * @note The acceptable range for a GMSEC_U8 value is 0 to 255.  Any value
	 * given that is not in this range will be normalized so that it is in
	 * the range.  For example, a value less than 0 will be adjusted to 0; a value
	 * greater than 255 will be set to 255.
	 */

	public GMSEC_U8(int value)
	{
		this.value = (value < 0 ? 0 : (value > 255 ? 255 : value));
	}


	/**
	 * @brief Copy-constructor.
	 * @param other
	 */
	public GMSEC_U8(GMSEC_U8 other)
	{
		Set(other.Get());
	}


	/**
	 * @brief Set the value associated with this GMSEC_U8 object.
	 * @param value - the value to associate with the GMSEC_U8 object.
	 */
	public void Set(int value)
	{
		this.value = value;
	}


	/**
	 * @brief Get the value associated with this GMSEC_U8 object.
	 * @return value between 0 and 255.
	 */
	public int Get()
	{
		return this.value;
	}


	/**
	 * @brief convert char to string
	 */
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		buf.append(this.value);
		return buf.toString();
	}
}
