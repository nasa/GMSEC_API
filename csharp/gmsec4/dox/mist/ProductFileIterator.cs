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
/// This class can be used to iterate over the ProductFile object(s) contained within a ProductFileMessage.
/// </summary>
///
/// <seealso cref="ProductFile"/>
/// <seealso cref="ProductFileMessage"/>

public class ProductFileIterator : global::System.IDisposable {

  /// <summary>
  /// Provides indication as to whether there are additional ProductFile objects that can be referenced using Next().
  /// </summary>
  ///
  /// <seealso cref="Next"/>
  /// <seealso cref="Reset"/>
  public bool HasNext() {
  }

  /// <summary>Returns a reference to the next available ProductFile object.</summary>
  ///
  /// <seealso cref="HasNext"/>
  /// <seealso cref="Reset"/>
  ///
  /// <exception cref="GmsecException">Thrown if the iterator has reached the end (i.e. there are no more ProductFiles.)</exception>
  public ProductFile Next() {
  }

  /// <summary>Resets the iterator to the beginning of the ProductFile list that is maintained by the ProductFileMessage.</summary>
  public void Reset() {
  }

}

}
