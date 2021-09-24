/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module DeviceParam
%{
#include <gmsec4/mist/DeviceParam.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/DeviceParam.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::DeviceParam

This class is a lightweight container for holding information on a DeviceParam, and is used to generate GMSEC Device messages by the ConnectionManager and Device classes

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::DeviceParam-E<gt>new($name, $timestamp, $value)>

	Initializes the DeviceParam object with a name, timestamp, and value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$name - The name of the DeviceParam
	$timestamp - The time at which the sampled value was taken
	$value  -The value of the parameter

=head3 DESTROY

	Destructor

=head3 getName

C<libgmsec_perl::DeviceParam-E<gt>getName()>

	Retrieves the name of the DeviceParam

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The name of the DeviceParam

=head3 getTimestamp

C<libgmsec_perl::DeviceParam-E<gt>getTimestamp()>

	Retrieves the timestamp of the DeviceParam

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The timestamp of the DeviceParam

=head3 getValue

C<libgmsec_perl::DeviceParam-E<gt>getValue()>

	Retrieves the value of the DeviceParam

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The value of the DeviceParam

=cut
%}
