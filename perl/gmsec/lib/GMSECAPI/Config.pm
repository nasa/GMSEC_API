 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

 
##############################################################################
# Config.pm
#
# Description: Class for managing configurations
#
##############################################################################

package GMSECAPI::Config;

use strict;
use base qw(DynaLoader);
use GMSECAPI::Util;

bootstrap GMSECAPI::Config;


sub new ($;$) {
    my ($this, $argv) = @_;

    my $class = ref($this) || $this;

    my $self = { pointer => undef };

    bless($self, $class);

    if (defined($argv) && ref($argv) eq 'ARRAY') {
	Config($self, $argv);
    } else {
	Config($self);
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

GMSECAPI::Config - Configuration Class for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    my $cfg = GMSECAPI::Config->new([ @ARGV ]);
    $cfg->AddValue('connectionType', $GMSEC_CONNECTION_ICSSWB);

=head1 DESCRIPTION

The C<GMSECAPI::Config> module is a class for managing configurations.  Please view the documentation for the C++ version of the GMSEC API to read a functionality overview of all methods listed below.  All method names are the same and arguments match up in most cases.  The main difference is that the status is stored in the Perl API and is returned in the C++ API.

=head1 METHODS

=over 8

=item $cfg = GMSECAPI::Config->new

=item $cfg = GMSECAPI::Config->new($args);

This method created a new configuration object.  The argument in the second constructor is an array reference of name-value configuration pairs.

=item $status = $cfg->Status

The method returns a GMSECAPI::Status object for the status of the last method call in the GMSECAPI::Config class.

=item $cfg->AddValue($name,$value)

=item $cfg->ClearValue($name)

=item $value = $cfg->GetValue($name)

=item $cfg->Clear

=item ($name, $value) = $cfg->GetFirst

=item ($name, $value) = $cfg->GetNext

=back

=head1 AUTHOR

 GMSEC API Team <gmsec-support@lists.nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

=for html <a href="ConfigFile.html">ConfigFile</a><br><br>

=for html <a href="ConnectionFactory.html">ConnectionFactory</a><br><br>

=for html <a href="Message.html">Message</a><br><br>

=for html <a href="Status.html">Status</a><br><br>

=for html <a href="Util.html">Util</a><br><br>

=for html <a href="../GMSECAPI.html">GMSECAPI</a><br><br>

=cut
