
%feature("docstring") gmsec::api::util::Condition "

    CONSTRUCTOR:

    Condition()

    Standard constructor. Initializes the internal platform dependent
    modules.



    Condition(mutex: Mutex)

    Standard constructor. Initializes the internal platform dependent
    modules with an externally managed Mutex.
";

%feature("docstring") gmsec::api::util::Condition::getMutex "

    get_mutex(self) -> Mutex

    Returns a reference to the mutex.
";

%feature("docstring") gmsec::api::util::Condition::wait "

    wait(self) -> int

    The function will hold at this point until the thread is signaled.



    wait(self, millis: int) -> int

    The function will hold at this point until the thread is signaled
    or the time specified has expired. If millis is not positive,
    timeout immediately.

    Parameters
    ----------
    millis : The length of time in milliseconds to wait for the thread
             to be signalled before continuing.
";

%feature("docstring") gmsec::api::util::Condition::waitUntil "

    wait_until(self, t: GMSEC_TimeSpec) -> int

    The function will cause the calling thread to wait until
    the condition is signalled or the time specified has been reached.

    Parameters
    ----------
    t : The absolute timeout.
";

%feature("docstring") gmsec::api::util::Condition::signal "

    signal(self, reason: int)

    This function signals any waiting threads to continue with their
    activities.

    Note: Only one thread should be responsible for signalling threads
    waiting on the Condition.
";

%feature("docstring") gmsec::api::util::Condition::broadcast "

    broadcast(self, reason: int)

    This function signals all threads that are waiting on the condition
    variable to continue with their activities.

    Note: Only one thread should be responsible for signalling threads
    waiting on the Condition.
";
