
%feature("docstring") gmsec::api::U16Field "

    Specialized Field class that can be used to store an unsigned
    16-bit value.


    CONSTRUCTOR:

    U16Field(self, name: str, value: ushort)

    Constructor for creating a specialized Field object containing an
    unsigned 16-bit value

    Parameters
    ----------
    name  : The name of the Field
    value : The value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";

%feature("docstring") gmsec::api::U16Field::getValue "

    get_value(self) -> ushort

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
