 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

 
##############################################################################
# Field.pm
#
# Description: Base class for message fields
#
##############################################################################

package GMSECAPI::Field;

use strict;
use base qw(DynaLoader);
use GMSECAPI::Util;

bootstrap GMSECAPI::Field;


sub new ($;$$$)
{
	my ($this, $type, $name, $value, $size) = @_;
	
	my $class = ref($this) || $this;
	
	my $self = { pointer => undef };
	
	bless($self, $class);
	
	if (defined($type) && $type =~ /^\d+$/)
	{
		if (defined($size) && $size =~ /^\d+$/ && $type == $GMSECAPI::GMSEC_TYPE_BIN)
		{
			Field($self, $type, $name, $value, $size);
		}
		else
		{
			Field($self, $type, $name, $value);
		}
	}
	else
	{
		Field($self);
	}
	
	return $self;
}


sub Status {
    my ($self) = @_;

    return GMSECAPI::Util::GetStatus($self);
}


1;

__END__


=head1 NAME

GMSECAPI::Field - Message Field Class for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    my $field = GMSECAPI::Field->new($GMSEC_TYPE_BOOL, 'bool_field', $GMSEC_TRUE);

    print $fld->ToXML(), "\n";

=head1 DESCRIPTION

The C<GMSECAPI::Field> module is a class used to manage and manipulate the individual fields of a message.  Please view the documentation for the C++ version of the GMSEC API to read a functionality overview of all methods listed below.  All method names are the same and arguments match up in most cases.  The main difference is that the status is stored in the Perl API and is returned in the C++ API.

=head1 METHODS

=over 8

=item $field = GMSECAPI::Field->new

=item $field = GMSECAPI::Field->new($type, $name, $value)

=item $field = GMSECAPI::Field->new($type, $name, $value, $size)

Above are three constructors for a message field object.  The second version sets, the type, name, and value.  The third version is for the $GMSEC_TYPE_BIN data type, which requires the value's size as a fourth argument.

=item $status = $field->Status

The method returns a GMSECAPI::Status object for the status of the last method call in the GMSECAPI::Field class.

=item $name = $field->GetName

=item $field->SetName($name)

=item $type = $field->GetType

=item $field->SetType($type)

=item $value = $field->GetValue

=item ($value, $size) = $field->GetValue

=item $field->SetValue($value)

=item $field->SetValue($value, $size)

=item $field->UnSet

=item $xml = $field->ToXML

=back

=head1 AUTHOR

 GMSEC API Team <gmsec-support@lists.nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

=for html <a href="Message.html">Message</a><br><br>

=for html <a href="Status.html">Status</a><br><br>

=for html <a href="Util.html">Util</a><br><br>

=for html <a href="../GMSECAPI.html">GMSECAPI</a><br><br>

=cut
