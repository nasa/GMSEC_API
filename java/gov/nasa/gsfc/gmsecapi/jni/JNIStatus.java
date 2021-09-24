/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.jni;

/** @class Status
 *
 * @brief This is the class for holding status information returned by the API
 * function calls to provide feedback.  Status provides an error code
 * and error message specific to the error type, but independent of error
 * source.
 *
 */
public class JNIStatus extends gov.nasa.gsfc.gmsecapi.Status
{
	private long swigCPtr;
	protected boolean swigCMemOwn;

	protected JNIStatus(long cPtr, boolean cMemoryOwn)
	{
		swigCMemOwn = cMemoryOwn;
		swigCPtr = cPtr;
	}

	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}

	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Status(swigCPtr, this);
			swigCMemOwn = false;
		}
		swigCPtr = 0;
	}

	protected static long getCPtr(JNIStatus obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}

	/**
	 * @brief Construct a clean Status
	 */
	public JNIStatus()
	{
		this(gmsecJNI.new_Status(), true);
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
	public JNIStatus(int eclass, long code, String text)
	{
		this(gmsecJNI.new_Status_Set(eclass, code, text), true);
	}

	/**
	 * @brief Construct a Status and copy state from an existing Status
	 * @param st
	 */
	public JNIStatus(JNIStatus st)
	{
		this(gmsecJNI.new_Status_Copy(JNIStatus.getCPtr(st), st), true);
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
		return gmsecJNI.Status_GetString(swigCPtr, this);
	}

	/**
	 * @brief Convenience method which concatenates the error class, error code, and
	 * textual description of the status into a single string\
	 *
	 * @return String - error class, error code, and descriptive error information
	 */
	public String Get()
	{
		return gmsecJNI.Status_Get(swigCPtr, this);
	}

	/**
	 * @brief Return the classification of the error type.  This is used to identify various
	 * error sources, which may include API generated errors, middleware errors,
	 * operating system errors, etc.
	 *
	 * @return int - an enumerated type defining the status class
	 */
	public int GetClass()
	{
		return gmsecJNI.Status_GetClass(swigCPtr, this);
	}

	/**
	 * @brief Get a GMSEC specific integer status code
	 *
	 * @return int - unique status code.  Codes <0 indicate an error occured, code >=0
	 *               indicate successful status codes
	 */
	public long GetCode()
	{
		return gmsecJNI.Status_GetCode(swigCPtr, this);
	}

	/**
	 * @brief Get integer custom status code
	 *
	 * @return int - custom (vendor specific) status code.
	 */
	public int GetCustomCode()
	{
		return gmsecJNI.Status_GetCustomCode(swigCPtr, this);
	}

	/**
	 * @brief Get a value indicating if the Status instance is an error
	 *
	 * @note This method has been deprecated; use IsError() instead.
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
		return gmsecJNI.Status_IsError(swigCPtr, this) != 0;
	}

	/**
	 * @brief Provide for setting the error message
	 *
	 * @param text - message to set
	 */
	public void SetString(String text)
	{
		gmsecJNI.Status_SetString(swigCPtr, this, text);
	}

	/**
	 * @brief Provide for setting the error classification
	 *
	 * @param eclass - status classification
	 */
	public void SetClass(int eclass)
	{
		gmsecJNI.Status_SetClass(swigCPtr, this, eclass);
	}

	/**
	 * @brief Provide for setting the error code
	 *
	 * @param code - code being assigned
	 */
	public void SetCode(long code)
	{
		gmsecJNI.Status_SetCode(swigCPtr, this, code);
	}

	/**
	 * @brief Provide for setting the custom error code
	 *
	 * @param code - custom code (vendor specific) being assigned
	 */
	public void SetCustomCode(int code)
	{
		gmsecJNI.Status_SetCustomCode(swigCPtr, this, code);
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
		gmsecJNI.Status_Set(swigCPtr, this, eclass, code, text);
	}

	/**
	 * @brief Clear the Status object to support reuse without reallocation
	 */
	public void ReSet()
	{
		gmsecJNI.Status_ReSet(swigCPtr, this);
	}
}
