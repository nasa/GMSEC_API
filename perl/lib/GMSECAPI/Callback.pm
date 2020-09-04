 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
##############################################################################
# Callback.pm
#
# Description: GMSEC API message callbacks.  This module acts as a sample
#    template to be used for creating your own callback class.  All it does
#    is simply print out the message in XML format.
#
##############################################################################

package GMSECAPI::Callback;

use strict;


sub new ($) {
    my $this = shift;
    my $class = ref($this) || $this;
    return bless({ }, $class);
}


sub OnMessage ($$$) {
    my ($self, $conn, $msg) = @_;

    if ( ref($msg) eq 'GMSECAPI::Message' ) {
	print $msg->ToXML, "\n";
    }
}


1;

__END__


=head1 NAME

GMSECAPI::Callback - Sample callback package for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    my $cfg = GMSECAPI::Config->new([ @ARGV ]);
    my $conn = GMSECAPI::ConnectionFactory::Create($cfg);
    $conn->Connect;

    my $cb = GMSECAPI::Callback->new;
    $conn->Subscribe("gmsec.*.publish", $cb);

    $conn->StartAutoDispatch;

=head1 DESCRIPTION

The C<GMSECAPI::Callback> module is a sample callback package that simply prints out the recieved message in an XML format.  It is meant to be used as a template for creating your own callback module.

=head1 METHODS

=over 8

=item $cb = GMSECAPI::Callback->new

This method created a new callback object.

=item $cb->OnMessage($conn, $msg)

OnMessage is a method used to handle the processing of messages.  $conn is a GMSECAPI::Connection object, and $msg is a GMSECAPI::Message object.  This method is called automatically by the message dispatcher.

=back

=head1 AUTHOR

 Timothy Esposito <timothy.esposito@nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

L<GMSECAPI>, L<GMSECAPI::Status>, L<GMSECAPI::Field>, L<GMSECAPI::Message>, L<GMSECAPI::Config>, L<GMSECAPI::ConnectionFactory>, L<GMSECAPI::Connection>

=cut
