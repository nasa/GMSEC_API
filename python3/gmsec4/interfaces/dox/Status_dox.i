
%feature("docstring") gmsec::api::Status "

    Returned by the API function calls to provide status feedback.
    Status provides an error code and error message specific to the
    error type, but independent of error source.

    CONSTRUCTORS:

    Status(errorClass: int, errorCode: int, errorMsg: str)

    Parameters
    ----------
    errorClass : Enumerated value indicating source of the error
    errorCode  : Enumerated value indicating reason for the error
    errorMsg   : Explanation for the error



    Status(errorClass: int, errorCode: int, errorMsg: str, customCode: int)

    Special constructor that can be used to set a custom error code.

    Parameters
    ----------
    errorClass : Enumerated value indicating source of the error
    errorCode  : Enumerated value indicating reason for the error
    errorMsg   : Explanation for the error
    customCode : Custom error code (typically middleware specific) on the root cause of the error



    Status(self, exception: GmsecError)

    Special constructor that uses information from a GmsecError. 

    Parameters
    ----------
    exception : An GmsecError to inherit information from



    Status(self, other: Status)

    Copy constructor

    Parameters
    ----------
    other : The other Status to copy.
";

%feature("docstring") gmsec::api::Status::reset "

    reset(self)

    This clears this Status to GMSEC_STATUS_NO_ERROR.
";

%feature("docstring") gmsec::api::Status::getCustomCode "

    get_custom_code(self) -> int

    This will return the custom error code number of this Status.
";

%feature("docstring") gmsec::api::Status::setCode "

    set_code(self, errorCode: int)

    This will set the specific error code.

    Parameters
    ----------
    errorCode : Enumerated value indicating reason for the error
";

%feature("docstring") gmsec::api::Status::setReason "

    set_reason(self, errorMsg: str)

    This will set the error string text.

    Parameters
    ----------
    errorMsg : Explanation for the error
";

%feature("docstring") gmsec::api::Status::getClass "

    get_class(self) -> int

    This function will return the error class ID.
";

%feature("docstring") gmsec::api::Status::isError "

    is_error(self) -> bool

    Used to determine whether the Status is reporting an error or not.

    Note: Only the status class is examine; the status code is not checked.

    Returns
    -------
    False if the status class is set to NO_ERROR; True otherwise.
";

%feature("docstring") gmsec::api::Status::get "

    get(self) -> str

    This function will return a verbose error string that contains the
    Status class, code, custom code and reason.  The format is as
    follows:

        [class,code,custom code] : reason
";

%feature("docstring") gmsec::api::Status::getCode "

    get_code(self) -> int

    This will return the error code number for this status for easy comparison.
";

%feature("docstring") gmsec::api::Status::getReason "

    get_reason(self) -> str

    This function will retrieve the string detailed description of this error.
";

%feature("docstring") gmsec::api::Status::set "

    set(self, errorClass: int, errorCode: int, errorMsg=None: str, customCode=0: int)

    This is a convience function that can be used to set class, code, error text and custom code.

    Parameters
    ----------
    errorClass : Enumerated value indicating source of the error
    errorCode  : Enumerated value indicating reason for the error
    errorMsg   : Explanation for the error
    customCode : Custom error code (typically middleware specific) on the root cause of the error
";

%feature("docstring") gmsec::api::Status::setClass "

    set_class(self, errorClass: int)

    This will set the erorr class ID.

    Parameters
    ----------
    errorClass : Enumerated value indicating source of the error
";

%feature("docstring") gmsec::api::Status::setCustomCode "

    set_custom_code(self, code)

    This will set the specific error code.

    Parameters
    ----------
    customCode : Custom error code (typically middleware specific) on the root cause of the error
";
