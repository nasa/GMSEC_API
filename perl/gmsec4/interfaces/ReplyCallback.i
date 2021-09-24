/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ReplyCallback
%{
#include <gmsec4/ReplyCallback.h>
using namespace gmsec::api;
%}

// Turn on director mapping for the Callback class
%feature("director") ReplyCallback;

%apply SWIGTYPE *DISOWN { ReplyCallback *cb };

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/ReplyCallback.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ReplyCallback

=head1 DESCRIPTION

This class is the abstract base class for received replies from asynchronous request messages. A user created class, derived from this class, can be passed into request() to have user code executed asynchronously when a reply is received or when an error occurs. Please note that because users are able to create their own ReplyCallback class, reentrancy is not guarunteed unless if they implement their own reentrancy rules. Also note that because a ReplyCallback can be registered to multiple connections, it can be run concurrently amongst those connections. Because of this, the use of a gmsec::api::util::Mutex is suggested to enforce thread safety.

Example ReplyCallback class:

        {
            package MyReplyCallback;
            use base 'libgmsec_perl::ReplyCallback';

            sub onReply
            {
                    my ($self, $connection, $request, $reply) = @_;

                    libgmsec_perl::LogInfo("[MyReplyCallback::onReply]\n" . $reply->toXML());
            }

            sub onEvent
            {
                    my ($self, $connection, $status, $event) = @_;

                    if ($status->isError())
                    {
                            libgmsec_perl::LogError("[MyReplyCallback::onError] " . $status->get() . ", event=" . $event);
                    }
            }
        }

Example ReplyCallback registration:

        my $rcb = MyReplyCallback->new();

        my $requestMsg = libgmsec_perl::Mesage->new("GMSEC.MY.REQUEST", $libgmsec_perl::Message::REQUEST);

        # Add Fields to the message
        ...

        $rcb->DISOWN();
        $conn->request($requestMsg, $timeout, $rcb);

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::ReplyCallback-E<gt>DESTROY()>

    Destructor.

=head3 onReply

C<libgmsec_perl::ReplyCallback-E<gt>onReply($connection, $request, $reply)>

        This function is called by the API in response to a reply recieved from a request, from within the request() call. A class derived from gmsec::api::ReplyCallback needs to be passed into the request() call.

        Please note that if a ReplyCallback is registered to multiple connections, onReply() can be invoked concurrently from the different connection threads.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>DO NOT DESTROY</b> the Connection or the Messages that are passed into this function by the API.  The are owned by the API and do not need to be managed by the client program.  In order to store the Messages, they must be copied.<br><br>

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Parameters:</b><br>

        $connection - A reference to the Connection object that is reporting the event
        $request - the pending request message
        $reply - the received reply message

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#request">libgmsec_perl::Connection->request()</a><br>

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
