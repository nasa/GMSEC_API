/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::F64Field "

    Specialized Field class that can be used to store a 64-bit floating
    point value.

    CONSTRUCTOR:

    F64Field(self, name: str, value: double, isHeader: bool)

    Constructor for creating a specialized Field containing a
    64-bit floating point value

    Parameters
    ----------
    name     : The name of the Field
    value    : The floating point value to store
    isHeader : Used to indicate if Field is a header field (default is False)

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, is an empty string, or otherwise is non-compliant.
";


%feature("docstring") gmsec::api5::F64Field::getValue "

    get_value(self) -> double

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
