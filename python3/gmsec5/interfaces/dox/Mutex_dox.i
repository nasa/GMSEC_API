/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::util::Mutex "

    This is the basic implementation of the cross-platform mutex

    CONSTRUCTOR:

    Mutex()
";

%feature("docstring") gmsec::api5::util::Mutex::enterMutex "

    enter_mutex(self)

    Waits until the Mutex is aquired and locks. This is a reentrant
    function.
";

%feature("docstring") gmsec::api5::util::Mutex::leaveMutex "

    leave_mutex(self)

    Releases the mutex. If the mutex has been \"entered\" multiple
    times, only one release is required.
";
