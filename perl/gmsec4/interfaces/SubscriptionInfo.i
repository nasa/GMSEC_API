/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module SubscriptionInfo
%{
#include <gmsec4/SubscriptionInfo.h>
using namespace gmsec::api;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/SubscriptionInfo.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::SubscriptionInfo

=head1 DESCRIPTION

Structure that contains the information the user has supplied when setting up a subscription.

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::SubscriptionInfo-E<gt>DESTROY()>

        Destructor

=head3 getSubject

C<libgmsec_perl::SubscriptionInfo-E<gt>getSubject()>

        Returns the subject string used when setting up the subscription.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        Subject string.

=head3 getCallback

C<libgmsec_perl::SubscriptionInfo-E<gt>getSubject()>

        Returns the Callback object, if any, used when setting up the subscription.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        Pointer to Callback object.

=head1 SEE ALSO

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">connection->subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#unsubscribe">connection->unsubscribe()</a><br>

=cut
%}
