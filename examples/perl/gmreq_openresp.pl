
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#



#
# gmreq_openresp.pl
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
#


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


# Requester ReplyCallback
{
	package Requester;
	use base 'libgmsec_perl::ReplyCallback';

	*isa = \&UNIVERSAL::isa;

	sub new
	{
		# Initialize the base Package (libgmsec_perl::Callback)
		my $class = shift;
		my $self = $class->SUPER::new();

		($self->{CONFIG}) = @_;

		return $self;
	}

	sub run
	{
		my ($self) = @_;

		# Output the GMSEC API version
		libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

		eval
		{
			$self->setup();
			my $connection = $self->{CONNECTION};
			my $request = $self->{REQUEST};

			my $timeout = 5000;

			# Display request message.
			libgmsec_perl::LogInfo("Issuing Request Message:\n" . $request->toXML());

			# Using Request(Message* request, GMSEC_I32 timeout, Message* reply)
			my $reply = $connection->request($request, $timeout);

			if ($reply)
			{
				# Display reply message.
				libgmsec_perl::LogInfo("Received Reply Message:\n" . $reply->toXML());

				$connection->release($reply);
			}
			else
			{
				libgmsec_perl::LogWarning("Failed to receive reply message after a timeout of " . $timeout . " msec.");
			}


			# Using Request(Message* request, GMSEC_I32 timeout, ReplyCallback* cb)
			$connection->request($request, $timeout, $self);

			# Display request message.
			libgmsec_perl::LogInfo("Issued Request Message:\n" . $request->toXML());

			# Allow some time for the ReplyCallback to receive the reply.
			Util::millisleep($timeout);

			# Reply messages are exposed.
			# Using receive() to receive replies.
			#
			# Note: Two replies are expected, one for each of the requests performed
			#       above.
			for (my $i = 0; $i < 2; ++$i)
			{
				$reply = $connection->receive($timeout);

				if ($reply)
				{
					# Display reply message.
					libgmsec_perl::LogInfo("Received exposed Reply Message using receive():\n" . $reply->toXML());

					$connection->release($reply);
				}
				else
				{
					libgmsec_perl::LogInfo("Failed to receive exposed reply message after a timeout of " . $timeout . " msec.");
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


	sub onReply
	{
		my ($self, $connection, $message, $reply) = @_;

		libgmsec_perl::LogInfo("[Requester::onReply] Received Reply:\n" . $reply->toXML());
	}


	sub onEvent
	{
		my ($self, $connection, $status, $event) = @_;

		if ($status->isError())
		{
			libgmsec_perl::LogWarning("[Requester::onEvent] " . $status->get() . ", event=" . $event);
		}
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

		my $subject = "GMSEC.SYSTEST.TEST1.RESP.>";

		# Subscribe to (anticipated) response/reply subject.
		$self->{SUBINFO} = $connection->subscribe($subject);

		libgmsec_perl::LogInfo("Subscribing to " . $subject);

		# Create request directive message.
		$self->{REQUEST} = libgmsec_perl::Message->new("GMSEC.SYSTEST.TEST1.REQ.DIR.MY-REQUEST", $libgmsec_perl::Message::REQUEST);

		# Add fields required to make request message GMSEC-compliant.
		addRequiredFields($self->{REQUEST});
	}


	sub addRequiredFields
	{
		my ($message) = @_;

		my $field = libgmsec_perl::F32Field->new("HEADER-VERSION", 2014);
		$message->addField($field);
		$message->addField("MESSAGE-TYPE", "REQ");
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
} # end Requester definition


{
	my $task = bless({});
	Util::driver($task, @ARGV);
}


sub printUsage
{
	my ($self, $config) = @_;
	my $hasConnectionType = Util::hasConnectionType($config);
	return 0 if ($hasConnectionType);

	print "\nusage: perl gmreq_openresp.pl connectiontype=<middleware> [ <parameter>=<value> ]" .
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

	my $requester = Requester->new($config);

	eval
	{
		$requester->run();
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
