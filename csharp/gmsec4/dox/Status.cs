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

/// <summary>
/// This is the class for holding status information returned by the API
/// calls to provide feedback. Status provides an error code and error
/// message specific to the error type, but independent of error source.
/// </summary>

public class Status : global::System.IDisposable {

  /// <summary>Default constructor.</summary>
  public Status() {
  }

  /// <summary>Create a status object with a specific error classification, error code, error reason and custom code (if any)</summary>
  ///
  /// <param name="eclass">Status class</param>
  /// <param name="code">Status code</param>
  /// <param name="text">Status reason</param>
  /// <param name="custom">Custom error code</param>
  public Status(StatusClass eclass, StatusCode code, string text, int custom) {
  }

  /// <summary>Create a status object with a specific error classification, error code, error reason and custom code (if any)</summary>
  ///
  /// <param name="eclass">Status class</param>
  /// <param name="code">Status code</param>
  /// <param name="text">Status reason</param>
  public Status(StatusClass eclass, StatusCode code, string text) {
  }

  /// <summary>Create a status object with a specific error classification, error code, error reason and custom code (if any)</summary>
  ///
  /// <param name="eclass">Status class</param>
  /// <param name="code">Status code</param>
  public Status(StatusClass eclass, StatusCode code) {
  }

  /// <summary>Create a status object using info from the given GmsecException object</summary>
  ///
  /// <param name="e">GmsecException object</param>
  public Status(GmsecException e) {
  }

  /// <summary>Copy constructor.</summary>
  public Status(Status other) {
  }

  /// <summary>This will return true if the status is indicating an error, false otherwise</summary>
  public bool IsError() {
  }

  /// <summary>This method will return a verbose status string as follows:
  /// [<i>class</i>,<i>code</i>,<i>custom</i>]: <i>reason</i></summary>
  ///
  /// <returns>Verbose status string</returns>
  public string Get() {
  }

  /// <summary>This is a convenience method that will set class, code, error text, and a custom error code.</summary>
  ///
  /// <param name="eclass">Status class</param>
  /// <param name="code">Status code</param>
  /// <param name="text">Status reason</param>
  /// <param name="custom">Custom error code</param>
  public void Set(StatusClass eclass, StatusCode code, string text, int custom) {
  }

  /// <summary>This is a convenience method that will set class, code, and error text.</summary>
  ///
  /// <param name="eclass">Status class</param>
  /// <param name="code">Status code</param>
  /// <param name="text">Status reason</param>
  public void Set(StatusClass eclass, StatusCode code, string text) {
  }

  /// <summary>This is a convenience method that will set class, code, and error text.</summary>
  ///
  /// <param name="eclass">Status class</param>
  /// <param name="code">Status code</param>
  public void Set(StatusClass eclass, StatusCode code) {
  }

  /// <summary>This method will return the status class.</summary>
  ///
  /// <seealso cref="StatusClass" />
  public StatusClass GetClass() {
  }

  /// <summary>This will set the status class</summary>
  ///
  /// <param name="eclass">Status class</param>
  public void SetClass(StatusClass eclass) {
  }

  /// <summary>This will return the status code.</summary>
  ///
  /// <seealso cref="StatusCode" />
  public StatusCode GetCode() {
  }

  /// <summary>This will set the specific status code.</summary>
  ///
  /// <param name="code">status code</param>
  public void SetCode(StatusCode code) {
  }

  /// <summary>This method will retrieve the string detailed description.</summary>
  ///
  /// <returns>Detailed Description String</returns>
  public string GetReason() {
  }

  /// <summary>This will set the status description.</summary>
  ///
  /// <param name="reason">reason text</param>
  public void SetReason(string reason) {
  }

  /// <summary>This will return the custom code number for this status.</summary>
  ///
  /// <returns>Custom Code</returns>
  public int GetCustomCode() {
  }

  /// <summary>This will set the custom code.</summary>
  ///
  /// <param name="code">custom code</param>
  public void SetCustomCode(int code) {
  }

  /// <summary>This clears this Status to NO_ERROR.</summary>
  public void Reset() {
  }

}

}
