/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module DeviceIterator
%{
#include <gmsec4/mist/DeviceIterator.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/DeviceIterator.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::DeviceIterator

This class supports iterating over the Device objects of a DeviceMessage.

Note that DeviceIterator is not thread safe.

A DeviceIterator can be retreived from a DeviceMessage by invoking DeviceMessage::getDeviceIterator().

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::DeviceIterator-E<gt>DESTROY()>

	Destructor

=head3 hasNext

C<libgmsec_perl::DeviceIterator-E<gt>hasNext()>

Provides informaiton as to whether there are additional Device objects that can be referenced using next()

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	1 if additional fields are available, 0 otherwise

=head3  next

C<libgmsec_perl::DeviceIterator-E<gt>next()>

Returns a reference to the next available device object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A reference to the Device object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the iterator has reached the end (i.e. there are no more Devices)

=head3 reset

C<libgmsec_perl::DeviceIterator-E<gt>reset()>

	Resets the iterator to the beginning of the Device list that is maintained by DeviceMessage

=cut
%}
