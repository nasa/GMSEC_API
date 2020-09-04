
// File: classlibgmsec__python_1_1MistMessage.xml


%feature("docstring") gmsec::api::mist::message::MistMessage "

    This Message object shares all of the functionality of standard messages,
    but can also be auto-populated based on how the message schema identifies itself within
    the given specification.

    See Also
    --------
    Message
    Config
    Specification
    Field
    MsgFieldIterator

    CONSTRUCTORS:

    MistMessage(self, subject, schemaID, spec)

    Initializes the message instance with a template determined by ID and spec.

    Parameters
    ----------
    subject: The subject string for the message.
    schemaID: The string used to identify the message schema in the GMSEC ISD. The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2016.00.GMSEC.MSG.C2CX.HB). Not all schemaIDs contain a messageSubtype.
    spec: A reference to the specification this message's schema will adhere to.

    Exceptions
    ----------
    An  exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.

    MistMessage(self, subject, schemaID, spec, config)

    Initializes the message instance with a template determined by ID, spec, and config.

    Parameters
    ----------
    subject: The subject string for the message.
    schemaID: The string used to identify the message schema in the GMSEC ISD. The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2016.00.GMSEC.MSG.C2CX.HB). Not all schemaIDs contain a messageSubtype.
    spec: A reference to the specification this message's schema will adhere to.
    config: A configuration to associate with the message.

    Exceptions
    ----------
    An  exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.

    Message(self, other)

    Copy constructor - initializes the message instance using the other given MistMessage.

    Parameters
    ----------
    other: The message to be copied

    Message(self, data)

    Initializes a MistMessage using an XML or JSON string.

    Parameters
    ----------
    data: XML of JSON string used to initialize the message.

    Exceptions
    ----------
    An  exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
";

%feature("docstring") gmsec::api::mist::message::MistMessage::setValue "

    setValue(self, fieldName, value)

    Sets the value for a field indicated by user.  The value's type is automatically
    determined by the function depending on the message schema being used.  If the field
    does not have a required type, a new StringField will be created with the specified name
    and value.

    Parameters
    ----------
    fieldName: name of the field to be modified/created
    value: the value of the field.  The value's type is automatically determined
        based on the message schema.

";

%feature("docstring") gmsec::api::mist::message::MistMessage::getSchemaID "

    getSchemaID(self) -> char const *

    Returns a string that identifies the schema that the Message is based off of.
";
