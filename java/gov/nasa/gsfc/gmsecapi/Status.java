/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

/** @class Status
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This is the class for holding status information returned by the API
 * function calls to provide feedback.  Status provides an error code
 * and error message specific to the error type, but independent of error
 * source.
 *
 */
@Deprecated
public class Status
{
	private int fEClass;
	private long fCode;
	private int fCustomCode;
	private String fText = new String();

	/**
	 * @brief Construct a clean Status
	 */
	public Status()
	{
	}

	/**
	 * @brief Construct a Status while setting class, code, and message text
	 *
	 * @param eclass - the classification this status code represents.
	 *		       The classification identifies the error source,
	 *		       such as application, middleware, operating system levels.
	 * @param code - the GMSEC unique error code
	 * @param text - the error source specific error message
	 */
	public Status(int eclass, long code, String text)
	{
		fEClass = eclass;
		fCode = code;
		fCustomCode = 0;
		fText = text;
	}

	/**
	 * @brief Construct a Status and copy state from an existing Status
	 * @param st
	 */
	public Status(Status st)
	{
		if (st != null)
		{
			fEClass = st.GetClass();
			fCode = st.GetCode();
			fCustomCode = st.GetCustomCode();
			fText = st.GetString();
		}
	}

	/**
	 * @brief Get a textual description of the status.  In the event of an error,
	 * this description may provide detailed information concerning the error.
	 * This message may include vendor specific error information
	 *
	 * @return String - descriptive error information
	 */
	public String GetString()
	{
		return fText;
	}

	/**
	 * @brief Convenience method which concatenates the error class, error code, and
	 * textual description of the status into a single string\
	 *
	 * @return String - error class, error code, and descriptive error information
	 */
	public String Get()
	{
		if (fCustomCode == 0)
		{
			return "[" + fEClass + "," + fCode + "] " + fText;
		}
		return "[" + fEClass + "," + fCode + "," + fCustomCode  + "] " + fText;
	}

	/**
	 * @brief Return the classification of the error type.  This is used to identify various
	 * error sources, which may include API generated errors, middleware errors,
	 * operating system errors, etc.
	 *
	 * @return int - an enumerated type defining the status class
	 * @sa gmsecAPI
	 */
	public int GetClass()
	{
		return fEClass;
	}

	/**
	 * @brief Get a GMSEC specific integer status code
	 *
	 * @return int - unique status code.  Codes <0 indicate an error occured, code >=0
	 *               indicate successful status codes
	 * @sa gmsecAPI
	 */
	public long GetCode()
	{
		return fCode;
	}

	/**
	 * @brief Get a GMSEC specific integer custom status code
	 *
	 * @return int - unique custom status code.
	 *
	 * @sa gmsecAPI
	 */
	public int GetCustomCode()
	{
		return fCustomCode;
	}

	/**
	 * @brief Get a value indicating if the Status instance is an error
	 *
	 * @note This method has been deprecated; use IsError() instead.
	 *
	 *
	 * @return int - true if Status instance contains an error,
	 *               false otherwise
	 */
	public boolean isError()
	{
		return IsError();
	}

	/**
	 * @brief Get a value indicating if the Status instance is an error
	 *
	 * @return int - true if Status instance contains an error,
	 *               false otherwise
	 */
	public boolean IsError()
	{
		return (fEClass != gmsecAPI.GMSEC_STATUS_NO_ERROR);
	}

	/**
	 * @brief Provide for setting the error message
	 *
	 * @param text - message to set
	 */
	public void SetString(String text)
	{
		fText = text;
	}

	/**
	 * @brief Provide for setting the error classification
	 *
	 * @param eclass - status classification
	 * @sa gmsecAPI
	 */
	public void SetClass(int eclass)
	{
		fEClass = eclass;
	}

	/**
	 * @brief Provide for setting the error code
	 *
	 * @param code - code being assigned
	 * @sa gmsecAPI
	 */
	public void SetCode(long code)
	{
		fCode = code;
	}

	/**
	 * @brief Provide for setting the custom error code
	 *
	 * @param code - code being assigned
	 * @sa gmsecAPI
	 */
	public void SetCustomCode(int code)
	{
		fCustomCode = code;
	}

	/**
	 * @brief Provide for setting the error message, error class, and error code
	 *
	 * @param eclass - status classification
	 * @param code - GMSEC error code
	 * @param text - vendor specific error message
	 */
	public void Set(int eclass, long code, String text)
	{
		fEClass = eclass;
		fCode = code;
		fCustomCode = 0;
		fText = text;
	}

	/**
	 * @brief Clear the Status object to support reuse without reallocation
	 */
	public void ReSet()
	{
		fEClass = gmsecAPI.GMSEC_STATUS_NO_ERROR;
		fCode = 0;
		fCustomCode = 0;
		fText = "";
	}
}
