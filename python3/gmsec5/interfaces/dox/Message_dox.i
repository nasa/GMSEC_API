/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::Message "

    The Message is a container for GMSEC Messages. The methods
    of this class allow the construction and manipulation of the data
    in a message.

    See Also
    --------
    Field
    MessageFieldIterator

    CONSTRUCTORS:

    Message(self)

    Constructor that creates a message of unknown kind and with no associated fields.



    Message(self, other: Message)

    Copy constructor for cloning a message.

    Parameters
    ----------
    other : The Message to copy.
";


%feature("docstring") gmsec::api5::Message::acknowledge "

    acknowledge(self)

    Acknowledges the processing of the message that has been received from the GMSEC Bus.
    Some middleware can retain a delivered message in a queue until said time
    the message has been acknowledged as being processed. Should the message not be
    acknowledged, the middleware will deliver the message to another subscriber client.
";


%feature("docstring") gmsec::api5::Message::destroy"

    destroy(message: Message)

    This method will destroy a Message.
    This method should NOT be used to destroy a Message object received within a callback.

    Parameters
    ----------
    message : Message to be destroyed

    Exceptions
    ----------
    A GmsecError is thrown on error.

    See Also
    --------
    Connection.receive()
    Connection.request()
";


%feature("docstring") gmsec::api5::Message::getSchemaID "

    get_schema_id(self) -> str

    Returns a string that identifies (if known) the schema from which the message is based.

    Returns
    -------
    A schema ID (if known) that was used to build the message.
";


%feature("docstring") gmsec::api5::Message::getVersion "

    get_version(self) -> int

    Returns the specification version number that was used to create the message.

    Returns
    -------
    A specification version number that was used to create the message.
";


%feature("docstring") gmsec::api5::Message::getSchemaLevel "

    get_schema_level(self) -> int

    Returns the specification schema level that was used to create the message.

    Returns
    -------
    A specification schema level that was used to creae the message.
";


%feature("docstring") gmsec::api5::Message::isCompliant "

    is_compliant(self) -> Status

    The contents of the message are compared to the template to ensure the message is in compliance with the message specification.

    Returns
    -------
    A Status object to indicate whether the message is valid. If invalid, a list of errors will be provided of issues found within the message will be reported.
";


%feature("docstring") gmsec::api5::Message::registerMessageValidator "

    register_message_validator(self, validator: MessageValidator )

    Registers the given message validator that is used when the message is checked for compliance.

    Parameter
    ---------
    validator : A custom message validation object.

    See Also
    --------
    MessgeValidator
";


%feature("docstring") gmsec::api5::Message::setFieldValue "

    set_field_value(self, fieldName: str, value: str)

    Adds/replaces a field with the given name and value to the message. If
    a field template for the schema in use is available for the named field, then
    the value's type will be coerced to match that which is described in the field
    template.

    Note
    ----------
    This method is not very efficient; if the type of the field is known,
    consider calling add_field() instead.

    Parameters
    ----------
    fieldName : Name of the field to be modified/created.
    value     : The string value of the field. The value's type is automatically determined based on the message schema.

    Returns
    -------
    Returns true if the field was replaced; false otherwise.

    Exception
    ---------
    GmsecError : Thrown if the field name is NULL or contains an empty string.
    GmsecError : Thrown if the input value's type cannot be converted into the type required by the field, or if the string is too big when converting to char.



    set_field_value(self, fieldName: str, value: int)

    Adds/replaces a field with the given name and value to the message. If
    a field template for the schema in use is available for the named field, then
    the value's type will be coerced to match that which is described in the field
    template.

    Note
    ----------
    This method is not very efficient; if the type of the field is known,
    consider calling add_field() instead.

    Parameters
    ----------
    fieldName : Name of the field to be modified/created.
    value     : The integer value of the field. The value's type is automatically determined based on the message schema.

    Returns
    -------
    Returns true if the field was replaced; false otherwise.

    Exception
    ---------
    GmsecError : Thrown if the field name is NULL or contains an empty string.
    GmsecError : Thrown if the input value's type cannot be converted into the type required by the field.



    set_field_value(self, fieldName: str, value: double)

    Adds/replaces a field with the given name and value to the message. If
    a field template for the schema in use is available for the named field, then
    the value's type will be coerced to match that which is described in the field
    template.

    Note
    ----------
    This method is not very efficient; if the type of the field is known,
    consider calling add_field() instead.

    Parameters
    ----------
    fieldName : Name of the field to be modified/created.
    value     : The floating point value of the field. The value's type is automatically determined based on the message schema.

    Returns
    -------
    Returns true if the field was replaced; false otherwise.

    Exception
    ---------
    GmsecError : Thrown if the field name is NULL or contains an empty string.
    GmsecError : Thrown if the input value's type cannot be converted into the type required by the field.
";


%feature("docstring") gmsec::api5::Message::setConfig "

    set_config(self, config: Config)

    This function sets the configuration items to the message. This can be
    used to pass middleware specific items to the Message.

    Note: If a configuration item included within the given config is already associated with the message, then the configuration item will be replaced with the new item.

    Parameters
    ----------
    config : Config that contains configuration values to add to the Message.
";


%feature("docstring") gmsec::api5::Message::getConfig "

    get_config(self) -> Config

    Returns the configuration associcated with the message.

    Returns
    -------
    A reference to the Config associated with the Message.
";


%feature("docstring") gmsec::api5::Message::setSubject "

    set_subject(self, subject: str)

    Allows for the setting/changing of the message subject.

    Parameters
    ----------
    subject : The subject/topic to associate with the message.

    Exceptions
    ----------
    A GmsecError is thrown if the subject is null, an empty-string, or if it is not compliant.
";


%feature("docstring") gmsec::api5::Message::getSubject "

    get_subject(self) -> str

    Returns the subject deduced using message field content. For subject elements that cannot be deduced, FILL is used as a substitution.

    Returns
    -------
    The Message subject.
";


%feature("docstring") gmsec::api5::Message::setKind "

    set_kind(self, kind: Message.Kind)

    This method can be used to set the message kind.

    Note: The API can auto-deduce the message kind, but there may be cases where
    this method may be useful. For most common Use Cases, this method should not
    need to be caled.

    Parameters
    ----------
    kind : The message kind (PUBLISH, REQUEST, or REPLY).
";


%feature("docstring") gmsec::api5::Message::getKind "

    get_kind(self) -> Message.Kind

    Returns the message kind.

    Returns
    -------
    The message kind (PUBLISH, REQUEST, or REPLY) assigned to the Message.
";


%feature("docstring") gmsec::api5::Message::addField "

    add_field(self, field: Field) -> bool

    This function will add a new field to this message. A new field
    will be added to the message that matches the field passed in.
    Therefore the field is owned by the client program and can be reused.
    The client program must also clean up that field when no longer needed.

    Example:
    replaced = msg.addField(I32Field(\"MyField\", 1234))
    if replaced:
        #field was replaced
    else:
        #new field added

    Parameters
    ----------
    field : Field to add to the Message

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.
";


%feature("docstring") gmsec::api5::Message::addFields "

    add_fields(self, fields: FieldList)

    This function will add the fields in the given FieldList to the message.

    Parameters
    ----------
    fields: FieldList of one or more Field objects

    Returns
    -------
    Returns True if any existing field in the Message has been replaced; False otherwise.
";


%feature("docstring") gmsec::api5::Message::clearFields "

    clear_fields(self)

    This function will remove all fields from the message.
";


%feature("docstring") gmsec::api5::Message::clearField "

    clear_field(self, name: str) -> bool

    This function will remove the specified field form this message, if it exists.

    Parameter
    ---------
    name : The name of the field

    Returns
    ---------
    True if the field was removed; False otherwise.
";


%feature("docstring") gmsec::api5::Message::hasField "

    has_field(self, name: str) -> bool

    Parameters
    ----------
    name : The name of the field to reference

    Returns
    ----------
    True if the field is found; false otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the given field name is NULL or contains an empty string.
";


%feature("docstring") gmsec::api5::Message::getStringValue "

    get_string_value(self, fieldName: str) -> str

    Attempts to convert the field value into string representation.

    Parameters
    ----------
    fieldName : The name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as a string value.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be found, or if it cannot successfully be converted to a string value.
";


%feature("docstring") gmsec::api5::Message::getBooleanValue "

    get_boolean_value(self, fieldName: str) -> str

    Attempts to convert the field value into boolean representation. Any non-zero value will be interpreted as True, otherwise as False.

    Parameters
    ----------
    fieldName : The name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as a boolean value.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be found, or if it cannot successfully be converted to a boolean value.
";


%feature("docstring") gmsec::api5::Message::getI64Value "

    get_integer_value(self, fieldName: str) -> GMSEC_I64

    Attempts to convert the field value into a signed 64-bit integer number representation.

    Parameters
    ----------
    fieldName : The name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as an integer value.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be found, or if it cannot successfully be converted to an integer.
";


%feature("docstring") gmsec::api5::Message::getF64Value "

    get_double_value(self, fieldName: str) -> double

    Attempts to convert the field value into an 64-bit floating-point number representation.

    Parameters
    ----------
    fieldName : The name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as a floating-point value.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be found, or if it cannot successfully be converted to a float-point value.
";


%feature("docstring") gmsec::api5::Message::getField "

    get_field(self, name: str) -> Field

    Parameters
    ----------
    name : The name of the field to reference

    Returns
    ----------
    A reference to the Field, if found, or None if not.

    Exceptions
    ----------
    A GmsecError is thrown if the given field name is NULL or contains an empty string.
";


%feature("docstring") gmsec::api5::Message::getFieldType "

    get_field_type(self, name: str) -> Field::FieldType

    Returns the FieldType associated with the named field.

    Parameters
    ----------
    name : The name of field

    Returns
    -------
    FieldType as defined in Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field cannot be found.
";


%feature("docstring") gmsec::api5::Message::getBinaryField "

    get_binary_field(self, name: str) -> BinaryField

    Returns a reference to the BinaryField associated with the named field.

    Parameters
    ----------
    name : The name of the field

    Returns
    -------
    A reference to a BinaryField.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT a BinaryField or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getBooleanField "

    get_boolean_field(self, name: str) -> BooleanField

    Returns a reference to the BooleanField associated with the named field.

    Parameters
    ----------
    name : The name of the field

    Returns
    -------
    A reference to a BooleanField.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT a BooleanField or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getCharField "

    get_char_field(self, name: str) -> CharField

    Returns a reference to the CharField associated with the named field.

    Parameter
    ---------
    name : The name of the field

    Returns
    -------
    A reference to a CharField.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT a CharField or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getF32Field "

    get_f32_field(self, name: str) -> F32Field

    Returns a reference to the F32Field associated with the named field.

    Parameter
    ---------
    name : The name of the field

    Returns
    -------
    A reference to an F32Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT an F32Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getF64Field "

    get_f64_field(self, name: str) -> F64Field

    Returns a reference to the I16Field associated with the named field.

    Parameters
    ----------
    name : The name of field

    Returns
    -------
    A reference to an F64Field.

    Exception
    ---------
    A GmsecError is thrown if the named field is NOT an F64Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getI8Field "

    get_i8_field(self, name: str) -> I8Field

    Returns a reference to the I8Field associated with the named field.

    Parameters
    -------
    name : The name of the field

    Returns
    -------
    A reference to an I8Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT an I8Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getI16Field "

    get_i16_field(self, name: str) -> I16Field

    Returns a reference to the I16Field associated with the named field.

    Parameters
    ----------
    name : The name of field

    Returns
    -------
    A reference to an I16Field.

    Exception
    ---------
    A GmsecError is thrown if the named field is NOT an I16Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getI32Field "

    get_i32_field(self, name: str) -> I32Field

    Returns a reference to the I32Field associated with the named field.

    Parameters
    ----------
    name : The name of the field to reference

    Returns
    -------
    A reference to an I32Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT an I32Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getI64Field "

    get_i64_field(self, name: str) -> I64Field

    Returns a reference to the I64Field associated with the named field.

    Parameters
    ----------
    name : The name of the field

    Returns
    -------
    A reference to an I64Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT an I64Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getU8Field "

    get_u8_field(self, name: str) -> U8Field

    Returns a reference to the U8Field associated with the named field.

    Parameters
    ----------
    name : The name of the field

    Returns
    -------
    A reference to a U8Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT a U8Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getU16Field "

    get_u16_field(self, name: str) -> U16Field

    Returns a reference to the U16Field associated with the named field.

    Parameters
    ----------
    name : The name of the field

    Returns
    -------
    A reference to a U16Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT a U16Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getU32Field "

    get_u32_field(self, name: str) -> U32Field

    Returns a reference to the U32Field associated with the named field.

    Parameters
    ----------
    name : The name of the field

    Returns
    -------
    A reference to a U32Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT an U32Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getU64Field "

    get_u64_field(self, name: str) -> U64Field

    Returns a reference to the U64Field associated with the named field.

    Parameters
    -------
    name : The name of the field

    Returns
    -------
    A reference to a U64Field.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT an U64Field or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getStringField "

    get_string_field(self, name: str) -> StringField

    Returns a reference to the StringField associated with the named field.

    Parameters
    ----------
    name : The name of the Field.

    Returns
    -------
    A reference to a StringField.

    Exceptions
    ----------
    A GmsecError is thrown if the named field is NOT a StringField or cannot be found.
";


%feature("docstring") gmsec::api5::Message::getFieldCount "

    get_field_count(self) -> int

    This function gets the number of fields contained in this message.

    Returns
    -------
    The number of fields contained within Message.
";


%feature("docstring") gmsec::api5::Message::copyFields "

    copy_fields(self, toMsg: Message)

    This function will copy all fields from this message to a new
    message in the fastest way possible

    Note: The fields are NOT copied by reference. As such, the
    original message and its fields can be deleted or modified after
    it has been copied to the new message.

    Parameters
    ----------
    toMsg : The message into which to copy the fields
";


%feature("docstring") gmsec::api5::Message::toXML "

    to_xml(self) -> str

    This function will dump a message to an XML formatted string.

    XML Format example:
    <MESSAGE SUBJECT=\"GMSEC.MISSION.CONST.SAT.EVT.MSG\" KIND=\"PUBLISH\">
        <FIELD TYPE=\"CHAR\" NAME=\"CHAR-FIELD\">c</FIELD>
        <FIELD TYPE=\"BOOL\" NAME=\"BOOL-FIELD\">TRUE</FIELD>
        <FIELD TYPE=\"I16\" NAME=\"SHORT-FIELD\">-123</FIELD>
        <FIELD TYPE=\"U16\" NAME=\"USHORT-FIELD\">123</FIELD>
        <FIELD TYPE=\"I32\" NAME=\"INT-FIELD\">-12345678</FIELD>
        <FIELD TYPE=\"U32\" NAME=\"UINT-FIELD\">123456789</FIELD>
        <FIELD TYPE=\"STRING\" NAME=\"STRING-FIELD\">This is a test of the Message::toXML function</FIELD>
        <FIELD TYPE=\"F32\" NAME=\"LONG-FIELD\" BITS=\"42F6E666\">123.45</FIELD>
        <FIELD TYPE=\"F64\" NAME=\"DOUBLE-FIELD\" BITS=\"4B94216CA930D75A\">1.234e56</FIELD>
        <FIELD TYPE=\"BIN\" NAME=\"BIN-FIELD\">4a4c4d4e4f5051</FIELD>
    </MESSAGE>

    Returns
    -------
    XML string representation of the Message.
";


%feature("docstring") gmsec::api5::Message::toJSON "

    to_json(self) -> str

    This function will dump a message to a JSON formatted string.

    JSON Format example:
    {
        \"MESSAGE\":{
            \"KIND\":\"PUBLISH\",
            \"SUBJECT\":\"GMSEC.MISSION.CONST.SAT.EVT.MSG\",
            \"FIELD\":[
                {
                    \"NAME\":\"CHAR-FIELD\",
                    \"TYPE\":\"CHAR\",
                    \"VALUE\":\"c\"
                },
                {
                    \"NAME\":\"BOOL-FIELD\",
                    \"TYPE\":\"BOOL\",
                    \"VALUE\":\"TRUE\"
                },
                {
                    \"NAME\":\"SHORT-FIELD\",
                    \"TYPE\":\"I16\",
                    \"VALUE\":\"-123\"
                },
                {
                    \"NAME\":\"USHORT-SHORT\",
                    \"TYPE\":\"U16\",
                    \"VALUE\":\"123\"
                },
                {
                    \"NAME\":\"INT-FIELD\",
                    \"TYPE\":\"I32\",
                    \"VALUE\":\"-12345678\"
                },
                {
                    \"NAME\":\"UINT-FIELD\",
                    \"TYPE\":\"U32\",
                    \"VALUE\":\"123456789\"
                },
                {
                    \"NAME\":\"STRING-FIELD\",
                    \"TYPE\":\"STRING\",
                    \"VALUE\":\"This is a test\"
                },
                {
                    \"NAME\":\"LONG-FIELD\",
                    \"TYPE\":\"F32\",
                    \"VALUE\":\"123.45\"
                    \"BITS\":\"42F6E666\"
                },
                {
                    \"NAME\":\"DOUBLE-FIELD\",
                    \"TYPE\":\"F64\",
                    \"VALUE\":\"1.234e56\"
                    \"BITS\":\"4B94216CA930D75A\"
                },
                {
                    \"NAME\":\"BIN-FIELD\",
                    \"TYPE\":\"BIN\",
                    \"VALUE\":\"4a4c4d4e4f5051\"
                }
            ]
        }
    }

    Returns
    -------
    A JSON formatted string
";


%feature("docstring") gmsec::api5::Message::getSize "

    get_size(self) -> int

    This function will return the physical storage size of the message
    in bytes.
    Please note that additional tracking data fields are added to
    messages at the time of publication which will affect the size of
    the message.

    Returns
    -------
    size of message in bytes
";


%feature("docstring") gmsec::api5::Message::getFieldIterator "

    get_field_iterator(self, selector=MessageFieldIterator.Selector_ALL_FIELDS) -> MessageFieldIterator

    Method that allows the callee to get the MessageFieldIterator
    associated with the Message.
    This iterator will allow for applications to iterate over the
    Fields stored within the Message.
    The iterator is reset each time getFieldIterator() is called.
    The iterator itself is destroyed when the Message is
    destroyed.

    Note: Only one MessageFieldIterator is associated with a
    Message; multiple calls to getFieldIterator() will return a
    reference to the same MessageFieldIterator. Each call will
    reset the iterator.

    Parameters
    ----------
    selector : Choices are Selector_ALL_FIELDS, Selector_HEADER_FIELDS, or Selector_NON_HEADER_FIELDS; default is Selector_ALL_FIELDS.

    Returns
    ----------
    A reference to a MessageFieldIterator.
";
