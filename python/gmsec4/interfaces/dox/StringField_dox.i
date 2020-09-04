
// File: classlibgmsec__python_1_1StringField.xml


%feature("docstring") gmsec::api::StringField "

    Specialized Field class that can be used to store a string value.


    CONSTRUCTOR:

    StringField(self, name, value)

    Constructor for creating a specialized Field object containing a string value

    Parameters
    ----------
    name: the name of the Field
    value: the string value to store

    Exceptions
    ----------
    An Exception is thrown if the name is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::StringField::toJSON "

    toJSON(self) -> char const *

    Convenience method that returns the JSON string representation of this object.

    Returns
    -------
    A JSON string.
";

%feature("docstring") gmsec::api::StringField::toXML "

    toXML(self) -> char const *

    Convenience method that returns the XML string representation of this object.

    Returns
    -------
    An XML string.
";

%feature("docstring") gmsec::api::StringField::getValue "

    getValue(self) -> char const *

    Exposes the underlying value held by the object.

    Returns
    -------
    Returns the value associated with the object.
";

