/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>
/// Specialized Field class that can be used to store an unsigned 64-bit value.
/// </summary>

public class U64Field : Field {

  /// <summary>Creates a U64Field object with the given value</summary>
  ///
  /// <param name="name">The name of the field</param>
  /// <param name="value">The value to store in the field</param>
  /// <param name="isHeader">Use to indicate if the field is a header field (default is false)</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or otherwise if the field name is non-compliant.</exception>
  public U64Field(string name, ulong value, bool isHeader = false) {
  }

  /// <summary>Copy constructor</summary>
  /// <param name="other">The other U64Field object to copy</param>
  public U64Field(U64Field other) {
  }

  /// <summary>Returns the value associated with the field</summary>
  ///
  /// <returns>The value associated with the field</returns>
  public ulong GetValue() {
  }

}

}
