/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::I32Field "

    Specialized Field class that can be used to store a signed 32-bit
    value.


    CONSTRUCTOR:

    I32Field(self, name: str, value: int, isHeader: bool)

    Constructor for creating a specialized Field containing a
    signed 32-bit value

    Parameters
    ----------
    name     : The name of the Field
    value    : The value to store
    isHeader : Used to indicate if Field is a header field (default is False)

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";

%feature("docstring") gmsec::api5::I32Field::getValue "

    get_value(self) -> int

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
