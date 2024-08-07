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
/// Specialized Field class that can be used to store a raw data value.
/// </summary>

public class BinaryField : Field {

  /// <summary>Creates a BinaryField object with the given data</summary>
  ///
  /// <param name="name">The name of the field</param>
  /// <param name="data">Array of raw data</param>
  /// <param name="isHeader">Use to indicate if the field is a header field (default is false)</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or otherwise if the field name is non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the data array is null</exception>
  public BinaryField(string name, byte[] data, bool isHeader = false) {
  }

  /// <summary>Copy constructor.</summary>
  ///
  /// <param name="other">The other BinaryField object to copy</param>
  public BinaryField(BinaryField other) {
  }

  /// <summary>Returns the length of the data associated with the field</summary>
  ///
  /// <returns>The length of the data associated with the field</returns>
  public int GetLength() {
  }

  /// <summary>Returns the data associated with the field.</summary>
  ///
  /// <returns>The data associated with the field.</returns>
  public byte[] GetValue() {
}

}

}
