/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::Specification "

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

    Initializes the Specification with the default message specification.

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

%feature("docstring") gmsec::api5::Specification::getSchemaIDIterator "

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

%feature("docstring") gmsec::api5::Specification::getVersion "

    get_version(self) -> unsigned int

    Returns the version of the message specification that is referenced by the Specification.
";

%feature("docstring") gmsec::api5::Specification::getSchemaLevel"

    get_schema_level(self) -> SchemaLevel

    Returns the schema level of the message specification in use.
";

%feature("docstring") gmsec::api5::Specification::getValidationLevel "

    get_validation_level(self) -> int

    Returns the validation level for the specification.

    Returns
    -------
    Returns the validation level for the specification.
";

%feature("docstring") gmsec::api5::Specification::getMessageSpecifications"

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

%feature("docstring") gmsec::api5::Specification::getHeaderFieldNames"

    getHeaderFieldNames(self) -> HeaderList

    Returns a list of the field names for the default header (which is the topmost schema level currently configured)

    Returns
    -------
    A list that contains the names of the header fields.



    getHeaderFieldNames(self, schemaName: str) -> HeaderList

    Returns a list of the field names for the specified header 

    Parameters
    ----------
    schemaName : The name of the specified schema level (e.g. C2MS, C2MS-PIPE, GMSEC, DEFAULT).  An empty string refers to DEFAULT.

    Returns
    -------
    A list that contains the names of the header fields.
";
