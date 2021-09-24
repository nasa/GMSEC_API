/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>This class defines the standard GMSEC exception.</summary>

public class GmsecException : global::System.ApplicationException
{

  /// <summary>Constructor</summary>
  ///
  /// <param name="clazz">The error class for the exception</param>
  /// <param name="code">The error code for the exception</param>
  /// <param name="reason">The error message for the exception</param>
  /// <param name="custom">The custom code for the exception</param>
  ///
  /// <seealso cref="StatusClass"/>
  /// <seealso cref="StatusCode"/>
  public GmsecException(StatusClass clazz, StatusCode code, string reason, int custom) {
  }

  /// <summary>Constructor using Status object</summary>
  ///
  /// <param name="status">Status object</param>
  public GmsecException(Status status) {
  }

  /// <summary>Copy Constructor</summary>
  ///
  /// <param name="other">The other GMSEC_Exception object to copy</param>
  public GmsecException(GmsecException other) {
  }

  /// <summary>This will return the error status class</summary>
  ///
  /// <returns>Error status class</returns>
  ///
  /// <seealso cref="StatusClass"/>
  public StatusClass GetErrorClass() {
  }

  /// <summary>This will return the error status code</summary>
  ///
  /// <returns>Error status code</returns>
  ///
  /// <seealso cref="StatusCode"/>
  public StatusCode GetErrorCode() {
  }

  /// <summary>This will return the error message string</summary>
  ///
  /// <returns>Error message string</returns>
  public string GetErrorMessage() {
  }

  /// <summary>This will return the custom error code</summary>
  ///
  /// <returns>Custom error code</returns>
  public int GetCustomCode() {
  }

  /// <summary>This will return a string with the format
  /// "[errorClass,errorCode,customCode]: errorMessage"</summary>
  ///
  /// <returns>Error message string</returns>
  ///
  /// <seealso cref="StatusClass"/>
  /// <seealso cref="StatusCode"/>
  public String ToString() {
  }

}

}
