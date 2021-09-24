/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") EventCallback
%{
#include <gmsec4/EventCallback.h>
using namespace gmsec::api;
%}
// Turn on director mapping for the Callback class
%feature("director") EventCallback;

%apply SWIGTYPE *DISOWN { EventCallback *cb };

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/EventCallback.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::EventCallback

=head1 DESCRIPTION

This class is the abstract base class for event callbacks. A user created class, derrived from this class, can be passed into registerEventCallback() to have user code executed asynchronously when an event occurs in the connection object. Please note that because users are able to create their own EventCallback class, reentrancy is not guaranteed unless if they implement their own reentrancy rules. Also note that because an EventCallback can be registered to multiple connections, it can be run concurrently amongst those connections. Because of this, the use of a gmsec::api::util::Mutex is suggested to enforce thread safety.

Example EventCallback class:

        {
            package MyEventCallback;
            use base 'libgmsec_perl::EventCallback';

            sub new
            {
                # Initialize the base Package (libgmsec_perl::EventCallback)
                my $class = shift;
                my $self = $class->SUPER::new();

                return $self;
            }

            sub onEvent
            {
                my ($self, $conn, $status, $event) = @_;

                libgmsec_perl::LogInfo("[MyEventCallback::onEvent] Status:\n" . $status->get());
            }
        }

Example EventCallback registration:

        my $evtCallback = MyEventCallback->new();
        $connection->registerEventCallback($libgmsec_perl::Connection::REQUEST_TIMEOUT, $evtCallback);

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::EventCallback-E<gt>DESTROY()>

    Destructor.

=head3 onEvent

C<libgmsec_perl::EventCallback-E<gt>onEvent($connection, $status, $event)>

        This function is called in response to a error after a call to registerEventCallback().

        Please note that if an EventCallback is registered to multiple connections, onEvent() can be invoked concurrently from the different connection threads.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>DO NOT DESTROY</b> the Connection that is passed into this function by the API.  It is owned by the API and do not need to be managed by the client program.  References to the Connection object, the Status object and the event string should not be stored by the client program beyond the scope of this callback function.<br><br>

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Parameters:</b><br>

        $connection - A reference to the Connection object that is reporting the event
        $status - A reference to a status object that contains information regarding the event
        $event - the event that led the callback to be summoned

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#registereventcallback">libgmsec_perl::Connection->registerEventCallback()</a><br>

=cut
%}
