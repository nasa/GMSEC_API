/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module mist_defs
%{
#include <gmsec4/mist/mist_defs.h>
using namespace gmsec::api::mist;
%}

%include<gmsec4/util/wdllexp.h>
%include <gmsec4/mist/mist_defs.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::mist_defs

MIST-related enumerations and constants

=head2 Public Types and Constants

=item libgmsec_perl::GMSEC_ISD_2014_00

	2014 version of the GMSEC Interface Specification Document (ISD)

=item libgmsec_perl::GMSEC_ISD_2016_00

	2016 version of the GMSEC Interface Specification Document (ISD)

=item libgmsec_perl::GMSEC_ISD_2018_00

	2016 version of the GMSEC Interface Specification Document (ISD)

=item libgmsec_perl::GMSEC_ISD_2019_00

	2016 version of the GMSEC Interface Specification Document (ISD)

=item libgmsec_perl::GMSEC_ISD_CURRENT

	Current version of the GMSEC Interface Specification Document (ISD)

=head3 ResponseStatus

=over

=item libgmsec_perl::ResponseStatus::ACKNOWLEDGEMENT

Basic acknowledgement

=item libgmsec_perl::ResponseStatus::WORKING_KEEP_ALIVE

Still working on operation

=item libgmsec_perl::ResponseStatus::SUCCESSFUL_COMPLETION

Operation successfully completed

=item libgmsec_perl::ResponseStatus::FAILED COMPLETION

Operation failed to complete

=item libgmsec_perl::ResponseStatus::INVALID_REQUEST

The request for an operation was invalid

=item libgmsec_perl::ResponseStatus::FINAL_MESSAGE

This message is the last in a set of responses

=back

=cut
%}
