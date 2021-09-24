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

/// <summary>Class that provides the logging interface.</summary>

public class Log : global::System.IDisposable {

  /// <summary>sets the maximum logging level</summary>
  ///
  /// <param name="level">The desired logging level</param>
  public static void SetReportingLevel(LogLevel level) {
  }

  /// <summary>Returns the current logging level</summary>
  public static LogLevel GetReportingLevel() {
  }

  /// <summary>
  /// Registers a log handler that will be called for all logging levels.
  /// Note: To unregister a log handler, pass a null log handler to this method.
  /// </summary>
  ///
  /// <param name="handler">The LogHandler to register</param>
  public static void RegisterHandler(LogHandler handler) {
  }

  /// <summary>
  /// Registers a log handler that will be called for indicated logging level.
  /// Note: To unregister a log handler, pass a null log handler to this method.
  /// </summary>
  ///
  /// <param name="level">The desired logging level</param>
  /// <param name="handler">The LogHandler to register</param>
  public static void RegisterHandler(LogLevel level, LogHandler handler) {
  }

  /// <summary>Converts the given log level value to a string value</summary>
  ///
  /// <param name="level">The LogLevel enumerated value</param>
  public static string ToString(LogLevel level) {
  }

  /// <summary>Converts the given log level string to a log level enumerated value</summary>
  ///
  /// <param name="level">The log level as a string</param>
  public static LogLevel FromString(string level) {
  }

  /// <summary>Logs a message at the error log level</summary>
  ///
  /// <param name="entry">The message to log</param>
  public static void Error(string entry) {
  }

  /// <summary>Logs a message at the secure log level</summary>
  ///
  /// <param name="entry">The message to log</param>
  public static void Secure(string entry) {
  }

  /// <summary>Logs a message at the warning log level</summary>
  ///
  /// <param name="entry">The message to log</param>
  public static void Warning(string entry) {
  }

  /// <summary>Logs a message at the info log level</summary>
  ///
  /// <param name="entry">The message to log</param>
  public static void Info(string entry) {
  }

  /// <summary>Logs a message at the verbose log level</summary>
  ///
  /// <param name="entry">The message to log</param>
  public static void Verbose(string entry) {
  }

  /// <summary>Logs a message at the debug log level</summary>
  ///
  /// <param name="entry">The message to log</param>
  public static void Debug(string entry) {
  }

  private Log() {
  }

}

}
