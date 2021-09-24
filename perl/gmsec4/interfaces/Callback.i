/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Callback
%{
#include <gmsec4/Callback.h>
using namespace gmsec::api;
%}

// Turn on director mapping for the Callback package
%feature("director") Callback;

%apply SWIGTYPE *DISOWN { Callback *cb };

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Callback.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Callback

=head1 DESCRIPTION

This package is the abstract base package for received message callbacks. A user created package, derived from this package, can be passed into Subscribe() and Request() to have user code executed asynchronously when a message is received. Please note that because users are able to create their own Callback package, reentrancy is not guarunteed unless if reentrancy rules are specified. Also note that because a Callback can be registered to multiple connections, it can be run concurrently amongst those connections. Because of this, the use of a libgmsec_perl::Mutex is suggested to enforce thread safety.

Example Callback package:

        {
            package PublishCallback;
            use base 'libgmsec_perl::Callback';

            sub new
            {
                # Initialize the base Package (libgmsec_perl::Callback)
                my $class = shift;
                my $self = $class->SUPER::new();

                return $self;
            }

            sub onMessage
            {
                my ($self, $conn, $message) = @_;

                libgmsec_perl::LogInfo("[PublishCallback::onMessage] Received:\n" . $message->toXML());
            }
        }

Example Callback registration:

        my $publishCallback = PublishCallback->new();
        $connection->subscribe($subject, $publishCallback));

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::Callback-E<gt>DESTROY()>

    Destructor.

=head3 onMessage

C<libgmsec_perl::Callback-E<gt>onMessage($connection, $message)>

    This function is called by the API in response to a message, from either the dispatchMsg() call or inside the Auto-dispatcher after a startAutoDispatch() call. A package derrived from libgmsec_perl::Callback needs to be registered with a connection, using subscribe() or request() in order to be called for a particular subject registration pattern.

    Please note that if a Callback is registered to multiple connections, onMessage() can be invoked concurrently from different connection threads.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>DO NOT DESTROY</b> the Connection or Message that is passed into this function by the API.  They are owned by the API and do not need to be managed by the client program.  Also, they can not be stored by the client program beyond the scope of this callback function.  In order to store the Message, it must be copied.<br><br>

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Parameters:</b><br>

        $connection - connection on which the message was received

        $message - the received message

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">libgmsec_perl::Connection->subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#request">libgmsec_perl::Connection->request()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#dispatchmessage">libgmsec_perl::Connection->dispatchMessage()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#startautodispatch">libgmsec_perl::Connection->startAutoDispatch()</a><br>

=cut
%}
