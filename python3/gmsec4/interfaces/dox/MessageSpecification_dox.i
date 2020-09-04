
%feature("docstring") gmsec::api::mist::MessageSpecification "

    This class is a container for holding information
    on a Message Specification

    See Also
    --------
    FieldSpecification
    Specification
";

%feature("docstring") gmsec::api::mist::MessageSpecification::getSchemaID "

    getSchemaID(self) -> string

    Accessor for the MessageSpecification schema ID

    Returns
    -------
    The MessageSpecification schema ID as a string
";

%feature("docstring") gmsec::api::mist::MessageSpecification::getFieldSpecifications "

    getFieldSpecifications(self) -> list

    Accessor for acquiring the list of FieldSpecification objects
    associated with the MessageSpecification

    Returns
    -------
    A list of FieldSpecification(s)
";
