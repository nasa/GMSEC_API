#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gmrpl.pl
#
# A perl example demonstration of GMSEC request / reply functionality.
# The associated example gmreq will provide the request.  
# gmrpl should be run before gmreq.
#


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

{
	my $task = bless({});
	Util::driver($task, @ARGV);
}


sub printUsage
{
	my ($self, $config) = @_;
	my $hasConnectionType = Util::hasConnectionType($config);
	return 0 if ($hasConnectionType);

	print "\nusage: perl gmrpl.pl connectiontype=<middleware> [ <parameter>=<value> ]" .
	"\n\n\tNote that the parameter 'connectiontype' is required." .
	"\n\tThe rest of other parameters are optional." .
	"\n\n\tserver=<server name> (required if middleware is not bolt/MB locally)" .
	"\n\tsubject=<subject name>" .
	"\n\tmsg_timeout_ms=<message timeout period (milliseconds)>" .
	"\n\tprog_timeout_s=<program timeout period (seconds)>" .
	"\n\tloglevel=<log level>" .
	"\n\tcfgfile=<config_filename.xml>" .
	"\n\nFor more information, see API User's Guide\n";

	return 1;
}


sub run
{
	my ($self, $config) = @_;
	$self->{CONFIG} = $config;

	my @subjects = Util::determineSubjects($config);
	my $msg_timeout_ms = Util::getInteger($config, 'MSG_TIMEOUT_MS', $libgmsec_perl::GMSEC_WAIT_FOREVER);
	my $prog_timeout_s = Util::getInteger($config, 'PROG_TIMEOUT_S', $libgmsec_perl::GMSEC_WAIT_FOREVER);

	# Output GMSEC API version
	libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

	eval
	{
		# Create the Connection
		my $connection = libgmsec_perl::Connection::create($config);
		$self->{CONNECTION} = $connection;

		# Connect
		$connection->connect();

		# Output middleware version
		libgmsec_perl::LogInfo("Middleware version = " . $connection->getLibraryVersion());

		# Output example program-specific information
		for my $subject (@subjects)
		{
			libgmsec_perl::LogInfo("Subscribing to " . $subject);
			push(@{$self->{SUBINFO}}, $connection->subscribe($subject));
		}
		if ($msg_timeout_ms > $libgmsec_perl::GMSEC_WAIT_FOREVER)
		{
			libgmsec_perl::LogInfo("Message receipt timeout set to occur after " . $msg_timeout_ms . " ms");
		}

		my $done = 0;
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

			my $request = $connection->receive($msg_timeout_ms);

			if ($prog_timeout_s != $libgmsec_perl::GMSEC_WAIT_FOREVER)
			{
				$currentTime = libgmsec_perl::TimeUtil->getCurrentTime_s();
				$elapsedTime += ($currentTime - $prevTime);
				$prevTime = $currentTime;
			}

			if (!$request)
			{
				libgmsec_perl::LogInfo("Timeout occurred");
			}
			else
			{
				libgmsec_perl::LogInfo("Received:\n" . $request->toXML());

				$done = $done || ($request->getSubject() eq "GMSEC.TERMINATE" ? 1 : 0);

				if ($request->getKind() == $libgmsec_perl::Message::REQUEST)
				{
					$self->reply($request);
				}

				$connection->release($request);
			}
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

	$self->cleanup();
}

sub reply
{
	my ($self, $request) = @_;
	my $connection = $self->{CONNECTION};
	my $config = $self->{CONFIG};

	# Determine the component which the request is coming from
	my $requestingComponent = $request->getStringField("COMPONENT")->getValue();

	# Set Status Code --  See GMSEC Message Interface Specification (ISD) for available status codes
	my $statusCode = "2"; # 1 = Acknowledgement

	my $replySubject = "GMSEC.MISSION.SAT_ID.RESP."
				. ($requestingComponent ? $requestingComponent : "UNKNOWN-COMPONENT") . "."
				. $statusCode;

	# Create the reply
	my $reply = libgmsec_perl::Message->new($replySubject, $libgmsec_perl::Message::REPLY);

	# Add fields to message
	$reply->addField("ANSWER", "Sure looks like it!");

	$reply->addField("COMPONENT", "gmrpl");

	if ($config->getBooleanValue("ENCRYPT", 0))
	{
		my $field = libgmsec_perl::BooleanField->new("SEC-ENCRYPT", 1);
		$reply->addField($field);
	}

	# Display XML representation of request message
	libgmsec_perl::LogInfo("Prepared reply:\n" . $reply->toXML());

	# Send reply message
	$connection->reply($request, $reply);
}


sub cleanup
{
	my ($self) = @_;

	my $connection = $self->{CONNECTION};

	if ($connection)
	{
		libgmsec_perl::Connection::destroy($connection);
	}

	libgmsec_perl::Connection::shutdownAllMiddlewares();
}

1;

__END__
