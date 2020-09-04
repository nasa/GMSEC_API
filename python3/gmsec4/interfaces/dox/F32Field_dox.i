
%feature("docstring") gmsec::api::F32Field "

    Specialized Field class that can be used to store a 32-bit
    floating point value.


    CONSTRUCTOR:

    F32Field(self, name: str, value: float)

    Constructor for creating a specialized Field containing a
    32-bit floating point value.

    Parameters
    ----------
    name  : The name of the Field
    value : The floating point value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";


%feature("docstring") gmsec::api::F32Field::getValue "

    get_value(self) -> float

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";
