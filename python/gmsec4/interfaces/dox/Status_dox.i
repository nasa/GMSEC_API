
// File: classlibgmsec__python_1_1Status.xml


%feature("docstring") gmsec::api::Status "

    Returned by the API function calls to provide status feedback.  Status provides an error
    code and error message specific to the error type, but independent of error source.

    See Also
    --------
    Errors.h

    CONSTRUCTORS:

    Status(self, errorClass, errorCode, errorMsg)

    Parameters
    ----------
    errorClass: Enumerated value indicating source of the error.
    errorCode: Enumerated value indicating reason for the error.
    errorMsg: Explanation for the error.

    See Also
    --------
    Errors.h

    Status(self, errorClass, errorCode, errorMsg, customCode)

    Special constructor that can be used to set a custom error code.

    Parameters
    ----------
    errorClass: Enumerated value indicating source of the error.
    errorCode: Enumerated value indicating reason for the error.
    errorMsg: Explanation for the error.
    customCode: Custom error code (typically middleware specific) on the root cause of the error.

    Status(self, exception)

    Special constructor that uses information from an Exception object. 

    Parameters
    ----------
    exception: An Exception object to inherit information from

    Status(self, other)

    Copy constructor

    Parameters
    ----------
    other: The other Status object to copy.
";

%feature("docstring") gmsec::api::Status::reset "

    reset(self)

    This clears this Status to GMSEC_STATUS_NO_ERROR.
";

%feature("docstring") gmsec::api::Status::getCustomCode "

    getCustomCode(self) -> GMSEC_I32

    This will return the custom error code number of this status object.
";

%feature("docstring") gmsec::api::Status::setCode "

    setCode(self, code)

    This will set the specific error code.

    See Also
    --------
    Errors.h
";

%feature("docstring") gmsec::api::Status::setReason "

    setReason(self, reason)

    This will set the error string text.
";

%feature("docstring") gmsec::api::Status::getClass "

    getClass(self) -> gmsec::api::StatusClass

    This function will return the error class ID.

    See Also
    --------
    Errors.h
";

%feature("docstring") gmsec::api::Status::isError "

    isError(self) -> bool

    Used to determine whether the Status object is reporting an error or not.

    Note: Only the status class is examine; the status code is not checked.

    Returns
    -------
    false if the status class is set to NO_ERROR; true otherwise.
";

%feature("docstring") gmsec::api::Status::get "

    get(self) -> char const *

    This function will return a verbose error string that contains the
    Status class, code, custom code and reason.  The format is as follows:
    [class,code,custom code] : reason
";

%feature("docstring") gmsec::api::Status::getCode "

    getCode(self) -> gmsec::api::StatusCode

    This will return the error code number for this status for easy comparison.

    See Also
    --------
    Errors.h
";

%feature("docstring") gmsec::api::Status::getReason "

    getReason(self) -> char const *

    This function will retrieve the string detailed description of this error.
";

%feature("docstring") gmsec::api::Status::set "

    set(self, eclass, code, text=None, custom=0)

    This is a convience function that can be used to set class, code, error text and custom code.

    Parameters
    ----------
    eclass: error class
    code: error code
    text: error string (optional)
    custom: custom error code (optional)

    See Also
    --------
    Errors.h

";

%feature("docstring") gmsec::api::Status::setClass "

    setClass(self, eclass)

    This will set the erorr class ID.

    See Also
    --------
    Errors.h
";

%feature("docstring") gmsec::api::Status::setCustomCode "

    setCustomCode(self, code)

    This will set the specific error code.
";
