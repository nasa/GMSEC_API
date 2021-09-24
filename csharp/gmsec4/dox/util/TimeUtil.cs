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
/// Contains the methods that support the acquisition of current
/// system time, and the ability to format such into a GMSEC-style time
/// string.
/// </summary>

public class TimeUtil : global::System.IDisposable {

  /// <summary>Sleeps for the specified number of milliseconds (at least 1).</summary>
  ///
  /// <param name="milliseconds">The amount of time in milliseconds to sleep</param>
  public static void Millisleep(uint milliseconds) {
  }

  /// <summary>Returns the current time in seconds since Jan 1, 1970.</summary>
  public static GMSEC_TimeSpec GetCurrentTime() {
  }

  /// <summary>
  ///  Returns the current time in (real) seconds since Jan 1, 1970.
  /// <p>
  /// If the GMSEC_TimeSpec is non-null, it will be populated.
  /// </summary>
  ///
  /// <param name="ts">The GMSEC_TimeSpec object, if any, to populate</param>
  ///
  /// <returns>The current time as a double value</returns>
  public static double GetCurrentTime_s(GMSEC_TimeSpec ts) {
  }

  /// <summary>Returns the current time in (real) seconds since Jan 1, 1970.</summary>
  ///
  /// <returns>The current time as a double value</returns>
  public static double GetCurrentTime_s() {
  }

  /// <summary>Returns the current time in seconds since Jan 1, 1970 as represented by the %GMSEC String.</summary>
  ///
  /// <param name="timeString">The GMSEC-style time string to convert</param>
  ///
  /// <returns>A GMSEC_TimeSpec object that contains the converted time</returns>
  public static GMSEC_TimeSpec GetCharTime(string timeString) {
  }

  /// <summary>Returns the current time in seconds since Jan 1, 1970 as represented by the %GMSEC String.</summary>
  ///
  /// <param name="timeString">The GMSEC-style time string to convert</param>
  ///
  /// <returns>The time as a double value</returns>
  public static double GetCharTime_s(string timeString) {
  }

  /// <summary>Computes the time-delta between the two given times.</summary>
  ///
  /// <param name="time1">The end time</param>
  /// <param name="time0">The start time</param>
  ///
  /// <returns>A GMSEC_TimeSpec object that contains the time-delta between the two given times</returns>
  public static GMSEC_TimeSpec DiffTime(GMSEC_TimeSpec time1, GMSEC_TimeSpec time0) {
  }

  /// <summary>Formats the given GMSEC_TimeSpec into a GMSEC-style time string with the format of YYYY-DDD-HH-MM-SS.sss</summary>
  ///
  /// <param name="ts">The GMSEC_TimeSpec containing the time to represent as a string</param>
  ///
  /// <returns>A GMSEC-style formatted time string</returns>
  public static string FormatTime(GMSEC_TimeSpec ts) {
  }

  /// <summary>Formats the given GMSEC_TimeSpec into a GMSEC-style time string with the format of YYYY-DDD-HH-MM-SS[.s*]</summary>
  ///
  /// <param name="ts">The GMSEC_TimeSpec containing the time to represent as a string</param>
  /// <param name="subs">The number of digits to allow for subseconds; range is 0 through 9.</param>
  ///
  /// <returns>A GMSEC-style formatted time string</returns>
  public static string FormatTime(GMSEC_TimeSpec ts, int subs) {
  }

  private TimeUtil() {
  }

}

}
