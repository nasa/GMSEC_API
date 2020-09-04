#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gmreq.pl
#
# A perl example demonstration of GMSEC request / reply functionality.
# The associated example gmrpl will provide the reply.  
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

	print "\nusage: perl gmreq.pl connectiontype=<middleware> [ <parameter>=<value> ]" .
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
	$self->{CONFIG} = $config;

	my $subject = Util::getString($config, 'SUBJECT', 'GMSEC.MISSION.SAT_ID.REQ.CMD.GMREQ');
	my $msg_timeout_ms = Util::getInteger($config, 'MSG_TIMEOUT_MS', $libgmsec_perl::GMSEC_WAIT_FOREVER);

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
		libgmsec_perl::LogInfo("Using subject: " . $subject);
		if ($msg_timeout_ms > $libgmsec_perl::GMSEC_WAIT_FOREVER)
		{
			libgmsec_perl::LogInfo("Message receipt timeout set to occur after " . $msg_timeout_ms . " ms");
		}

		$self->request($msg_timeout_ms, $subject);
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

sub request
{
	my ($self, $msg_timeout_ms, $subject) = @_;
	my $connection = $self->{CONNECTION};
	my $config = $self->{CONFIG};

	# Create message
	my $request = libgmsec_perl::Message->new($subject, $libgmsec_perl::Message::REQUEST);

	# Add fields to message
	$request->addField("COMPONENT", "gmreq");
	$request->addField("QUESTION", "Does the request/reply functionality still work?");
	$request->addField("STRING-FIELD", "gmreq_cb");

	if ($config->getBooleanValue("ENCRYPT", 0))
	{
		my $field = libgmsec_perl::BooleanField->new("SEC-ENCRYPT", 1);
		$request->addField($field);
	}

	# Display XML representation of request message
	libgmsec_perl::LogInfo("Requesting:\n" . $request->toXML());

	# Send request message
	my $reply = $connection->request($request, $msg_timeout_ms);

	if ($reply)
	{
		# Got reply; display XML representation of reply message
		libgmsec_perl::LogInfo("Received Reply:\n" . $reply->toXML());

		# Destroy the reply
		$connection->release($reply);
	}
	else
	{
		libgmsec_perl::LogWarning("Reply message not received after a timeout of " . $msg_timeout_ms . " msec.");
	}
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
