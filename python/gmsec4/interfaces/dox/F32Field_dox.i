
// File: classlibgmsec__python_1_1F32Field.xml


%feature("docstring") gmsec::api::F32Field "

    Specialized Field class that can be used to store a 32-bit floating point value.


    CONSTRUCTOR:

    F32Field(self, name, value)

    Constructor for creating a specialized Field object containing a 32-bit floating point value

    Parameters
    ----------
    name: the name of the Field
    value: the floating point value to store

    Exceptions
    ----------
    An Exception is thrown if the name is NULL, or is an empty string.
";


%feature("docstring") gmsec::api::F32Field::toXML "

    toXML(self) -> char const *

    Convenience method that returns the XML string representation of this object.

    Returns
    -------
    An XML string.
";


%feature("docstring") gmsec::api::F32Field::getValue "

    getValue(self) -> GMSEC_F32

    Exposes the underlying value held by the object.

    Returns
    -------
    Returns the value associated with the object.
";


%feature("docstring") gmsec::api::F32Field::toJSON "

    toJSON(self) -> char const *

    Convenience method that returns the JSON string representation of this object.

    Returns
    -------
    A JSON string.
";
