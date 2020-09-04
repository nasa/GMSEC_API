%module Condition

%{
#include <gmsec4/util/Condition.h>
using namespace gmsec::api;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Condition.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Condition

=head1 DESCRIPTION

This class provides a platform independent mutex with conditional waiting calls which can be used to handle multithreading scenarios.

=head2 Public Types and Constants

=head3 Signal Reasons

=over

=item libgmsec_perl::Condition::INVALID

Invalid

=item libgmsec_perl::Condition::TIMEOUT

Timeout-triggered

=item libgmsec_perl::Condition::USER

User-triggered

=back

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::Condition-E<gt>new()>

        Standard constructor. Initializes the internal platform dependent modules.

=head3 DESTROY

C<libgmsec_perl::Condition-E<gt>DESTROY()>

        Destructor.

=head3 getMutex

C<libgmsec_perl::Condition-E<gt>getMutex()>

        Returns a reference to the mutex.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A reference to the mutex.

=head3 wait

C<libgmsec_perl::Condition-E<gt>wait()>

        This function will hold the execution of code at this point until the thread is signaled.

C<libgmsec_perl::Condition-E<gt>wait($millis)>

        This function will hold the execution of code at this point until the thread is either signaled or the time specified has expired.  If millis is not positive, timeout immediately.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $millis - The length of time in milliseconds to wait for the thread to be signalled before continuing.

=head3 waitUntil

C<libgmsec_perl::Condition-E<gt>waitUntil($gmsecTimeSpec)>

        The function will cause the calling thread to wait until the condition is signalled or the time specified has been reached.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $gmsecTimeSpec - The absolute period in time when this call will timeout.

=head3 signal

C<libgmsec_perl::Condition-E<gt>signal($reason)>

        This function signals any waiting threads to continue with their activities.

        Note: Only one thread should be responsible for signalling threads waiting on the Condition.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $reason - The enumerated value reason for why the Condition call to wait() returned

=head3 broadcast

C<libgmsec_perl::Condition-E<gt>broadcast($reason)>

        This function signals all threads that are waiting on the condition variable to continue with their activities.

        Note: Only one thread should be responsible for signalling threads waiting on the Condition.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $reason - The enumerated value reason for why the Condition call to wait() returned

=cut
%}
