
%feature("docstring") gmsec::api::StringField "

    Specialized Field class that can be used to store a string value.


    CONSTRUCTOR:

    StringField(name, value)

    Constructor for creating a specialized Field containing a string value

    Parameters
    ----------
    name  : The name of the Field
    value : The string value to store

    Exceptions
    ----------
    A GmsecError is thrown if the name is None, or is an empty string.
";

%feature("docstring") gmsec::api::StringField::getValue "

    get_value(self) -> str

    Exposes the underlying value held by the field.

    Returns
    -------
    Returns the value associated with the field.
";

