#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#



# replier.pl
#
# This is a sample application built with the perl
# GMSEC API.  This application demonstrates several
# key components within the GMSEC API including loading
# from an xml file, producing a heartbeat, and request
# reply functionality using callbacks.  This works in sync
# with Requestor.pl
#  


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

# Define RequestReplyCallback Callback
{
	package RequestReplyCallback;
	use base 'libgmsec_perl::Callback';

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

	sub onMessage
	{
		my ($self, $connection, $message) = @_;

		my $cfgFile = $self->{CONFIGFILE};

		eval
		{
			# We don't care about anything but request messages
			if ($message->getKind() != $libgmsec_perl::Message::REQUEST)
			{
				return;
			}

			# Obtain the DIRECTIVE-KEYWORD field from the message -- It holds the request count
			my $requestCount = $message->getStringField("DIRECTIVE-KEYWORD")->getValue();
			libgmsec_perl::LogInfo("[RequestReplyCallback::onMessage] Received Request (" . $requestCount . ")");

			# Load the directive response template $message definition
			my $replyTemplate = $cfgFile->lookupMessage("DIRECTIVE-RESPONSE");

			# Create the reply message, with the request message's subject
			my $reply = libgmsec_perl::Message->new($message->getSubject(), $libgmsec_perl::Message::REPLY);

			# Copy fields from the reply message template to our actual reply $message
			$replyTemplate->copyFields($reply);

			# Replace the DATA field with the request number we are responding to
			my $field = libgmsec_perl::StringField->new("DATA", "$requestCount");
			$reply->addField($field);

			# Replace the TIME-COMPLETE field with the correct time
			my $theTime = "YYYY-DDD-HH:MM:SS.sss";
			libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
			$reply->addField("TIME-COMPLETED", $theTime);

			# Send the reply
			$connection->reply($message, $reply);

			libgmsec_perl::LogInfo("[RequestReplyCallback::onMessage] Reply Sent (" . $requestCount . ")");

			# Find and load the config file log message definition
			my $logMessage = $cfgFile->lookupMessage("LOG-REP");

			# Fill the fields within the log message
			$logMessage->addField("EVENT-TIME", $theTime);
			$logMessage->addField("MSG-TEXT", $requestCount);

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
	print "\nusage: perl replier.pl <filename>\n";
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

		# Look up configuration information
		my $progConfig    = $cfgFile->lookupConfig("program-config");
		my $connConfig    = $cfgFile->lookupConfig("connection-config");
		my $heartbeatMsg  = $cfgFile->lookupMessage("C2CX-HEARTBEAT-REP");

		# Get program config values
		my $updateRate    = Util::getInteger($progConfig, "update-rate", 1);
		my $loopCountdown = Util::getInteger($progConfig, "loop-time", 30);
		my $hbPubRate     = $heartbeatMsg->getI16Field("PUB-RATE")->getValue();

		# Create a $connection object off of the configuration
		my $connection = libgmsec_perl::Connection::create($connConfig);

		# Connect to the GMSEC bus
		$connection->connect();

		# Output version information
		libgmsec_perl::LogInfo("API version = " . libgmsec_perl::Connection::getAPIVersion());
		libgmsec_perl::LogInfo("Middleware version = " . $connection->getLibraryVersion());

		# Create subscriptions from subscription templates in the config file using callback
		my $cb = RequestReplyCallback->new($cfgFile);
		my $topic = $cfgFile->lookupSubscription("DIRECTIVE-REQUEST");

		$self->{SUBINFO} = $connection->subscribe($topic, $cb);

		# Output some general program information
		libgmsec_perl::LogInfo("Publishing for " . $loopCountdown . " seconds->");
		libgmsec_perl::LogInfo("Publishing Heartbeat Messages every " . $hbPubRate . " seconds->");

		# Call to create the thread that will automatically retrieve $messages off the message queue
		$connection->startAutoDispatch();

		# Display start time
		my $theTime = "YYYY-DDD-HH:MM:SS.sss";
		libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
		libgmsec_perl::LogInfo("Start Time: " . $theTime);

		# Publishing loop
		my $heartbeatCount     = 0;
		my $heartbeatCountdown = 0;

		for (; $loopCountdown > 0; --$loopCountdown)
		{
			# When the countdown reaches 0, we publish a heartbeat $message and reset the counter
			if ($heartbeatCountdown < 1)
			{
				++$heartbeatCount;

				$heartbeatMsg->addField("COUNTER", $heartbeatCount);

				# Publish the heartbeat $message
				$connection->publish($heartbeatMsg);

				# Output a heartbeat marker and reset the counter
				libgmsec_perl::LogInfo("Published Heartbeat");
				$heartbeatCountdown = $hbPubRate;
			}

			# Decrement the counters
			$heartbeatCountdown -= $updateRate;

			# Sleep
			Util::millisleep($updateRate * 1000);
		}

		# End time
		libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
		libgmsec_perl::LogInfo("End Time: " . $theTime);

		$connection->stopAutoDispatch();
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


1;

__END__
