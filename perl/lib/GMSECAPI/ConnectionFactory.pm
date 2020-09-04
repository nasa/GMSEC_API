 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








##############################################################################
# ConnectionFactory.pm
#
# Description: Class for creating and destroying connections
#
##############################################################################

package GMSECAPI::ConnectionFactory;

use strict;
use base qw(DynaLoader);
use GMSECAPI::Util;

bootstrap GMSECAPI::ConnectionFactory;


use vars qw($statusPointer);


sub Status {
    if (defined($statusPointer) && $statusPointer =~ /^-?\d+$/) {
	return bless({ pointer => $statusPointer }, 'GMSECAPI::Status');
    } else {
	return undef;
    }
}


1;

__END__


=head1 NAME

GMSECAPI::ConnectionFactory - Connection Creation and Destruction for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    my $cfg = GMSECAPI::Config->new([ @ARGV ]);

    my $conn = GMSECAPI::ConnectionFactory::Create($cfg);

    GMSECAPI::ConnectionFactory::Destroy($conn);

=head1 DESCRIPTION

The C<GMSECAPI> module loads all required packages into the environment and defines all of the constants needed.

=head1 FUNCTIONS

=over 8

=item $conn = GMSECAPI::ConnectionFactory::Create($connType, $cfg)

=item $conn = GMSECAPI::ConnectionFactory::Create($cfg)

This function creates a connection to the GMSEC message bus.  The first version has the connection type specified as the first argument.  The second version must have the connection type specified in the GMSECAPI::ConnectionFactory object, which is the first argument.

=item GMSECAPI::ConnectionFactory::Destroy($conn)

This function destroys the connection to the GMSEC message bus.

=item $status = GMSECAPI::ConnectionFactory::Status()

This function returns a GMSECAPI::Status object for the status of the last method call in the GMSECAPI::Connection class.

=back

=head1 AUTHOR

 Timothy Esposito <timothy.esposito@nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

L<GMSECAPI>, L<GMSECAPI::Status>, L<GMSECAPI::Field>, L<GMSECAPI::Message>, L<GMSECAPI::Config>, L<GMSECAPI::Connection>, L<GMSECAPI::Callback>

=cut
