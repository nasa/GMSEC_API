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
/// The Message class serves a container for %GMSEC Messages.
/// The methods of this class allow for the construction and manipulation of the data in a message.
///
/// <seealso cref="MistMessage"/>
/// </summary>

public class Message : global::System.IDisposable {

  /// <summary>Basic message Constructor</summary>
  ///
  /// <param name="subject">The subject/topic to associate with the message</param>
  /// <param name="kind">The message kind to associate with the message</param>
  ///
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string</exception>
  public Message(string subject, Message.MessageKind kind) {
  }

  /// <summary>
  /// %Message constructor that accepts a configuration.
  /// <p>
  /// Only Message-related configuration options are held by the Message object.
  /// These configuration options are NOT included with the message when it is sent
  /// across the %GMSEC Bus.
  /// </summary>
  ///
  /// <param name="subject">The subject/topic to associate with the message</param>
  /// <param name="kind">The message kind to associate with the message</param>
  /// <param name="config">A configuration object that contains message-specific options</param>
  ///
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string</exception>
  public Message(string subject, Message.MessageKind kind, Config config) {
  }

  /// <summary>Specialized constructor that initializes the message instance using either
  /// XML or JSON text</summary>
  ///
  /// <param name="data">An XML or JSON formatted string</param>
  ///
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string</exception>
  public Message(string data) {
  }

  /// <summary>Basic (and safe) deep copy constructor</summary>
  ///
  /// <param name="other">Message object to copy</param>
  public Message(Message other) {
  }

  /// <summary>
  /// Associatates a configuration object with the message.
  /// <p>
  /// Only Message-related configuration options are held by the Message object.
  /// These configuration options are NOT included with the message when it is sent
  /// across the %GMSEC Bus.
  /// </summary>
  ///
  /// <param name="config">The configuration object</param>
  /// <exception cref="GmsecException">If the Config object is null</exception>
  public void AddConfig(Config config) {
  }

  /// <summary>Returns configuration associcated with the message</summary>
  public Config GetConfig() {
  }

  /// <summary>Returns message subject</summary>
  public string GetSubject() {
  }

  /// <summary>Allows for the setting/changing of the message subject</summary>
  ///
  /// <param name="subject">The subject/topic to associate with the message</param>
  ///
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string</exception>
  public void SetSubject(string subject) {
  }

  /// <summary>Returns message kind</summary>
  public Message.MessageKind GetKind() {
  }

  /// <summary>Adds a copy of the field object to the message</summary>
  /// <param name="field">The field object</param>
  /// <returns>True if a new field was added; false if an existing field was replaced.</returns>
  /// <exception cref="GmsecException">If the Field object is null</exception>
  public bool AddField(Field field) {
  }

  /// <summary>Adds each Field in the given list to the Message.</summary>
  /// <param name="fields">The list of field objects</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">If the Field list is null</exception>
  public bool AddFields(FieldList fields) {
  }


  /// <summary>Adds a BinaryField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="bin">Byte array of data</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  /// <exception cref="GmsecException">Throw if the byte arrray is null.</exception>
  public bool AddField(string name, byte[] bin) {
  }

  /// <summary>Adds a BooleanField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, bool value) {
  }

  /// <summary>Adds a CharField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, char value) {
  }

  /// <summary>Adds an F32Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, float value) {
  }

  /// <summary>Adds an F64Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, double value) {
  }

  /// <summary>Adds an I8Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, sbyte value) {
  }

  /// <summary>Adds an I16Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, short value) {
  }

  /// <summary>Adds an I32Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, int value) {
  }

  /// <summary>Adds an I64Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, long value) {
  }

  /// <summary>Adds a StringField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  /// <exception cref="GmsecException">Throw if the value is null.</exception>
  public bool AddField(string name, string value) {
  }

  /// <summary>Adds a U8Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, byte value) {
  }

  /// <summary>Adds a U16Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, ushort value) {
  }

  /// <summary>Adds a U32Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, uint value) {
  }

  /// <summary>Adds a U64Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Throw if the name is null or contains an empty-string.</exception>
  public bool AddField(string name, ulong value) {
  }

  /// <summary>Removes all fields associated with the message</summary>
  public void ClearFields() {
  }

  /// <summary>Removes the fields, identified by name, that is associated with the message</summary>
  /// <param name="name">The name of the field to remove</param>
  /// <returns>True if the operation was a success; false otherwise.</returns>
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string.</exception>
  public bool ClearField(string name) {
  }

  /// <summary>Attempts to convert the field value into a signed 64-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a signed integer.</returns>
  /// <exception cref="GmsecException">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public long GetIntegerValue(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into an unsigned 64-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as an unsigned integer.</returns>
  /// <exception cref="GmsecException">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public ulong GetUnsignedIntegerValue(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into a 64-bit floating point number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a floating point number.</returns>
  /// <exception cref="GmsecException">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to a double.</exception>
  public double GetDoubleValue(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into string representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a string.</returns>
  /// <exception cref="GmsecException">An Exception is thrown if the field cannot be found, or if it cannot successfully be converted to a string.</exception>
  public string GetStringValue(string fieldName) {
  }

  /// <summary>Reports whether the field, identified by name, is present in the Message.</summary>
  /// <param name="name">The name of the field</param>
  /// <returns>True if the field is found; false otherwise.</returns>
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string.</exception>
  public bool HasField(string name) {
  }

  /// <summary>Returns the field, identified by name, that is associated with the message</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>Returns a handle to a field on success, or null on lookup failure.</returns>
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string.</exception>
  public Field GetField(string name) {
  }

  /// <summary>Returns the type of the field, identified by name, that is associated with the message</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>Returns the field type.</returns>
  /// <seealso Field::FieldType/>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string or if the field cannot be referenced.</exception>
  public Field.FieldType GetFieldType(string name) {
  }

  /// <summary>Returns a handle to the BinaryField, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the BinaryField.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a BinaryField.</exception>
  public BinaryField GetBinaryField(string name) {
  }

  /// <summary>Returns a handle to the BooleanField, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the BooleanField.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a BooleanField.</exception>
  public BooleanField GetBooleanField(string name) {
  }

  /// <summary>Returns a handle to the CharField, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the CharField.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a CharField.</exception>
  public CharField GetCharField(string name) {
  }

  /// <summary>Returns a handle to the F32Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the F32Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a F32Field.</exception>
  public F32Field GetF32Field(string name) {
  }

  /// <summary>Returns a handle to the F64Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the F64Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a F64Field.</exception>
  public F64Field GetF64Field(string name) {
  }

  /// <summary>Returns a handle to the I8Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I8Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a I8Field.</exception>
  public I8Field GetI8Field(string name) {
  }

  /// <summary>Returns a handle to the I16Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I16Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a I16Field.</exception>
  public I16Field GetI16Field(string name) {
  }

  /// <summary>Returns a handle to the I32Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I32Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a I32Field.</exception>
  public I32Field GetI32Field(string name) {
  }

  /// <summary>Returns a handle to the I64Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I64Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a I64Field.</exception>
  public I64Field GetI64Field(string name) {
  }

  /// <summary>Returns a handle to the U8Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U8Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a U8Field.</exception>
  public U8Field GetU8Field(string name) {
  }

  /// <summary>Returns a handle to the U16Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U16Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a U16Field.</exception>
  public U16Field GetU16Field(string name) {
  }

  /// <summary>Returns a handle to the U32Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U32Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a U32Field.</exception>
  public U32Field GetU32Field(string name) {
  }

  /// <summary>Returns a handle to the U64Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U64Field.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a U64Field.</exception>
  public U64Field GetU64Field(string name) {
  }

  /// <summary>Returns a handle to the StringField, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the StringField.</returns>
  /// <exception cref="GmsecException">If the subject is null, contains an empty-string if the field is non-existent,
  /// or cannot be interpreted as a StringField.</exception>
  public StringField GetStringField(string name) {
  }

  /// <summary>Returns the number of fields associated with the message.</summary>
  public int GetFieldCount() {
  }

  /// <summary>Copies the fields associated with the message to another message.</summary>
  /// <param name="toMsg">The destination message where to copy the fields.</param>
  public void CopyFields(Message toMsg) {
  }

  /// <summary>Returns the XML string representation of the message.</summary>
  public string ToXML() {
  }

  /// <summary>Returns the JSON string representation of the message.</summary>
  public string ToJSON() {
  }

  /// <summary>Returns the size of the message, in bytes.</summary>
  public int GetSize() {
  }

  /// <summary>
  /// Method that allows the callee to get the MessageFieldIterator associated with the %Message.
  /// This iterator will allow for applications to iterate over the Field objects stored within the %Message.
  /// The iterator is reset each time GetFieldIterator() is called. The iterator itself is destroyed when the
  /// %Message object is destroyed.
  /// <p>
  /// Only one MessageFieldIterator object is associated with a %Message object; multiple calls to
  /// GetFieldIterator() will return a reference to the same MessageFieldIterator object. Each call will reset the iterator.
  ///</summary>
  ///
  /// <returns>A reference to a MessageFieldIterator object.</returns>
  public MessageFieldIterator GetFieldIterator() {
  }

  /// <summary>
  /// Returns a handle to the specialized MessageFieldIterator, which can then be used to iterate over the Fields contained within the message object.
  /// </summary>
  ///
  /// <param name="selector">The specialized type of MessageFieldIterator to return.</param>
  ///
  /// <returns>A reference to a MessageFieldIterator object.</returns>
  ///
  /// <seealso cref="MessageFieldIterator.Selector"/>
  public MessageFieldIterator GetFieldIterator(MessageFieldIterator.Selector selector) {
  }

  /// <summary>Enumerated list of possible %Message types.</summary>
  public enum MessageKind {

    /// <summary>For messages that will be published.</summary>
    PUBLISH = 1,

    /// <summary>For messages that will be issued as requests.</summary>
    REQUEST = 2,

    /// <summary>For messages that will be issued as a reply (to a request).</summary>
    REPLY = 3
  }

}

}
