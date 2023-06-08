/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file GmsecException.java
 */


package gov.nasa.gsfc.gmsec.api5;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.regex.Matcher;
import java.util.regex.Pattern;


/**
 * This class defines the standard GMSEC exception.
 */
public final class GmsecException extends Exception
{
	/**
	 * The regex expression to match API generated exception messages.
	 */
	private final String EXCEPTION_REGEX = "^(\\[(\\d+)\\,(\\d+)\\,(\\d+)\\])( : )(.*)";

	/**
	 * The error class associated with the GMSEC Exception object.
	 */
	private int m_errorClass;

	/**
	 * The error code associated with the GMSEC Exception object.
	 */
	private int m_errorCode;

	/**
	 * The custom code associated with the GMSEC Exception object.
	 */
	private int m_customCode;

	/**
	 * The error message associated with the GMSEC Exception object.
	 */
	private String m_errorMsg;


	/**
	 * Constructor.
	 * This constructor is intended for internal GMSEC API use only!
	 * @param errorMsg Error message string.
	 */
	public GmsecException(String errorMsg)
	{
		super(errorMsg);

		Pattern pattern = Pattern.compile(EXCEPTION_REGEX);
		Matcher matcher = pattern.matcher(errorMsg);

		if (matcher.matches() && matcher.groupCount() == 6)
		{
			try
			{
				m_errorClass = Integer.parseInt(matcher.group(2).trim());
				m_errorCode  = Integer.parseInt(matcher.group(3).trim());
				m_customCode = Integer.parseInt(matcher.group(4).trim());
				m_errorMsg   = matcher.group(6).trim();
			}
			catch (IllegalArgumentException e) // includes NumberFormatException
			{
				Log.warning("Unable to parse GmsecException error message");
			}
		}
		else
		{
			m_errorMsg = errorMsg;
		}
	}


	/**
	 * Constructor.
	 *
	 * @param errorClass Value indicating source of the error
	 * @param errorCode  Value indicating reason for the error
	 * @param errorMsg   Explanation for the error
	 *
	 * @see StatusClassification
	 * @see StatusCode
	 */
	public GmsecException(int errorClass, int errorCode, String errorMsg)
	{
		super(generateDetails(errorClass, errorCode, 0, errorMsg));

		m_errorClass = errorClass;
		m_errorCode  = errorCode;
		m_customCode = 0;
		m_errorMsg   = errorMsg;
	}


	/**
	 * Constructor.
	 *
	 * @param errorClass Value indicating source of the error
	 * @param errorCode  Value indicating reason for the error
	 * @param customCode Custom error code (typically middleware specific) regarding the root cause of the error
	 * @param errorMsg   Explanation for the error
	 *
	 * @see StatusClassification
	 * @see StatusCode
	 */
	public GmsecException(int errorClass, int errorCode, int customCode, String errorMsg)
	{
		super(generateDetails(errorClass, errorCode, customCode, errorMsg));

		m_errorClass = errorClass;
		m_errorCode  = errorCode;
		m_customCode = customCode;
		m_errorMsg   = errorMsg;
	}


	/**
	 * Constructor that accepts Status object.
	 *
	 * @param status Status object to convert to an exception
	 */
	public GmsecException(Status status)
	{
		super(generateDetails(status.getClassification(), status.getCode(), status.getCustomCode(), status.getReason()));

		m_errorClass = status.getClassification();
		m_errorCode  = status.getCode();
		m_customCode = status.getCustomCode();
		m_errorMsg   = status.getReason();
	}


	/**
	 * Copy Constructor
	 *
	 * @param other GmsecException object to copy
	 */
	public GmsecException(GmsecException other)
	{
		super(generateDetails(other.getErrorClassification(), other.getErrorCode(), other.getCustomCode(), other.getErrorMessage()));

		m_errorClass = other.getErrorClassification();
		m_errorCode  = other.getErrorCode();
		m_customCode = other.getCustomCode();
		m_errorMsg   = other.getErrorMessage();
	}


	/**
	 * Returns the error class associated with the exception.
	 *
	 * @return An StatusClassification value.
	 *
	 * @see StatusClassification
	 */
	public int getErrorClassification()
	{
		return m_errorClass;
	}


	/**
	 * Returns the error code associated with the exception.
	 *
	 * @return An StatusCode value.
	 *
	 * @see StatusCode
	 */
	public int getErrorCode()
	{
		return m_errorCode;
	}


	/**
	 * Returns the custom code associated with the exception.
	 *
	 * @return An integer value.
	 */
	public int getCustomCode()
	{
		return m_customCode;
	}


	/**
	 * Returns the error message associated with the exception.
	 *
	 * @return A string containing the error message.
	 */
	public String getErrorMessage()
	{
		return m_errorMsg;
	}


	/**
	 * Returns string with the format of [errorClass,errorCode,customCode] : errorMessage
	 *
	 * @return A string containing the error class, code, custom code and message.
	 */
	public String toString()
	{
		return getMessage();
	}


	private static final String generateDetails(int errorClass, int errorCode, int customCode, String errorMsg)
	{
		StringBuilder sb = new StringBuilder();

		sb.append("[").append(errorClass).append(",").append(errorCode).append(",")
			.append(customCode).append("]: ").append(errorMsg);

		return sb.toString();
	}
}
