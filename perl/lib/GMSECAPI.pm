 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
##############################################################################
# GMSECAPI.pm
#
# Description: GMSEC API constants
#
##############################################################################

BEGIN {
 use Config;
 if (!$Config{usethreads})
 {
    print STDERR "ERROR:  in order to use GMSEC, perl must be compiled with threads\n";
    exit 1;
  }
}

package GMSECAPI;

use strict;

use GMSECAPI::Status;
use GMSECAPI::Field;
use GMSECAPI::Message;
use GMSECAPI::Config;
use GMSECAPI::ConnectionFactory;
use GMSECAPI::Connection;
use GMSECAPI::Callback;
use GMSECAPI::ConfigFile;

use base qw(DynaLoader Exporter);

bootstrap GMSECAPI;


use vars qw(@EXPORT);

@EXPORT = qw( $GMSEC_CONNECTION_ICSSWB
	      $GMSEC_CONNECTION_RENDEZVOUS
	      $GMSEC_CONNECTION_SMARTSOCKETS

	      $GMSEC_TYPE_UNSET
	      $GMSEC_TYPE_CHAR
	      $GMSEC_TYPE_BOOL
	      $GMSEC_TYPE_SHORT
	      $GMSEC_TYPE_USHORT
	      $GMSEC_TYPE_LONG
	      $GMSEC_TYPE_ULONG
	      $GMSEC_TYPE_FLOAT
	      $GMSEC_TYPE_DOUBLE
	      $GMSEC_TYPE_STRING
	      $GMSEC_TYPE_BIN
	      $GMSEC_TYPE_I8
	      $GMSEC_TYPE_U8
	      $GMSEC_TYPE_I16
	      $GMSEC_TYPE_U16
	      $GMSEC_TYPE_I32
	      $GMSEC_TYPE_U32
	      $GMSEC_TYPE_I64
	      $GMSEC_TYPE_F32
	      $GMSEC_TYPE_F64

	      $GMSEC_TRUE
	      $GMSEC_FALSE

	      $GMSEC_MSG_UNSET
	      $GMSEC_MSG_PUBLISH
	      $GMSEC_MSG_REQUEST
	      $GMSEC_MSG_REPLY

	      $GMSEC_NO_WAIT
	      $GMSEC_WAIT_FOREVER

	      $NULL

	      $GMSEC_STATUS_NO_ERROR
	      $GMSEC_STATUS_FACTORY_ERROR
	      $GMSEC_STATUS_CONNECTION_ERROR
	      $GMSEC_STATUS_CONFIG_ERROR
	      $GMSEC_STATUS_MIDDLEWARE_ERROR
	      $GMSEC_STATUS_MESSAGE_ERROR
	      $GMSEC_STATUS_FIELD_ERROR
	      $GMSEC_STATUS_CALLBACK_ERROR
	      $GMSEC_STATUS_CALLBACKLKP_ERROR

	      $GMSEC_LIBRARY_LOAD_FAILURE
	      $GMSEC_INVALID_CONNECTION_TYPE
	      $GMSEC_AUTODISPATCH_FAILURE
	      $GMSEC_INVALID_CONNECTION
	      $GMSEC_FEATURE_NOT_SUPPORTED
	      $GMSEC_INVALID_CONFIG_VALUE
	      $GMSEC_CONFIG_END_REACHED
	      $GMSEC_INVALID_MESSAGE
	      $GMSEC_UNKNOWN_MSG_TYPE
	      $GMSEC_FIELDS_END_REACHED
	      $GMSEC_FIELD_TYPE_MISMATCH
	      $GMSEC_UNKNOWN_FIELD_TYPE
	      $GMSEC_INVALID_CALLBACK
	      $GMSEC_REQDISPATCH_FAILURE
	      $GMSEC_MSGCONVERT_FAILURE

	      $GMSEC_NO_MESSAGE_AVAILABLE

	      $GMSEC_TIMEOUT_OCCURRED
	      $GMSEC_TRACKING_FAILURE
	      $GMSEC_UNUSED_CONFIG_ITEM
	      $GMSEC_INVALID_FIELD
	      $GMSEC_XML_PARSE_ERROR
	      $GMSEC_INVALID_CONFIG
	      $GMSEC_INVALID_FIELD_NAME

	      $GMSEC_OTHER_ERROR );


1;

__END__


=head1 NAME

GMSECAPI - Standard Perl Interface to the GMSEC API

=head1 CAVEAT

B<A threaded version of Perl is required for this API to work.>  Please note that thread support is not always built in by default.  All binaries retrieved from ActiveState at L<http://www.activestate.com/ActivePerl> are supported.  This includes the Linux, Solaris, and Windows versions.  It has been tested successfully with Perl versions 5.6.2 and 5.8.3.  Below are directions for configuration options needed to build a compatible version of perl.  Also note that the Perl GMSEC API built with either of these two versions of perl are not compatible with the other.  The GMSEC releases are built with the latest ActiveState release of perl.

    1. Download http://www.cpan.org/src/5.0/perl-5.6.2.tar.gz
    2. Unzip and untar the downloaded file in a separate directory.
    3. Use the following commands to build and install ...
       ./Configure -de -Dcc=gcc -Dusethreads -Dprefix=~/perl-5.6.2 -Uinstallusrbinperl
       make
       make test
       make install

    1. Download http://www.cpan.org/src/5.0/perl-5.8.3.tar.gz
    2. Unzip and untar the downloaded file in a separate directory.
    3. Use the following commands to build and install ...
       ./Configure -de -Dcc=gcc -Dusethreads -Duseperlio -Dprefix=~/perl-5.8.3 -Uinstallusrbinperl
       make
       make test
       make install

=head1 SYNOPSIS

    use GMSECAPI;

=head1 DESCRIPTION

The C<GMSECAPI> module loads all required packages into the environment and defines all of the constants needed.

=head1 CONSTANTS

=over 8

=item $GMSEC_CONNECTION_ICSSWB

=item $GMSEC_CONNECTION_RENDEZVOUS

=item $GMSEC_CONNECTION_SMARTSOCKETS

Constants are used to define the connection type, or which GMSEC middleware should be used.

=item $GMSEC_TYPE_UNSET

=item $GMSEC_TYPE_CHAR

=item $GMSEC_TYPE_BOOL

=item $GMSEC_TYPE_SHORT

=item $GMSEC_TYPE_USHORT

=item $GMSEC_TYPE_LONG

=item $GMSEC_TYPE_ULONG

=item $GMSEC_TYPE_FLOAT

=item $GMSEC_TYPE_DOUBLE

=item $GMSEC_TYPE_I8

=item $GMSEC_TYPE_U8

=item $GMSEC_TYPE_I16

=item $GMSEC_TYPE_U16

=item $GMSEC_TYPE_I32

=item $GMSEC_TYPE_U32

=item $GMSEC_TYPE_I64

=item $GMSEC_TYPE_F32

=item $GMSEC_TYPE_F64

=item $GMSEC_TYPE_STRING

=item $GMSEC_TYPE_BIN

There are 10 different defined variable types that can be used for message fields.

=item $GMSEC_TRUE

=item $GMSEC_FALSE

True and false values are defined for the GMSEC API.

=item $GMSEC_MSG_UNSET

=item $GMSEC_MSG_PUBLISH

=item $GMSEC_MSG_REQUEST

=item $GMSEC_MSG_REPLY

These are the different message types, including those needed for the publish-subscribe and request-reply paradigms.

=item $GMSEC_NO_WAIT

=item $GMSEC_WAIT_FOREVER

Constants are defined for connections to not wait at all or wait forever for receiving messages on the message bus.

=item $NULL

This is a defined value for NULL.

=item $GMSEC_STATUS_NO_ERROR

=item $GMSEC_STATUS_FACTORY_ERROR

=item $GMSEC_STATUS_CONNECTION_ERROR

=item $GMSEC_STATUS_CONFIG_ERROR

=item $GMSEC_STATUS_MIDDLEWARE_ERROR

=item $GMSEC_STATUS_MESSAGE_ERROR

=item $GMSEC_STATUS_FIELD_ERROR

=item $GMSEC_STATUS_CALLBACK_ERROR

=item $GMSEC_STATUS_CALLBACKLKP_ERROR

Codes are defined for certain classes of errors.

=item $GMSEC_LIBRARY_LOAD_FAILURE

=item $GMSEC_INVALID_CONNECTION_TYPE

=item $GMSEC_AUTODISPATCH_FAILURE

=item $GMSEC_INVALID_CONNECTION

=item $GMSEC_FEATURE_NOT_SUPPORTED

=item $GMSEC_INVALID_CONFIG_VALUE

=item $GMSEC_CONFIG_END_REACHED

=item $GMSEC_INVALID_MESSAGE

=item $GMSEC_UNKNOWN_MSG_TYPE

=item $GMSEC_FIELDS_END_REACHED

=item $GMSEC_FIELD_TYPE_MISMATCH

=item $GMSEC_UNKNOWN_FIELD_TYPE

=item $GMSEC_INVALID_CALLBACK

=item $GMSEC_REQDISPATCH_FAILURE

=item $GMSEC_MSGCONVERT_FAILURE

=item $GMSEC_NO_MESSAGE_AVAILABLE

=item $GMSEC_OTHER_ERROR

Constants are defined for many of the common GMSEC errors.

=back

=head1 AUTHOR

 Timothy Esposito <timothy.esposito@nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

L<GMSECAPI::Status>, L<GMSECAPI::Field>, L<GMSECAPI::Message>, L<GMSECAPI::Config>, L<GMSECAPI::ConnectionFactory>, L<GMSECAPI::Connection>, L<GMSECAPI::Callback>

=cut
