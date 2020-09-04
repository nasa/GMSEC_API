
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

>>> MistMessage(self, subject, schemaID, spec)

    Initializes the message instance with a template determined by ID and spec.

    Parameters
    ----------
    subject: The subject string for the message.
    schemaID: The string used to identify the message schema in the GMSEC ISD. The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2016.00.GMSEC.MSG.C2CX.HB). Not all schemaIDs contain a messageSubtype.
    spec: A reference to the specification this message's schema will adhere to.

    Exceptions
    ----------
    An Exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.



>>> MistMessage(self, subject, schemaID, config, spec)

    Initializes the message instance with a template determined by ID, spec, and config.

    Parameters
    ----------
    subject: The subject string for the message.
    schemaID: The string used to identify the message schema in the GMSEC ISD. The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2016.00.GMSEC.MSG.C2CX.HB). Not all schemaIDs contain a messageSubtype.
    config: A configuration to associate with the message.
    spec: A reference to the specification this message's schema will adhere to.

    Exceptions
    ----------
    An Exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.



>>> MistMessage(self, other)

    Copy constructor - initializes the message instance using the other given MistMessage.

    Parameters
    ----------
    other: The other MistMessage to be copied



>>> MistMessage(self, msg)

    Constructor - initializes the message instance using the other given Message.

    Parameters
    ----------
    msg: The other Message to be copied

    Exceptions
    ----------
    An Exception is thrown if the given Message does not house sufficient information to construct a MistMessage.



>>> MistMessage(self, msg, specConfig)

    Constructor - initializes the message instance using the other given Message.

    Parameters
    ----------
    msg: The other Message to be copied
    specConfig: The specification configuration to apply when constructing the MistMessage

    Exceptions
    ----------
    An Exception is thrown if the given Message does not house sufficient information to construct a MistMessage.



>>> MistMessage(self, data)

    Initializes a MistMessage using an XML or JSON string.

    Parameters
    ----------
    data: XML of JSON string used to initialize the message.

    Exceptions
    ----------
    An Exception is thrown if the given data does not represent a valid XML or JSON statement.

    Note
    ----------
    This constructor has been deprecated. Use MistMessage(self, spec, data) instead.



>>> MistMessage(self, spec, data)

    Initializes a MistMessage using the given XML or JSON data representation of the message, and ensures the message adheres to the provided specification.

    Parameters
    ----------
    spec: The specification this message's schema will adhere to.
    data: XML of JSON string used to initialize the message.

    Exceptions
    ----------
    An Exception is thrown if the given data does not represent a valid XML or JSON statement.
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
