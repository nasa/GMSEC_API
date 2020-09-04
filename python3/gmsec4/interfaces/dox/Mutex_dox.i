
%feature("docstring") gmsec::api::util::Mutex "

    This is the basic implementation of the cross-platform mutex

    CONSTRUCTOR:

    Mutex()
";

%feature("docstring") gmsec::api::util::Mutex::enterMutex "

    enter_mutex(self)

    Waits until the Mutex is aquired and locks. This is a reentrant
    function.
";

%feature("docstring") gmsec::api::util::Mutex::leaveMutex "

    leave_mutex(self)

    Releases the mutex. If the mutex has been \"entered\" multiple
    times, only one release is required.
";
