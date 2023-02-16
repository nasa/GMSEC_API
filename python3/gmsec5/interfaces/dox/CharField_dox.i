/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::CharField "

    Specialized Field class that can be used to store a character value.


    CONSTRUCTOR:

    CharField(name: str, value: char, isHeader: bool)

    Constructor for creating a specialized Field containing a character value.

    Parameters
    ----------
    name     : The name of the Field
    value    : The character value to store
    isHeader : Used to indicate if Field is a header field (default is False)

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, or is an empty string.
";


%feature("docstring") gmsec::api5::CharField::getValue "

    get_value(self) -> char

    Exposes the underlying character value held by the field.

    Returns
    -------
    Returns the character value associated with the field.
";

