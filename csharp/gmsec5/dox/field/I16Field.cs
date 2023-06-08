/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>
/// Specialized Field class that can be used to store a signed 16-bit value.
/// </summary>

public class I16Field : Field {

  /// <summary>Creates a I16Field object with the given value</summary>
  ///
  /// <param name="name">The name of the field</param>
  /// <param name="value">The value to store in the field</param>
  /// <param name="isHeader">used to indicate if Field is a header field (default is false)</param>
  ///
  /// <exception cref="GmsecException">If the name is null or contains an empty-string</exception>
  public I16Field(string name, short value, bool isHeader = false) {
  }

  /// <summary>Copy constructor</summary>
  /// <param name="other">The other I16Field object to copy</param>
  public I16Field(I16Field other) {
  }

  /// <summary>Returns the value associated with the field</summary>
  ///
  /// <returns>The value associated with the field</returns>
  public short GetValue() {
  }

}

}
