
%feature("docstring") gmsec::api::Field "

    Base class for all field types.
";

%feature("docstring") gmsec::api::Field::getName "

    get_name(self) -> str

    Returns the name associated with the Field.

    Returns
    -------
    String containing Field name.
";

%feature("docstring") gmsec::api::Field::setName "

    set_name(self, name: str)

    Sets the field name with the given string.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is null or contains an empty string.
";

%feature("docstring") gmsec::api::Field::getType "

    get_type(self) -> int

    Returns the type of the Field.

    Returns
    -------
    FieldType enumerated value.
";

%feature("docstring") gmsec::api::Field::isHeader "

    is_header(self) -> bool

    Returns whether the Field represents a header field.

    Returns
    -------
    Returns True if a header field; False otherwise.
";

%feature("docstring") gmsec::api::Field::toXML "

    to_XML(self) -> str

    Converts the Field into an XML string representation.

    Returns
    -------
    Returns an XML string.
";

%feature("docstring") gmsec::api::Field::toJSON "

    to_JSON(self) -> str

    Converts the Field into a JSON string representation.

    Returns
    -------
    Returns an JSON string.
";

%feature("docstring") gmsec::api::Field::getIntegerValue "

    get_integer_value(self) -> int

    Attempts to convert the field value into a signed 64-bit integer number representation.

    Returns
    -------
    Returns the field value as an integer.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be successfully converted to an integer.
";

%feature("docstring") gmsec::api::Field::getUnsignedIntegerValue "

    get_unsigned_integer_value(self) -> unsigned int

    Attempts to convert the field value into an unsigned 64-bit integer number representation.

    Returns
    -------
    Returns the field value as an unsigned integer.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be successfully converted to an unsigned integer.
";

%feature("docstring") gmsec::api::Field::getDoubleValue "

    get_double_value(self) -> double

    Attempts to convert the field value into a 64-bit floating point number representation.

    Returns
    -------
    Returns the field value as a floating point number.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be successfully converted to a double.
";

%feature("docstring") gmsec::api::Field::getStringValue "

    get_string_value(self) -> str

    Attempts to convert the field value into string representation.

    Returns
    -------
    Returns the field value as a string.

    Exceptions
    ----------
    A GmsecError is thrown if the field cannot be successfully converted to a string.
";

%feature("docstring") gmsec::api::Field::clone "

    clone(self) -> Field

    Creates/returns a clone of the Field.

    Returns
    -------
    A clone of the Field.
";

%feature("docstring") gmsec::api::Field::destroyClone "

    destroy_clone(self, clone: Field) 

    Destroys the cloned Field.
";

