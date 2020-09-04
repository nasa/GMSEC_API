 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








##############################################################################
# GMSECAPI::Util.pm
#
# Description: Internal utility functions for Perl GMSEC API.
#
##############################################################################

package GMSECAPI::Util;

use strict;


sub GetStatus ($)
{
	my ($object) = @_;

	if (defined($object->{status}) and $object->{status} =~ /^-?\d+$/) {
		return bless({ pointer => $object->{status} }, 'GMSECAPI::Status');
	}
	else {
		return undef;
	}
}



1;

__END__


=head1 NAME

GMSECAPI::Util - Utility functions internal to the Perl GMSEC API.

=head1 SYNOPSIS

    use GMSECAPI::Util;

    my $object = <some GMSECAPI type>;
    $object->CallFunction;
    return GMSECAPI::Util::GetStatus($object);

=head1 DESCRIPTION

The C<GMSECAPI::Util> module is to provide a central location for functions internal to the Perl GMSEC API.

=head1 METHODS

=over 8

=item $status = GMSECAPI::Util::GetStatus($object);

This method retrieves the GMSECAPI::Status from $object.

=back

=head1 AUTHOR

 NASA Goddard Space Flight Center

=head1 SEE ALSO

L<GMSECAPI>

=cut
