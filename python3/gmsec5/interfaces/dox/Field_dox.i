/*
 * Copyright 2007-2024 United States Government as represented by the
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


%feature("docstring") gmsec::api5::Field::isTracking "

    is_tracking(self) -> bool

    Returns whether the Field represents a GMSEC API tracking field.

    Returns
    -------
    Returns True if a tracking field; False otherwise.
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


%feature("docstring") gmsec::api5::Field::toBinaryField "

    toBinaryField(field: Field) -> BinaryField

    Downcasts the Field object into a BinaryField.

    Parameters
    ----------
    field : The field object to downcast to a BinaryField.

    Returns
    -------
    A BinaryField object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toBooleanField "

    toBooleanField(field: Field) -> BooleanField

    Downcasts the Field object into a BooleanField.

    Parameters
    ----------
    field : The field object to downcast to a BooleanField.

    Returns
    -------
    A BooleanField object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toCharField "

    toCharField(field: Field) -> CharField

    Downcasts the Field object into a CharField.

    Parameters
    ----------
    field : The field object to downcast to a CharField.

    Returns
    -------
    A CharField object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toF32Field "

    toF32Field(field: Field) -> F32Field

    Downcasts the Field object into an F32Field.

    Parameters
    ----------
    field : The field object to downcast to an F32Field.

    Returns
    -------
    An F32Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toF64Field "

    toF64Field(field: Field) -> F64Field

    Downcasts the Field object into an F64Field.

    Parameters
    ----------
    field : The field object to downcast to an F64Field.

    Returns
    -------
    An F64Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toI16Field "

    toI16Field(field: Field) -> I16Field

    Downcasts the Field object into an I16Field.

    Parameters
    ----------
    field : The field object to downcast to an I16Field.

    Returns
    -------
    An I16Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toI32Field "

    toI32Field(field: Field) -> I32Field

    Downcasts the Field object into an I32Field.

    Parameters
    ----------
    field : The field object to downcast to an I32Field.

    Returns
    -------
    An I32Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toI64Field "

    toI64Field(field: Field) -> I64Field

    Downcasts the Field object into an I64Field.

    Parameters
    ----------
    field : The field object to downcast to an I64Field.

    Returns
    -------
    An I64Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toI8Field "

    toI8Field(field: Field) -> I8Field

    Downcasts the Field object into an I8Field.

    Parameters
    ----------
    field : The field object to downcast to an I8Field.

    Returns
    -------
    An I8Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toStringField "

    toStringField(field: Field) -> StringField

    Downcasts the Field object into a StringField.

    Parameters
    ----------
    field : The field object to downcast to a StringField.

    Returns
    -------
    A StringField object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toU16Field "

    toU16Field(field: Field) -> U16Field

    Downcasts the Field object into a U16Field.

    Parameters
    ----------
    field : The field object to downcast to a U16Field.

    Returns
    -------
    A U16Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toU32Field "

    toU32Field(field: Field) -> U32Field

    Downcasts the Field object into a U32Field.

    Parameters
    ----------
    field : The field object to downcast to a U32Field.

    Returns
    -------
    A U32Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toU64Field "

    toU64Field(field: Field) -> U64Field

    Downcasts the Field object into a U64Field.

    Parameters
    ----------
    field : The field object to downcast to a U64Field.

    Returns
    -------
    A U64Field object, or None if the cast fails.
";


%feature("docstring") gmsec::api5::Field::toU8Field "

    toU8Field(field: Field) -> U8Field

    Downcasts the Field object into a U8Field.

    Parameters
    ----------
    field : The field object to downcast to a U8Field.

    Returns
    -------
    A U8Field object, or None if the cast fails.
";

