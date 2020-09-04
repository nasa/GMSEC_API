/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Status.java
 *
 * @brief This is the class for holding status information returned by the API
 * function calls to provide feedback.
 */

package gov.nasa.gsfc.gmsec.api;


/**
 * @class Status
 *
 * @brief This is the class for holding status information returned by the API
 * function calls to provide feedback.  Status provides an error code
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
	 * @fn Status()
	 *
	 * @brief Default Constructor.
	 */
	public Status()
	{
		this(StatusClassification.NO_ERROR, StatusCode.NO_ERROR_CODE, "", 0);
	}


	/**
	 * @fn Status(StatusClassification clazz, StatusCode code, String reason)
	 *
	 * @brief Constructs a Status object with the given error class, code and message.
	 *
	 * @param clazz  - value indicating source of the error
	 * @param code   - value indicating reason for the error
	 * @param reason - explanation for the error
	 *
	 * @sa StatusClassification.java
	 * @sa StatusCode.java
	 */
	public Status(StatusClassification clazz, StatusCode code, String reason)
	{
		this(clazz, code, reason, 0);
	}


	/**
	 * @fn Status(StatusClassification clazz, StatusCode code, String reason, int customCode)
	 *
	 * @brief Constructs a Status object with the given error class, code, custom code and message.
	 *
	 * @param clazz      - value indicating source of the error
	 * @param code       - value indicating reason for the error
	 * @param reason     - explanation for the error
	 * @param customCode - custom error code (typically middleware specific) regarding the root cause of the error
	 *
	 * @sa StatusClassification.java
	 * @sa StatusCode.java
	 */
	public Status(StatusClassification clazz, StatusCode code, String reason, int customCode)
	{
		m_class      = clazz;
		m_code       = code;
		m_reason     = reason;
		m_customCode = customCode;
	}


	/**
	 * @fn Status(Status other)
	 *
	 * @brief Construct Status object using values from an existing Status object.
	 *
	 * @param other - Status object to copy.
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
	 * @fn isError()
	 *
	 * @brief Used to determine whether the Status object is reporting an error or not.
	 *
	 * @note Only the status class is examine; the status code is not checked.
	 *
	 * @return bool - false if the status class is set to NO_ERROR; true otherwise.
	 */
	public boolean isError()
	{
		return m_class != StatusClassification.NO_ERROR;
	}


	/**
	 * @fn get()
	 *
	 * @brief This function will return a verbose error string that contains the
	 * Status class, code, custom code and reason.  The format is as follows:@n
	 * [<i>class</i>,<i>code</i>,<i>custom code<i>] : <i>reason</i>
	 */
	public String get()
	{
		StringBuilder sb = new StringBuilder();
		sb.append("[").append(m_class.getValue()).append(",").append(m_code.getValue()).append(",")
			.append(m_customCode).append("] : ").append(m_reason);

		return sb.toString();
	}


	/**
	 * @fn set(StatusClassification clazz, StatusCode code, String reason)
	 *
	 * @brief Used to set error class, error code, and error text for the Status object.
	 *
	 * @param clazz  - error class
	 * @param code   - error code
	 * @param reason - error string
	 *
	 * @sa StatusClassification.java
	 * @sa StatusCode.java
	 */
	public void set(StatusClassification clazz, StatusCode code, String reason)
	{
		set(clazz, code, reason, 0);
	}


	/**
	 * @fn set(StatusClassification clazz, StatusCode code, int customCode, String reason)
	 *
	 * @brief Used to set error class, error code, custom code, and error text for the Status object.
	 *
	 * @param clazz      - error class
	 * @param code       - error code
	 * @param reason     - error string
	 * @param customCode - custome code (typically from middleware)
	 *
	 * @sa StatusClassification.java
	 * @sa StatusCode.java
	 */
	public void set(StatusClassification clazz, StatusCode code, String reason, int customCode)
	{
		m_class      = clazz;
		m_code       = code;
		m_reason     = (reason == null ? "" : reason);
		m_customCode = customCode;
	}


	/**
	 * @fn StatusClassification getClassification()
	 *
	 * @brief Returns the error class associated with the Status object.
	 *
	 * @return A status class.
	 *
	 * @sa StatusClassification.java
	 */
	public StatusClassification getClassification()
	{
		return m_class;
	}


	/**
	 * @fn setClassification(StatusClassification clazz)
	 *
	 * @brief This will set the error class.
	 *
	 * @param clazz - error class value
	 *
	 * @sa StatusClassification.java
	 */
	public void setClassification(StatusClassification clazz)
	{
		m_class = clazz;
	}


	/**
	 * @fn StatusCode getCode()
	 *
	 * @brief Returns the error code associated with the Status object.
	 *
	 * @return A status code.
	 *
	 * @sa StatusCode.java
	 */
	public StatusCode getCode()
	{
		return m_code;
	}


	/**
	 * @fn setCode(StatusCode code)
	 *
	 * @brief This will set the error code.
	 *
	 * @param code - error value
	 *
	 * @sa StatusCode.java
	 */
	public void setCode(StatusCode code)
	{
		m_code = code;
	}


	/**
	 * @fn String getReason()
	 *
	 * @brief Returns the error message associated with the Status object.
	 *
	 * @return A string containing the error message.
	 */
	public String getReason()
	{
		return m_reason;
	}


	/**
	 * @fn setReason(String reason)
	 *
	 * @brief This will set the error string text.
	 */
	public void setReason(String reason)
	{
		m_reason = reason;
	}


	/**
	 * @fn int getCustomCode()
	 *
	 * @brief Returns the custom code associated with the Status object.
	 *
	 * @return A integer value.
	 */
	public int getCustomCode()
	{
		return m_customCode;
	}


	/**
	 * @fn setCustomCode(int customCode)
	 *
	 * @brief This will set the (middleware) specific error code.
	 *
	 * @param customCode - error value, typically middleware specific
	 */
	public void setCustomCode(int customCode)
	{
		m_customCode = customCode;
	}


	/**
	 * @fn reset()
	 *
	 * @brief This clears resets the Status object to a 'no error' state.
	 */
	public void reset()
	{
		m_class      = StatusClassification.NO_ERROR;
		m_code       = StatusCode.NO_ERROR_CODE;
		m_reason     = "";
		m_customCode = 0;
	}
}
