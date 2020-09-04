
// File: classlibgmsec__python_1_1I64Field.xml


%feature("docstring") gmsec::api::I64Field "

    Specialized Field class that can be used to store a signed 64-bit value.


    CONSTRUCTOR:

    I64Field(self, name, value)

    Constructor for creating a specialized Field object containing a signed 64-bit value

    Parameters
    ----------
    name: the name of the Field
    value: the value to store

    Exceptions
    ----------
    An Exception is thrown if the name is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::I64Field::getValue "

    getValue(self) -> GMSEC_I64

    Exposes the underlying value held by the object.

    Returns
    -------
    Returns the value associated with the object.
";

%feature("docstring") gmsec::api::I64Field::toXML "

    toXML(self) -> char const *

    Convenience method that returns the XML string representation of this object.

    Returns
    -------
    An XML string.
";

%feature("docstring") gmsec::api::I64Field::toJSON "

    toJSON(self) -> char const *

    Convenience method that returns the JSON string representation of this object.

    Returns
    -------
    A JSON string.
";
