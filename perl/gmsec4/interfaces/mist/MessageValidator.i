/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") MessageValidator
%{
#include <gmsec4/mist/MessageValidator.h>
using namespace gmsec::api::mist;
%}

%feature("director") MessageValidator;

%apply SWIGTYPE *DISOWN {MessageValidator *val};

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MessageValidator.h>

%perlcode%{

=pod

=head1 NAME

libgmsec_perl::MessageValidator

=head1 DESCRIPTION

This class is the abstract base class for performing custom message validation.
A user created class, derived from this class, can be passed into registerCustomMessageValidator() to have user code executed when a message is being validated.

This custom message validator will be used to supplement the already built-in message validator offered by the ConnectionManager (via a Specification object). The ConnectionManager will first call on the custom message validator, and if the returned status is nominal, then the API message validator will be called.

Note that because users are able to create their own MessageValidator class, reentrancy is not guaranteed unless if reentrancy rules are specified.


Example MessageValidator class:

        {
            package MyMessageValidator;
            use base 'libgmsec_perl::MessageValidator';

            sub new
            {
                # Initialize the base Package (libgmsec_perl::MessageValidator)
                my $class = shift;
                my $self = $class->SUPER::new();

                return $self;
            }

            sub validateMessage
            {
                my ($self, $message) = @_;

                my $status = libgmsec_perl::Status->new();

                # Validate message

                if ($error)
                    $status->set(...);

                return $status;
            }
        }

Example MessageValidator registration:

        my $validator = MyMessageValidator>new();
        $connMgr>registerMessageValidator($validator);

=head2 Public Member Subroutines

=head3 DESTROY

C<libgmsec_perl::MessageValidator<gt>DESTROY()>

    Destructor.

=head3 validateMessage

C<libgmsec_perl::MessageValidator<gt>validateMessage($msg)>

    This method is called by the API to perform a user-defined message validation.

    If a MessageValidator is registered to multiple connections, validateMessage() can be invoked concurrently from different connection threads.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>DO NOT STORE</b> the Message object for use beyond the scope of the function.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $msg - the message to validate

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Status of the operation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See Also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConnectionManager.html#registerMessageValidator">libgmsec_perl::ConnectionManager->registerMessageValidator()</a><br>
=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Specification.html#registerMessageValidator">libgmsec_perl::Specification>registerMessageValidator()</a><br>


=cut
%}
