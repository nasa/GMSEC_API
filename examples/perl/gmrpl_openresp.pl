#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#



#
# gmrpl_openresp.pl
#
# A perl example demonstrating the reply/request abilities of 
# GMSEC.  This example demonstrates the use of the Open-Response
# option for Request/Reply messages which requires client (requester)
# to subscribe to anticipated reply message subject in order to
# receive the reply.
#
# Note:  The Open-Response feature is only available with ActiveMQ,
#        Bolt and WebSphere.  The feature was first introduced
#        with GMSEC API 3.6.1.
# 


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


# Replier
{
	package Replier;

	*isa = \&UNIVERSAL::isa;

	sub new
	{
		my $self = {};
		bless $self, shift;

		($self->{CONFIG}) = @_;

		return $self;
	}


	sub run()
	{
		my ($self) = @_;

		# Output the GMSEC API version
		libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

		eval
		{
			$self->setup();
			my $connection = $self->{CONNECTION};

			# Wait 30 seconds for a request message
			my $timeout = $libgmsec_perl::GMSEC_WAIT_FOREVER;

			for (my $i = 0; $i < 2; ++$i)
			{
				libgmsec_perl::LogInfo("Waiting for a request message (from gmreq_openresp)...");
				my $request = $connection->receive($timeout);

				if ($request)
				{
					# Display request message.
					libgmsec_perl::LogInfo("Received Request Message:\n" . $request->toXML());

					# Create reply message.
					my $reply = libgmsec_perl::Message->new("GMSEC.SYSTEST.TEST1.RESP.MY-REPLY", $libgmsec_perl::Message::REPLY);

					# Add fields required to make reply message GMSEC-compliant.
					addRequiredFields($reply);

					# Issue reply message.
					$connection->reply($request, $reply);

					# Display reply message.
					libgmsec_perl::LogInfo("Issued Reply Message:\n" . $reply->toXML());

					# Destroy the request message.
					$connection->release($request);
				}
				else
				{
					libgmsec_perl::LogWarning("Failed to receive a request message after a timeout of " . $timeout . " msec.");
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


	sub setup
	{
		my ($self) = @_;

		# Create connection object.
		$self->{CONNECTION} = libgmsec_perl::Connection::create($self->{CONFIG});
		my $connection = $self->{CONNECTION};

		# Establish a connection.
		$connection->connect();

		# Output the middleware information
		libgmsec_perl::LogInfo("Middleware version = " . $connection->getLibraryVersion());

		my $subject = "GMSEC.SYSTEST.TEST1.REQ.DIR.>";

		# Subscribe to receive request directive messages.
		$self->{SUBINFO} = $connection->subscribe($subject);

		libgmsec_perl::LogInfo("Subscribing to " . $subject);
	}


	sub addRequiredFields
	{
		my ($message) = @_;

		my $field = libgmsec_perl::F32Field->new("HEADER-VERSION", 2014);
		$message->addField($field);
		$message->addField("MESSAGE-TYPE", "RESP");
		$message->addField("MESSAGE-SUBTYPE", "DIR");
		$message->addField("MISSION-ID", "Systest");
		$message->addField("FACILITY", "GMSEC Lab");
		$message->addField("COMPONENT", "gmreq-openresp");
	}


	sub cleanup
	{
		my ($self) = @_;

		my $connection = $self->{CONNECTION};

		if ($connection)
		{
			my $subInfo = $self->{SUBINFO};

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

			libgmsec_perl::Connection::destroy($connection);
		}

		libgmsec_perl::Connection::shutdownAllMiddlewares();
	}
} # end Replier definition


{
	my $task = bless({});
	Util::driver($task, @ARGV);
}


sub printUsage
{
	my ($self, $config) = @_;
	my $hasConnectionType = Util::hasConnectionType($config);
	return 0 if ($hasConnectionType);

	print "\nusage: perl gmrpl_openresp.pl connectiontype=<middleware> [ <parameter>=<value> ]" .
	"\n\n\tNote that the parameter 'connectiontype' is required." .
	"\n\tThe rest of other parameters are optional." .
	"\n\n\tserver=<server name> (required if middleware is not bolt/MB locally)" .
	"\n\tsubject=<subject name>" .
	"\n\tmsg_timeout_ms=<timeout period (milliseconds)>" .
	"\n\tloglevel=<log level>" .
	"\n\tcfgfile=<config_filename.xml>" .
	"\n\nFor more information, see API User's Guide\n";

	return 1;
}


sub run
{
	my ($self, $config) = @_;

	# Configuration option, introduced with GMSEC API 3.6.1, to enable
	# Open-Response behavior.
	$config->addValue("GMSEC-REQ-RESP", "OPEN-RESP");

	my $replier = Replier->new($config);

	eval
	{
		$replier->run();
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


1;

__END__
