/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Status
%{
#include <gmsec4/Status.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::Status::operator=(const Status&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Status.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Status

=head1 DESCRIPTION

Returned by the API function calls to provide status feedback. Status provides an error code and error message specific to the error type, but independent of error source.

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::Status-E<gt>new()>

    Create a status set to GMSEC_STATUS_NO_ERROR.

    Returns a reference to a Status object.

C<libgmsec_perl::Status-E<gt>new($eClass, $eCode, $text, $custom)>

    Create a status with specific class, code, and string.

    Returns a reference to a Status object.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Parameters:</b>

        $eClass  - error class
        $eCode   - error code
        $text    - error string (optional)
        $custom  - custom error code (optional)

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a>

C<libgmsec_perl::Status-E<gt>new($exception)>

    Create a status with information from an Exception object.

    Returns a reference to a Status object.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Parameters:</b>

        $exception - Exception object

=head3 DESTROY

C<libgmsec_perl::Status-E<gt>DESTROY()>

    Destructor.

=head3 isError

C<libgmsec_perl::Status-E<gt>isError()>

    Used to determine whether the Status object is reporting an error or not.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Note:</b>

        Only the status class is examined; the status code is not checked.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        0 if the status class is set to $libgmsec_perl::NO_ERROR; 1 otherwise.

=head3 get

C<libgmsec_perl::Status-E<gt>get()>

    This function will return a verbose error string that contains the Status class, code, custom code and reason. The format is as follows:

        [class,code,custom code] : reason

=head3 set

C<libgmsec_perl::Status-E<gt>set($eClass, $eCode, $text, $custom)>

    This is a convience function that can be used to set class, code, error text and custom code.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Parameters:</b>

        $eClass  - error class
        $eCode   - error code
        $text    - error string (optional)
        $custom  - custom error code (optional)

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a><br><br>

=head3 getClass

C<libgmsec_perl::Status-E<gt>getClass()>

    This function will return the error class ID.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a><br><br>

=head3 setClass

C<libgmsec_perl::Status-E<gt>setClass($eClass)>

    This will set the error class ID.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a><br><br>

=head3 getCode

C<libgmsec_perl::Status-E<gt>getCode()>

    This will return the error code number for this status for easy comparison.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a><br><br>

=head3 setCode

C<libgmsec_perl::Status-E<gt>setCode($eCode)>

    This will set the specific error code.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a><br><br>

=head3 getReason

C<libgmsec_perl::Status-E<gt>getReason()>

    This function will retrieve the string detailed description of this error.

=head3 setReason

C<libgmsec_perl::Status-E<gt>setReason($reason)>

    This will set the error string text.

=head3 getCustomCode

C<libgmsec_perl::Status-E<gt>getCustomCode()>

    This will return the custom error code number of this status object.

=head3 setCustomCode

C<libgmsec_perl::Status-E<gt>setCustomCode($code)>

    This will set the specific error code.

=head3 reset

C<libgmsec_perl::Status-E<gt>reset()>

    This clears this Status to GMSEC_STATUS_NO_ERROR.

=cut
%}
