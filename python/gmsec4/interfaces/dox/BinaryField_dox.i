
// File: classlibgmsec__python_1_1BinaryField.xml


%feature("docstring") gmsec::api::BinaryField "

    Specialized Field class that can be used to retain raw data.


    CONSTRUCTOR:

    BinaryField(self, name, blob, length)

    Constructor for creating a specialized Field object containing raw data.

    Parameters
    ----------
    name: the name of the Field
    blob: the raw data
    length: the length (size) of the raw data

    Exceptions
    ----------
    An Exception is thrown if the name is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::BinaryField::getLength "

    getLength(self) -> size_t

    Accessor to obtain the length of the raw data held by the object.

    Returns
    -------
    The length (size) of the data held by this object.
";

%feature("docstring") gmsec::api::BinaryField::getValue "

    getValue(self) -> std::string

    Exposes the underlying data held by the object.

    Returns
    -------
    Returns a pointer to the underlying data held by this object.
";


%feature("docstring") gmsec::api::BinaryField::toXML "

    toXML(self) -> char const *

    Convenience method that returns the XML string representation of this object.

    Returns
    -------
    An XML string.
";

%feature("docstring") gmsec::api::BinaryField::toJSON "

    toJSON(self) -> char const *

    Convenience method that returns the JSON string representation of this object.

    Returns
    -------
    A JSON string.
";

