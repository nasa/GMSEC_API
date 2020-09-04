
// File: classlibgmsec__python_1_1U16Field.xml


%feature("docstring") gmsec::api::U16Field "

    Specialized Field class that can be used to store an unsigned 16-bit value.


    CONSTRUCTOR:

    U16Field(self, name, value)

    Constructor for creating a specialized Field object containing an unsigned 16-bit value

    Parameters
    ----------
    name: the name of the Field
    value: the value to store

    Exceptions
    ----------
    An Exception is thrown if the name is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::U16Field::toJSON "

    toJSON(self) -> char const *

    Convenience method that returns the JSON string representation of this object.

    Returns
    -------
    A JSON string.
";

%feature("docstring") gmsec::api::U16Field::getValue "

    getValue(self) -> GMSEC_U16

    Exposes the underlying value held by the object.

    Returns
    -------
    Returns the value associated with the object.
";

%feature("docstring") gmsec::api::U16Field::toXML "

    toXML(self) -> char const *

    Convenience method that returns the XML string representation of this object.

    Returns
    -------
    An XML string.
";
