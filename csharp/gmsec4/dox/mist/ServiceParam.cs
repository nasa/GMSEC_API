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
/// This class is a lightweight container for holding information on a
/// Simple Service Parameter, and used to generate a %GMSEC Simple Service
/// %Messages by the ConnectionManager class.
/// </summary>

public class ServiceParam : global::System.IDisposable {

  /// <summary>Constructor for initializing the ServiceParam object with a sampled value.</summary>
  ///
  /// <param name="name">The name of the service parameter</param>
  /// <param name="value">The value of the parameter</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string</exception>
  public ServiceParam(string name, Field value) {
  }

  /// <summary>Copy-constructor that can be used to make a deep-copy of another service parameter object.</summary>
  ///
  /// <param name="other">The service parameter object to copy</param>
  public ServiceParam(ServiceParam other) {
  }

  /// <summary>Returns the name of the service parameter.</summary>
  public string GetName() {
  }

  /// <summary>Returns the parameter value.</summary>
  public Field GetValue() {
  }

}

}
