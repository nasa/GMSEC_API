/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ConnectionManagerReplyCallback
%{
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
using namespace gmsec::api::mist;
%}

%feature("director") ConnectionManagerReplyCallback;

%apply SWIGTYPE *DISOWN {ConnectionManagerReplyCallback *cb};

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManagerReplyCallback.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ConnectionManagerReplyCallback

=head1 DESCRIPTION

This class is the abstract base class for received replies from asynchronous request messages.  A user created class, derived from this class, can be passed into request() to have user code executed asynchronously when a reply is received or when an error occurs.
Please note that because users are able to create their own ConnectionManagerReplyCallback class, reentrancy is not guaranteed unless they implement their own reentrancy rules.
Also note that because a ConnectionManagerReplyCallback can be registered to multiple connections, it can be run concurrently amogst those connections.  Because of this, the use of libgmsec_perl::Mutex is suggested to enforce threat safety.

Example ConnectionManagerReplyCallback class:

        {
            package MyReplyCallback;
            use base 'libgmsec_perl::ConnectionManagerReplyCallback';

            sub onReply
            {
                my ($self, $connMgr, $request, $reply) = @_;

                libgmsec_perl::LogInfo("[MyReplyCallback::onReply]\n" . $reply->toXML());
            }

            sub onEvent
            {
                my ($self, $connMgr, $status, $event) = @_;

                if($status->isError())
                {
                    libgmsec_perl::LogError("[MyReplyCallback::onError] " . $status.get() . ", event'" . $event);
                }
            }
        }

Example ConnectionManagerReplyCallback registration:

    my $rcb = MyReplyCallback->new();

    my $requestMsg = libgmsec_perl::Message->new("GMSEC.MY.REQUEST", $libgmsec_perl::Message::REQUEST);

    # Add Fields to the message
    ...

    $rcb->DISOWN();
    $connMgr->request($requestMsg, $timeout, $rcb);

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::ConnectionManagerReplyCallback-E<gt>DESTROY()>

    Destructor.

=head3 onReply

C<libgmsec_perl::ConnectionManagerReplyCallback-E<gt>onReply($connMgr, $request, $reply)>

    This function is called by the API in response to a reply received from a request, from within the Request call.  A class derived from libgmsec_perl::COnnectionManagerReplyCallback needs to be passed into the request() call.

    Please note that if a callback is registered to multiple connections, onReply can be invoked concurrently from the different connection threads.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>DO NO DESTROY</b> the ConnectionManager, or the Messages that are passed into this function by the API.  They are owned by the API and do not need to be managed by the client program.  Also, they can not be stored by the client program beyond the scope of this callback function.  In order to store a Message, the message must be copied.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $connMgr - ConnectionManager object on which the message was received
        $request - the pending request
        $reply - the received reply

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See Also</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#request">libgmsec_perl::ConnectionManager->request()</a><br>

=cut
%}
