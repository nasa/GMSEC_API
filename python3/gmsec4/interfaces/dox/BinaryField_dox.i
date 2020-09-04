
%feature("docstring") gmsec::api::BinaryField "

    Specialized Field class that can be used to retain raw data.


    CONSTRUCTOR:

    BinaryField(name: str, blob: bytearray, len: int)

    Constructor for creating a specialized Field containing raw
    data.

    Parameters
    ----------
    name : the name of the Field
    blob : bytearray containing data
    len  : length of the bytearray

    Exceptions
    ----------
    A GmsecError is thrown if the name is NULL, or is an empty string.
";

%feature("docstring") gmsec::api::BinaryField::getValue "

    get_value(self) -> bytearray

    Exposes the underlying data held by the field.

    Returns
    -------
    Returns a pointer to the underlying data held by this field.
";

%feature("docstring") gmsec::api::BinaryField::getLength "

    get_length(self) -> int

    Accessor to obtain the length of the raw data held by the field.

    Returns
    -------
    The length (size) of the data held by this field.
";
