
%feature("docstring") gmsec::api::I16Field "

    Specialized Field class that can be used to store a signed 16-bit
    value.


    CONSTRUCTOR:

    I16Field(self, name: str, value: short)

    Constructor for creating a specialized Field containing a
    signed 16-bit value

    Parameters
    ----------
    name  : The name of the Field
    value : The value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";

%feature("docstring") gmsec::api::I16Field::getValue "

    get_value(self) -> short

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
