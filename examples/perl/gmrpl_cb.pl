#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gmrpl_cb.pl
#
# Another perl example demonstrating the reply/request abilities of 
# GMSEC.  This example involves the use of Callback functionality,
# which allows for code to be performed anytime a Message is 
# received.
#


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


# Define the RequestCallback Reply Callback Package
{
	package RequestCallback;
	use base 'libgmsec_perl::Callback';

	*isa = \&UNIVERSAL::isa;

	sub onMessage
	{
		my ($self, $connection, $message) = @_;

		eval
		{
			print "In onMessage!\n";
			# Display the XML representation of the received message
			libgmsec_perl::LogInfo("[RequestCallback:onMessage] Received:\n" . $message->toXML());

			# Construct Reply subject
			my $component = 0;

			eval
			{
				$component = $message->getStringField("COMPONENT")->getValue();
			};
			if (isa($@, 'libgmsec_perl::Exception'))
			{
				libgmsec_perl::LogWarning("COMPONENT field is not available: " . $@->what());
			}
			elsif($@)
			{
				libgmsec_perl::LogError($@);
			}

			# Set Status Code.  See API Interface Specification for available status codes.
			my $status_code = "1"; # 1 = Acknowledgement

			# Create reply message
			my $reply_subject = "GMSEC.MISSION.SAT_ID.RESP." . ($component ? $component : "UNKNOWN-COMPONENT") . "." . $status_code;

			my $reply = libgmsec_perl::Message->new($reply_subject, $libgmsec_perl::Message::REPLY);

			# Add fields to the reply message
			$reply->addField("COMPONENT", "GMRPL");
			$reply->addField("ANSWER", "Sure looks like it!");

			# Display XML representation of reply message
			libgmsec_perl::LogInfo("Prepared Reply:\n" . $reply->toXML());

			# Send Reply
			$connection->reply($message, $reply);
		};
		if (isa($@, 'libgmsec_perl::Exception'))
		{
			libgmsec_perl::LogError("[RequestCallback::onMessage] " . $@->what());
		}
		elsif($@)
		{
			libgmsec_perl::LogError($@);
		}
	}
} # end RequestCallback definition

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

		my $rcb = RequestCallback->new();
		$rcb->DISOWN();

		# Output example program-specific information
		for my $subject (@subjects)
		{
			libgmsec_perl::LogInfo("Subscribing to " . $subject);
			push(@{$self->{SUBINFO}}, $connection->subscribe($subject, $rcb));
		}

		my $done = 0;
		my $count = 0;
		my $elapsedTime = 0;
		my $currentTime = 0;
		my $prevTime = 0;

		while (!$done)
		{
			if ($prog_timeout_s != $libgmsec_perl::GMSEC_WAIT_FOREVER && $elapsedTime >= $prog_timeout_s)
			{
				libgmsec_perl::LogInfo("Program Timeout Reached!");
				last;
			}

			# Receive next message
			my $message = $connection->receive($msg_timeout_ms);

			if ($prog_timeout_s != $libgmsec_perl::GMSEC_WAIT_FOREVER)
			{
				$currentTime = libgmsec_perl::TimeUtil->getCurrentTime_s();
				$elapsedTime += ($currentTime - $prevTime);
				$prevTime = $currentTime;
			}

			if (!$message)
			{
				libgmsec_perl::LogInfo("timeout occurred");
				last;
			}
			else
			{
				# Display the XML representation of the received message
				libgmsec_perl::LogInfo("Received:\n" . $message->toXML());

				# dispatch callbacks
				$connection->dispatch($message);

				if ($message->getSubject() eq "GMSEC.TERMINATE")
				{
					$done = 1;
				}

				# Destroy received message
				$connection->release($message);
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

	$reply->addField("COMPONENT", "gmrpl_cb");

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
