 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
##############################################################################
# Message.pm
#
# Description: Base class for GMSEC messages
#
##############################################################################

package GMSECAPI::Message;

use strict;
use base qw(DynaLoader);
use GMSECAPI::Util;

bootstrap GMSECAPI::Message;


sub Status {
    my ($self) = @_;

    return GMSECAPI::Util::GetStatus($self);
}


1;

__END__


=head1 NAME

GMSECAPI::Message - Message class for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    my $cfg = GMSECAPI::Config->new([ @ARGV ]);

    my $conn = GMSECAPI::ConnectionFactory::Create($cfg);
    my $status = $conn->Status;
    $conn->Connect;

    my $msg = $conn->CreateMessage('gmsec.test.publish', $GMSEC_MSG_PUBLISH);

    if ( $msg->isValid ) {
        $msg->SetKind($GMSEC_MSG_REQUEST);
        $msg->SetSubject('gmsec.test.request');

        my $fld = GMSECAPI::Field->new($GMSEC_TYPE_CHAR, 'char_field', 'C');
        $msg->AddField($fld);

        print $msg->ToXML, "\n";
    }

    $conn->Disconnect;
    GMSECAPI::ConnectionFactory::Destroy($conn);

=head1 DESCRIPTION

The C<GMSECAPI::Message> module is a class for managing and manipulating messages on the GMSEC message bus.  Messages are either created directly with the connection object or returned from a request.  Please view the documentation for the C++ version of the GMSEC API to read a functionality overview of all methods listed below.  All method names are the same and arguments match up in most cases.  The main difference is that the status is stored in the Perl API and is returned in the C++ API.

=head1 METHODS

=over 8

=item $status = $msg->Status

This method returns a GMSECAPI::Status object for the status of the last method call in the GMSECAPI::Message class.

=item $valid = $msg->isValid

=item $libname = $msg->GetLibraryRootName

=item $msg->SetKind($kind)

=item $kind = $msg->GetKind

=item $msg->SetSubject($subject)

=item $subject = $msg->GetSubject

=item $msg->AddField($field)

=item $field = $msg->GetField($name)

=item $count = $msg->GetFieldCount

=item $field = $msg->GetFirstField

=item $field = $msg->GetNextField

=item $xml = $msg->ToXML

=item $size = $msg->GetMsgSize

=item $time = $msg->GetTime

=back

=head1 AUTHOR

 Timothy Esposito <timothy.esposito@nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

L<GMSECAPI>, L<GMSECAPI::Status>, L<GMSECAPI::Field>, L<GMSECAPI::Config>, L<GMSECAPI::ConnectionFactory>, L<GMSECAPI::Connection>, L<GMSECAPI::Callback>

=cut
