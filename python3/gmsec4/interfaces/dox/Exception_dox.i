
%feature("docstring") gmsec::api::Exception "

    This class defines the standard GMSEC exception.

    CONSTRUCTORS:

    GmsecError(self, errorClass: int, errorCode: int, errorMsg: str)

    Parameters
    ----------
    errorClass : Enumerated value indicating source of the error.
    errorCode  : Enumerated value indicating reason for the error.
    errorMsg   : Explanation for the error.



    GmsecError(self, errorClass: int, errorCode: int, customCode: int, errorMsg: str)

    Special constructor that can be used to set a custom error code.

    Parameters
    ----------
    errorClass : Enumerated value indicating source of the error.
    errorCode  : Enumerated value indicating reason for the error.
    customCode : Custom error code (typically middleware specific) on the root cause of the error.
    errorMsg   : Explanation for the error.



    GmsecError(self, status: Status)

    Special constructor that uses information from a Status object. 

    Parameters
    ----------
    status : A Status object



    GmsecError(self, other: GmsecError)

    Copy constructor

    Parameters
    ----------
    other : The other GmsecError object to copy.
";

%feature("docstring") gmsec::api::Exception::getErrorCode "

    getErrorCode(self) -> int

    Returns the error code associated with the exception.

    Returns
    -------
    A StatusCode enumerated value.
";

%feature("docstring") gmsec::api::Exception::what "

    what(self) -> str

    Returns string with the format of:
        [errorClass, errorCode, customCode] : message

    Returns
    -------
    A string containing the error class, code, custom code and message.
";

%feature("docstring") gmsec::api::Exception::getCustomCode "

    getCustomCode(self) -> int

    An error code originating from a third party library (middleware
    or otherwise) related to the error that occurred.

    Returns
    -------
    A StatusCode enumerated value.
";

%feature("docstring") gmsec::api::Exception::getErrorClass "

    getErrorClass(self) -> int

    Returns the error class associated with the exception.

    Returns
    -------
    A StatusClass enumerated value.
";

%feature("docstring") gmsec::api::Exception::getErrorMessage "

    getErrorMessage(self) -> str

    Returns the error message associated with the exception.

    Returns
    -------
    A string containing the error message.
";
