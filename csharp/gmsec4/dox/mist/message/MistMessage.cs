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
/// This specialized Message class that shares all of the functionality of standard messages,
/// but can also be auto-populated based on how the message schema identifies itself within
/// the given specification.
/// </summary>
///
/// <seealso cref="Message"/>
/// <seealso cref="Config"/>
/// <seealso cref="Specification"/>

public class MistMessage : Message {

  /// <summary>Initializes the message instance with a template determined by ID and spec.</summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="schemaID">
  /// The string used to identify the message schema in C2MS or other message specification
  /// (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
  /// Not all schemaIDs contain a messageSubtype
  /// </param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the subject is null or contains an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the schemaID is null or contains an empty-string.</exception>
  /// <exception cref="GmsecException">Throw if the schemaID is not a valid ID.</exception>
  /// <exception cref="GmsecException">Thrown if the Specification is null.</exception>
  public MistMessage(string subject, string schemaID, Specification spec) {
  }

  /// <summary>Initializes the message instance with a template determined by ID and spec.</summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="schemaID">
  /// The string used to identify the message schema in C2MS or other message specification
  /// (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
  /// </param>
  /// <param name="config">A configuration to associate with the message.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the subject is null or contains an empty-string.</exception>
  /// <exception cref="GmsecException">Thrown if the schemaID is null or contains an empty-string.</exception>
  /// <exception cref="GmsecException">Throw if the schemaID is not a valid ID.</exception>
  /// <exception cref="GmsecException">Thrown if the Specification is null.</exception>
  public MistMessage(string subject, string schemaID, Config config, Specification spec) {
  }

  /// <summary>Copy constructor - initializes the message instance using the other given MistMessage</summary>
  ///
  /// <param name="other">The message to be copied.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given MistMessage is null.</exception>
  public MistMessage(MistMessage other) {
  }

  /// <summary>Special constructor that constructs a MistMessage using a simple Message</summary>
  ///
  /// <param name="msg">The simple Message to reference while building the MistMessage</param>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the given Message object is null, or if the given Message does not contain
  /// sufficient information to construct a MistMessage.
  /// </exception>
  public MistMessage(Message msg) {
  }

  /// <summary>Special constructor that constructs a MistMessage using a simple Message</summary>
  ///
  /// <param name="msg">The simple Message to reference while building the MistMessage</param>
  /// <param name="specConfig">The specification configuration to apply when constructing the MistMessage</param>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the given Message object is null, or if the given Message does not contain
  /// sufficient information to construct a MistMessage.
  /// </exception>
  public MistMessage(Message msg, Config specConfig) {
  }

  /// <summary>Constructs a MistMessage using an XML or JSON string.</summary>
  ///
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GMSEC_Exception">
  /// Thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
  /// </exception>
  ///
  /// <remarks>This method has been deprecated; use MistMessage(Specification, string) instead.</remarks>
  public MistMessage(string data) {
  }

  /// <summary>Constructs a MistMessage using an XML or JSON string, and ensures the message adheres to the provided specification</summary>
  ///
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GMSEC_Exception">
  /// Thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
  /// </exception>
  public MistMessage(Specification spec, string data) {
  }

  /// <summary>
  /// Sets the internal list of fields which are to be automatically placed in all
  /// MistMessage objects that are created.  Internal copies of the Fields are made, and
  /// thus ownership of the fields that are provided are not retained by MistMessage.
  /// </summary>
  ///
  /// <param name="standardFields">The list of fields to set as standard fields</param>
  public static void SetStandardFields(FieldList standardFields) {
  }

  /// <summary>Destroys the lists of standard fields that are included with MistMessage objects.</summary>
  public static void ClearStandardFields() {
  }

  /// <summary>Returns a string that identifies the schema that the Message is based off of.</summary>
  public string GetSchemaID() {
  }

  /// <summary>
  /// Sets the value for a field indicated by user. The value's type is automatically
  /// determined by the function depending on the message schema being used. If the field 
  /// does not have a required type, a new StringField will be created with the specified name 
  /// and value.
  /// </summary>
  ///
  /// <param name="fieldName">Name of the field to be modified/created</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL or contains an empty string.</exception>
  /// <exception cref="GmsecException">
  /// Thrown if the input value's type cannot be converted into the type required
  /// by the field, or if the string is too big when converting to char.
  /// </exception>
  public void SetValue(string fieldName, string value) {
  }

  /// <summary>
  /// Sets the value for a field indicated by user. The value's type is automatically
  /// determined by the function depending on the message schema being used. If the field 
  /// does not have a required type, a new I64Field will be created with the specified name 
  /// and value.
  /// </summary>
  ///
  /// <param name="fieldName">Name of the field to be modified/created</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL or contains an empty string.</exception>
  /// <exception cref="GmsecException">
  /// Thrown if the input value's type cannot be converted into the type required by the field.
  /// </exception>
  public void SetValue(string fieldName, long value) {
  }

  /// <summary>
  /// Sets the value for a field indicated by user. The value's type is automatically
  /// determined by the function depending on the message schema being used. If the field 
  /// does not have a required type, a new F64Field will be created with the specified name 
  /// and value.
  /// </summary>
  ///
  /// <param name="fieldName">Name of the field to be modified/created</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL or contains an empty string.</exception>
  /// <exception cref="GmsecException">
  /// Thrown if the input value's type cannot be converted into the type required by the field.
  /// </exception>
  public void SetValue(string fieldName, double value) {
  }

}

}
