/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file GMSEC_Exception.java
 *
 * @brief This file contains the standard class for GMSEC exceptions. It holds
 * the information previously used by Status objects to convey error states. 
 * This is not a total replacement of the Status object, the Status object
 * will still have a purpose for more elaborate error definitions, such as that
 * pertaining to validation.
 */


package gov.nasa.gsfc.gmsec.api;


/**
 * @class GMSEC_Exception
 * @brief This class defines the standard GMSEC exception.
 */
public class GMSEC_Exception extends Exception
{
	private StatusClassification m_errorClass;
	private StatusCode           m_errorCode;
	private int                  m_customCode;
	private String               m_errorMsg;


	public GMSEC_Exception(String errorMsg)
	{
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
	 * @fn GMSEC_Exception(StatusClassification errorClass, StatusCode errorCode, String errorMsg)
	 *
	 * @brief Default Constructor.
	 *
	 * @param errorClass - value indicating source of the error
	 * @param errorCode  - value indicating reason for the error
	 * @param errorMsg   - explanation for the error
	 *
	 * @sa StatusClassification.java
	 * @sa StatusCode.java
	 */
	public GMSEC_Exception(StatusClassification errorClass, StatusCode errorCode, String errorMsg)
	{
		m_errorClass = errorClass;
		m_errorCode  = errorCode;
		m_customCode = 0;
		m_errorMsg   = errorMsg;
	}


	/**
	 * @fn GMSEC_Exception(StatusClassification errorClass, StatusCode errorCode, int customCode, String errorMsg)
	 *
	 * @brief Default Constructor.
	 *
	 * @param errorClass - value indicating source of the error
	 * @param errorCode  - value indicating reason for the error
	 * @param customCode - custom error code (typically middleware specific) regarding the root cause of the error
	 * @param errorMsg   - explanation for the error
	 *
	 * @sa StatusClassification.java
	 * @sa StatusCode.java
	 */
	public GMSEC_Exception(StatusClassification errorClass, StatusCode errorCode, int customCode, String errorMsg)
	{
		m_errorClass = errorClass;
		m_errorCode  = errorCode;
		m_customCode = customCode;
		m_errorMsg   = errorMsg;
	}


	/**
	 * @fn StatusClassification getErrorClassification()
	 *
	 * @brief Returns the error class associated with the exception.
	 *
	 * @return An StatusClassification value.
	 *
	 * @sa StatusClassification.java
	 */
	public StatusClassification getErrorClassification()
	{
		return m_errorClass;
	}


	/**
	 * @fn StatusCode getCode()
	 *
	 * @brief Returns the error code associated with the exception.
	 *
	 * @return An StatusCode value.
	 *
	 * @sa StatusCode.java
	 */
	public StatusCode getErrorCode()
	{
		return m_errorCode;
	}


	/**
	 * @fn int getCustomCode()
	 *
	 * @brief Returns the custom code associated with the exception.
	 *
	 * @return A integer value.
	 */
	public int getCustomCode()
	{
		return m_customCode;
	}


	/**
	 * @fn String getErrorMessage()
	 *
	 * @brief Returns the error message associated with the exception.
	 *
	 * @return A string containing the error message.
	 */
	public String getErrorMessage()
	{
		return m_errorMsg;
	}


	/**
	 * @fn String toString()
	 *
	 * @brief Returns string with the format of [errorClass,errorCode,customCode] : errorMessage
	 *
	 * @return A string containing the error class, code, custom code and message.
	 */
	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append("[").append(m_errorClass.getValue()).append(",").append(m_errorCode.getValue()).append(",")
			.append(m_customCode).append("] : ").append(m_errorMsg);

		return sb.toString();
	}
}
