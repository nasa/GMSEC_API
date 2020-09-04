%module Mutex
%{
#include <gmsec4/util/Mutex.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::util::AutoMutex;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Mutex.h>

%perlcode%{

=pod

=head1 NAME

libgmsec_perl::Mutex

=head1 DESCRIPTION

This is the basic implementation of the cross-platform mutex.

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::Mutex-E<gt>new()>

        Basic constructor.

=head3 DESTROY

C<libgmsec_perl::Mutex-E<gt>DESTROY()>

        Destructor.

=head3 enterMutex

C<libgmsec_perl::Mutex-E<gt>enterMutex()>

        Waits until the Mutex is aquired and locks. This is a reentrant function.

=head3 leaveMutex

C<libgmsec_perl::Mutex-E<gt>leaveMutex()>

        Releases the mutex. If the mutex has been "entered" multiple times, only one release is required.

=cut
%}
