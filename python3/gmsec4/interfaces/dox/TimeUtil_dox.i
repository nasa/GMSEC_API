

%feature("docstring") gmsec::api::util::TimeUtil "

    Contains the functions that support the acquisition of current
    system time, and the ability to format such into a GMSEC-style time
    string.
";

%feature("docstring") gmsec::api::util::TimeUtil::diffTime "

    diff_time(time1: GMSEC_TimeSpec, time0: GMSEC_TimeSpec) -> GMSEC_TimeSpec

    Computes the time-delta between the two given times.

    Parameters
    ----------
    time1 : Time taken at the second measurement
    time0 : Time taken at the first measurement

    Returns
    -------
    A GMSEC_TimeSpec that contains the time-delta (i.e. time1 - time0).

";

%feature("docstring") gmsec::api::util::TimeUtil::millisleep "

    millisleep(milliseconds: int)

    Sleeps for the specified number of milliseconds (at least 1).

    Parameters:
    -----------
    milliseconds : The number of milliseconds to sleep
";

%feature("docstring") gmsec::api::util::TimeUtil::getCharTime "

    get_char_time(timeString: str) -> GMSEC_TimeSpec

    Returns the current time in seconds since Jan 1, 1970 as represented by the string

    Parameters
    ----------
    timestring : Time string with the format of \"YYYY-DDD-HH:MM:SS.sss\".

    Returns
    -------
    The current time in seconds since Jan 1, 1970 as represented by the string
";

%feature("docstring") gmsec::api::util::TimeUtil::getCurrentTime_s "

    get_current_time_s() -> double

    Returns the current time in seconds since Jan 1, 1970.

    Returns
    -------
    The current time represented as a floating point value.
";

%feature("docstring") gmsec::api::util::TimeUtil::formatTime "

    format_time(ts: GMSEC_TimeSpec) -> str

    Formats the given TimeSpec into a GMSEC time string with the format of YYYY-DDD-HH-MM-SS.sss

    Parameters
    ----------
    ts : The GMSEC_TimeSpec containing time information

    Returns
    -------
    A time string with the format of YYYY-DDD-HH-MM-SS.sss



    format_time(ts: GMSEC_TimeSpec, subs: int) -> str

    Stores a GMSEC time string with the format of YYYY-DDD-HH-MM-SS[.s*] into the given buffer.

    Parameters
    ----------
    ts   : The GMSEC_TimeSpec containing time information
    subs : the number of digits to allow for subseconds [0 .. 9].

    Returns
    -------
    A time string with the format of YYYY-DDD-HH-MM-SS[.s*]
";

%feature("docstring") gmsec::api::util::TimeUtil::getCharTime_s "

    get_char_time_s(timeString: str) -> double

    Returns the current time in seconds since Jan 1, 1970 as represented by the string

    Returns
    -------
    The current time as the number of seconds since the 01/01/1970 epoch.
";

%feature("docstring") gmsec::api::util::TimeUtil::getCurrentTime "

    get_current_time() -> GMSEC_TimeSpec
    
    Returns the current time in seconds since Jan 1, 1970.

    Returns
    -------
    A GMSEC_TimeSpec with the current time
";
