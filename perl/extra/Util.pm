# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








use strict;

use Config;

package Util;


sub args
{
	my ($module) = @_;

	my $osname = $^O;
	my $inc = '-I../include -I../../framework/include';
	my $define = '';
	my $ccflags = $Config::Config{ccflags} || '';
	my $lddlflags = '';
	my $ldfrom = '';
	my $shortInstall = '';

	my $perlcc = $ENV{PERL_CC};

	resetCFLAGS();
	resetLDDLFLAGS();
	resetLDLOADLIBS();
	
	my @lib;

	if ($osname =~ /Win32/) {

		my $vc6 = $ENV{VC6_HOME};
		my $vshome = $ENV{VS_HOME}
			|| "c:/Program Files/Microsoft Visual Studio 9.0";
		my $msvc = $vc6 || "$vshome/VC";
		my $msdk = $ENV{MSDK_HOME}
			|| "c:/Program Files/Microsoft SDKs/Windows/v6.0a";

		my @libdir;
		my $win64 = $ENV{GMSEC_x64};
		if ($win64)
		{
			@libdir = (
				qq(/libpath:../../objects/Release),
				qq(/libpath:"$msvc/lib/amd64"),
				qq(/libpath:"$msdk/lib/x64"),
				'/libpath:..\\blib\\arch\\auto\\PERLGMSEC',
			);
		}
		else
		{
			@libdir = (
				qq(/libpath:../../objects/Release),
				qq(/libpath:"$msvc/lib"),
				qq(/libpath:"$msdk/lib"),
				'/libpath:..\\blib\\arch\\auto\\PERLGMSEC',
			);
		}
		

		$ldfrom = '$(OBJECT)';
		if (defined($module) and $module ne 'GMSECAPI') {
			$ldfrom .= ' ..\\GMSECAPI\\gmsec_perl$(OBJ_EXT)';
		}
		$ldfrom .= ' msvcprt.lib gmsecapi.lib';
		$inc .= qq( -I"$msvc/include" -I"$msdk/include");
		$define = '-DWIN32';

		if ($vc6) {
			$define .= ' -DGMSEC_VC6=1';
		}

		$ccflags .= " /EHsc";
		$lddlflags = join(' ', $Config::Config{lddlflags}, @libdir);
	}
	else {
		$ldfrom = '$(OBJECT)';
		if (defined($module) and $module ne 'GMSECAPI') {
			$ldfrom .= ' ../GMSECAPI/gmsec_perl$(OBJ_EXT)';
		}
		# ack! we have our own encoding from OS + compiler to bindir
		# this tag does not reliably match our platform identifiers
		# my $tag = $^O;
		# my $bin = "../bin/$tag";
		my $bin = "../bin";
		push(@lib, "-L../$bin -lGMSECAPI");
		$shortInstall = "$bin/lib";
	}

	my %args = (
		NAME => $module,
		VERSION => 1.0,
		INC => $inc,
		CCFLAGS => $ccflags,
		DEFINE => $define,
		LIBS => [ join(' ', @lib) ],
		INSTALLMAN1DIR => 'none',
		INSTALLMAN3DIR => 'none',
		INSTALLSITEMAN1DIR => 'none',
		INSTALLSITEMAN3DIR => 'none',
	);

	if ($perlcc) {
		$args{CC} = $perlcc;
	}

	if ($lddlflags) {
		$args{LDDLFLAGS} = $lddlflags;
	}

	if ($ldfrom) {
		$args{LDFROM} = $ldfrom;
	}

	if ($shortInstall) {
		foreach my $key (qw(INSTALLARCHLIB INSTALLPRIVLIB
				INSTALLSITEARCH INSTALLSITELIB)) {
			$args{$key} = $shortInstall;
		}
	}

	return %args;
}


# Adjust CFLAGS if necessary (currently only for HP-UX with gcc).
# It may simplify maintenance to pass in the flags through an
# environment variable- see config/hpux.pa:GMSEC_PERL_FLAGS.

sub resetCFLAGS
{
	my $text = q(
package MY;
sub cflags
{
	my ($self, @args) = @_;
	my $cflags = $self->SUPER::cflags(@args);
use English;
	my $cxx = $ENV{PERL_CC} || '';
# print "OSNAME=$OSNAME cxx=$cxx\n";
	if ($OSNAME eq 'hpux' && $cxx =~ /gcc|g\+\+/) {
		my $flag = '-include gmsec/util/hp_bug.h';
		$cflags =~ s/CFLAGS\s*=/CFLAGS = $flag/;
	}
	return $cflags;
}
);

	my $result = eval($text);
	# print "resetCFLAGS => $result\n";
}

# Adjust LDDLFLAGS if necessary (currently only for Win64).
sub resetLDDLFLAGS
{
	my $re = qr^\\\(x86\\\)^;
	my $text = q(
package MY;
sub const_config
{
	my ($self, @args) = @_;
	my $temp = $self->SUPER::const_config(@args);
use English;
#	print "const_config string is '$temp'";
	my $cxx = $ENV{PERL_CC} || '';
	my $x64 = $ENV{GMSEC_x64} || '';
# print "OSNAME=$OSNAME cxx=$cxx GMSEC_x64=$x64\n";
	if ($OSNAME eq 'MSWin32' && $x64) {
		# Removing the extra escape chars \ in Program Files (x86) libpath
		$temp =~ s/$re/(x86)/g;
	}
	return $temp;
}
);
	my $result = eval($text);
}

# Adjust LDLOADLIBS if necessary (currently only for Win64).
sub resetLDLOADLIBS
{
	my $re = qr^\\\(x86\\\)^;
	my $text = q(
package MY;
sub const_loadlibs
{
	my ($self, @args) = @_;
	my $temp = $self->SUPER::const_loadlibs(@args);
use English;
#	print "const_loadlibs string is '$temp'";
	my $cxx = $ENV{PERL_CC} || '';
	my $x64 = $ENV{GMSEC_x64} || '';
# print "OSNAME=$OSNAME cxx=$cxx GMSEC_x64=$x64\n";
	if ($OSNAME eq 'MSWin32' && $x64) {
		# Removing bufferoverflowU.lib
		$temp =~ s/bufferoverflowU.lib //i;
	}
	return $temp;
}
);
	my $result = eval($text);
}


1;

