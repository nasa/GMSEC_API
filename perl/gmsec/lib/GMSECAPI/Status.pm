 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

 
##############################################################################
# Status.pm
#
# Description: Class for holding status information
#
##############################################################################

package GMSECAPI::Status;

use strict;
use base qw(DynaLoader);

bootstrap GMSECAPI::Status;


sub new ($;$$$) {
    my ($this, $eclass, $code, $text) = @_;

    my $class = ref($this) || $this;

    my $self = { pointer => undef };

    bless($self, $class);

    if (defined $eclass && defined $code && defined $text) {
	Status($self, $eclass, $code, $text);
    } else {
	Status($self);
    }

    return $self;
}


1;

__END__


=head1 NAME

GMSECAPI::Status - Status Class for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    $status = GMSECAPI::Status->new($GMSEC_STATUS_CONFIG_ERROR,
                                    $GMSEC_INVALID_CONFIG_VALUE,
                                    'Config value not set');

    print $status->Get, "\n";

=head1 DESCRIPTION

The C<GMSECAPI::Status> module is used to store the status of the last executed command for a particular class.  Please view the documentation for the C++ version of the GMSEC API to read a functionality overview of all methods listed below.  All method names are the same and arguments match up in most cases.

=head1 METHODS

=over 8

=item $status = GMSECAPI::Status->new

=item $status = GMSECAPI::Status->new($class, $code, $text)

The above two methods are constructors for the status class.

=item $string = $status->GetString

=item $string = $status->Get

=item $class = $status->GetClass

=item $code = $status->GetCode

=item $status->SetString($text)

=item $status->SetClass($class)

=item $status->SetCode($code)

=item $status->Set($class, $code, $text)

=item $status->ReSet

=back

=head1 AUTHOR

 GMSEC API Team <gmsec-support@lists.nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

=for html <a href="Util.html">Util</a><br><br>

=for html <a href="../GMSECAPI.html">GMSECAPI</a><br><br>

=cut
