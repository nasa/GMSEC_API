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
/// This class is a lightweight container for holding information on a %DeviceParam,
/// and is used to generate a %GMSEC DeviceMessage which contains one or more Device objects.
/// </summary>

public class DeviceParam : global::System.IDisposable {

  /// <summary>Constructor for initializing the %DeviceParam object with a sampled value.</summary>
  ///
  /// <param name="name">The name of the device parameter</param>
  /// <param name="timestamp">The timestamp of when the sampled value was collected</param>
  /// <param name="value">The sampled value</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string"</exception>
  /// <exception cref="GmsecException">Thrown if the timestamp is null or contains an empty-string"</exception>
  public DeviceParam(string name, string timestamp, Field value) {
  }

  /// <summary>Copy-constructor that can be used to make a deep-copy of another %DeviceParam object.</summary>
  ///
  /// <param name="other">The %DeviceParam object to copy</param>
  public DeviceParam(DeviceParam other) {
  }

  /// <summary>Returns the name of the device parameter.</summary>
  public string GetName() {
  }

  /// <summary>Returns the timestamp of when the device parameter sample was taken.</summary>
  public string GetTimestamp() {
  }

  /// <summary>Returns the sample value.</summary>
  public Field GetValue() {
  }

}

}
