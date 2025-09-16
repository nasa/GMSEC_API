/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>
/// The %Specification class loads a list of templates from a designated directory
/// that are used to create and validate message schemas. Each template has an associated ID
/// that is used to identify the kind of message schema the template will be defining.
/// </summary>
///
/// <seealso cref="Config"/>
/// <seealso cref="MessageSpecification"/>
/// <seealso cref="ShemaIDIterator"/>
///

public class Specification : global::System.IDisposable {

  /// <summary>Initializes the %Specification with the default message specification (NASA/GMSEC Addendum).</summary>
  ///
  /// <exception cref="GmsecException">Thrown if an error occurs loading the message specification.</exception>
  public Specification() {
  }

  /// <summary>Initializes the %Specification with the given configuration.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if expected values from the configuration are missing.</exception>
  /// <exception cref="GmsecException">Thrown if an error occurs loading the message specification.</exception>
  ///
  /// <seealso cref="Config"/>
  public Specification(Config config) {
  }

  /// <summary>Copy constructor - initializes the %Specification instance based on another instance.</summary>
  ///
  /// <param name="other">The %Specification object to be copied.</param>
  public Specification(Specification other) {
  }

  /// <summary>Returns a SchemaIDIterator which can be used to iterate over the IDs of the loaded templates.</summary>
  ///
  /// <remarks>
  /// Only one SchemaIDIterator object is associated with a %Specification object; multiple calls to 
  /// getSchemaIDIterator() will return a reference to the same SchemaIDIterator object. Each call
  /// will reset the iterator.
  /// </remarks>
  ///
  /// <returns>A reference to a SchemaIDIterator object</returns>
  public SchemaIDIterator GetSchemaIDIterator() {
  }

  /// <summary>Returns the version of the message specification that is referenced by the Specification.</summary>
  public uint GetVersion() {
  }

  /// <summary>Returns the schema level of the message specification in use.</summary>
  public Specification.SchemaLevel GetSchemaLevel() {
  }

  /// <summary>Returns the validation level for the specification.</summary>
  public uint GetValidationLevel() {
  }

  /// <summary>
  /// Accessor for acquiring the list of MessageSpecification objects associated with the %Specification.
  /// </summary>
  public MessageSpecificationList GetMessageSpecifications() {
  }

  /// <summary>
  /// Returns a list of the field names for the default header (which is the topmost schema level currently configured)
  /// </summary>
  public HeaderList GetHeaderFieldNames() {
  }

  /// <summary>
  /// Returns a list of the field names for the default header (which is the topmost schema level currently configured)
  /// </summary>
  /// <param name="schemaName">The name of the specified schema level (e.g. C2MS, C2MS-PIPE, GMSEC, DEFAULT).  An empty string refers to DEFAULT.</param>
  public HeaderList GetHeaderFieldNames(string schemaName) {
  }

  /// <summary>Schema Levels for representing message specification.</summary>
  public enum SchemaLevel
  {
    /// <summary>C2MS.</summary>
    LEVEL_0,

    /// <summary>C2MS Extensions, or optionally user-defined.</summary>
    LEVEL_1,

    /// <summary>NASA/GMSEC Addendum, or optionally user-defined.</summary>
    LEVEL_2,

    /// <summary>User-defined.</summary>
    LEVEL_3,

    /// <summary>User-defined.</summary>
    LEVEL_4,

    /// <summary>User-defined.</summary>
    LEVEL_5,

    /// <summary>User-defined.</summary>
    LEVEL_6
  }

}

}
