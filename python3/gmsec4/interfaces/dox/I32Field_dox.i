
%feature("docstring") gmsec::api::I32Field "

    Specialized Field class that can be used to store a signed 32-bit
    value.


    CONSTRUCTOR:

    I32Field(self, name: str, value: int)

    Constructor for creating a specialized Field containing a
    signed 32-bit value

    Parameters
    ----------
    name  : The name of the Field
    value : The value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";

%feature("docstring") gmsec::api::I32Field::getValue "

    get_value(self) -> int

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
