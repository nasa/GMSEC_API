
// File: classlibgmsec__python_1_1Mutex.xml


%feature("docstring") gmsec::api::util::Mutex "

    This is the basic implementation of the cross-platform mutex

    CONSTRUCTOR:

    Mutex(self)
";

%feature("docstring") gmsec::api::util::Mutex::enterMutex "

    enterMutex(self)

    Waits until the Mutex is aquired and locks. This is a reentrant function.
";

%feature("docstring") gmsec::api::util::Mutex::leaveMutex "

    leaveMutex(self)

    Releases the mutex. If the mutex has been \"entered\" multiple times, only one
    release is required.
";
