/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::MessageSpecification "

    This class is a container for holding information
    on a Message Specification

    See Also
    --------
    FieldSpecificationList
    FieldSpecification
    Specification
";

%feature("docstring") gmsec::api5::MessageSpecification::getSchemaID "

    get_schema_id(self) -> str

    Accessor for the MessageSpecification schema ID

    Returns
    -------
    The MessageSpecification schema ID as a string
";

%feature("docstring") gmsec::api5::MessageSpecification::getSubjectTemplate"

    get_subject_template(self) -> str

    Accessor for the MessageSpecification subject template

    Returns
    -------
    The MessageSpecification subject template as a string
";

%feature("docstring") gmsec::api5::MessageSpecification::getFieldSpecifications "

    get_field_specifications(self) -> FieldSpecificationList

    Accessor for acquiring the list of FieldSpecification objects
    associated with the MessageSpecification.

    Returns
    -------
    A FieldSpecificationList
";
