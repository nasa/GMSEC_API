
%feature("docstring") gmsec::api::CharField "

    Specialized Field class that can be used to store a character value.


    CONSTRUCTOR:

    CharField(name: str, value: char)

    Constructor for creating a specialized Field containing a character value.

    Parameters
    ----------
    name  : The name of the Field
    value : The character value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::CharField::getValue "

    get_value(self) -> char

    Exposes the underlying character value held by the field.

    Returns
    -------
    Returns the character value associated with the field.
";

