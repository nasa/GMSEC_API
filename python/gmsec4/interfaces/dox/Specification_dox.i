
// File: classlibgmsec__python_1_1Specification.xml


%feature("docstring") gmsec::api::mist::Specification "

    The Specification class loads a list of templates from a designated directory
    that are used to create and validate message schemas. Each template has an associated ID
    that is used to identify the kind of message schema the template will be defining.

    See Also
    --------
    SchemaIDIterator
";

%feature("docstring") gmsec::api::mist::Specification::validateMessage "

    validateMessage(self, msg)

    Looks up the message subject in the message registry to grab the
    appropriate template (based on its assigned schema ID).  The contents of the
    message are then compared to the template to ensure the message complies with
    the ISD. This function will also register the message with an appropriate schema
    ID if it is not found in the registry.

    Parameters
    ----------
    msg: The message to be validated.

    Exceptions
    ----------
    An exception is thrown if the message fails to pass validation.
        A list of errors will be given of any issues found with the message.

";

%feature("docstring") gmsec::api::mist::Specification::getTemplateXML "

    getTemplateXML(self, subject, schemaID) -> char const *

    Returns a message schema from the template with the matching schemaID.
    This message schema can be used as input for a Message data constructor

    Parameters
    ----------
    subject: message subject
    schemaID: id used to look up the appropriate template

    Returns
    -------
    XML string representation of the message schema.

    Exceptions
    ----------
    An exception is thrown if the provided schemaID is not available in the list of loaded schema.

";

%feature("docstring") gmsec::api::mist::Specification::getVersion "

    getVersion(self) -> unsigned int

    returns the version of the ISD being used as an unsigned integer.
";

%feature("docstring") gmsec::api::mist::Specification::getSchemaIDIterator "

    getSchemaIDIterator(self) -> SchemaIDIterator

    returns a SchemaIDIterator, which can be used to iterated over the IDs
    of the loaded templates.

    Note: Only one SchemaIDIterator object is associated with a Specification object; multiple calls to
        getSchemaIDIterator() will return a reference to the same SchemaIDIterator object.  Each call will reset the iterator.

    Returns
    -------
    a reference to a SchemaIDIterator object
";
