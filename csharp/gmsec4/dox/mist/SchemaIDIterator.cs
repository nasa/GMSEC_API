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
/// This class supports the iteration of Specification's loaded schema templates.
/// </summary>
///
/// <seealso cref="Specification.GetSchemaIDIterator"/>
///

public class SchemaIDIterator : global::System.IDisposable {

  /// <summary>
  /// Provides information as to whether there are additional Schema IDs that can be
  /// referenced using Next().
  /// </summary>
  ///
  /// <seealso cref="Next"/>
  /// <seealso cref="Reset"/>
  public bool HasNext() {
  }

  /// <summary>Returns a reference to the next available Schema ID.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if the iterator has reached the end (i.e. there are no more IDs).</exception>
  ///
  /// <seealso cref="HasNext"/>
  public string Next() {
  }

  /// <summary>Resets the interator to the beginning of the Schema list that is maintained by Specification.</summary>
  public void Reset() {
  }

}

}
