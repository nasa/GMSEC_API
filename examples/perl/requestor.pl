#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#



# requestor.pl
#
# This is a sample application built with the perl
# GMSEC API.  This application demonstrates several
# key components within the GMSEC API including loading
# from an xml file, producing a heartbeat, and request
# reply functionality using callbacks. This works in sync
# with Replier.pl.
#  


use strict;
use feature qw(say switch);

use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


# Define RequestReplyCallback
{
	package RequestReplyCallback;
	use base 'libgmsec_perl::ReplyCallback';

	*isa = \&UNIVERSAL::isa;

	sub new
	{
		# Initialize the base Package
		my $class = shift;
		my $self = $class->SUPER::new();

		# Initialize member data from arguments
		($self->{CONFIGFILE}) = @_;

		return $self;
	}

	sub onReply
	{
		my ($self, $connection, $request, $reply) = @_;

		eval
		{
			# Get the status of the response
			my $responseStatus = $reply->getI16Field("RESPONSE-STATUS")->getValue();
			my $data           = $reply->getStringField("DATA")->getValue();

			given ($responseStatus)
			{
				when(1)
				{
					libgmsec_perl::LogInfo("[RequestReplyCallback::onReply] Acknowledgement");
				}
				when (2)
				{
					libgmsec_perl::LogInfo("[RequestReplyCallback::onReply] Working/keep alive");
				}
				when(3)
				{
					libgmsec_perl::LogInfo("[RequestReplyCallback::onReply] Received Reply (DATA=" . $data . ")");
				}
				when(4)
				{
					libgmsec_perl::LogInfo("[RequestReplyCallback::onReply] Failed completion");
				}
				when(5)
				{
					libgmsec_perl::LogInfo("[RequestReplyCallback::onReply] Invalid Request");
				}
				default
				{
					libgmsec_perl::LogInfo("[RequestReplyCallback::onReply] <unknown response status>");
				}
			}

			# Lookup from the config file the template log message definition
			my $logMessage = $self->{CONFIGFILE}->lookupMessage("LOG-REQ");

			# Get the current time
			my $theTime = "YYYY-DDD-HH:MM:SS.sss";
			libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);

			# Fill fields within the log message
			$logMessage->addField("EVENT-TIME", $theTime);
			$logMessage->addField("MSG-TEXT", $data);

			# Publish the log message
			$connection->publish($logMessage);
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

	sub onEvent
	{
		my ($self, $connection, $status, $event) = @_;

		if ($status->isError())
		{
			libgmsec_perl::LogWarning("[RequestReplyCallback:onEvent] status = " . $status->get() . ", event = " . $event);
		}
	}
} # end RequestReplyCallback definition


{
	my $self = bless({});

	# If no arguments were provided, print usage and exit with an erroneous code value
	if (scalar(@ARGV) < 1)
	{
		$self->printUsage();
		exit -1;
	}

	# Initialize logging in the GMSEC API in order to display output
	$self->init();
	
	eval
	{
		$self->run($ARGV[0]);
	};
	if ($@)
	{
		print "caught: $@";
	}
}


sub printUsage
{
	print "\nusage: perl requestor.pl <filename>\n";
}


sub init
{
	my $config = libgmsec_perl::Config->new();
	$config->addValue("LOGLEVEL", "INFO");
}


sub run
{
	my ($self, $cfgFileName) = @_;

	eval
	{
		# Load the config file
		my $cfgFile = libgmsec_perl::ConfigFile->new();
		$cfgFile->load($cfgFileName);

		# Look up program configuration information, and get program config values
		my $progConfig = $cfgFile->lookupConfig("program-config");
		my $connConfig = $cfgFile->lookupConfig("connection-config");

		# Load the heartbeat message definition
		my $heartbeatMsg = $cfgFile->lookupMessage("C2CX-HEARTBEAT-REQ");

		# Load the directive request message definition
		my $directiveReqMsg = $cfgFile->lookupMessage("DIRECTIVE-REQUEST");

		# Get program config values
		my $updateRate    = $progConfig->getIntegerValue("update-rate");
		my $loopCountdown = $progConfig->getIntegerValue("loop-time");
		my $dirPubRate    = $progConfig->getIntegerValue("send-directive-rate");
		my $hbPubRate     = $heartbeatMsg->getI16Field("PUB-RATE")->getValue();

		# Subscription callback
		my $cb = RequestReplyCallback->new($cfgFile);

		# Create a connection object off of the configuration
		$self->{CONNECTION} = libgmsec_perl::Connection::create($connConfig);
		my $connection = $self->{CONNECTION};

		# Connect to the GMSEC bus
		$connection->connect();

		# output GMSEC API version
		libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

		# Output middleware version
		libgmsec_perl::LogInfo("Middleware version = " . $connection->getLibraryVersion());

		# Output some general program information
		libgmsec_perl::LogInfo("Publishing for " . $loopCountdown . " seconds.");
		libgmsec_perl::LogInfo("Publishing Directive Request Messages every " . $dirPubRate . " seconds.");
		libgmsec_perl::LogInfo("Publishing C2CX Heartbeat Messages every " . $hbPubRate . " seconds.");

		# Display start time
		my $theTime = "YYYY-DDD-HH:MM:SS.sss";
		libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
		libgmsec_perl::LogInfo("Start Time: " . $theTime);

		my $heartbeatCount     = 0;
		my $requestCount       = 0;
		my $heartbeatCountdown = 0;
		my $directiveCountdown = 0;

		# Publishing loop
		for (; $loopCountdown > 0; --$loopCountdown)
		{
			# When the countdown reaches 0, we publish a directive message and reset the counter
			if ($directiveCountdown < 1)
			{
				++$requestCount;

				# Alter the directive request to a unique string
				my $field = libgmsec_perl::StringField->new("DIRECTIVE-KEYWORD", "$requestCount");
				$directiveReqMsg->addField($field);
				$directiveReqMsg->addField("DIRECTIVE-STRING", "DIRECTIVE REQUEST MESSAGE " . "$requestCount");

				# Publish time
				libgmsec_perl::LogInfo("Publishing Directive Request Message (" . $requestCount . ")");

				# Send the request with the reply being send to the callback we created
				$connection->request($directiveReqMsg, $libgmsec_perl::GMSEC_WAIT_FOREVER, $cb);

				# Reset the countdown
				$directiveCountdown = $dirPubRate;
			}

			# When the countdown reaches 0, we publish a heartbeat message and reset the counter
			if ($heartbeatCountdown < 1)
			{
				++$heartbeatCount;

				# Update the message counter field
				$heartbeatMsg->addField("COUNTER", $heartbeatCount);

				# Publish the heartbeat message
				$connection->publish($heartbeatMsg);

				# Output a heartbeat marker and reset the counter
				libgmsec_perl::LogInfo("Published Heartbeat");
				$heartbeatCountdown = $hbPubRate;
			}

			# Decrement the counters
			$heartbeatCountdown -= $updateRate;
			$directiveCountdown -= $updateRate;

			# Sleep for updateRate seconds
			Util::millisleep($updateRate * 1000);
		}

		# End time
		libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
		libgmsec_perl::LogInfo("End Time: " . $theTime);
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
