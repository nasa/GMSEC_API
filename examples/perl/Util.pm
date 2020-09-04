
package Util;

use strict;
use GMSECAPI;


sub driver
{
	my ($task, @args) = @_;

	eval {
		my $config = GMSECAPI::Config->new(\@args);

		addToConfigFromFile($config);

		my $printUsage = $task->printUsage($config); 
		if (not $printUsage) {
			$task->run($config);
		}
	};

	if ($@) {
		warning("caught: $@");
	}

	eval {
		$task->cleanup;
	};
}


sub check
{
	my ($tag, $status) = @_;
	if (not checkNoThrow($tag, $status)) {
		die("$tag: " . $status->Get());
	}
	return 1;
}


sub checkNoThrow
{
	my ($tag, $status) = @_;
	if (not $status->isError()) {
		return 1;
	}

	warning("$tag: " . $status->Get());

	return undef;
}


sub report
{
	my ($s)  = @_;
	print "\n", $s, "\n";
}


sub warning
{
	my ($s)  = @_;
	print "warning: $s\n";
}

sub getSubjects
{
   my ($config) = @_;
   my @subjects = ();

   my $value = getString($config, "SUBJECT", "");
   if (length($value) != 0) {
      push(@subjects, $value);
   }
   else {
      # Subjects are to be input using SUBJECT.1="" SUBJECT.2="" 
      for (my $i = 1; ; ++$i) {
          my $subject = "SUBJECT." . $i;
          my $tmp = getString($config, $subject, "");
          last if (length($tmp) == 0);
          push(@subjects, $tmp);
      }
   }

   if (scalar(@subjects) == 0) {
      push(@subjects, "GMSEC.>");
   } 
   else {
      push(@subjects, "GMSEC.TERMINATE");
   } 

   return @subjects;
}

sub getString
{
	my ($config, $key, $default) = @_;
	my $value = $config->GetValue($key);
	if (not defined($value)) {
	 	 $value = $default;
	}
	return $value;
}


sub getInteger
{
	my ($config, $key, $default) = @_;
	my $value = $config->GetValue($key);
	if (not defined($value)) {
		$value = $default;
	}
	elsif ($value !~ /^\d+$/) {
		$value = $default;
	}
	return $value;
}


sub getReal
{
	my ($config, $key, $default) = @_;
	my $value = $config->GetValue($key);
	if (not defined($value)) {
		$value = $default;
	}
	elsif (not Scalar::looks_like_number($value)) {
		$value = $default;
	}
	return $value;
}

sub hasConnectionType
{
	my ($config) = @_;
	my $connectionType = Util::getString($config, "connectionType", "");
	my $mw_id = Util::getString($config, "mw-id", "");

	return 0 if (length($connectionType) == 0 && length($mw_id) == 0); 

	return 1;
}

sub addToConfigFromFile
{
	my ($config) = @_;

	my $loglevel    = Util::getString($config, "LOGLEVEL", "");
	my $cfgfilename = Util::getString($config, "CFGFILE", "");

	#
	# This assumes, now, that there is only _one_ CFGFILE=filename.xml arg
	# specified.
	#
	if ($cfgfilename ne "")
	{
		my $file_text_contents = "";
		my $text_line          = "";

		open(FH, "<" . $cfgfilename) || die "Can't open CFGFILE $cfgfilename";

		while (<FH>)
		{
			$text_line = $_;

			if ($text_line ne "")
			{
				$file_text_contents = $file_text_contents . $text_line;

				if ($loglevel eq "DEBUG")
				{
					print text_line;
				}

			}
		}

		close(FH);

		my $config_from_file = GMSECAPI::Config->new();

		my $result = $config_from_file->FromXML($file_text_contents);

		if ($config->Status->GetClass == $GMSEC_STATUS_NO_ERROR)
		{
			my $name = "";
			my $value = "";

			#
			# Read all the config file name, value pairs
			# and add them to the current base
			# configuration (which can have its
			# (name, value) pairs from both command-line
			# pairs and from the input current config
			# file that was read).
			#
			($name, $value) = $config_from_file->GetFirst();

			while ($config_from_file->Status->GetClass ==
			       $GMSEC_STATUS_NO_ERROR)
			{
				if ($loglevel eq "DEBUG")
				{
					print "Adding (" . $name . ", " .
					      $value . ")" .
					      "from config file " .
					      $cfgfilename;
				}

				$config->AddValue($name, $value);

				($name, $value) = $config_from_file->GetNext();

			}

		}
		else
		{
			if ($loglevel eq "WARNING" ||
				$loglevel eq "INFO"    ||
				$loglevel eq "VERBOSE" ||
				$loglevel eq "DEBUG")
			{
				print "Error in config FromXML in " .
				      "addToConfigFromFile";
			}
		}

	}// if there is a config file argument at all

	return;
}

sub millisleep
{
	my ($millis) = @_;
	select(undef, undef, undef, $millis / 1000);
}


1;

