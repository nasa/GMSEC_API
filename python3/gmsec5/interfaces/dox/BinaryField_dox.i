/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::BinaryField "

    Specialized Field class that can be used to retain raw data.


    CONSTRUCTOR:

    BinaryField(name: str, blob: bytearray, len: int, isHeader: bool)

    Constructor for creating a specialized Field containing raw
    data.

    Parameters
    ----------
    name     : the name of the Field
    blob     : bytearray containing data
    len      : length of the bytearray
    isHeader : Used to indicate if Field is a header field (default is False)

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, is an empty string, or otherwise is non-compliant.
";


%feature("docstring") gmsec::api5::BinaryField::getValue "

    get_value(self) -> bytearray

    Exposes the underlying data held by the field.

    Returns
    -------
    Returns a pointer to the underlying data held by this field.
";


%feature("docstring") gmsec::api5::BinaryField::getLength "

    get_length(self) -> int

    Accessor to obtain the length of the raw data held by the field.

    Returns
    -------
    The length (size) of the data held by this field.
";
