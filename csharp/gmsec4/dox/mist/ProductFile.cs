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
/// This class is a lightweight container for holding information on a Product File,
/// and used to generate a %GMSEC ProductFileMessage.
/// </summary>


public class ProductFile : global::System.IDisposable {

  /// <summary>Constructor for initializing the %ProductFile object with a URI to describe a commonly accessible resource.</summary>
  ///
  /// <param name="name">The name of the device parameter</param>
  /// <param name="description">The description of the file</param>
  /// <param name="version">The version of the file</param>
  /// <param name="format">The description of the file's format</param>
  /// <param name="uri">The URI for the file</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the description is null or contains an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the version is null or contains an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the format is null or contains an empty-string</exception>
  public ProductFile(string name, string description, string version, string format, string uri) {
  }

  /// <summary>Constructor for initializing the %ProductFile object with the binary contents of a file.</summary>
  ///
  /// <param name="name">The name of the device parameter</param>
  /// <param name="description">The description of the file</param>
  /// <param name="version">The version of the file</param>
  /// <param name="format">The description of the file's format</param>
  /// <param name="data">The binary contents of the file</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the description is null or contains an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the version is null or contains an empty-string</exception>
  /// <exception cref="GmsecException">Thrown if the format is null or contains an empty-string</exception>
  public ProductFile(string name, string description, string version, string format, byte[] data) {
  }

  /// <summary>Copy-constructor that can be used to make a deep-copy of another product file object.</summary>
  ///
  /// <param name="other">The product file object to copy</param>
  public ProductFile(ProductFile other) {
  }

  /// <summary>Returns the name of the product file.</summary>
  public string GetName() {
  }

  /// <summary>Returns the description of the product file.</summary>
  public string GetDescription() {
  }

  /// <summary>Returns the version of the product file.</summary>
  public string GetVersion() {
  }

  /// <summary>Returns the format of the product file.</summary>
  public string GetFormat() {
  }

  /// <summary>Returns whether a URI has been set for the product file.</summary>
  public bool URIAvailable() {
  }

  /// <summary>Returns the URI, if any, that has been set for the product file.</summary>
  public string GetURI() {
  }

  /// <summary>Returns whether the contents for the product file are available.</summary>
  public bool ContentsAvailable() {
  }

  /// <summary>Returns the contents, if available, for the product file.</summary>
  public byte[] GetContents() {
  }

}

}
