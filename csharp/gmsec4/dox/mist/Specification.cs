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
/// The %Specification class loads a list of templates from a designated directory
/// that are used to create and validate message schemas. Each template has an associated ID
/// that is used to identify the kind of message schema the template will be defining.
/// </summary>
///
/// <seealso cref="Config"/>
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

  /// <summary>
  /// Looks up the message subject in the message registry to grab the
  /// appropriate template (based on its assigned schema ID). The contents of the 
  /// message are then compared to the template to ensure the message complies with 
  /// the ISD. This method will also register the message with an appropriate schema 
  /// ID if it is not found in the registry.
  /// </summary>
  ///
  /// <param name="msg">The message to be validated.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the message fails to pass validation.</exception>
  ///
  /// <seealso cref="Message"/>
  /// <seealso cref="MistMessage"/>
  public virtual void ValidateMessage(Message msg) {
  }

  /// <summary>Returns a SchemaIDIterator, which can be used to iterate over the IDs of the loaded templates.</summary>
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

  /// <summary>Returns the version of the ISD being used as an unsigned integer.</summary>
  public uint GetVersion() {
  }

  /// <summary>Returns the schema level of the message specification in use.</summary>
  public Specification.SchemaLevel GetSchemaLevel() {
  }

  /// <summary>
  /// Accessor for acquiring the list of MessageSpecification objects associated with the %Specification.
  /// </summary>
  public MessageSpecificationList GetMessageSpecifications() {
  }

  /// <summary>
  /// Registers the given MessageValidator to be used when message validation takes place.
  /// </summary>
  ///
  /// <param name="validator">The custom MessageValidator to validate messages.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the MessageValidator is null.</exception>
  ///
  /// <seealso cref="MessageValidator"/>
  /// <seealso cref="ConnectionManager.RegisterMessageValidator"/>
  public void RegisterMessageValidator(MessageValidator validator) {
  }
}

}
