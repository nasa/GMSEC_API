# Copyright 2009-2017 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# Modify GMSEC API Visual C++ 2008 project files for use with Visual C++ 6.
#
# usage: perl vc6.pl [<file>|<directory>] ...


use strict;

use DirHandle;
use FileHandle;


use constant PROJECT_FILE => 'Project';
use constant SOLUTION_FILE => 'Solution';


{
	my %data = (
		BACKUP => '.orig',
		SIGNAL_FILE => 'vcproj.converted',
		COUNT_VCPROJ => 0,
		COUNT_SLN => 0,
	);

	my $task = bless(\%data);

	foreach my $arg (@ARGV) {
		if (-d $arg) {
			if (-e "$arg/$data{SIGNAL_FILE}") {
				$task->warning("directory $arg already processed");
				next;
			}
			$task->iterateDirectory($arg, sub {
				my ($info) = @_;
				my $name = $info->{NAME};
				my $full = $info->{FULL};
				if ($name =~ /\.vcproj$/) {
					$task->processFile(PROJECT_FILE, $full);
				}
				elsif ($name =~ /\.sln$/) {
					$task->processFile(SOLUTION_FILE, $full);
				}
			});

			my $fh = FileHandle->new("$arg/$data{SIGNAL_FILE}", 'w');
			$fh->print("$task->{COUNT_VCPROJ} .vcproj files converted\n");
			$fh->print("$task->{COUNT_SLN} .sln files converted\n");
			$fh->close;

		}
		elsif (-f $arg) {
			if ($arg =~ /\.sln$/) {
				$task->processFile(SOLUTION_FILE, $arg);
			}
			else {
				$task->processFile(PROJECT_FILE, $arg);
			}
		}
		else {
			$task->warning("ignoring $arg [not a file or directory");
		}
	}
}


sub dirContents
{
	my ($dir) = @_;

	my $dh = DirHandle->new($dir);
	if (not $dh) {
		die("unable to open $dir [$!]");
	}

	my @names = $dh->read;
	undef($dh);

	my $sep = '/';

	my @contents;
	foreach my $name (@names) {
		next if $name eq '.';
		next if $name eq '..';
		my $path = "$dir$sep$name";
		my %info = (
			DIR => $dir,
			NAME => $name,
			FULL => $path,
		);
		push(@contents, \%info);
	}

	return @contents;
}



sub iterateDirectory
{
	my ($self, $dir, $filesub, $dirsub) = @_;

	my @contents = dirContents($dir);

	foreach my $info (@contents) {
		if (-d $info->{FULL}) {
			if ($dirsub) {
				$dirsub->($info);
			}
			else {
				# recurse by default
				$self->iterateDirectory($info->{FULL}, $filesub, $dirsub);
			}
		}
		elsif (-f $info->{FULL}) {
			$filesub->($info);
		}
	}
}


sub processFile
{
	my ($self, $type, $full) = @_;

	print "process $type file $full\n";

	my $old = "$full$self->{BACKUP}";

	if (-e $old) {
		$self->warning("$old already exists");
	}

	rename($full, $old)
		or die("unable to rename($full, $old) [$!]");

	my $in = FileHandle->new($old)
		or die("unable to open $old [$!]");

	my $out = FileHandle->new($full, 'w')
		or die("unable to create $full [$!]");


	my $method = 'process' . $type . 'File';
	$self->$method($in, $out);

	$out->close;

	undef($in);
}


sub processProjectFile
{
	my ($self, $in, $out) = @_;

	++$self->{COUNT_PROJECT};

	my $inTool = undef;
	my $tool = undef;

	while (<$in>) {
		
		my $line = $_;

		if (not $inTool) {
			if (/<Tool/) {
				$inTool = 1;
			}
			elsif (m|/>|) {
				$inTool = 0;
			}
		}
		elsif (not $tool) {
			if (/Name="(.+)"/) {
				$tool = $1;
# print "started tool '$tool'\n";
			}
		}
		else {
			if (/RandomizedBaseAddress=/) {
				$line =~ s/Randomized.+/RandomizedBaseAddress="0"/;
			}
			elsif (/BasicRuntimeChecks=/) {
				$line =~ s/BasicRuntime.+/BasicRuntimeChecks="0"/;
			}
			elsif (/LinkIncremental=/) {
				$line =~ s/LinkIncremental.+/LinkIncremental="0"/;
			}
			elsif (/Detect64BitPortabilityProblems/) {
				$line =~ s/Detect64BitPort.+/Detect64BitPortabilityProblems="false"/;
			}
			elsif ($tool eq 'VCCLCompilerTool' and /PreprocessorDefinitions="(.+)"/) {
				# add GMSEC_VC6
				$line =~ s/="/="GMSEC_VC6;/;
			}
			elsif ($tool eq 'VCPostBuildEventTool' and /CommandLine/) {
				# change to copy VC6 redistributables
			}
			elsif (m|/>|) {
				# finish Tool
# print "\tfinishing '$tool'\n";

				# set ErrorReporting for compiler
				if ($tool eq 'VCCLCompilerTool') {
					$self->put($out, qq(UseUnicodeResponseFiles="false"));
					$self->put($out, qq(AdditionalOptions="/GR"));
					$self->put($out, qq(ErrorReporting="0"));

				}

				# disable manifest
				if ($tool eq 'VCLinkerTool') {
					$self->put($out, qq(UseUnicodeResponseFiles="false"));
					$self->put($out, qq(GenerateManifest="false"));
					$self->put($out, qq(ErrorReporting="0"));
				}

				$tool = undef;
				$inTool = 0;
			}
		}

		if (defined($line)) {
			$out->print($line);
		}

		$line =~ /^(\s*)/;
		$self->{PREFIX} = $1;
	}
}


sub processSolutionFile
{
	my ($self, $in, $out) = @_;

	++$self->{COUNT_SOLUTION};

	my $inGlobal = undef;
	my $tool = undef;

	my $reDiscard = qr(gmsec_activemq2|gmsec_activemq3|logging_c);
	my $reID = qr(\{[-A-Z0-9]+\});
	my %discard;

	while (<$in>) {
		
		my $line = $_;

		if (not $inGlobal) {
			if ($line =~ /^Global/) {
				$inGlobal = 1;
			}
			elsif ($line =~ /Project\("$reID"\)
					\s*=\s*"$reDiscard",\s*
					"[^"]+",\s*
					"($reID)"/x) {
				my $id = $1;
				print "found discard project '$id'\n";
				$discard{$id} = 1;
			}
		}
		elsif ($line =~ /^\s+($reID)\.
					(Release|Debug)\|(Win32|x64)\.(ActiveCfg|Build.0)
					\s*=\s*(Release|Debug)\|(Win32|x64)
					/x) {
			my $id = $1;
			if ($discard{$id}) {
				print "ignoring $line";
				undef($line);
			}
		}

		if (defined($line)) {
			$out->print($line);
		}

		$line =~ /^(\s*)/;
		$self->{PREFIX} = $1;
	}
}


sub warning
{
	shift;
	print "warning: @_\n";
}


sub put
{
	my ($self, $out, $text) = @_;
	$out->print("$self->{PREFIX}$text\n");
}
