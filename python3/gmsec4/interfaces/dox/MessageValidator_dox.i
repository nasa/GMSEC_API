
%feature("docstring") gmsec::api::mist::MessageValidator "

    This class is the abstract base class for performing custom message validation.
    A user created class, derived from this class, can be passed into
    registerMessageValidator() to have user code executed when a message is being validated.

    This custom message validator will be used to supplement the already built-in message validator
    offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
    call on the custom message validator, and if the returned status is nominal, then the API's
    message validator will be called.
    Note that because users are able to create their own MessageValidator class,
    reentrancy is not guaranteed unless if reentrancy rules are specified.

    In addition, if a MessageValidator is registered to multiple ConnectionManager objects,
    validateMessage() can be invoked concurrently from different threads. Use of a Mutex
    is suggested to enforce thread safety.

    Example MesssageValidator class:
        class MyMessageValidator(libgmsec_python3.MessageValidator):
            def __init__(self):
                libgmsec_python3.MessageValidator.__init__(self)
                self.mutex = libgmsec_python3.Mutex()

            def validate_message(self, msg):
                self.mutex.enter_mutex()
                status = libgmsec_python3.Status()
                # perform message validation
                if error:
                    status.set(...)
                self.mutex.leave_mutex()
                return status

    Example MessageValidator registration:
        validator = MyMessageValidator()
        connMgr.registerMessageValidator(validator)
";

%feature("docstring") gmsec::api::mist::MessageValidator::validateMessage "

    validate_message(self, msg: Message)

    This method is called by the API to perform a user-defined message validation.

    If a MessageValidator is registered to multiple connections, validateMessage() can be
    invoked concurrently from different connection threads.

    This custom message validator will be used to supplement the already built-in message validator
    offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
    call on the custom message validator, and if the returned status is nominal, then the API's
    message validator will be called.

    Note: DO NOT STORE the Message object for use beyond the scope of the function.

    Parameters
    ----------
    msg  : The Message to validate

    Return
    ----------
    status : The status of the operation
";

