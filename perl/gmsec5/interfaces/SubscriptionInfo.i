/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module SubscriptionInfo

%{
#include <gmsec5/SubscriptionInfo.h>
using namespace gmsec::api5;
%}

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/SubscriptionInfo.h>

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

C<libgmsec_perl::SubscriptionInfo-E<gt>getSubjectPattern()>

        Returns the subject pattern string used when setting up the subscription.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        Subject string.


=head1 SEE ALSO

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">Connection->subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#unsubscribe">Connection->unsubscribe()</a><br>

=cut
%}
