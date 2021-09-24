/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Status.java
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * This is the class for holding status information returned by the API
 * function calls to provide feedback. Status provides an error code
 * and error message specific to the error type, but independent of error
 * source.
 */
public class Status
{
	private StatusClassification m_class;
	private StatusCode           m_code;
	private String               m_reason;
	private int                  m_customCode;


	/**
	 * Constructor.
	 */
	public Status()
	{
		this(StatusClassification.NO_ERROR, StatusCode.NO_ERROR_CODE, "", 0);
	}


	/**
	 * Constructs a Status object with the given error class, code and message.
	 *
	 * @param clazz  Value indicating source of the error
	 * @param code   Value indicating reason for the error
	 * @param reason Explanation for the error
	 *
	 * @see StatusClassification
	 * @see StatusCode
	 */
	public Status(StatusClassification clazz, StatusCode code, String reason)
	{
		this(clazz, code, reason, 0);
	}


	/**
	 * Constructs a Status object with the given error class, code, custom code and message.
	 *
	 * @param clazz      Value indicating source of the error
	 * @param code       Value indicating reason for the error
	 * @param reason     Explanation for the error
	 * @param customCode Custom error code (typically middleware specific) regarding the root cause of the error
	 *
	 * @see StatusClassification
	 * @see StatusCode
	 */
	public Status(StatusClassification clazz, StatusCode code, String reason, int customCode)
	{
		m_class      = clazz;
		m_code       = code;
		m_reason     = reason;
		m_customCode = customCode;
	}


	/**
	 * Constructs a Status object with info from the given GMSEC_Exception object.
	 *
	 * @param e GMSEC_Exception object indicating source/reason for the error
	 *
	 * @throws IllegalArgumentException Thrown if the exception object is null.
	 */
	public Status(GMSEC_Exception e)
		throws IllegalArgumentException
	{
		if (e == null)
		{
			throw new IllegalArgumentException("GMSEC_Exception object is null");
		}

		m_class      = e.getErrorClassification();
		m_code       = e.getErrorCode();
		m_reason     = e.getErrorMessage();
		m_customCode = e.getCustomCode();
	}


	/**
	 * Construct Status object using values from an existing Status object.
	 *
	 * @param other Status object to copy.
	 */
	public Status(Status other)
	{
		if (other != null)
		{
			m_class      = other.m_class;
			m_code       = other.m_code;
			m_reason     = other.m_reason;
			m_customCode = other.m_customCode;
		}
	}


	/**
	 * Used to determine whether the Status object is reporting an error or not.
	 * <p>
	 * Note: Only the status class is examine; the status code is not checked.
	 *
	 * @return False if the status class is set to NO_ERROR; true otherwise.
	 */
	public boolean isError()
	{
		return m_class != StatusClassification.NO_ERROR;
	}


	/**
	 * This function will return a verbose error string that contains the
	 * Status class, code, custom code and reason.
	 * <p>
	 * The format is as follows:
	 * <pre>{@code
	 * [class,code,custom code] : reason
	 * }</pre>
	 *
	 * @return The error string associated with the Status object.
	 */
	public String get()
	{
		StringBuilder sb = new StringBuilder();
		sb.append("[").append(m_class.getValue()).append(",").append(m_code.getValue()).append(",")
			.append(m_customCode).append("] : ").append(m_reason);

		return sb.toString();
	}


	/**
	 * Used to set error class, error code, and error text for the Status object.
	 *
	 * @param clazz  Error class
	 * @param code   Error code
	 * @param reason Error string
	 *
	 * @see StatusClassification
	 * @see StatusCode
	 */
	public void set(StatusClassification clazz, StatusCode code, String reason)
	{
		set(clazz, code, reason, 0);
	}


	/**
	 * Used to set error class, error code, custom code, and error text for the Status object.
	 *
	 * @param clazz      Error class
	 * @param code       Error code
	 * @param reason     Error string
	 * @param customCode Custom code (typically from middleware)
	 *
	 * @see StatusClassification
	 * @see StatusCode
	 */
	public void set(StatusClassification clazz, StatusCode code, String reason, int customCode)
	{
		m_class      = clazz;
		m_code       = code;
		m_reason     = (reason == null ? "" : reason);
		m_customCode = customCode;
	}


	/**
	 * Returns the error class associated with the Status object.
	 *
	 * @return A StatusClassification enumerated value.
	 *
	 * @see StatusClassification
	 */
	public StatusClassification getClassification()
	{
		return m_class;
	}


	/**
	 * This will set the status classification.
	 *
	 * @param clazz Status classification value
	 *
	 * @see StatusClassification
	 */
	public void setClassification(StatusClassification clazz)
	{
		m_class = clazz;
	}


	/**
	 * Returns the status code associated with the Status object.
	 *
	 * @return A StatusCode enumerated value.
	 *
	 * @see StatusCode
	 */
	public StatusCode getCode()
	{
		return m_code;
	}


	/**
	 * This will set the status code.
	 *
	 * @param code Status code enumerated value
	 *
	 * @see StatusCode
	 */
	public void setCode(StatusCode code)
	{
		m_code = code;
	}


	/**
	 * Returns the error message associated with the Status object.
	 *
	 * @return A string containing the error message.
	 */
	public String getReason()
	{
		return m_reason;
	}


	/**
	 * This will set the error string text.
	 *
	 * @param reason The error text string.
	 */
	public void setReason(String reason)
	{
		m_reason = reason;
	}


	/**
	 * Returns the custom code associated with the Status object.
	 *
	 * @return The custom code.
	 */
	public int getCustomCode()
	{
		return m_customCode;
	}


	/**
	 * This will set the (middleware) specific error code.
	 *
	 * @param customCode Error value, typically middleware specific
	 */
	public void setCustomCode(int customCode)
	{
		m_customCode = customCode;
	}


	/**
	 * This clears the Status object to a 'no error' state.
	 */
	public void reset()
	{
		m_class      = StatusClassification.NO_ERROR;
		m_code       = StatusCode.NO_ERROR_CODE;
		m_reason     = "";
		m_customCode = 0;
	}
}
