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
/// The MessageFieldIterator class can be used to iterate over the Fields contained within a Message.
///
/// <seealso cref="Message"/>
/// </summary>

public class MessageFieldIterator : global::System.IDisposable {

  /// <summary>Provides information as to whether there are additional fields that can be referenced using next().</summary>
  public bool HasNext() {
  }

  /// <summary>Returns a reference to the next available Field object.</summary>
  public Field Next() {
  }

  /// <summary>Resets the interator to the beginning of the Field list that is maintained by the Message.</summary>
  public void Reset() {
  }

  /// <summary>The type of MessageFieldIterator to set up.</summary>
  ///
  /// <seealso cref="Message.GetFieldIterator(Selector)"/>
  public enum Selector {

    /// <summary>Used for iterating over all available Fields in a Message.</summary>
    ALL_FIELDS,

    /// <summary>Used for iterating only over header Fields.</summary>
    HEADER_FIELDS,

    /// <summary>Used for iterating only over non-header Fields.</summary>
    NON_HEADER_FIELDS
  }

}

}
