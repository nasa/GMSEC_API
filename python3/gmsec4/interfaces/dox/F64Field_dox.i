
%feature("docstring") gmsec::api::F64Field "

    Specialized Field class that can be used to store a 64-bit floating
    point value.

    CONSTRUCTOR:

    F64Field(self, name: str, value: double)

    Constructor for creating a specialized Field containing a
    64-bit floating point value

    Parameters
    ----------
    name  : The name of the Field
    value : The floating point value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, or is an empty string.
";


%feature("docstring") gmsec::api::F64Field::getValue "

    get_value(self) -> double

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
