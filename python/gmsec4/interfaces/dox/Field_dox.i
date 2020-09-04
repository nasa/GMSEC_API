
// File: classlibgmsec__python_1_1Field.xml


%feature("docstring") gmsec::api::Field "

    Base class for all field types.
";

%feature("docstring") gmsec::api::Field::getDoubleValue "

    getDoubleValue(self) -> GMSEC_F64

    Attempts to convert the field value into a 64-bit floating point number representation.

    Returns
    -------
    Returns the field value as a floating point number.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be successfully converted to a double.
";

%feature("docstring") gmsec::api::Field::getType "

    getType(self) -> gmsec::api::Field::FieldType

    Returns the type of the Field object.

    Returns
    -------
    FieldType enumerated value.
";

%feature("docstring") gmsec::api::Field::getIntegerValue "

    getIntegerValue(self) -> GMSEC_I64

    Attempts to convert the field value into a signed 64-bit integer number representation.

    Returns
    -------
    Returns the field value as an integer.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be successfully converted to an integer.
";

%feature("docstring") gmsec::api::Field::toXML "

    toXML(self) -> char const *

    Converts the Field object into an XML string representation.

    Returns
    -------
    Returns an XML string.
";

%feature("docstring") gmsec::api::Field::getUnsignedIntegerValue "

    getUnsignedIntegerValue(self) -> GMSEC_U64

    Attempts to convert the field value into an unsigned 64-bit integer number representation.

    Returns
    -------
    Returns the field value as an unsigned integer.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be successfully converted to an unsigned integer.
";

%feature("docstring") gmsec::api::Field::getName "

    getName(self) -> char const *

    Returns the name associated with the Field object.

    Returns
    -------
    String containing Field name.
";

%feature("docstring") gmsec::api::Field::toJSON "

    toJSON(self) -> char const *

    Converts the Field object into a JSON string representation.

    Returns
    -------
    Returns an JSON string.
";

%feature("docstring") gmsec::api::Field::isHeader "

    isHeader(self) -> bool

    Returns whether the Field represents a header field.

    Returns
    -------
    Returns true if a header field; false otherwise.
";

%feature("docstring") gmsec::api::Field::getStringValue "

    getStringValue(self) -> char const *

    Attempts to convert the field value into string representation.

    Returns
    -------
    Returns the field value as a string.

    Exceptions
    ----------
    An Exception is thrown if the field cannot be successfully converted to a string.
";
