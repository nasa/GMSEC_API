
// File: classlibgmsec__python_1_1Message.xml


%feature("docstring") gmsec::api::Message "

    The Message object is a container for GMSEC Messages.  The methods of this class
    allow the construction and manipulation of the data in a message.

    See Also
    --------
    Config
    Connection
    Field
    MessageFieldIterator

    CONSTRUCTORS:

    Message(self, subject, kind)

    Initializes the Message instance.

    Parameters
    ----------
    subject: The subject string for the message.
    kind: The kind of message to instantiate.

    Exceptions
    ----------
    An Exception is thrown if the subject is NULL or contains an empty-string.

    Message(self, subject, kind, config)

    Initializes the Message instance and associates a Configuration object.

    Parameters
    ----------
    subject: The subject string for the message.
    kind: The kind of message to instantiate.
    config: A configuration to associate with the message.

    Exceptions
    ----------
    An Exception is thrown if the subject is NULL or contains an empty-string.

    Message(self, data)

    Initializes the message instance using XML or JSON text

    Parameters
    ----------
    data: An XML or JSON string.

    Exceptions
    ----------
    An Exception is thrown if the XML or JSON string cannot be parsed.

    Message(self, other)

    Copy constructor - initializes the message instance using the other given Message.

    Parameters
    ----------
    other: The other Message object to copy.
";

%feature("docstring") gmsec::api::Message::getStringField "

    getStringField(self, name) -> StringField

    Returns a reference to the StringField associated with the named field.

    Returns
    -------
    A reference to a StringField object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT a StringField or cannot be found.
";

%feature("docstring") gmsec::api::Message::setSubject "

    setSubject(self, subject)

    Allows for the setting/changing of the message subject.

    Parameters
    ----------
    subject: the subject/topic to associate with the message.

    Exceptions
    ----------
    An Exception is thrown if the subject is NULL or contains an empty-string.
";

%feature("docstring") gmsec::api::Message::getFieldType "

    getFieldType(self, name) -> gmsec::api::Field::FieldType

    Returns the FieldType associated with the named field.

    Parameters
    ----------
    name: name of field to get the type for

    Returns
    -------
    FieldType as defined in Field.

    Exceptions
    ----------
    An Exception is thrown if the named field cannot be found.
";

%feature("docstring") gmsec::api::Message::getI16Field "

    getI16Field(self, name) -> I16Field

    Returns a reference to the I16Field associated with the named field.

    Returns
    -------
    A reference to an I16Field object.

    Exception
    ---------
    An Exception is thrown if the named field is NOT an I16Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::getF64Field "

    getF64Field(self, name) -> F64Field

    Parameters
    ----------
    name: char const *
";

%feature("docstring") gmsec::api::Message::addField "

    addField(self, field) -> bool

    This function will add a new field to this message. A new field will be added to the message
    that matches the field passed in. Therefore the field is owned by the client program and can be reused.
    The client program must also clean up that field when no longer needed.

    Example:
    replaced = msg.addField(libgmsec_python.I32Field(\"MyField\", 1234))
    if replaced:
        #field was replaced
    else:
        #new field added

    Parameters
    ----------
    field: field object to copy info from

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.


    addField(self, name, bin, len) -> bool

    Adds a BinaryField to the Message.

    Parameters
    ----------
    name: char const *
    bin: GMSEC_BIN
    len: size_t

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds a BooleanField to the Message.

    Parameters
    ----------
    name: char const *
    value: bool

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds a CharField to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_CHAR

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an F32Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_F32

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an F64Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_F64

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an I16Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_I8

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an I16Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_I16

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds I32Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_I32

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds I64Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_I64

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds StringField to the Message.

    Parameters
    ----------
    name: char const *
    value: char const *

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an U8Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_U8

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an U16Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_U16

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an U32Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_U32

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)


    addField(self, name, value) -> bool

    Adds an U64Field to the Message.

    Parameters
    ----------
    name: char const *
    value: GMSEC_U64

    Returns
    -------
    Returns true if the Field is replacing one with the same name; false otherwise.

    See Also
    --------
    addField(self, field)
";

%feature("docstring") gmsec::api::Message::getU32Field "

    getU32Field(self, name) -> U32Field

    Returns a reference to the U32Field associated with the named field.

    Returns
    -------
    A refernce to an U32Field object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT an U32Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::getBinaryField "

    getBinaryField(self, name) -> BinaryField

    Returns a reference to the BinaryField associated with the named field.

    Returns
    -------
    A refences to a BinaryField object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT a BinaryField or cannot be found.
";

%feature("docstring") gmsec::api::Message::getSize "

    getSize(self) -> size_t

    This function will return the physical storage size of the message in bytes.
    Please note that additional tracking data fields are added to messages at the
    time of publication which will affect the size of the message.

    Returns
    -------
    size of message in bytes
";

%feature("docstring") gmsec::api::Message::getI64Field "

    getI64Field(self, name) -> I64Field

    Returns a reference to the I64Field associated with the named field.

    Returns
    -------
    A reference to an I64Field object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT an I64Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::toXML "

    toXML(self) -> char const *

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

    getIntegerValue(self, fieldName) -> GMSEC_I64

    Attempts to convert the field value into a signed 64-bit integer number representation.

    Parameters
    ----------
    fieldName: the name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as an integer value.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be found, or if it cannot successfully
        be converted to an integer.
";

%feature("docstring") gmsec::api::Message::toJSON "

    toJSON(self) -> char const *

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
                    \"VALUE\":\"This is a test of the Message::toJSON function\"
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

    getFieldCount(self) -> size_t

    This function gets the number of fields contained in this message.

    Returns
    -------
    The number of fields contained within Message.
";

%feature("docstring") gmsec::api::Message::clearField "

    clearField(self, name) -> bool

    This function will remove all fields form this message.

";

%feature("docstring") gmsec::api::Message::getCharField "

    getCharField(self, name) -> CharField

    Returns a reference to the CharField associated with the named field.

    Returns
    -------
    A reference to a CharField object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT a CharField or cannot be found.
";

%feature("docstring") gmsec::api::Message::getF32Field "

    getF32Field(self, name) -> F32Field

    Returns a reference to the F32Field associated with the named field.

    Returns
    -------
    A reference to an F32Field object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT an F32Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::getSubject "

    getSubject(self) -> char const *

    This function get the subject this message will be/was published upon.

    Returns
    -------
    The subject assigned to the Message.
";

%feature("docstring") gmsec::api::Message::getConfig "

    getConfig(self) -> Config

    This function gets a copy of the Configuration associated with the message.

    Returns
    -------
    A reference to the Config object associated with the Message.
";

%feature("docstring") gmsec::api::Message::getUnsignedIntegerValue "

    getUnsignedIntegerValue(self, fieldName) -> GMSEC_U64

    Attempts to convert the field value into an unsigned 64-bit integer number representation.

    Parameters
    ----------
    fieldName: the name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as an unsigned 64-bit integer value.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be found, or if it cannot successfully
        be converted to an unsigned 64-bit integer.
";

%feature("docstring") gmsec::api::Message::getField "

    getField(self, name) -> Field

    Parameters
    ----------
    name: char const *
";

%feature("docstring") gmsec::api::Message::getI32Field "

    getI32Field(self, name) -> I32Field

    This function will return the named Field object contained within the Message object,
    if it exists.  Otherwise a NULL pointer is returned.

    Example:
    field = msg.getField(\"BOOL_FIELD\")

    if field.getType() == libgmsec_python.Field.BOOL_TYPE:
        value = field.getValue()
    else:
        #handle error

    Parameters
    ----------
    name: name of field to get

    Returns
    -------
    If found, a pointer to the field; otherwise NULL.
";

%feature("docstring") gmsec::api::Message::clearFields "

    clearFields(self)

    This function will remove all fields form this message.

";

%feature("docstring") gmsec::api::Message::copyFields "

    copyFields(self, toMsg)

    This function will copy all fields from this message to a new message in the fastest way possible

    Note: The fields are NOT copied by reference. As such, the original message and its fields can be
        deleted or modified after it has been copied to the new message.

    Parameters
    ----------
    toMsg: the message to copy fields into
";

%feature("docstring") gmsec::api::Message::getI8Field "

    getI8Field(self, name) -> I8Field

    Returns a reference to the I8Field associated with the named field.

    Returns
    -------
    A reference to an I8Field object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT an I8Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::getU64Field "

    getU64Field(self, name) -> U64Field

    Returns a reference to the U64Field associated with the named field.

    Returns
    -------
    A reference to an U64Field object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT an U64Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::getFieldIterator "

    getFieldIterator(self, selector=libgmsec_python.MessageFieldIterator.ALL_FIELDS) -> MessageFieldIterator

    Method that allows the callee to get the MessageFieldIterator associated with the Message.
    This iterator will allow for applications to iterate over the Field objects stored within the Message.
    The iterator is reset each time getFieldIterator() is called.  The iterator itself is destroyed when the
    Message object is destroyed.

    Note: Only one MessageFieldIterator object is associated with a Message object; multiple calls to
        getFieldIterator() will return a reference to the same MessageFieldIterator object.  Each call will reset the iterator.

    Returns
    -------
    A reference to a MessageFieldIterator object.
";

%feature("docstring") gmsec::api::Message::getBooleanField "

    getBooleanField(self, name) -> BooleanField

    Returns a reference to the BooleanField associated with the named field.

    Returns
    -------
    A reference to a BooleanField object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT a BooleanField or cannot be found.
";

%feature("docstring") gmsec::api::Message::getKind "

    getKind(self) -> gmsec::api::Message::MessageKind

    This function gets the 'kind' of this message.

    Returns
    -------
    The MessageKind assigned to the Message.

    See Also
    --------
    MessageKind
";
%feature("docstring") gmsec::api::Message::getU8Field "

    getU8Field(self, name) -> U8Field

    Returns a reference to the U8Field associated with the named field.

    Returns
    -------
    A reference to an U8Field object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT an U8Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::getStringValue "

    getStringValue(self, fieldName) -> char const *

    Attempts to convert the field value into string representation.

    Parameters
    ----------
    fieldName: the name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as a string value.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be found, or if it cannot successfully
        be converted to a string value.
";

%feature("docstring") gmsec::api::Message::getU16Field "

    getU16Field(self, name) -> U16Field

    Returns a reference to the U16Field associated with the named field.

    Returns
    -------
    A reference to an U16Field object.

    Exceptions
    ----------
    An Exception is thrown if the named field is NOT an U16Field or cannot be found.
";

%feature("docstring") gmsec::api::Message::getDoubleValue "

    getDoubleValue(self, fieldName) -> GMSEC_F64

    Attempts to convert the field value into an 64-bit floating-point number representation.

    Parameters
    ----------
    fieldName: the name of the field from which to reference the value.

    Returns
    -------
    Returns the field value as a floating-point value.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be found, or if it cannot successfully
        be converted to a float-point value.
";

%feature("docstring") gmsec::api::Message::addConfig "

    addConfig(self, config)

    This function adds configuration items to the message.  This can be used to
    pass middleware specific items to the Message object.

    Note: If a configuration item included within the given config is already associated
        with the message, then the message's configuration item will be replaced with the
        new item.

    Parameters
    ----------
    config: Config that contains configuration values to add to the Message.
";
