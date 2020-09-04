
%feature("docstring") gmsec::api::mist::message::MistMessage "

    This specialized Message shares all of the functionality of the standard Message class,
    but can also be auto-populated based on how the message schema identifies itself within
    the given specification.

    See Also
    --------
    Message
    Config
    Specification
    Field
    MessageFieldIterator

    CONSTRUCTORS:

    MistMessage(self, subject: str, schemaID: str, spec: Specification)

    Initializes the message instance with a template determined by ID and spec.

    Parameters
    ----------
    subject  : The subject string for the message.
    schemaID : The string used to identify the message schema in C2MS or other message specification
               (e.g. MSG.HB).
    spec     : A reference to the specification this message's schema will adhere to.

    Exceptions
    ----------
    A GmsecError is thrown if Specification fails to load the template directory or if
    schemaID is not a valid ID.



    MistMessage(self, subject: str, schemaID: str, config: Config, spec: Specification)

    Initializes the message instance with a template determined by ID, spec, and config.

    Parameters
    ----------
    subject  : The subject string for the message.
    schemaID : The string used to identify the message schema in C2MS or other message specification
               (e.g. MSG.HB).
    config   : Config to associate with the message.
    spec     : A reference to the specification this message's schema will adhere to.

    Exceptions
    ----------
    A GmsecError is thrown if Specification fails to load the template directory or if
    schemaID is not a valid ID.



    MistMessage(self, other: MistMessage)

    Copy constructor - initializes the message instance using the other given MistMessage.

    Parameters
    ----------
    other : The other MistMessage to be copied



    MistMessage(self, msg: Message)

    Constructor - initializes the message instance using the other given Message.

    Parameters
    ----------
    msg : The other Message to be copied

    Exceptions
    ----------
    A GmsecError is thrown if the given Message does not house sufficient information to construct a MistMessage.



    MistMessage(self, msg: Message, specConfig: Config)

    Constructor - initializes the message instance using the other given Message.

    Parameters
    ----------
    msg        : The other Message to be copied
    specConfig : The specification Config to apply when constructing the MistMessage

    Exceptions
    ----------
    A GmsecError is thrown if the given Message does not house sufficient information to construct a MistMessage.



    MistMessage(self, data: str)

    Initializes a MistMessage using an XML or JSON string.

    Parameters
    ----------
    data : XML of JSON string used to initialize the message.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.

    Note
    ----------
    This constructor has been deprecated. Use MistMessage(self, spec, data) instead.



    MistMessage(self, spec: Specification, data: str)

    Initializes a MistMessage using the given XML or JSON data representation of the message, and ensures
    the message adheres to the provided specification.

    Parameters
    ----------
    spec : The specification this message's schema will adhere to.
    data : XML of JSON string used to initialize the message.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.
";

%feature("docstring") gmsec::api::mist::message::MistMessage::setStandardFields "

    set_standard_fields(fieldList: FieldList)

    Sets the internal list of fields which are to be automatically
    placed in all MistMessage objects that are created. Internal
    copies of the Fields are made, and thus ownership of the fields
    that are provided are not retained by MistMessage.

    Parameters
    ----------
    fieldList : A FieldList containing one or more Fields

    See also:
    ----------
    clear_standard_fields()
";

%feature("docstring") gmsec::api::mist::message::MistMessage::clearStandardFields "

    clear_standard_fields()

    Destroys the lists of standard fields that are included with
    MistMessage objects.
";

%feature("docstring") gmsec::api::mist::message::MistMessage::setValue "

    set_value(self, fieldName: str, value: str)

    Sets the value for a field indicated by user. The value's type is
    automatically determined by the function depending on the message
    schema being used. If the field does not have a required type, a
    new StringField will be created with the specified name and value.

    Parameters
    ----------
    fieldName : Name of the field to be modified/created
    value     : The value of the field. The value's type is automatically
                determined based on the message schema.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is NULL or contains an empty string.
    A GmsecError is thrown if the input value's type cannot be converted into the type required
        by the field, or if the string is too big when converting to char.



    set_value(self, fieldName: str, value: longlong)

    Sets the value for a field indicated by user. The value's type is
    automatically determined by the function depending on the message
    schema being used. If the field does not have a required type, a
    new StringField will be created with the specified name and value.

    Parameters
    ----------
    fieldName : Name of the field to be modified/created
    value     : The value of the field. The value's type is automatically
                determined based on the message schema.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is NULL or contains an empty string.
    A GmsecError is thrown if the input value's type cannot be converted into the type required
        by the field, or if the string is too big when converting to char.



    set_value(self, fieldName: str, value: double)

    Sets the value for a field indicated by user. The value's type is
    automatically determined by the function depending on the message
    schema being used. If the field does not have a required type, a
    new StringField will be created with the specified name and value.

    Parameters
    ----------
    fieldName : Name of the field to be modified/created
    value     : The value of the field. The value's type is automatically
                determined based on the message schema.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is NULL or contains an empty string.
    A GmsecError is thrown if the input value's type cannot be converted into the type required
        by the field, or if the string is too big when converting to char.
";

%feature("docstring") gmsec::api::mist::message::MistMessage::getSchemaID "

    get_schema_id(self) -> str

    Returns a string that identifies the schema that the Message is based off of.
";
