
%feature("docstring") gmsec::api::mist::Specification "

    The Specification class loads a list of templates from a designated
    directory that are used to create and validate message schemas.
    Each template has an associated ID that is used to identify the
    kind of message schema the template will be defining.

    See Also
    --------
    Config
    SchemaIDIterator
    MessageSpecificationList

    CONSTRUCTOR:

    Specification()

    Initializes the Specification with the default message specification (NASA/GMSEC Addendum).

    Exceptions
    ----------
    A GmsecError is thrown if, for whatever reason, the Specification cannot load message templates.


    Specification(config: Config)

    Initializes the Specification instance using the given configuration.

    Parameters
    ----------
    config : The Config used to configure the specification.

    Exceptions
    ----------
    A GmsecError is thrown if the given Config is null.
    A GmsecError is thrown if, for whatever reason, the Specification cannot load message templates.



    Specification(other: Specification)

    Copy-constructor for initializing a Specification instance.

    Parameters
    ----------
    other : The Specification to copy.

    Exceptions
    ----------
    A GmsecError is thrown if the given Specification is null.
";

%feature("docstring") gmsec::api::mist::Specification::validateMessage "

    validate_message(self, msg: Message)

    Looks up the message subject in the message registry to grab the
    appropriate template (based on its assigned schema ID).  The
    contents of the message are then compared to the template to
    ensure the message complies with the ISD. This function will also
    register the message with an appropriate schema ID if it is not
    found in the registry.

    Parameters
    ----------
    msg : The message to be validated.

    Exceptions
    ----------
    A GmsecError is thrown if the message fails to pass validation.
        A list of errors will be given of any issues found with the
        message.

";

%feature("docstring") gmsec::api::mist::Specification::getVersion "

    get_version(self) -> unsigned int

    Returns the version of the ISD being used as an unsigned integer.
";

%feature("docstring") gmsec::api::mist::Specification::getSchemaLevel"

    get_schema_level(self) -> SchemaLevel

    Returns the schema level of the message specification in use.
";

%feature("docstring") gmsec::api::mist::Specification::getSchemaIDIterator "

    get_schema_id_iterator(self) -> SchemaIDIterator

    Returns a SchemaIDIterator, which can be used to iterated over the
    IDs of the loaded templates.

    Note: Only one SchemaIDIterator is associated with a
    Specification; multiple calls to getSchemaIDIterator() will
    return a reference to the same SchemaIDIterator. Each call
    will reset the iterator.

    Returns
    -------
    A reference to a SchemaIDIterator
";

%feature("docstring") gmsec::api::mist::Specification::getMessageSpecifications"

    get_message_specifications(self) -> MessageSpecificationList

    Accessor that returns the list of MessageSpecification object(s)
    associated with the Specification.

    Example usage:

        config = libgmsec_python3.Config()

        config.add_value(\"gmsec-specification-version\", libgmsec_python3.GMSEC_ISD_CURRENT)

        spec = libgmsec_python3.Specification(config)

        msgSpecs = spec.get_message_specifications()

        for msgSpec in msgSpecs:
            fieldSpecs = msgSpec.get_field_specifications()

    Returns
    -------
    A MessageSpecificationList associated with the Specification
";

%feature("docstring") gmsec::api::mist::Specification::registerMessageValidator"

    registerMessageValidator(self, validator: MessageValidator) -> void

    Registers the give message validator to be used when message validation takes place.

    Example usage:

    class CustomMessageValidator(libgmsec_python3.MessageValidator):
        def __init__(self):
            libgmsec_python3.MessageValidator.__init__(self)

        def validate_message(self, message):
            status = libgmsec_python3.Status()
            # validate message
            if error:
                status.set(...)
            return status


    ...
    validator = CustomMessageValidator()
    ...
    specification.registerMessageValidator(validator)

";
