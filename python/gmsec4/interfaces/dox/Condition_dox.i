
// File: classlibgmsec__python_1_1Condition.xml


%feature("docstring") gmsec::api::util::Condition "

    CONSTRUCTOR:

    Condition()

    Standard constructor. Initializes the internal platform dependent modules.

    Condition(mutex)

    Standard constructor. Initializes the internal platform dependent modules with an externally managed mutex.
"

%feature("docstring") gmsec::api::util::Condition::waitUntil "

    waitUntil(self, t) -> int

    The function will cause the calling thread to wait until
    the condition is signalled or the time specified has been reached.

    Parameters
    ----------
    t: The absolute timeout.
";

%feature("docstring") gmsec::api::util::Condition::getMutex "

    getMutex(self) -> Mutex

    Returns a reference to the mutex.

";

%feature("docstring") gmsec::api::util::Condition::broadcast "

    broadcast(self, reason)

    This function signals all threads that are waiting on the condition
    variable to continue with their activities.

    Note: Only one thread should be responsible for signalling threads
    waiting on the Condition.

";

%feature("docstring") gmsec::api::util::Condition::signal "

    signal(self, reason)

    This function signals any waiting threads to continue with their
    activities.

    Note: Only one thread should be responsible for signalling threads
    waiting on the Condition.

";

%feature("docstring") gmsec::api::util::Condition::wait "

    wait(self) -> int

    The function will hold at this point until the thread is signaled.

    wait(self, millis) -> int

    The function will hold at this point until the thread is signaled or
    the time specified has expired.  If millis is not positive, timeout immediately.

    Parameters
    ----------
    millis: The length of time in milliseconds to wait for the thread to
            be signalled before continuing.
";
