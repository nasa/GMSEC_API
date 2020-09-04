
// File: classlibgmsec__python_1_1ProductFile.xml


%feature("docstring") gmsec::api::mist::ProductFile "

    This class is a lightweight container for holding information
    on a Product File, and used to generate GMSEC Product File messages by the
    ConnectionManager class

    See Also
    --------
    ConnectionManager

    CONSTRUCTOR:

    ProductFile(self, pName, pDescription, pVersion, pFormat, pUri)

    Initializes the ProductFile object with a URI to describe a commonly accessible resource.

    Parameters
    ----------
    pName: The name of the file
    pDescription: The description of the file
    pVersion: The version of the file
    pFormat: The descriptor for the file format
    pUri: The URI for the file

    Exceptions
    ----------
    An exception is thrown if pName, pDescription, pVersion, or pFormat are NULL or empty strings.

    ProductFile(self, pName, pDescription, pVersion, pFormat, pData, pFileSize)

    Initializes the ProductFile object with the binary contents of a file.

    Parameters
    ----------
    pName: The name of the file
    pDescription: The description of the file
    pVersion: The version of the file
    pFormat: The descriptor for the file format
    pData: The binary contents of the file
    pFileSize: The size of the binary contents of the file

    Exceptions
    ----------
    An exception is thrown if pName, pDescription, pVersion, or pFormat are NULL or empty strings.

    ProductFile(self, other)

    Copy constructor - Initializes the ProductFile object with the information from another ProjectFile.

    Parameters
    ----------
    other: The other ProductFile object
";

%feature("docstring") gmsec::api::mist::ProductFile::getDescription "

    getDescription(self) -> char const *

    Retrieves the description of the ProductFile

    Returns
    -------
    The description of the ProductFile
";

%feature("docstring") gmsec::api::mist::ProductFile::getName "

    getName(self) -> char const *

    Retrieves the name of the ProductFile

    Returns
    -------
    The name of the ProductFile
";

%feature("docstring") gmsec::api::mist::ProductFile::getFormat "

    getFormat(self) -> char const *

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

    getURI(self) -> char const *

    Accessor for the ProductFile status

    Returns
    -------
    A pointer to the ProductFile URI string, or NULL if he URI is not available.
";

%feature("docstring") gmsec::api::mist::ProductFile::uriAvailable "

    uriAvailable(self) -> bool

    Returns the availability of a ProductFile URI

    Returns
    -------
    True if a ProductFile URI has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::ProductFile::contentsAvailable "

    contentsAvailable(self) -> bool

    Returns the availability of a ProductFile file content

    Returns
    -------
    True if a ProductFile file content has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::ProductFile::getVersion "

    getVersion(self) -> char const *

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

    getContents(self, contents) -> size_t

    Accessor for the ProductFile file content

    Returns
    -------
    A reference to the ProductFile file content

    Exceptions
    ----------
    An exception is thrown if the file content has not been set
";
