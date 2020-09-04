
%feature("docstring") gmsec::api::mist::ProductFile "

    This class is a lightweight container for holding information
    on a Product File, and used to generate GMSEC Product File messages
    by the ConnectionManager class

    See Also
    --------
    ConnectionManager

    CONSTRUCTOR:

    ProductFile(name: str, description: str, version: str, format: str, uri: str)

    Initializes the ProductFile with a URI to describe a
    commonly accessible resource.

    Parameters
    ----------
    name        : The name of the file
    description : The description of the file
    version     : The version of the file
    format      : The descriptor for the file format
    uri         : The URI for the file

    Exceptions
    ----------
    A GmsecError is thrown if name, description, version, or format are None or empty strings.



    ProductFile(name: str, description: str, version: str, format: str, data: bytearray, fileSize: int)

    Initializes the ProductFile with the binary contents of a file.

    Parameters
    ----------
    name        : The name of the file
    description : The description of the file
    version     : The version of the file
    format      : The descriptor for the file format
    data        : The binary contents of the file
    fileSize    : The size of the binary contents of the file

    Exceptions
    ----------
    A GmsecError is thrown if name, description, version, or format are None or empty strings.



    ProductFile(other: ProductFile)

    Copy constructor - Initializes the ProductFile with the information from another ProjectFile.

    Parameters
    ----------
    other : The ProductFile to copy
";

%feature("docstring") gmsec::api::mist::ProductFile::getDescription "

    get_description(self) -> str

    Retrieves the description of the ProductFile

    Returns
    -------
    The description of the ProductFile
";

%feature("docstring") gmsec::api::mist::ProductFile::getName "

    get_name(self) -> str

    Retrieves the name of the ProductFile

    Returns
    -------
    The name of the ProductFile
";

%feature("docstring") gmsec::api::mist::ProductFile::getFormat "

    get_format(self) -> str

    Retrieves the format of the ProductFile

    Returns
    -------
    The format of the ProductFile
";

%feature("docstring") gmsec::api::mist::ProductFile::__ne__ "

    __ne__(self, productFile) -> bool

    Parameters
    ----------
    productFile: gmsec::api::mist::ProductFile const &
";

%feature("docstring") gmsec::api::mist::ProductFile::getURI "

    get_URI(self) -> str

    Accessor for the ProductFile status

    Returns
    -------
    A pointer to the ProductFile URI string, or None if he URI is not available.
";

%feature("docstring") gmsec::api::mist::ProductFile::uriAvailable "

    URI_available(self) -> bool

    Returns the availability of a ProductFile URI

    Returns
    -------
    True if a ProductFile URI has been set, False otherwise
";

%feature("docstring") gmsec::api::mist::ProductFile::contentsAvailable "

    contents_available(self) -> bool

    Returns the availability of a ProductFile file content

    Returns
    -------
    True if a ProductFile file content has been set, False otherwise
";

%feature("docstring") gmsec::api::mist::ProductFile::getVersion "

    get_version(self) -> str

    Retrieves the version of the ProductFile

    Returns
    -------
    The version of the ProductFile
";

%feature("docstring") gmsec::api::mist::ProductFile::__eq__ "

    __eq__(self, productFile) -> bool

    Parameters
    ----------
    productFile: gmsec::api::mist::ProductFile const &
";

%feature("docstring") gmsec::api::mist::ProductFile::getContents "

    get_contents(self, contents) -> bytearray

    Accessor for the ProductFile file content

    Returns
    -------
    A reference to the ProductFile file content

    Exceptions
    ----------
    A GmsecError is thrown if the file content has not been set
";
