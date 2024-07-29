/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::BooleanField "

    Specialized Field class that can be used to store a boolean value.


    CONSTRUCTOR:

    BooleanField(name: str, value: bool, isHeader: bool)

    Constructor for creating a specialized Field containing a boolean value.

    Parameters
    ----------
    name     : The name of the Field
    value    : The boolean value to store
    isHeader : Used to indicate if Field is a header field (default is False)

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, is an empty string, or otherwise is non-compliant.
";


%feature("docstring") gmsec::api5::BooleanField::getValue "

    get_value(self) -> bool

    Exposes the underlying boolean value held by the field.

    Returns
    -------
    Returns the boolean value associated with the field.
";

