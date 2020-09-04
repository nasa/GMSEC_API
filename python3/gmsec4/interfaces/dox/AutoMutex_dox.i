
%feature("docstring") gmsec::api::util::AutoMutex "

    This class implements the Resource Acquisition Is Initialization
    (RAII) pattern where the resource is ownership of a mutex.

    CONSTRUCTOR:

    AutoMutex(self, mutex, acquire)
";

%feature("docstring") gmsec::api::util::AutoMutex::enter "

    enter(self)

    If the mutex has been manually released, this function is used to
    re-enter.
";

%feature("docstring") gmsec::api::util::AutoMutex::leave "

    leave(self)

    This function manually releases the mutex.
";
