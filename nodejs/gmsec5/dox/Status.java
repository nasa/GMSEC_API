/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Status.java
 */

package gmsec;


/**
 * This is the class for holding status information returned by the API
 * function calls to provide feedback. Status provides an error code
 * and error message specific to the error type, but independent of error
 * source.
 */
public class Status
{
	/**
	 * Constructor.
	 */
	public Status();


	/**
	 * Constructs a Status object with the given error class, code and message.
	 *
	 * @param clazz  Integer value indicating source of the error
	 * @param code   Integer value indicating reason for the error
	 * @param reason String providing explanation for the error
	 */
	public Status(clazz, code, reason);


	/**
	 * Constructs a Status object with the given error class, code, custom code and message.
	 *
	 * @param clazz      Integer value indicating source of the error
	 * @param code       Integer value indicating reason for the error
	 * @param reason     String providing explanation for the error
	 * @param customCode Custom integer error code (typically middleware specific) regarding the root cause of the error
	 */
	public Status(clazz, code, reason, customCode);


	/**
	 * Construct Status object using values from an existing Status object.
	 *
	 * @param other Status object to copy.
	 *
	 * @throws SWIG_Exception Thrown if the given Status object is null.
	 */
	public Status(other);


	/**
	 * Used to determine whether the Status object is reporting an error or not.
	 * <p>
	 * Note: Only the status class is examine; the status code is not checked.
	 *
	 * @return False if the status class is set to NO_ERROR; true otherwise.
	 */
	public hasError();


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
	public get();


	/**
	 * Used to set error class, error code, and error text for the Status object.
	 *
	 * @param clazz  Integer value indicating source of the error
	 * @param code   Integer value indicating reason for the error
	 * @param reason String providing explanation for the error
	 */
	public set(clazz, code, reason);


	/**
	 * Used to set error class, error code, custom code, and error text for the Status object.
	 *
	 * @param clazz      Integer value indicating source of the error
	 * @param code       Integer value indicating reason for the error
	 * @param reason     String providing explanation for the error
	 * @param customCode Custom integer error code (typically middleware specific) regarding the root cause of the error
	 */
	public set(clazz, code, reason, customCode);


	/**
	 * Returns the error class associated with the Status object.
	 *
	 * @return A error class value as an integer.
	 */
	public getClassification();


	/**
	 * This will set the status classification.
	 *
	 * @param clazz  Integer value indicating source of the error
	 */
	public setClassification(clazz);


	/**
	 * Returns the status code associated with the Status object.
	 *
	 * @return A status code value as an integer.
	 */
	public getCode();


	/**
	 * This will set the status code.
	 *
	 * @param code Integer value indicating reason for the error
	 */
	public setCode(code);


	/**
	 * Returns the error message associated with the Status object.
	 *
	 * @return A string containing the error message.
	 */
	public getReason();


	/**
	 * This will set the error string text.
	 *
	 * @param reason String providing explanation for the error
	 */
	public setReason(reason);


	/**
	 * Returns the custom code associated with the Status object.
	 *
	 * @return A custom code value as an integer.
	 */
	public getCustomCode();


	/**
	 * This will set the (middleware) specific error code.
	 *
	 * @param customCode Integer value for custom code, typically middleware specific.
	 */
	public setCustomCode(customCode);


	/**
	 * This clears the Status object to a 'no error' state.
	 */
	public reset();
}
