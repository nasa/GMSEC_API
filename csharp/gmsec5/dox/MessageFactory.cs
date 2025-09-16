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

/// <summary>This class provides the public interface for the MessageFactory.</summary>

public class MessageFactory : global::System.IDisposable {

  /// <summary>Basic constructor that initializes the MessageFactory to produce messages based on the latest available message specification.</summary>
  public MessageFactory() {
  }

  /// <summary>Constructor that initializes the MessageFactory to produce messages based on the information contained within the configuration object (e.g. gmsec-message-specification=201900).</summary>
  ///
  /// <param name="config">A configuration object</param>
  public MessageFactory(Config config) {
  }

  /// <summary>Allows for the setting of common (standard) fields that should be applied to all messages created by the MessageFactory.</summary>
  ///
  /// <param name="standardFields">The list of common/standard fields to apply to all messages.</param>
  public void SetStandardFields(FieldList standardFields) {
  }

  /// <summary>Removes any common (standard) fields that have previously been associated with the MessageFactory.</summary>
  public void ClearStandardFields() {
  }

  /// <summary>Allows for the setting of a common message configuration that will be applied to all messages created by the MessageFactory.</summary>
  ///
  /// <param name="msgConfig">The message configuration object.</param>
  public void SetMessageConfig(Config msgConfig) {
  }

  /// <summary>
  /// Creates and returns a Message object.
  /// <p>
  /// The Message will contain any user-supplied message attributes (such as a message configuration, standard fields, and custom message validator).
  /// </p>
  /// </summary>
  ///
  /// <seealso cref="MessageFactory.SetMessageConfig"/>
  /// <seealso cref="MessageFactory.SetStandardFields"/>
  /// <seealso cref="MessageFactory.RegisterMessageValidator"/>
  public Message CreateMessage() {
  }

  /// <summary>
  /// Creates and returns a Message object that is populated with the fields derived from the working message specification and schema ID.
  /// <p>
  /// In addtion, the Message will contain any user-supplied message attributes (such as a message configuration, standard fields, and custom message validator).
  /// </p>
  /// </summary>
  ///
  /// <param name="schemaID">The string identifying which message to create (e.g. "HB").</param>
  ///
  /// <exception cref="GmsecException">Thrown if the schemaID string is NULL, contains an empty string, or references an unknown/illegal schema ID.</exception>
  ///
  /// <seealso cref="MessageFactory.SetMessageConfig"/>
  /// <seealso cref="MessageFactory.SetStandardFields"/>
  /// <seealso cref="MessageFactory.RegisterMessageValidator"/>
  public Message CreateMessage(string schemaID) {
  }

  /// <summary>Creates and returns a Message object based on the given XML or JSON data.</summary>
  ///
  /// <param name="data">An XML or JSON data string that represents a GMSEC-style message.</param>
  /// <param name="type">the type of data being provided</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given data string is NULL, or does not represent a GMSEC-style message in either XML or JSON format.</exception>
  public Message FromData(string data, DataType type) {
  }

  /// <summary>Returns a reference to the Specification object associated with the MessageFactory.</summary>
  public Specification GetSpecification() {
  }

  /// <summary>Registers the given custom MessageValidator with each message created using the MessageFactory.</summary>
  ///
  /// <param name="validator">A custom MessageValidator object.</param>
  ///
  /// <seealso cref="MessageValidator"/>
  public void RegisterMessageValidator(MessageValidator validator) {
  }

}

}
