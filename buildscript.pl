# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

 
######################################################################
#  Build Script
#  This script will read the values from files and build an include
#  file which will include a unique build number and other debugging
#  information.
#
#  This program should be called with the following command (probably
#  in a make file):
#  > buildscript.pl
#
######################################################################

use strict;

eval
{
	my $cxx_file     = 'framework/include/gmsec_version.h';
	my $java_file    = 'java/gov/nasa/gsfc/gmsecapi/ApiVersion.java';
	my $java_file_4x = 'java/gov/nasa/gsfc/gmsec/api/ApiVersion.java';
	my $bolt_file    = 'wrapper/bolt/java/bolt/Version.java';

	if ( ($#ARGV == 0) && ($ARGV[0] eq "clean") )
	{
		unlink($cxx_file);
		unlink($java_file);
		unlink($java_file_4x);
		unlink($bolt_file);
	}
	else
	{
		my %info;

		getTimestamp(\%info);
		# getBuild(\%info);
		getGMSEC(\%info);
		getOS(\%info);
		getCxx(\%info);
		getJava(\%info);
		getPerl(\%info);

		writeCxx($cxx_file, \%info);
		writeJava($java_file, "gmsecapi", \%info);
		writeJava($java_file_4x, "gmsec.api", \%info);
		writeBolt($bolt_file, "bolt", \%info);

		updateDoxygen(\%info, glob('doxygen/*_4x_*.html'));
	}

	print "done\n";
};

if ($@) {
	print "exception: $@\n";
	return 1;
}

exit 0;


#  End of Script


##############################################################
#
#  Beginning of Subroutines.
#
##############################################################
# Get the build number and increment it in the file

sub slurp
{
	my ($path) = @_;
	open(FH, $path)
		or die("unable to open $path [$!]");
	my @lines = <FH>;
	chomp(@lines);
	close(FH);
	return @lines;
}


sub getTimestamp
{
	my ($ref) = @_;

	my ($sec,$min,$hour,$day,$mon,$year,$wday,$yday,$isdst) = localtime(time);
	++$mon;
	$year += 1900;

	my $datestamp = sprintf('%02d/%02d/%d', $mon, $day, $year);
	my $timestamp = sprintf('%02d:%02d:%02d', $hour, $min, $sec);

	$ref->{DATESTAMP} = $datestamp;
	$ref->{TIMESTAMP} = $timestamp;
}


sub getBuild
{
	my ($ref) = @_;

	my @in = slurp('lastbuild.inf');

	my $build = $in[0];
	$build++;

	$ref->{BUILD} = $build;

	# Write updated info back to file
	open(FH,">lastbuild.inf") or die "Unable to clobber lastbuild.inf";
	print FH "$build"; 
	close FH;

} # getBuild


sub getGMSEC
{
	my ($ref) = @_;

	my @in = slurp('version.inf');

	my $version = $in[0];
	$version =~ s/^\s*(.*?)\s*$/$1/g;

	my @t = split(/\./, $version);
	my $x = @t;

	my $api_version = "0x";

	for (my $i=0; $i < $x; $i++) {
		$api_version .= sprintf("%02X", @t[$i]);
	}
	if ($x == 2) {
		$api_version .= "00";
	}

	$ref->{API} = $version;
	$ref->{API_VERSION} = $api_version;

} # getGMSEC


##############################################################
# Get the Perl version number
sub getPerl {

	use Config;

	my ($ref) = @_;

	my $version = sprintf('%vd', $^V);
	my $compiler = $Config{ccname} || $Config{cc};
	$compiler .= " $Config{ccversion}" if $Config{ccversion};
	my $details = "Perl v$version, $compiler, $Config{archname}";

	$ref->{PERL} = $details;

} # getPerl


##############################################################
# Get the java version number
sub getJava {

	my ($ref) = @_;

	my $path = 'java.ver';
	open(STDERR, '>' . $path);
	system("java -version");

	my @in = slurp($path);
	my $java = $in[0];
	$java =~ s/\"/$1/g;
	$java =~ s/^\s*(.*?)\s*$/$1/g;

	$ref->{JAVA} = $java;

} # getJava


##############################################################
# Get the C++ version number
sub getCxx
{
	my ($ref) = @_;

	my $cxx = '';
	my $c_env = "$ENV{CXX}";

	my $path = 'c.ver';

	if ($c_env ne "") 
	{
		open(STDERR, '>' . $path);

		system("$c_env -v");

		my @in = slurp($path);
		foreach my $tmp (@in) {
			if ($tmp =~ /version ([\d\.]+)/) {
				$cxx = $tmp;
			}
		}
		if (not $cxx) {
			$cxx = $in[0];
		}
	}
	else
	{   # Means they are using windows.
		# try running cl.exe
		my $tmpfile = 'msc.tmp';
		unlink($tmpfile);
		if (system("cl.exe 2> $tmpfile")) {
			print "unable to determine cl.exe version- using default\n";
		}
		elsif (-f $tmpfile) {
			my @in = slurp($tmpfile);
			foreach (@in) {
				if (/Version (\S+)/i) {
					$cxx = "Microsoft C/C++ $1";
				}
			}
		}

		if (not $cxx) {
			# use manually created msc.ver
			my @in = slurp('msc.ver');

			foreach (@in) {
				# Take the string if the word 'version' is in it...
				if (/version/g) {
					$cxx = $_;
				}
			} # while(<FH>)

			$cxx =~ s/\"/$1/g;
			$cxx =~ s/^\s*(.*?)\s*$/$1/g;
		}
	}

	$ref->{CXX} = $cxx;

} # getCxx


##############################################################
# Get the OS version number
sub getOS
{
	my ($ref) = @_;

	my $path = 'os.ver';
	my $c_env = "$ENV{CXX}";

	if ($c_env ne "") 
	{
		system("uname -s -r -m > $path");
	}
	else
	{   # Means they are using windows. 
		system("ver > $path");
	}

	# Read File in
	my @in = slurp($path);

	my $os = '';
	foreach (@in) {
		$os = $_;
	}

	$os =~ s/\"/$1/g;
	$os =~ s/^\s*(.*?)\s*$/$1/g;

	$ref->{OS} = $os;

} # getOS


sub writeCxx
{
	my ($path, $ref) = @_;

	open(FH, '>' . $path)
		or die("unable to open $path [$!]");

print FH qq(
/*
	gmsec_version.h autogenerated on:
	$ref->{DATESTAMP} $ref->{TIMESTAMP}
*/

#ifndef gmsec_version_h
#define gmsec_version_h

#define GMSEC_VERSION	"GMSEC API v$ref->{API} [$ref->{DATESTAMP}]"
#define GMSEC_VERSION_NUMBER "$ref->{API}"
#define GMSEC_API_VERSION $ref->{API_VERSION}
#define GMSEC_OS	"$ref->{OS}"
#define GMSEC_CXX	"$ref->{CXX}"
#define GMSEC_JAVA	"$ref->{JAVA}"
#define GMSEC_PERL	"$ref->{PERL}"


#ifdef GMSEC_OLD_MACROS

#define OS		"$ref->{OS}"
#define JAVA_VERSION	"$ref->{JAVA}"
#define C_PLUS_PLUS_VERSION "$ref->{CXX}"

#endif /* GMSEC_OLD_MACROS */

#endif
);

	close(FH);
} # end of writeCxx


sub writeJava
{
	my ($path, $pkgname, $ref) = @_;

	open(FH, '>' . $path)
		or die("unable to open $path [$!]");

print FH qq(
package gov.nasa.gsfc.$pkgname;

public class ApiVersion {

	public final static String GMSEC = "$ref->{API} [$ref->{DATESTAMP}]";
	public final static String OS = "$ref->{OS}";
	public final static String JAVA = "$ref->{JAVA}";
	public final static String CXX = "$ref->{CXX}";
	public final static String MB = "$ref->{API} [$ref->{DATESTAMP}]";

	public final static String GMSEC_VERSION = "$ref->{API} [$ref->{DATESTAMP}]";
	public final static String GMSEC_VERSION_NUMBER = "$ref->{API}";
	public final static int    GMSEC_API_VERSION = $ref->{API_VERSION};

	public final static String JAVA_VERSION = "$ref->{JAVA}";
	public final static String C_PLUS_PLUS_VERSION = "$ref->{CXX}";
	public final static String GMSEC_MB_VERSION = "MB v$ref->{API} [$ref->{DATESTAMP}]";

}

);

	close(FH);
} # end of writeJava


sub writeBolt
{
	my ($path, $pkgname, $ref) = @_;

	open(FH, '>' . $path)
		or die("unable to open $path [$!]");

print FH qq(
package $pkgname;

public class Version {

	public final static String GMSEC = "$ref->{API} [$ref->{DATESTAMP}]";
	public final static String OS = "$ref->{OS}";
	public final static String JAVA = "$ref->{JAVA}";

	public final static String GMSEC_VERSION = "$ref->{API} [$ref->{DATESTAMP}]";
	public final static String JAVA_VERSION = "$ref->{JAVA}";
	public final static String GMSEC_BOLT_VERSION = "Bolt v$ref->{API} [$ref->{DATESTAMP}]";
}

);

	close(FH);
} # end of writeBolt


sub updateDoxygen
{
	my ($ref, @files) = @_;

	my $reRelease = qr(Release ([0-9.]+));
	foreach my $file (@files) {
		my $update = 0;
		my @lines = slurp($file);
		foreach my $line (@lines) {
			if ($line =~ $reRelease) {
				if ($1 ne $ref->{API}) {
					++$update;
					my @a = split('-', $ref->{API});
					$line =~ s/$reRelease/Release $a[0]/g;
					print "updateDoxygen: $line\n";
				}
			}
		}
		if ($update) {
			open(FH, ">$file")
				or die("unable to rewrite $file [$!]");
			map { print FH "$_\n" } @lines;
			close(FH);
		}
	}
}

