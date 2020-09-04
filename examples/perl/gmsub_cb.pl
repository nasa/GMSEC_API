#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gmsub_disp.pl
# 
# A perl example demonstrating the subscription abilities of 
# GMSEC.  This example involves the use of Callback functionality,
# in combination with the AutoDispatcher for receipt of messages.
# 


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

# Define the PublishCallback Callback Package
{
	package PublishCallback;
	use base 'libgmsec_perl::Callback';

	sub new
	{
		# Initialize the base Package (libgmsec_perl::Callback)
		my $class = shift;
		my $self = $class->SUPER::new();

		return $self;
	}

	sub onMessage
	{
		my ($self, $conn, $message) = @_;

		libgmsec_perl::LogInfo("[PublishCallback::onMessage] Received:\n" . $message->toXML());
	}
} # end PublishCallback definition


{
	my $task = bless({});
	Util::driver($task, @ARGV);
}


sub printUsage
{
	my ($self, $config) = @_;
	my $hasConnectionType = Util::hasConnectionType($config);
	return 0 if ($hasConnectionType);

	print "\nusage: perl gmsub_disp.pl connectiontype=<middleware> [ <parameter>=<value> ]" .

	"\n\n\tNote that the parameter 'connectiontype' is required." .
	"\n\tThe rest of other parameters are optional." .

	"\n\n\tserver=<server name> (required if middleware is not bolt/MB locally)" .
	"\n\tsubject=<subject name>" .
	"\n\tsubject.N=<sub-subject name>" .
	"\n\t\tNote: N must be 1 or greater" .
	"\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B"  .
	"\n\titerations=<iterations>" .
	"\n\tloglevel=<log level>" .
	"\n\tcfgfile=<config_filename.xml>" .
	"\n\nFor more information, see API User's Guide\n";

	return 1;
}


sub run
{
	my ($self, $config) = @_;

	# Determine run-time settings
	my @subjects = Util::determineSubjects($config);
	my $iterations = Util::getInteger($config, "ITERATIONS", 0);
	my $msg_timeout_ms = Util::getInteger($config, "MSG_TIMEOUT_MS", $libgmsec_perl::GMSEC_WAIT_FOREVER);
	my $prog_timeout_s = Util::getInteger($config, "PROG_TIMEOUT_S", $libgmsec_perl::GMSEC_WAIT_FOREVER);

	if ($iterations > 0)
	{
		libgmsec_perl::LogInfo("Waiting for up to " . $iterations . " messages");
	}

	# Output GMSEC API version
	libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

	my $connection;

	eval
	{
		# Create the Connection
		$connection = libgmsec_perl::Connection::create($config);
		$self->{CONNECTION} = $connection;

		# Connect
		$connection->connect();

		# Output middleware version
		libgmsec_perl::LogInfo("Middleware version = " . $connection->getLibraryVersion());

		# Output example program-specific information
		libgmsec_perl::LogInfo("Using subjects: " . join(", ", @subjects));
		if ($iterations > 0)
		{
			libgmsec_perl::LogInfo("Receiving " . $iterations . " message(s)");
		}

		# Subscribe using a PublishCallback
		my $condition = libgmsec_perl::Condition->new();
		my $publishCallback = PublishCallback->new($iterations, $condition);
		$publishCallback->DISOWN();

		# Subscribe
		for my $subject (@subjects)
		{
			libgmsec_perl::LogInfo("Subscribing to " . $subject);
			push(@{$self->{SUBINFO}}, $connection->subscribe($subject, $publishCallback));
		}
		my $done = 0;

		my $count = 0;
		my $elapsedTime = 0;
		my $currentTime = 0;
		my $prevTime = 0;

		# Receive messages
		while (!$done)
		{
			if ($prog_timeout_s != $libgmsec_perl::GMSEC_WAIT_FOREVER && $elapsedTime >= $prog_timeout_s)
			{
				libgmsec_perl::LogInfo("Program timeout reached!");
				$done = 1;
				next;
			}

			my $message = $connection->receive($msg_timeout_ms);

			if ($prog_timeout_s != $libgmsec_perl::GMSEC_WAIT_FOREVER)
			{
				$currentTime = libgmsec_perl::TimeUtil->getCurrentTime_s();
				$elapsedTime += ($currentTime - $prevTime);
				$prevTime = $currentTime;
			}

			if (!$message)
			{
				libgmsec_perl::LogInfo("Timeout occurred");
				last;
			}

			$connection->dispatch($message);

			if ($message->getSubject() eq "GMSEC.TERMINATE")
			{
				$done = 1;
			}

			++$count;
			if ($iterations > 0 && $count >= $iterations)
			{
				$done = 1;
			}

			$connection->release($message);
		}
	};
	if (isa($@, 'libgmsec_perl::Exception'))
	{
		libgmsec_perl::LogError($@->what());
	}
	elsif($@)
	{
		libgmsec_perl::LogError($@);
	}

	$connection->stopAutoDispatch();

	$self->cleanup();
}


sub cleanup
{
	my ($self) = @_;

	my $connection = $self->{CONNECTION};

	if ($connection)
	{
		for my $subInfo (@{$self->{SUBINFO}})
		{
			eval
			{
				libgmsec_perl::LogInfo("Unsubscribing from " . $subInfo->getSubject());
				$connection->unsubscribe($subInfo);
			};
			if (isa($@, 'libgmsec_perl::Exception'))
			{
				libgmsec_perl::LogError($@->what());
			}
			elsif($@)
			{
				libgmsec_perl::LogError($@);
			}
		}

		libgmsec_perl::Connection::destroy($connection);
	}

	libgmsec_perl::Connection::shutdownAllMiddlewares();
}


1;

__END__
