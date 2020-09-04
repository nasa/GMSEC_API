%module TimeUtil
%{
#include <gmsec4/util/TimeUtil.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char*, int); /* deprecated */

%ignore gmsec::api::util::operator==(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore gmsec::api::util::operator>(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore gmsec::api::util::operator<(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);

%ignore operator==(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator>(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator<(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/TimeUtil.h>

%perlcode%{

=pod

=head1 NAME

libgmsec_perl::TimeUtil

=head1 DESCRIPTION

This package provides a number of time-based utility functions.

=head2 Public Member Subroutines

=head3 millisleep

C<libgmsec_perl::TimeUtil-E<gt>millisleep($milliseconds)>

        Sleeps for the specified number of milliseconds (at least 1).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $milliseconds - Time in milliseconds to sleep. Must be at least one millisecond, or this function will immediately return.

=head3 getCurrentTime

C<libgmsec_perl::TimeUtil-E<gt>getCurrentTime()>

        Returns the current time in seconds since Jan 1, 1970.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A GMSEC_TimeSpec object representing the time in seconds since Jan 1, 1970.

=head3 getCurrentTime_s

C<libgmsec_perl::TimeUtil-E<gt>getCurrentTime_s($gmsecTimeSpec)>

        Populates the GMSEC_TimeSpec with the current time in (real) milliseconds since Jan 1, 1970.

        Note: If timespec buffer is non-zero, it will be populated.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec - The GMSEC_TimeSpec object to be populated with the current time.

=head3 getCharTime

C<libgmsec_perl::TimeUtil-E<gt>getCharTime($timeStr)>

        Places the current time in seconds since Jan 1, 1970 into the String passed into this function.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $timeStr - An output variable which will be populated with the current time string

=head3 getCharTime_s

C<libgmsec_perl::TimeUtil-E<gt>getCharTime_s($timeStr)>

        Places the current time in seconds since Jan 1, 1970 into the String passed into this function.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $timeStr - An output variable which will be populated with the current time string

=head3 formatTime

C<libgmsec_perl::TimeUtil-E<gt>formatTime($gmsecTimeSpec, $buffer)>

        Stores a GMSEC time string with the format of YYYY-DDD-HH-MM-SS.sss into the given buffer.

        Note: When using this function in the Perl binding, ensure that you have created a string variable with a large enough buffer to fit the output data.

        Example usage:

                my $theTime = "YYYY-DDD-HH:MM:SS.sss";
                libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
                libgmsec_perl::LogInfo("Start Time: " . $theTime);

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec - A GMSEC_TimeSpec object containing time information
        $buffer - the array where the formatted time string will be stored

C<libgmsec_perl::TimeUtil-E<gt>formatTime($gmsecTimeSpec, $subs, $buffer)>

        Stores a GMSEC time string with the format of YYYY-DDD-HH-MM-SS[.s*] into the given buffer.

        Note: Buffer must be at least $libgmsec_perl::GMSEC_TIME_BUFSIZE characters.

        Perl-specific Note: When using this function in the Perl binding, ensure that you have created a string variable with a large enough buffer to fit the output data.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec - A GMSEC_TimeSpec object containing time information
        $subs - The number of digits to allow for seconds [0 .. 9]
        $buffer - the array where the formatted time string will be stored

=head3 diffTime

C<libgmsec_perl::TimeUtil-E<gt>diffTime($gmsecTimeSpec1, gmsecTimeSpec0)>

        Computes the time-delta between the two given times.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec1 - A GMSEC_TimeSpec object containing time information
        $gmsecTimeSpec0 - A GMSEC_TimeSpec object containing time information

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A GMSEC_TimeSpec that contains the time-delta.

=cut
%}
