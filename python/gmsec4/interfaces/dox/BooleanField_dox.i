
// File: classlibgmsec__python_1_1BooleanField.xml


%feature("docstring") gmsec::api::BooleanField "

    Specialized Field class that can be used to store a boolean value.


    CONSTRUCTOR:

    BooleanField(self, name, value)

    Constructor for creating a specialized Field object containing a boolean value.

    Parameters
    ----------
    name: the name of the Field
    value: the boolean value to store

    Exceptions
    ----------
    An Exception is thrown if the name is NULL, or is an empty string.
";



%feature("docstring") gmsec::api::BooleanField::getValue "

    getValue(self) -> bool

    Exposes the underlying boolean value held by the object.

    Returns
    -------
    Returns the boolean value associated with the object.
";

%feature("docstring") gmsec::api::BooleanField::toJSON "

    toJSON(self) -> char const *

    Convenience method that returns the JSON string representation of this object.

    Returns
    -------
    A JSON string.
";


