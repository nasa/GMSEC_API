/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::Field "

    Base class for all field types.
";


%feature("docstring") gmsec::api5::Field::getName "

    get_name(self) -> str

    Returns the name associated with the Field.

    Returns
    -------
    String containing Field name.
";


%feature("docstring") gmsec::api5::Field::getType "

    get_type(self) -> int

    Returns the type of the Field.

    Returns
    -------
    FieldType enumerated value.
";


%feature("docstring") gmsec::api5::Field::isHeader "

    is_header(self) -> bool

    Returns whether the Field represents a header field.

    Returns
    -------
    Returns True if a header field; False otherwise.
";


%feature("docstring") gmsec::api5::Field::toXML "

    to_xml(self) -> str

    Converts the Field into an XML string representation.

    Returns
    -------
    Returns an XML string.
";


%feature("docstring") gmsec::api5::Field::toJSON "

    to_json(self) -> str

    Converts the Field into a JSON string representation.

    Returns
    -------
    Returns an JSON string.
";


%feature("docstring") gmsec::api5::Field::getStringValue "

    get_string_value(self) -> str

    Attempts to convert the field value into string representation.

    Returns
    -------
    Returns the field value as a string.
";


%feature("docstring") gmsec::api5::Field::getBooleanValue "

    get_boolean_value(self) -> bool

    Attempts to convert the field value into boolean representation.

    Returns
    -------
    Returns the field value as a boolean.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be successfully converted to a boolean.
";


%feature("docstring") gmsec::api5::Field::getI64Value "

    get_integer_value(self) -> int

    Attempts to convert the field value into a signed 64-bit integer number representation.

    Returns
    -------
    Returns the field value as a signed 64-bit integer.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be successfully converted to a signed 64-bit integer.
";


%feature("docstring") gmsec::api5::Field::getF64Value "

    get_double_value(self) -> double

    Attempts to convert the field value into a 64-bit floating point number representation.

    Returns
    -------
    Returns the field value as a floating point number.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be successfully converted to a double.
";

