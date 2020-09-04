
%feature("docstring") gmsec::api::BooleanField "

    Specialized Field class that can be used to store a boolean value.


    CONSTRUCTOR:

    BooleanField(name: str, value: bool)

    Constructor for creating a specialized Field containing a boolean value.

    Parameters
    ----------
    name  : The name of the Field
    value : The boolean value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::BooleanField::getValue "

    get_value(self) -> bool

    Exposes the underlying boolean value held by the field.

    Returns
    -------
    Returns the boolean value associated with the field.
";

