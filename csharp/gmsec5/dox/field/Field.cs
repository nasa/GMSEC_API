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
/// Base class for all field types.
/// </summary>

public class Field : global::System.IDisposable {

  /// <summary>Returns the name of the field object</summary>
  /// <returns>A string containing the field name</returns>
  public string GetName() {
  }

  /// <summary>Returns the type of the field object</summary>
  /// <returns>The field type</returns>
  public Field.Type GetFieldType() {
  }

  /// <summary>Returns whether the field object is considered a header field</summary>
  /// <returns>True if a header field; false otherwise</returns>
  public bool IsHeader() {
  }

  /// <summary>Returns whether the field object is considered a GMSEC API tracking field</summary>
  /// <returns>True if a tracking field; false otherwise</returns>
  public bool IsTracking() {
  }

  /// <summary>Converts the field object into an XML string representation</summary>
  /// <returns>An XML string</returns>
  public virtual string ToXML() {
  }

  /// <summary>Converts the field object into a JSON string representation</summary>
  /// <returns>A JSON string</returns>
  public virtual string ToJSON() {
  }

  /// <summary>Attempts to convert the field value into string representation.</summary>
  /// <returns>Returns the field value as a string.</returns>
  public string GetStringValue() {
  }

  /// <summary>Attempts to convert the field value into a boolean representation.</summary>
  /// <returns>Returns the field value as a boolean.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to a boolean.</exception>
  public bool GetBooleanValue() {
  }

  /// <summary>Attempts to convert the field value into a signed 16-bit integer number representation.</summary>
  /// <returns>Returns the field value as a signed 16-bit integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to an integer.</exception>
  public short GetI16Value() {
  }

  /// <summary>Attempts to convert the field value into a signed 32-bit integer number representation.</summary>
  /// <returns>Returns the field value as a signed 32-bit integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to an integer.</exception>
  public long GetI32Value() {
  }

  /// <summary>Attempts to convert the field value into a signed 64-bit integer number representation.</summary>
  /// <returns>Returns the field value as a signed 64-bit integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to an integer.</exception>
  public long GetI64Value() {
  }

  /// <summary>Attempts to convert the field value into an unsigned 16-bit integer number representation.</summary>
  /// <returns>Returns the field value as an unsigned 16-integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to an integer.</exception>
  public ushort GetU16Value() {
  }

  /// <summary>Attempts to convert the field value into an unsigned 32-bit integer number representation.</summary>
  /// <returns>Returns the field value as an unsigned 32-integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to an integer.</exception>
  public uint GetU32Value() {
  }

  /// <summary>Attempts to convert the field value into an unsigned 64-bit integer number representation.</summary>
  /// <returns>Returns the field value as an unsigned 64-integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to an integer.</exception>
  public ulong GetU64Value() {
  }

  /// <summary>Attempts to convert the field value into a 64-bit floating point number representation.</summary>
  /// <returns>Returns the field value as a floating point number.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot successfully be converted to a double.</exception>
  public double GetF64Value() {
  }

  /// <summary>Enumeration of possible field types.</summary>
  public enum Type {
    /// <summary>Field containing a character value.</summary>
    CHAR = 1,

    /// <summary>Field containing a boolean value.</summary>
    BOOL = 2,

    /// <summary>Field containing a signed 16-bit value.</summary>
    I16 = 3,

    /// <summary>Field containing an unsigned 16-bit value.</summary>
    U16 = 4,

    /// <summary>Field containing a signed 32-bit value.</summary>
    I32 = 5,

    /// <summary>Field containing an unsigned 32-bit value.</summary>
    U32 = 6,

    /// <summary>Field containing a 32-bit float value.</summary>
    F32 = 7,

    /// <summary>Field containing a 64-bit float value.</summary>
    F64 = 8,

    /// <summary>Field containing a string value.</summary>
    STRING = 9,

    /// <summary>Field containing raw data.</summary>
    BINARY = 10,

    /// <summary>Field containing a signed 8-bit value.</summary>
    I8 = 20,

    /// <summary>Field containing an unsigned 8-bit value.</summary>
    U8 = 21,

    /// <summary>Field containing a signed 64-bit value.</summary>
    I64 = 22,

    /// <summary>Field containing an unsigned 64-bit value.</summary>
    U64 = 23
  }

}

}
