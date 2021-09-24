/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file GMSEC_Exception.java
 */


package gov.nasa.gsfc.gmsec.api;


/**
 * This class defines the standard GMSEC exception.
 */
public class GMSEC_Exception extends Exception
{
	private StatusClassification m_errorClass;
	private StatusCode           m_errorCode;
	private int                  m_customCode;
	private String               m_errorMsg;


	/**
	 * This constructor is for internal GMSEC API use only
	 * @param errorMsg Error message string.
	 */
	public GMSEC_Exception(String errorMsg)
	{
		super(errorMsg);

		String[] tmp = errorMsg.split(":");

		if (tmp.length >= 2)
		{
			int openBracket  = tmp[0].indexOf('[');
			int closeBracket = tmp[0].indexOf(']');

			String tmp2 = tmp[0].substring(openBracket + 1, closeBracket);

			String[] tmp3 = tmp2.split(",");

			if (tmp3.length == 3)
			{
				m_errorClass = StatusClassification.getUsingValue(Integer.parseInt(tmp3[0]));
				m_errorCode  = StatusCode.getUsingValue(Integer.parseInt(tmp3[1]));
				m_customCode = Integer.parseInt(tmp3[2]);
			}

			StringBuilder sb = new StringBuilder();

			for (int i = 1; i < tmp.length; ++i)
			{
				sb.append(tmp[i].trim());

				if (i < tmp.length - 1)
				{
					sb.append(": ");
				}
			}

			m_errorMsg = sb.toString();
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
	public GMSEC_Exception(StatusClassification errorClass, StatusCode errorCode, String errorMsg)
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
	public GMSEC_Exception(StatusClassification errorClass, StatusCode errorCode, int customCode, String errorMsg)
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
	public GMSEC_Exception(Status status)
	{
		super(generateDetails(status.getClassification(), status.getCode(), status.getCustomCode(), status.getReason()));

		m_errorClass = status.getClassification();
		m_errorCode  = status.getCode();
		m_customCode = status.getCustomCode();
		m_errorMsg   = status.getReason();
	}


	/**
	 * Returns the error class associated with the exception.
	 *
	 * @return An StatusClassification value.
	 *
	 * @see StatusClassification
	 */
	public StatusClassification getErrorClassification()
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
	public StatusCode getErrorCode()
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


	private static String generateDetails(StatusClassification errorClass, StatusCode errorCode, int customCode, String errorMsg)
	{
		StringBuilder sb = new StringBuilder();

		sb.append("[").append(errorClass.getValue()).append(",").append(errorCode.getValue()).append(",")
			.append(customCode).append("]: ").append(errorMsg);

		return sb.toString();
	}
}
