/*
 * Copyright 2007-2020 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>
/// Specialized Field class that can be used to store a signed 8-bit value.
/// </summary>

public class I8Field : Field {

  /// <summary>Creates a I8Field object with the given value</summary>
  ///
  /// <param name="name">The name of the field</param>
  /// <param name="value">The value to store in the field</param>
  ///
  /// <exception cref="GmsecException">If the name is null or contains an empty-string</exception>
  public I8Field(string name, sbyte value) {
  }

  /// <summary>Copy constructor</summary>
  /// <param name="other">The other I8Field object to copy</param>
  public I8Field(I8Field other) {
  }

  /// <summary>Returns the value associated with the field</summary>
  ///
  /// <returns>The value associated with the field</returns>
  public sbyte GetValue() {
  }

}

}
