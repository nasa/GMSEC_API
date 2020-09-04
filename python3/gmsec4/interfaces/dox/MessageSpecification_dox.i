
%feature("docstring") gmsec::api::mist::MessageSpecification "

    This class is a container for holding information
    on a Message Specification

    See Also
    --------
    FieldSpecificationList
    FieldSpecification
    Specification
";

%feature("docstring") gmsec::api::mist::MessageSpecification::getSchemaID "

    get_schema_id(self) -> str

    Accessor for the MessageSpecification schema ID

    Returns
    -------
    The MessageSpecification schema ID as a string
";

%feature("docstring") gmsec::api::mist::MessageSpecification::getFieldSpecifications "

    get_field_specifications(self) -> FieldSpecificationList

    Accessor for acquiring the list of FieldSpecification objects
    associated with the MessageSpecification.

    Returns
    -------
    A FieldSpecificationList
";
