/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ConnectionManagerCallback
%{
#include <gmsec4/mist/ConnectionManagerCallback.h>
using namespace gmsec::api::mist;
%}

%feature("director") ConnectionManagerCallback;

%apply SWIGTYPE *DISOWN {ConnectionManagerCallback *cb};

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManagerCallback.h>

%perlcode%{

=pod

=head1 NAME

libgmsec_perl::ConnectionmanagerCallback

=head1 DESCRIPTION

This class is the abstract base class for received message callbacks.  A user created class, derived from this class, can be passed into Subscribe() to have user code executed asynchronously when a message is received.
Please note that because users are able to create their own Callback class, reentrancy is not guaranteed unless if reentrancy rules are specified.
Also note that because a ConnectionManagerCallback can be registered to multiple ConnectionManagers, it can be run concurrently amongst those ConnectionManagers.  Because of this, the use of libgmsec_perl::Mutex is suggested to enforce thread safety.

Example ConnectionManagerCallback class:

        {
            package MyCallback;
            use base 'libgmsec_perl::ConnectionManagerCallback';

            sub new
            {
                # Initialize the base Package (libgmsec_perl::ConnectionManagerCallback)
                my $class = shift;
                my $self = $class->SUPER::new();

                return $self;
            }

            sub onMessage
            {
                my ($self, $connMgr, $message) = @_;

                libgmsec_perl::LogInfo("[MyCallback::onMessage] Received:\n" . $message->toXML());
            }
        }

Example Callback registration:

        my $publishCallback = PublishCallback->new();
        $connection->subscribe($subject, $publishCallback);

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::ConnectionManagerCallback-E<gt>DESTROY()>

    Destructor.

=head3 onMessage

C<libgmsec_perl::ConnectionManagerCallback-E<gt>onMessage($connMgr, $msg)>

    This function is called by the API in response to a message, from either the dispatch() call or inside the Auto-dispatcher after a startAutoDispatch() call.  A class derived from ConnectionManagerCallback needs to be registered with a connection, using subscribe() in order to be called for a particular subject registration pattern. 

    Please nore that if a ConnectionManagerCallback is registered to multiple connections, onMessage can be invoked concurrently from different connection threads.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>DO NOT DESTROY</b> the ConnectionManager or message that is passed into this function by the API.  They are owned by the API and do not need to be managed by the client program.  Also, they can not be stored by the client program beyond the scope of this callback function.  In order to store the Message, a copy must be made.  

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $connMgr - ConnectionManager object on which the message was received
        $msg - the received message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See Also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#subscribe">libgmsec_perl::ConnectionManager->subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#dispatch">libgmsec_perl::ConnectionManager->dispatch()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#startAutoDispatch">libgmsec_perl::ConnectionManager->startAutoDispatch()</a><br>

=cut
%}
