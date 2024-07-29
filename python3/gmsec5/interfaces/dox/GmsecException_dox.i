/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::GmsecException "

    This class defines the standard GMSEC exception.

    CONSTRUCTORS:

    GmsecError(self, errorClass: int, errorCode: int, errorMsg: str)

    Parameters
    ----------
    errorClass : Value indicating source of the error.
    errorCode  : Value indicating reason for the error.
    errorMsg   : Explanation for the error.



    GmsecError(self, errorClass: int, errorCode: int, customCode: int, errorMsg: str)

    Special constructor that can be used to set a custom error code.

    Parameters
    ----------
    errorClass : Value indicating source of the error.
    errorCode  : Value indicating reason for the error.
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


%feature("docstring") gmsec::api5::GmsecException::getErrorClass "

    get_error_class(self) -> int

    Returns the error class associated with the exception.

    Returns
    -------
    An integer value
";


%feature("docstring") gmsec::api5::GmsecException::getErrorCode "

    get_error_code(self) -> int

    Returns the error code associated with the exception.

    Returns
    -------
    An integer value
";


%feature("docstring") gmsec::api5::GmsecException::getCustomCode "

    get_custom_code(self) -> int

    An error code originating from a third party library (middleware
    or otherwise) related to the error that occurred.

    Returns
    -------
    An integer value
";


%feature("docstring") gmsec::api5::GmsecException::getErrorMessage "

    get_error_message(self) -> str

    Returns the error message associated with the exception.

    Returns
    -------
    A string containing the error message.
";


%feature("docstring") gmsec::api5::GmsecException::what "

    what(self) -> str

    Returns string with the format of:
        [errorClass,errorCode,customCode] : message

    Returns
    -------
    A string containing the error class, code, custom code and message.
";
