
%feature("docstring") gmsec::api::I64Field "

    Specialized Field class that can be used to store a signed 64-bit
    value.


    CONSTRUCTOR:

    I64Field(self, name: str, value: longlong)

    Constructor for creating a specialized Field containing a
    signed 64-bit value

    Parameters
    ----------
    name  : The name of the Field
    value : The value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";

%feature("docstring") gmsec::api::I64Field::getValue "

    get_value(self) -> longlong

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
