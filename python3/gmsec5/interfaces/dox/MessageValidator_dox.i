/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::MessageValidator "

    This class is the abstract base class for performing custom message validation.
    A user created class, derived from this class, can be passed into
    register_message_validator() to have user code executed when a message is being validated.

    This custom message validator will be used to supplement the already built-in API message
    validator. The custom message validator will be called on first, and if the returned status
    is nominal, then the API's message validator will be called.

    Note that because users are able to create their own MessageValidator class,
    reentrancy is not guaranteed unless if reentrancy rules are specified.

    In addition, if a MessageValidator is registered to multiple Message objects,
    ValidateMessage() can be invoked concurrently from different threads. Use of a Mutex
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
        conn = libgmsec_python3.Connection(config)
        conn.get_message_factory().register_message_validator(validator)
";

%feature("docstring") gmsec::api5::MessageValidator::validateMessage "

    validate_message(self, msg: Message) -> Status

    This method is called by the API to perform a user-defined message validation.

    Note: DO NOT STORE the Message object for use beyond the scope of the function.

    Parameters
    ----------
    msg : The Message to validate

    Return
    ----------
    The Status of the operation
";

