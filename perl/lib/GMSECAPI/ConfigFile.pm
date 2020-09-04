 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
##############################################################################
# ConfigFile.pm
#
# Description: Class for managing config files
#
##############################################################################

package GMSECAPI::ConfigFile;

use strict;
use base qw(DynaLoader);
use GMSECAPI::Util;

bootstrap GMSECAPI::ConfigFile;


sub new ($;$) {
    my ($this, $filepath) = @_;

    my $class = ref($this) || $this;

    my $self = { pointer => undef };

    bless($self, $class);

    ConfigFile($self, $filepath);

    return $self;
}


sub Status {
    my ($self) = @_;

    return GMSECAPI::Util::GetStatus($self);
}


1;

__END__


=head1 NAME

GMSECAPI::ConfigFile - ConfigFile Class for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    my $cfg_file = GMSECAPI::ConfigFile->new($filepath);

=head1 DESCRIPTION

The C<GMSECAPI::ConfigFile> module is a class for managing configuration files.  Please view the documentation for the C++ version of the GMSEC API to read a functionality overview of all methods listed below.  All method names are the same and arguments match up in most cases.  The main difference is that the status is stored in the Perl API and is returned in the C++ API.

=head1 METHODS

=over 8

=item $cfg_file = GMSECAPI::ConfigFile->Load

=item $cfg_file = GMSECAPI::ConfigFile->LookupMessage($msgName, $msg);

=item $cfg_file = GMSECAPI::ConfigFile->LookupConfig($cfgName, $cfg);

=item $cfg_file = GMSECAPI::ConfigFile->LookupConfig($subName, $sub);

=item $status = $cfg_file->Status

The method returns a GMSECAPI::Status object for the status of the last method call in the GMSECAPI::ConfigFile class.

=back

=head1 AUTHOR

 Bill Duvall <wduvall@csc.com>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

L<GMSECAPI>, L<GMSECAPI::Status>, L<GMSECAPI::Field>, L<GMSECAPI::Message>, L<GMSECAPI::ConnectionFactory>, L<GMSECAPI::Connection>, L<GMSECAPI::Callback>

=cut
