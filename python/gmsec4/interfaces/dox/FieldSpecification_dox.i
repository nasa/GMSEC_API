
%feature("docstring") gmsec::api::mist::FieldSpecification "

    This class is a container for holding information
    on a Field Specification

    See Also
    --------
    MessageSpecification
    Specification
";

%feature("docstring") gmsec::api::mist::FieldSpecification::getName "

    getName(self) -> string

    Accessor for the FieldSpecification name

    Returns
    -------
    The FieldSpecification name as a string
";

%feature("docstring") gmsec::api::mist::FieldSpecification::getType"

    getType(self) -> string

    Accessor for the FieldSpecification type (e.g. STRING, I16, etc.)

    Returns
    -------
    The FieldSpecification type as a string
";

%feature("docstring") gmsec::api::mist::FieldSpecification::getMode "

    getMode(self) -> string

    Accessor for the FieldSpecification mode (e.g. REQUIRED, OPTIONAL, etc.)

    Returns
    -------
    The FieldSpecification mode as a string
";

%feature("docstring") gmsec::api::mist::FieldSpecification::getClassification "

    getClassification(self) -> string

    Accessor for the FieldSpecification classification

    Returns
    -------
    The FieldSpecification classification as a string
";

%feature("docstring") gmsec::api::mist::FieldSpecification::getValue "

    getValue(self) -> string

    Accessor for the FieldSpecification default value, if any

    Returns
    -------
    The FieldSpecification default value as a string, or an empty string if no value is available
";

%feature("docstring") gmsec::api::mist::FieldSpecification::getDescription "

    getDescription(self) -> string

    Accessor for the FieldSpecification description

    Returns
    -------
    The FieldSpecification description as a string
";

