
%feature("docstring") gmsec::api::U8Field "

    Specialized Field class that can be used to store an unsigned 8-bit
    value.


    CONSTRUCTOR:

    U8Field(self, name: str, value: ubyte)

    Constructor for creating a specialized Field containing an
    unsigned 8-bit value

    Parameters
    ----------
    name  : The name of the Field
    value : The value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";

%feature("docstring") gmsec::api::U8Field::getValue "

    get_value(self) -> ubyte

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
