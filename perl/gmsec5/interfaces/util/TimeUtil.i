/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module TimeUtil
%{
#include <gmsec5/util/TimeUtil.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char*);
%ignore gmsec::api5::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, int, char*);

%ignore operator==(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator>(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator<(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);

%include "std_string.i"

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/util/TimeUtil.h>


%extend gmsec::api5::util::TimeUtil {
    static std::string CALL_TYPE formatTime(const GMSEC_TimeSpec& ts)
    {
        char tmp[GMSEC_TIME_BUFSIZE];
        gmsec::api5::util::TimeUtil::formatTime(ts, tmp);
        return std::string(tmp);
    }

    static std::string CALL_TYPE formatTime(const GMSEC_TimeSpec& ts, int subs)
    {
        char tmp[GMSEC_TIME_BUFSIZE];
        gmsec::api5::util::TimeUtil::formatTime(ts, subs, tmp);
        return std::string(tmp);
    }
};


%perlcode%{

=pod

=head1 NAME

libgmsec_perl::TimeUtil

=head1 DESCRIPTION

This package provides a number of time-based utility functions.

=head2 Public Static Subroutines

=head3 millisleep

C<libgmsec_perl::TimeUtil::millisleep($milliseconds)>

        Sleeps for the specified number of milliseconds (at least 1).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $milliseconds - Time in milliseconds to sleep. Must be at least one millisecond, or this function will immediately return.


=head3 getCurrentTime

C<libgmsec_perl::TimeUtil::getCurrentTime()>

        Returns the current time in seconds since Jan 1, 1970.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A GMSEC_TimeSpec object representing the time in seconds since Jan 1, 1970.


=head3 getCurrentTime_s

C<libgmsec_perl::TimeUtil::getCurrentTime_s($gmsecTimeSpec)>

        Populates the GMSEC_TimeSpec with the current time in (real) milliseconds since Jan 1, 1970.

        Note: If timespec buffer is non-zero, it will be populated.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec - The GMSEC_TimeSpec object to be populated with the current time.


=head3 convertTimeString

C<libgmsec_perl::TimeUtil::convertTimeString($timeStr)>

        Converts the given time string into a TimeSpec.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $timeStr - A time string in the format of YYYY-DDD-HH-MM-SS[.sss]

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A TimeSpec object


=head3 convertTimeString_s

C<libgmsec_perl::TimeUtil::convertTimeString_s($timeStr)>

        Converts the given time string into a double value representing the number of seconds since Jan 1, 1970.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $timeStr - A time string in the format of YYYY-DDD-HH-MM-SS[.sss]

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A TimeSpec object


=head3 formatTime

C<libgmsec_perl::TimeUtil::formatTime($gmsecTimeSpec)>

        Returns a GMSEC time string with the format of YYYY-DDD-HH-MM-SS.sss.

        Example usage:

                my $theTime = libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime());
                libgmsec_perl::logInfo("Start Time: " . $theTime);

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec - A GMSEC_TimeSpec object containing time information


=head3 formatTime

C<libgmsec_perl::TimeUtil::formatTime($gmsecTimeSpec, $subs)>

        Returns a GMSEC time string with the format of YYYY-DDD-HH-MM-SS[.s*].

        Example usage:

                my $theTime = libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), 6);
                libgmsec_perl::logInfo("Start Time: " . $theTime);

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec - A GMSEC_TimeSpec object containing time information
        $subs - The number of digits to allow for seconds [0 .. 9]


=head3 diffTime

C<libgmsec_perl::TimeUtil::diffTime($gmsecTimeSpec1, gmsecTimeSpec0)>

        Computes the time-delta between the two given times.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec1 - A GMSEC_TimeSpec object containing time information
        $gmsecTimeSpec0 - A GMSEC_TimeSpec object containing time information

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A GMSEC_TimeSpec that contains the time-delta.

=cut
%}
