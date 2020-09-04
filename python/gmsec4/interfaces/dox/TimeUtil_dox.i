
// File: classlibgmsec__python_1_1TimeUtil.xml


%feature("docstring") gmsec::api::util::TimeUtil "

    Contains the functions that support the acquisition of current
    system time, and the ability to format such into a GMSEC-style time
    string.
";

%feature("docstring") gmsec::api::util::TimeUtil::diffTime "

    diffTime(time1, time0) -> GMSEC_TimeSpec

    Computes the time-delta between the two given times.

    Returns
    -------
    A GMSEC_TimeSpec that contains the time-delta.

";

%feature("docstring") gmsec::api::util::TimeUtil::millisleep "

    millisleep(milliseconds)

    Sleeps for the specified number of milliseconds (at least 1).
";

%feature("docstring") gmsec::api::util::TimeUtil::getCharTime "

    getCharTime(timeString) -> GMSEC_TimeSpec

    Returns the current time in seconds since Jan 1, 1970 as represented by the GMSEC String
";

%feature("docstring") gmsec::api::util::TimeUtil::getCurrentTime_s "

    getCurrentTime_s(ts=None) -> double

    Returns the current time in seconds since Jan 1, 1970 as represented by the GMSEC String
";

%feature("docstring") gmsec::api::util::TimeUtil::formatTime "

    formatTime(ts, buffer)

    Formats the given TimeSpec into a GMSEC time string with the format of YYYY-DDD-HH-MM-SS.sss

    Parameters
    ----------
    ts: a GMSEC_TimeSpec containing time information
    buffer: the array where to store the formatted time string.

    Note: buffer must be at least GMSEC_TIME_BUFSIZE characters

    formatTime(ts, subs, buffer)

    Stores a GMSEC time string with the format of YYYY-DDD-HH-MM-SS[.s*] into the given buffer.

    Parameters
    ----------
    ts: a GMSEC_TimeSpec containing time information
    subs: the number of digits to allow for subseconds [0 .. 9].
    buffer: the array where to store the formatted time string.

    Note: buffer must be at least GMSEC_TIME_BUFSIZE characters

    formatTime(ts, buffer, subs)

    Deprecated since v4.2 - nevertheless we keep for binary compatibility reasons
";

%feature("docstring") gmsec::api::util::TimeUtil::getCharTime_s "

    getCharTime_s(timeString) -> double

    Returns the current time in seconds since Jan 1, 1970 as represented by the GMSEC String
";

%feature("docstring") gmsec::api::util::TimeUtil::getCurrentTime "

    getCurrentTime() -> GMSEC_TimeSpec
    
    Returns the current time in seconds since Jan 1, 1970.
";
