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

/// The Message class serves a container for %GMSEC Messages.
/// The methods of this class allow for the construction and manipulation of the data in a message.
/// </summary>

public class Message : global::System.IDisposable {

  /// <summary>Constructor that creates a message of unknown kind and with no associated fields.</summary>
  public Message() {
  }

  /// <summary>Copy constructor for cloning a message.</summary>
  ///
  /// <param name="other">Message object to copy</param>
  public Message(Message other) {
  }

  /// <summary>Acknowledges the processing of the message that has been received from the
  /// GMSEC Bus.
  /// Some middleware can retain a delivered message in a queue until said time
  /// the message has been acknowledged as being processed. Should the message not be
  /// acknowledged, the middleware can deliver the message to another subscriber client.</summary>
  /// <seealso cref="Connection.Receive"/>
  /// <seealso cref="Connection.Request"/>
  public void Acknowledge() {
  }

  /// <summary>Destroys a Message object.
  /// This method should NOT be used to destroy a Message object received within a callback.</summary>
  /// <seealso cref="Connection.Receive"/>
  /// <seealso cref="Connection.Request"/>
  public static void Destroy(Message msg) {
  }

  /// <summary>Returns a string that identifies (if known) the schema from which the message is based.</summary>
  public string GetSchemaID() {
  }

  /// <summary>Returns the specification version number (if known) that was used to create the message.</summary>
  public uint GetVersion() {
  }

  /// <summary>Returns the specification schema level (if known) that was used to create the message.</summary>
  public Specification.SchemaLevel GetSchemaLevel() {
  }

  /// <summary>The contents of the message are compared to the template to ensure the message is in compliance with the message specification.</summary>
  /// <returns>A Status object to indicate whether the message is valid. If invalid, a list of errors will be provided of issues found within the message will be reported.</returns>
  public Status IsCompliant() {
  }

  /// <summary>Registers the given message validator that is used when the message is checked for compliance.</summary>
  /// <param name="validator">A custom message validation object.</param>
  /// <seealso MessageValidator/>
  /// <seealso MessageFactory::RegisterMessageValidator/>
  public void RegisterMessageValidator(MessageValidator validator) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>
  ///
  /// <param name="fieldName">Name of the field to be modified/created</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field, or if the string is too big when converting to char.</exception>
  public bool SetFieldValue(string fieldName, string value) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>
  ///
  /// <param name="fieldName">Name of the field to be modified/created.</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field.</exception>
  public bool SetFieldValue(string fieldName, short value) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>
  ///
  /// <param name="fieldName">Name of the field to be modified/created.</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field.</exception>
  public bool SetFieldValue(string fieldName, ushort value) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>
  ///
  /// <param name="fieldName">Name of the field to be modified/created.</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field.</exception>
  public bool SetFieldValue(string fieldName, int value) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>>
  ///
  /// <param name="fieldName">Name of the field to be modified/created.</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field.</exception>
  public bool SetFieldValue(string fieldName, uint value) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>
  ///
  /// <param name="fieldName">Name of the field to be modified/created.</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field.</exception>
  public bool SetFieldValue(string fieldName, long value) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>
  ///
  /// <param name="fieldName">Name of the field to be modified/created.</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field.</exception>
  public bool SetFieldValue(string fieldName, ulong value) {
  }

  /// <summary>
  /// Adds/replaces a field with the given name and value to the message. If
  /// a field template for the schema in use is available for the named field, then
  /// the value's type will be coerced to match that which is described in the field
  /// template.
  /// </summary>
  ///
  /// <remarks>
  /// This method is not very efficient; if the type of the field is known,
  /// consider calling AddField() instead.
  /// </remarks>
  ///
  /// <param name="fieldName">Name of the field to be modified/created.</param>
  /// <param name="value">The value of the field. The value's type is automatically determined based on the message schema.</param>
  ///
  /// <returns>Returns true if the field was replaced; false otherwise.</returns>
  ///
  /// <exception cref="GmsecException">Thrown if the field name is NULL, contains an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the input value's type cannot be converted into the type required by the field.</exception>
  public bool SetFieldValue(string fieldName, double value) {
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
  public void SetConfig(Config config) {
  }

  /// <summary>Returns the configuration associcated with the message</summary>
  public Config GetConfig() {
  }

  /// <summary>Allows for the setting/changing of the message subject</summary>
  ///
  /// <param name="subject">The subject/topic to associate with the message</param>
  ///
  /// <exception cref="GmsecException">If the subject is null or contains an empty-string</exception>
  public void SetSubject(string subject) {
  }

  /// <summary>
  /// Allows for the setting of individual subject elements. The name of the elements are defined
  /// by the message's corresponding message template. This value will be overridden by automatic subject
  /// generation if the subject element is defined by an existing field in the message, or if the subject
  /// was manually defined with setSubject.
  /// </summary>
  ///
  /// <param name="name">The name of the subject element</param>
  /// <param name="value">The value of the subject element. An empty or null value will be seen as FILL in the subject line.</param>
  ///
  /// <exception cref="GmsecException">if the name is NULL, an empty string, or does not match a subject element
  /// name defined the message template, or if the message does not have a corresponding message template.
  /// </exception>
  public void SetSubjectElement(string name, string value) {
  }

  /// <summary>Returns message subject</summary>
  public string GetSubject() {
  }

  /// <summary>Sets the message kind.
  /// <p>
  /// The API can auto-deduce the message kind, but there may be cases where
  /// this method may be useful. For most common Use Cases, this method should not
  /// need to be called.
  /// </summary>
  ///
  /// <param name="kind">The message kind</param>
  public void SetKind(Message.Kind kind) {
  }

  /// <summary>Returns message kind</summary>
  public Message.Kind GetKind() {
  }

  /// <summary>Adds a copy of the field object to the message</summary>
  /// <param name="field">The field object</param>
  /// <returns>True if a new field was added; false if an existing field was replaced.</returns>
  /// <exception cref="GmsecException">Thrown if the Field object is null</exception>
  public bool AddField(Field field) {
  }

  /// <summary>Adds a BinaryField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="bin">Byte array of data</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the byte arrray is null.</exception>
  public bool AddField(string name, byte[] bin) {
  }

  /// <summary>Adds a BooleanField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, bool value) {
  }

  /// <summary>Adds a CharField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, char value) {
  }

  /// <summary>Adds an F32Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, float value) {
  }

  /// <summary>Adds an F64Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, double value) {
  }

  /// <summary>Adds an I8Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, sbyte value) {
  }

  /// <summary>Adds an I16Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, short value) {
  }

  /// <summary>Adds an I32Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, int value) {
  }

  /// <summary>Adds an I64Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, long value) {
  }

  /// <summary>Adds a StringField to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  /// <exception cref="GmsecException">Thrown if the value is null.</exception>
  public bool AddField(string name, string value) {
  }

  /// <summary>Adds a U8Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, byte value) {
  }

  /// <summary>Adds a U16Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, ushort value) {
  }

  /// <summary>Adds a U32Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, uint value) {
  }

  /// <summary>Adds a U64Field to the message.</summary>
  /// <param name="name">The name of the field</param>
  /// <param name="value">Field value</param>
  /// <returns>True if the Field is replacing one with the same name; false otherwise.</returns>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is otherwise non-compliant.</exception>
  public bool AddField(string name, ulong value) {
  }

  /// <summary>Adds each Field in the given list to the Message.</summary>
  /// <param name="fields">The list of field objects</param>
  /// <exception cref="GmsecException">Thrown if the name is null, is an empty string, or is non-compliant.</exception>
  public bool AddFields(FieldList fields) {
  }

  /// <summary>Removes all fields associated with the message</summary>
  public void ClearFields() {
  }

  /// <summary>Removes the fields, identified by name, that is associated with the message</summary>
  /// <param name="name">The name of the field to remove</param>
  /// <returns>True if the operation was a success; false otherwise.</returns>
  public bool ClearField(string name) {
  }

  /// <summary>Reports whether the field, identified by name, is present in the Message.</summary>
  /// <param name="name">The name of the field</param>
  /// <returns>True if the field is found; false otherwise.</returns>
  public bool HasField(string name) {
  }

  /// <summary>Attempts to convert the field value into string representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a string.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to a string.</exception>
  public string GetStringValue(string fieldName) {
  }

  /// <summary>
  /// Attempts to convert the field value into boolean representation.
  /// <p>
  /// Any non-zero value will be interpreted as true, otherwise as false.
  /// </p>
  /// </summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a boolean value.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to a boolean.</exception>
  public string GetBooleanValue(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into a signed 16-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a signed 16-bit integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public short GetI16Value(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into a signed 32-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a signed 32-bit integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public long GetI32Value(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into a signed 64-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a signed 64-bit integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public long GetI64Value(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into an unsigned 16-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as an unsigned 16-integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public ushort GetU16Value(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into an unsigned 32-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as an unsigned 32-integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public uint GetU32Value(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into an unsigned 64-bit integer number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as an unsigned 64-integer.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to an integer.</exception>
  public ulong GetU64Value(string fieldName) {
  }

  /// <summary>Attempts to convert the field value into a 64-bit floating point number representation.</summary>
  /// <param name="fieldName">The name of the field to access</param>
  /// <returns>Returns the field value as a floating point number.</returns>
  /// <exception cref="GmsecException">Thrown if the field cannot be found, or if it cannot successfully be converted to a double.</exception>
  public double GetF64Value(string fieldName) {
  }

  /// <summary>Returns the field, identified by name, that is associated with the message</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>Returns a handle to a field on success, or null on lookup failure.</returns>
  public Field GetField(string name) {
  }

  /// <summary>Returns the type of the field, identified by name, that is associated with the message</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>Returns the field type.</returns>
  /// <seealso Field::FieldType/>
  /// <exception cref="GmsecException">Thrown if the named field cannot be found.</exception>
  public Field.FieldType GetFieldType(string name) {
  }

  /// <summary>Returns a handle to the BinaryField, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the BinaryField.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT a BinaryField or cannot be found.</exception>
  public BinaryField GetBinaryField(string name) {
  }

  /// <summary>Returns a handle to the BooleanField, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the BooleanField.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT a BooleanField or cannot be found.</exception>
  public BooleanField GetBooleanField(string name) {
  }

  /// <summary>Returns a handle to the CharField, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the CharField.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT a CharField or cannot be found.</exception>
  public CharField GetCharField(string name) {
  }

  /// <summary>Returns a handle to the F32Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the F32Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT an F32Field or cannot be found.</exception>
  public F32Field GetF32Field(string name) {
  }

  /// <summary>Returns a handle to the F64Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the F64Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT an F64Field or cannot be found.</exception>
  public F64Field GetF64Field(string name) {
  }

  /// <summary>Returns a handle to the I8Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I8Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT an I8Field or cannot be found.</exception>
  public I8Field GetI8Field(string name) {
  }

  /// <summary>Returns a handle to the I16Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I16Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT an I16Field or cannot be found.</exception>
  public I16Field GetI16Field(string name) {
  }

  /// <summary>Returns a handle to the I32Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I32Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT an I32Field or cannot be found.</exception>
  public I32Field GetI32Field(string name) {
  }

  /// <summary>Returns a handle to the I64Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the I64Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT an I64Field or cannot be found.</exception>
  public I64Field GetI64Field(string name) {
  }

  /// <summary>Returns a handle to the U8Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U8Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT a U8Field or cannot be found.</exception>
  public U8Field GetU8Field(string name) {
  }

  /// <summary>Returns a handle to the U16Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U16Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT a U16Field or cannot be found.</exception>
  public U16Field GetU16Field(string name) {
  }

  /// <summary>Returns a handle to the U32Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U32Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT a U32Field or cannot be found.</exception>
  public U32Field GetU32Field(string name) {
  }

  /// <summary>Returns a handle to the U64Field, as identified by the given name.</summary>
  /// <param name="name">The name of the field to fetch</param>
  /// <returns>On success, returns a handle to the U64Field.</returns>
  /// <exception cref="GmsecException">Thrown if the named field is NOT a U64Field or cannot be found.</exception>
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
  public enum Kind {

    /// <summary>For messages that will be published.</summary>
    PUBLISH = 1,

    /// <summary>For messages that will be issued as requests.</summary>
    REQUEST = 2,

    /// <summary>For messages that will be issued as a reply (to a request).</summary>
    REPLY = 3
  }

  /// <summary>Possible values for the GMSEC Message's response to a request.</summary>
  public enum ResponseStatus
  {
    /// <summary>Basic acknowledgement.</summary>
    ACKNOWLEDGEMENT = 1,

    /// <summary>Still working on operation.</summary>
    WORKING_KEEP_ALIVE = 2,

    /// <summary>Operation successfully completed.</summary>
    SUCCESSFUL_COMPLETION = 3,

    /// <summary>Operation failed to complete.</summary>
    FAILED_COMPLETION = 4,

    /// <summary>The request for an operation was invalid.</summary>
    INVALID_REQUEST = 5,

    /// <summary>This message is the last in a set of responses.</summary>
    FINAL_MESSAGE = 6
  }
}

}
