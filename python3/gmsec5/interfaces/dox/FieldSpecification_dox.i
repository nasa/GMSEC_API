/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::FieldSpecification "

    This class is a container for holding information on a Field Specification.

    See Also
    --------
    MessageSpecification
    Specification
";


%feature("docstring") gmsec::api5::FieldSpecification::getName "

    get_name(self) -> str

    Accessor for the FieldSpecification name.

    Returns
    -------
    The FieldSpecification name as a string
";


%feature("docstring") gmsec::api5::FieldSpecification::getType"

    get_type(self) -> str

    Accessor for the FieldSpecification type (e.g. STRING, I16, etc.)

    Returns
    -------
    The FieldSpecification type as a string
";


%feature("docstring") gmsec::api5::FieldSpecification::getMode "

    get_mode(self) -> str

    Accessor for the FieldSpecification mode (e.g. REQUIRED, OPTIONAL, etc.)

    Returns
    -------
    The FieldSpecification mode as a string
";


%feature("docstring") gmsec::api5::FieldSpecification::getClassification "

    get_classification(self) -> str

    Accessor for the FieldSpecification classification.

    Returns
    -------
    The FieldSpecification classification as a string
";


%feature("docstring") gmsec::api5::FieldSpecification::getValue "

    get_value(self) -> str

    Accessor for the FieldSpecification default value, if any.

    Returns
    -------
    The FieldSpecification default value as a string, or an empty
    string if no value is available
";


%feature("docstring") gmsec::api5::FieldSpecification::getDescription "

    get_description(self) -> str

    Accessor for the FieldSpecification description

    Returns
    -------
    The FieldSpecification description as a string
";

