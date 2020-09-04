#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gmsub_disp_rr.pl
# 
# This file contains an example subscriber using the GMSEC API dispatcher.
# The callback is registered with the Connection.  A separate thread, transmits a request
# and waits for a reply.  The Dispatcher is then started, which launches in its own
# thread.
# 


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

# PublishCallback Callback Package
{
	package PublishCallback;
	use base 'libgmsec_perl::Callback';

	sub onMessage
	{
		my ($self, $conn, $message) = @_;

		libgmsec_perl::LogInfo("[PublishCallback::onMessage] Received:\n" . $message->toXML());
	}
} # end PublishCallback definition


# RequestCallback Callback Package
{
	package RequestCallback;
	use base 'libgmsec_perl::ReplyCallback';

	sub onReply
	{
		my ($self, $conn, $message, $reply) = @_;

		libgmsec_perl::LogInfo("[ReplyCallback::onMessage] Received Reply:\n" . $reply->toXML());
	}

	sub onEvent
	{
		my ($self, $status, $event) = @_;

		libgmsec_perl::LogInfo("[ReplyCallback::onEvent] Status=\n" . $status->get()
					. " event=" . $event);
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

	print "\nusage: perl gmsub_disp_rr.pl connectiontype=<middleware> [ <parameter>=<value> ]" .

	"\n\n\tNote that the parameter 'connectiontype' is required." .
	"\n\tThe rest of other parameters are optional." .

	"\n\n\tserver=<server name> (required if middleware is not bolt/MB locally)" .
	"\n\tsubject=<subject name>" .
	"\n\tsubject.N=<sub-subject name>" .
	"\n\t\tNote: N must be 1 or greater" .
	"\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B"  .
	"\n\tcfgfile=<config_filename.xml>" .
	"\n\tmsg_timeout_ms=<message timeout period (milliseconds)>" .
	"\n\tprog_timeout_s=<program timeout period (seconds)>" .
	"\n\nFor more information, see API User's Guide\n";

	return 1;
}


sub run
{
	my ($self, $config) = @_;

	# Determine run-time settings
	my @subjects = Util::determineSubjects($config);
	my $msg_timeout_ms = Util::getInteger($config, 'MSG_TIMEOUT_MS', $libgmsec_perl::GMSEC_WAIT_FOREVER);
	my $prog_timeout_s = Util::getInteger($config, 'PROG_TIMEOUT_S', $libgmsec_perl::GMSEC_WAIT_FOREVER);

	# Output GMSEC API version
	libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

	my $connection;

	eval
	{
		$config->addValue("isThreaded", "true");

		# Create the Connection
		$connection = libgmsec_perl::Connection::create($config);
		$self->{CONNECTION} = $connection;

		# Connect
		$connection->connect();

		# Output middleware version
		libgmsec_perl::LogInfo("Middleware version = " . $connection->getLibraryVersion());

		# Output example program-specific information
		libgmsec_perl::LogInfo("Using subjects: " . join(", ", @subjects));
		if ($msg_timeout_ms > $libgmsec_perl::GMSEC_WAIT_FOREVER)
		{
			libgmsec_perl::LogInfo("Message receipt timeout set to occur after " . $msg_timeout_ms . " ms");
		}

		# Subscribe using PublishCallback
		my $condition = libgmsec_perl::Condition->new();
		my $publishCallback = PublishCallback->new();
		$publishCallback->DISOWN();

		# Subscribe
		for my $subject (@subjects)
		{
			libgmsec_perl::LogInfo("Subscribing to " . $subject);
			push(@{$self->{SUBINFO}}, $connection->subscribe($subject, $publishCallback));
		}

		# Register a RequestCallback with the Connection to trigger when a Request times out
		my $requestCallback = RequestCallback->new();
		$connection->registerEventCallback($libgmsec_perl::Connection::REQUEST_TIMEOUT_EVENT, $requestCallback);

		if ($connection->startAutoDispatch())
		{
			libgmsec_perl::LogInfo("Starting AutoDispatcher");
		}
		else
		{
			libgmsec_perl::LogError("Failed to start AutoDispatcher; Exiting the program");
			last; #Break out of the eval statement
		}

		my $requestSubject = "GMSEC.TEST.PUBLISH.NOREPLY";
		if (!($subjects[0] eq "GMSEC.>"))
		{
			$requestSubject = $subjects[0];
		}

		libgmsec_perl::LogInfo("Creating a Request message with subject: " . $requestSubject);
		my $request = libgmsec_perl::Message->new($requestSubject, $libgmsec_perl::Message::REQUEST);
		$request->addField("COMPONENT", "gmsub_disp_rr");

		libgmsec_perl::LogInfo("Sending the Request message");
		$connection->request($request, $msg_timeout_ms, $requestCallback);

		if ($prog_timeout_s == $libgmsec_perl::GMSEC_WAIT_FOREVER)
		{
			libgmsec_perl::LogInfo("Press the enter key to exit");
			my $throwaway = <STDIN>;
		}
		else
		{
			Util::millisleep($prog_timeout_s);
			libgmsec_perl::LogInfo("Program timeout reached!");
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
