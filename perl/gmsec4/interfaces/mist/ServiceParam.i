/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ServiceParam
%{
#include <gmsec4/mist/ServiceParam.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ServiceParam.h>


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ServiceParam

This class is a lightweight container for holding information on a Simple Service Parameter, and is used to generate GMSEC Simple Service messages by the ConnectionManager class

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::DeviceParam-E<gt>new($name, $value)>

	Initializes the ServiceParam object with a name and value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$name - the name of the ServiceParam
	$value - the value of the Parameter

=head3 DESTROY

C<libgmsec_perl::ServiceParam-E<gt>DESTROY()>

    Destructor

=head3 getName

C<libgmsec_perl::ServiceParam-E<gt>getName()>

    Retrieves the name of the ServiceParam

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The name of the ServiceParam

=head3 getValue

C<libgmsec_perl::ServiceParam-E<gt>getValue()>

    Retrieves the value of the ServiceParam

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The value of the ServiceParam

=cut
%}
