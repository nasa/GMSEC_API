/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module GmsecException
%{
#include <gmsec5/GmsecException.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::GmsecException::operator=(const GmsecException&);

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/GmsecException.h>

%feature("director:except") {
    if ($error != NULL) {
        void*  argp1 = 0;
        int res1 = SWIG_Perl_ConvertPtr($error, &argp1, SWIGTYPE_p_gmsec__api5__GmsecException, 0);

        if (SWIG_IsOK(res1)) {
            gmsec::api5::GmsecException* e = reinterpret_cast<gmsec::api5::GmsecException*>(argp1);
            throw *e;
        }
        else {
            STRLEN len;
            char* e = SvPV($error, len);
            if (e != NULL) {
                throw gmsec::api5::GmsecException(OTHER_ERROR, OTHER_ERROR_CODE, e);
            }
            else {
                throw gmsec::api5::GmsecException(OTHER_ERROR, OTHER_ERROR_CODE, "Unknown error");
            }
        }
    }
}

%exception {
    try
    {
        $action
    }
    catch (const gmsec::api5::GmsecException& e)
    {
        %raise (SWIG_NewPointerObj(new gmsec::api5::GmsecException(e), SWIGTYPE_p_gmsec__api5__GmsecException, SWIG_OWNER | SWIG_SHADOW),
                "libgmsec_perl::GmsecException",
                SWIGTYPE_p_gmsec__api5__GmsecException);
    }
}

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::GmsecException

=head1 DESCRIPTION

This class defines the standard GMSEC exception.

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::GmsecException-E<gt>new($errorClass, $errorCode, $errorMessage)>

        Default constructor

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $errorClass - value indicating source of the error
        $errorCode - value indicating reason for the error
        $errorMessage - explanation for the error

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a>

C<libgmsec_perl::GmsecException-E<gt>new($errorClass, $errorCode, $customCode, $errorMessage)>

        Special constructor that can be used to set a custom error code.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $errorClass - value indicating source of the error
        $errorCode - value indicating reason for the error
        $customCode - custom error code (typically middleware specific) on the root cause of the error
        $errorMessage - explanation for the error

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a>

C<libgmsec_perl::GmsecException-E<gt>new($status)>

        Special constructor that uses information from a Status object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $status - A Status object

C<libgmsec_perl::GmsecException-E<gt>new($exception)>

        Copy constructor

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $exception - the GmsecException object to copy

=head3 DESTROY

C<libgmsec_perl::GmsecException-E<gt>DESTROY()>

        Destructor

=head3 getErrorClass

C<libgmsec_perl::GmsecException-E<gt>getErrorClass()>

        Returns the error class associated with the exception.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        A numeric value

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a>

=head3 getErrorCode

C<libgmsec_perl::GmsecException-E<gt>getErrorCode()>

        Returns the error code associated with the exception.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        A numeric value

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a>

=head3 getCustomCode

C<libgmsec_perl::GmsecException-E<gt>getCustomCode()>

        Returns the custom error code associated with the exception.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        An error code originating from a third party library (middleware or otherwise) related to the error that occurred.

=head3 getErrorMessage

C<libgmsec_perl::GmsecException-E<gt>getErrorMessage()>

        Returns the error message associated with the exception.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        A string containing the error message

=head3 what

C<libgmsec_perl::GmsecException-E<gt>what()>

        Returns string with the format of [errorClass,errorCode,customCode] : message.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;Returns:</b>

        A string containing the error class, code, custom code and message.

=for html <b>&nbsp;&nbsp;&nbsp;&nbsp;See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Errors.html">Error Values</a>

=cut
%}
