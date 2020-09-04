
// File: classlibgmsec__python_1_1CharField.xml


%feature("docstring") gmsec::api::CharField "

    Specialized Field class that can be used to store a character value.


    CONSTRUCTOR:

    CharField(self, name, value)

    Constructor for creating a specialized Field object containing a character value.

    Parameters
    ----------
    name: the name of the Field
    value: the character value to store

    Exceptions
    ----------
    An Exception is thrown if the name is NULL, or is an empty string.
";



%feature("docstring") gmsec::api::CharField::toJSON "

    toJSON(self) -> char const *

    Convenience method that returns the JSON string representation of this object.

    Returns
    -------
    A JSON string.
";

%feature("docstring") gmsec::api::CharField::toXML "

    toXML(self) -> char const *

    Convenience method that returns the XML string representation of this object.

    Returns
    -------
    An XML string.
";

%feature("docstring") gmsec::api::CharField::getValue "

    getValue(self) -> GMSEC_CHAR

    Exposes the underlying character value held by the object.

    Returns
    -------
    Returns the character value associated with the object.
";


