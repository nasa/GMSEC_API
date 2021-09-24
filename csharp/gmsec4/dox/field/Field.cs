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
/// Base class for all field types.
/// </summary>

public class Field : global::System.IDisposable {

  /// <summary>Returns the name of the field object</summary>
  /// <returns>A string containing the field name</returns>
  public string GetName() {
  }

  /// <summary>Sets the name of the Field object</summary>
  /// <param name="name">The name of the Field object.</param>
  /// <exception cref="GmsecException">An exception is thrown if the name is null or contains an empty string.</exception>
  public void SetName(string name) {
  }

  /// <summary>Returns the type of the field object</summary>
  /// <returns>The field type</returns>
  public Field.FieldType GetFieldType() {
  }

  /// <summary>Returns whether the field object is considered a header field</summary>
  /// <returns>True if a header field; false otherwise</returns>
  public bool IsHeader() {
  }

  /// <summary>Can be used to indicate whether the Field object is a header field</summary>
  /// <param name="header">True if a header field; false otherwise</param>
  public void IsHeader(bool header) {
  }

  /// <summary>Converts the field object into an XML string representation</summary>
  /// <returns>An XML string</returns>
  public virtual string ToXML() {
  }

  /// <summary>Converts the field object into a JSON string representation</summary>
  /// <returns>A JSON string</returns>
  public virtual string ToJSON() {
  }

  /// <summary>Attempts to convert the field value into a signed 64-bit integer number representation.</summary>
  /// <returns>Returns the field value as a signed integer.</returns>
  /// <exception cref="GmsecException">An exception is thrown if the field cannot be successfully converted to an integer.</exception>
  public long GetIntegerValue() {
  }

  /// <summary>Attempts to convert the field value into an unsigned 64-bit integer number representation.</summary>
  /// <returns>Returns the field value as an unsigned integer.</returns>
  /// <exception cref="GmsecException">An exception is thrown if the field cannot be successfully converted to an integer.</exception>
  public ulong GetUnsignedIntegerValue() {
  }

  /// <summary>Attempts to convert the field value into a 64-bit floating point number representation.</summary>
  /// <returns>Returns the field value as a floating point number.</returns>
  /// <exception cref="GmsecException">An exception is thrown if the field cannot be successfully converted to a double.</exception>
  public double GetDoubleValue() {
  }

  /// <summary>Attempts to convert the field value into string representation.</summary>
  /// <returns>Returns the field value as a string.</returns>
  /// <exception cref="GmsecException">An exception is thrown if the field cannot be successfully converted to a string.</exception>
  public string GetStringValue() {
  }

  /// <summary>Creates/returns a clone of the Field object.</summary>
  /// <returns>A clone of the Field object.</returns>
  public Field Clone() {
  }

  /// <summary>Destroys the cloned Field object.</summary>
  /// <param name="clone">The cloned Field object to destroy.</param>
  public void DestroyClone(Field clone) {
  }

  /// <summary>Enumeration of possible field types.</summary>
  public enum FieldType {
    /// <summary>Field containing a character value.</summary>
    CHAR_TYPE = 1,

    /// <summary>Field containing a boolean value.</summary>
    BOOL_TYPE = 2,

    /// <summary>Field containing a signed 16-bit value.</summary>
    I16_TYPE = 3,

    /// <summary>Field containing an unsigned 16-bit value.</summary>
    U16_TYPE = 4,

    /// <summary>Field containing a signed 32-bit value.</summary>
    I32_TYPE = 5,

    /// <summary>Field containing an unsigned 32-bit value.</summary>
    U32_TYPE = 6,

    /// <summary>Field containing a 32-bit float value.</summary>
    F32_TYPE = 7,

    /// <summary>Field containing a 64-bit float value.</summary>
    F64_TYPE = 8,

    /// <summary>Field containing a string value.</summary>
    STRING_TYPE = 9,

    /// <summary>Field containing raw data.</summary>
    BIN_TYPE = 10,

    /// <summary>Field containing a signed 8-bit value.</summary>
    I8_TYPE = 20,

    /// <summary>Field containing an unsigned 8-bit value.</summary>
    U8_TYPE = 21,

    /// <summary>Field containing a signed 64-bit value.</summary>
    I64_TYPE = 22,

    /// <summary>Field containing an unsigned 64-bit value.</summary>
    U64_TYPE = 23
  }

}

}
