
%feature("docstring") gmsec::api::Message "

    The Message is a container for GMSEC Messages. The methods
    of this class allow the construction and manipulation of the data
    in a message.

    See Also
    --------
    Config
    Connection
    Field
    MessageFieldIterator

    CONSTRUCTORS:

    Message(self, subject: str, kind: Message.MessageKind)

    Initializes the Message instance.

    Parameters
    ----------
    subject : The subject string for the message.
    kind    : The kind of message to instantiate.

    Exceptions
    ----------
    A GmsecError is thrown if the subject is None or contains an empty-string.


    Message(self, subject: str, kind: Message.MessageKind, config: Config)

    Initializes the Message instance and associates a Configuration.

    Parameters
    ----------
    subject : The subject string for the message.
    kind    : The kind of message to instantiate.
    config  : A Config to associate with the message.

    Exceptions
    ----------
    A GmsecError is thrown if the subject is None or is an empty-string.


    Message(self, data: str)

    Initializes the message instance using XML or JSON text

    Parameters
    ----------
    data : An XML or JSON string.

    Exceptions
    ----------
    A GmsecError is thrown if the XML or JSON string cannot be parsed.


    Message(self, other: Message)

    Copy constructor - initializes the message instance using the other given Message.

    Parameters
    ----------
    other : The Message to copy.
";


%feature("docstring") gmsec::api::Message::getStringField "

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

%feature("docstring") gmsec::api::Message::setSubject "

    set_subject(self, subject: str)

    Allows for the setting/changing of the message subject.

    Parameters
    ----------
    subject : The subject/topic to associate with the message.

    Exceptions
    ----------
    A GmsecError is thrown if the subject is None or is an empty-string.
";

%feature("docstring") gmsec::api::Message::getFieldType "

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

%feature("docstring") gmsec::api::Message::getI16Field "

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

%feature("docstring") gmsec::api::Message::getF64Field "

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


%feature("docstring") gmsec::api::Message::addField "

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



    add_field(self, name: str, blob: bytearray, len: int) -> bool

    Adds a BinaryField to the Message.

    Parameters
    ----------
    name : The name of the field
    blob : The bytearray containing data
    len  : The length of the bytearray

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: bool) -> bool

    Adds a BooleanField to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: char) -> bool

    Adds a CharField to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: float) -> bool

    Adds an F32Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: double) -> bool

    Adds an F64Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: byte) -> bool

    Adds an I8Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: short) -> bool

    Adds an I16Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: int) -> bool

    Adds I32Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: longlong) -> bool

    Adds I64Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: str) -> bool

    Adds StringField to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: ubyte) -> bool

    Adds an U8Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)



    add_field(self, name: str, value: ushort) -> bool

    Adds an U16Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)


    add_field(self, name: str, value: uint) -> bool

    Adds an U32Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)


    add_field(self, name: str, value: ulonglong) -> bool

    Adds an U64Field to the Message.

    Parameters
    ----------
    name  : The name of the field
    value : The value of the field

    Returns
    -------
    Returns True if the Field is replacing one with the same name; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is None or is an empty-string.

    See Also
    --------
    add_field(self, field)
";


%feature("docstring") gmsec::api::Message::getU32Field "

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


%feature("docstring") gmsec::api::Message::getBinaryField "

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


%feature("docstring") gmsec::api::Message::getSize "

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


%feature("docstring") gmsec::api::Message::getI64Field "

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


%feature("docstring") gmsec::api::Message::toXML "

    to_XML(self) -> str

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


%feature("docstring") gmsec::api::Message::getIntegerValue "

    get_integer_value(self, fieldName: str) -> GMSEC_I64

    Attempts to convert the field value into a signed 64-bit integer
    number representation.

    Parameters
    ----------
    fieldName : The name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as an integer value.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be found, or if it
    cannot successfully be converted to an integer.
";


%feature("docstring") gmsec::api::Message::toJSON "

    to_JSON(self) -> str

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
    a JSON formatted string
";


%feature("docstring") gmsec::api::Message::getFieldCount "

    get_field_count(self) -> int

    This function gets the number of fields contained in this message.

    Returns
    -------
    The number of fields contained within Message.
";


%feature("docstring") gmsec::api::Message::clearField "

    clear_field(self, name: str) -> bool

    This function will remove the specified field form this message, if it exists.

    Parameter
    ---------
    name : The name of the field

    Returns
    ---------
    True if the field was removed; False otherwise.
";


%feature("docstring") gmsec::api::Message::getCharField "

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


%feature("docstring") gmsec::api::Message::getF32Field "

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


%feature("docstring") gmsec::api::Message::getSubject "

    get_subject(self) -> str

    This function get the subject this message will be/was published upon.

    Returns
    -------
    The subject assigned to the Message.
";


%feature("docstring") gmsec::api::Message::getConfig "

    get_config(self) -> Config

    This function gets a copy of the Configuration associated with the message.

    Returns
    -------
    A reference to the Config associated with the Message.
";


%feature("docstring") gmsec::api::Message::getUnsignedIntegerValue "

    get_unsigned_integer_value(self, fieldName: str) -> ulonglong

    Attempts to convert the field value into an unsigned 64-bit integer
    number representation.

    Parameters
    ----------
    fieldName : The name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as an unsigned 64-bit integer value.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be found, or if it
    cannot successfully be converted to an unsigned 64-bit integer.
";


%feature("docstring") gmsec::api::Message::hasField "

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


%feature("docstring") gmsec::api::Message::getField "

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


%feature("docstring") gmsec::api::Message::getI32Field "

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


%feature("docstring") gmsec::api::Message::addFields "

    add_fields(self, fields: FieldList)

    This function will add the fields in the given FieldList to the message.

    Parameters
    ----------
    fields: FieldList of one or more Field objects

    Returns
    -------
    Returns True if any existing field in the Message has been replaced; False otherwise.
";


%feature("docstring") gmsec::api::Message::clearFields "

    clear_fields(self)

    This function will remove all fields from the message.
";


%feature("docstring") gmsec::api::Message::copyFields "

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


%feature("docstring") gmsec::api::Message::getI8Field "

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


%feature("docstring") gmsec::api::Message::getU64Field "

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


%feature("docstring") gmsec::api::Message::getFieldIterator "

    get_field_iterator(self, selector=MessageFieldIterator.ALL_FIELDS) -> MessageFieldIterator

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
    selector : Choices are ALL_FIELDS, HEADER_FIELDS, or NON_HEADER_FIELDS; default is ALL_FIELDS.

    Returns
    ----------
    A reference to a MessageFieldIterator.
";


%feature("docstring") gmsec::api::Message::getBooleanField "

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


%feature("docstring") gmsec::api::Message::getKind "

    get_kind(self) -> Message.MessageKind

    This function gets the 'kind' of this message.

    Returns
    -------
    The MessageKind (PUBLISH, REQUEST, or REPLY) assigned to the Message.
";


%feature("docstring") gmsec::api::Message::getU8Field "

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


%feature("docstring") gmsec::api::Message::getStringValue "

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
    A GmsecError is thrown if the field cannot be found, or if it cannot successfully
    be converted to a string value.
";


%feature("docstring") gmsec::api::Message::getU16Field "

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


%feature("docstring") gmsec::api::Message::getDoubleValue "

    get_double_value(self, fieldName: str) -> double

    Attempts to convert the field value into an 64-bit floating-point
    number representation.

    Parameters
    ----------
    fieldName : The name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as a floating-point value.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be found, or if it
    cannot successfully be converted to a float-point value.
";


%feature("docstring") gmsec::api::Message::addConfig "

    add_config(self, config: Config)

    This function adds configuration items to the message. This can be
    used to pass middleware specific items to the Message.

    Note: If a configuration item included within the given config is
    already associated with the message, then the message's
    configuration item will be replaced with the new item.

    Parameters
    ----------
    config : Config that contains configuration values to add to the Message.
";
