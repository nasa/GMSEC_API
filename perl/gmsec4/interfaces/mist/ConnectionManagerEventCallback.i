/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ConnectionManagerEventCallback
%{
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
using namespace gmsec::api::mist;
%}

%feature("director") ConnectionManagerEventCallback;

%apply SWIGTYE *DISOWN {ConnectionManagerEventCallback *cb};

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManagerEventCallback.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ConnectionManagerEventCallback

=head1 DESCRIPTION

This class is the abstract base class for MIST event callbacks.  A user created class, derived from this class, can be passed into registerEventCallback() to have user code executed asynchronously when an event (typically an error) occurs in the connection object.
Please note that because users are able to create their own ConnectionManagerEventCallback class, reentrancy is not guaranteed unless if they implement their own reentrancy rules.
A,so note that because ConnectionManagerEventCallback can be registered to multiple connections, it can be run concurrently amongst those connections.  Because of this, the use of libgmsec_perl::Mutex is suggested to enforce thread safety.

Example EventCallback class:

        {
            package MyEventCallback;
            use base 'libgmsec_perl::ConnectionManagerEventCallback';

            sub new
            {
                # Initialize the base Package (libgmsec_perl::ConnectionManagerEventCallback)
                my $class = shift;
                my $self = $class->SUPER::new();

                return $self;
            }

            sub onEvent
            {
                my ($self, $connMgr, $status, $event) = @_;

                libgmsec_perl::LogInfo("[MyEventCallback::onEvent] Status:\n" . $status->get());
            }
        }

Example EventCallback registration:

        my $evtCallback = MyEventCallback->new();
        $connection->registerEventCallback($libgmsec_perl::Connection::REQUEST_TIMEOUT, $evtCallback);

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::ConnectionManagerEventCallback-E<gt>DESTROY()>

    Destructor.

=head3 onMessage

C<libgmsec_perl::ConnectionManagerEventCallback-E<gt>onMessage($connMgr, $status, $event)>

    This function is called in response to an event after a call to registerEventCallback().

    Please Note that if a callback is registered to multiple connections, onEvent() can be invoked concurrently from the different connection threads.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>DO NOT DESTROY</b> the ConnectionManager that is passed into this function by the API.  It is owned by the API and does not need to be managed by the client program.  Also, the ConnectionManager object should not be stored by the client program beyond the scope of this callback function.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $connMgr - A reference to the ConnectionManager object that is reporting the event
    $status - A reference to a status object that contains information regarding the event
    $event - the event that led the callback to be summoned

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See Also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#registerEventCallback">libgmsec_perl::ConnectionManager->registerEventCallback()</a><br>

=cut
%}

