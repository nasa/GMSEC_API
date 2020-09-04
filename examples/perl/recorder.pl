#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#/


#
# recorder.pl
#
# This is a sample application built with the perl
# GMSEC API.  This application demonstrates several
# key components within the GMSEC API including loading
# from an XML file, producing a heartbeat, and publishing.
#
# Please use the XML configuration: sampleAppsCommon.xml
#


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

# LogCallback Callback definition
{
	package LogCallback;
	use base 'libgmsec_perl::Callback';

	sub onMessage
	{
		my ($self, $connection, $message) = @_;
		libgmsec_perl::LogInfo("[LogCallback::onMessage] Received:\n" . $message->toXML());
	}
}


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
	print "\nusage: perl recorder.pl <filename>\n";
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

		# Lookup subscription subjects/topics
		my $rcvLogSubject = $cfgFile->lookupSubscription("RECEIVE-LOG");
		my $sndLogSubject = $cfgFile->lookupSubscription("SEND-LOG");

		if (!$rcvLogSubject)
		{
			die libgmsec_perl::Exception->new($libgmsec_perl::CONFIGFILE_ERROR, $libgmsec_perl::OTHER_ERROR_CODE, "RECEIVE-LOG subscription entry not found");
		}
		if (!$sndLogSubject)
		{
			die libgmsec_perl::Exception->new($libgmsec_perl::CONFIGFILE_ERROR, $libgmsec_perl::OTHER_ERROR_CODE, "SEND-LOG subscription entry not found");
		}

		# Look up the heartbeat message definition, and attempt to acquire the PUB-RATE
		my $heartbeatMsg = $cfgFile->lookupMessage("C2CX-HEARTBEAT-REC");

		my $loopCountdown = Util::getInteger($progConfig, 'loop-time', 30);
		my $updateRate = Util::getInteger($progConfig, 'update-rate', 30);
		my $pubRate = 30;

		eval
		{
			$pubRate = $heartbeatMsg->getI16Field("PUB-RATE")->getValue();
		};
		if ($@)
		{
			libgmsec_perl::LogWarning("Unable to find PUB-RATE; will use default publish rate of " . $pubRate . " seconds.");
		}

		# Look up the config file value that contains the configuration for the middleware
		my $connConfig = $cfgFile->lookupConfig("connection-config");

		# Create a connection object off of the configuration
		$self->{CONNECTION} = libgmsec_perl::Connection::create($connConfig);
		my $connection = $self->{CONNECTION};

		# Connect to the GMSEC bus
		$connection->connect();

		# output GMSEC API version
		libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

		# Output middleware version
		libgmsec_perl::LogInfo("Middleware version = " . $connection->getLibraryVersion());

		# Create subscriptions from subscription templates in the config file using callback
		my $cb = LogCallback->new();
		$cb->DISOWN();

		libgmsec_perl::LogInfo("Subscribing to " . $rcvLogSubject);
		push(@{$self->{SUBINFO}}, $connection->subscribe($rcvLogSubject, $cb));

		libgmsec_perl::LogInfo("Subscribing to " . $sndLogSubject);
		push(@{$self->{SUBINFO}}, $connection->subscribe($sndLogSubject, $cb));

		# Output some general program information
		libgmsec_perl::LogInfo("Publishing for " . $loopCountdown . " seconds.");
		libgmsec_perl::LogInfo("Publishing C2CX Heartbeat messages every " . $pubRate . " seconds.");

		# Call to create the thread that will automatically retrieve messages off the message queue
		$connection->startAutoDispatch();

		# Display start time
		my $theTime = "YYYY-DDD-HH:MM:SS.sss";
		libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
		libgmsec_perl::LogInfo("Start Time: " . $theTime);

		my $heartbeatCount     = 0;
		my $heartbeatCountdown = 0;

		for (; $loopCountdown > 0; --$loopCountdown)
		{
			# When the countdown reaches 0, we publish a heartbeat message and reset the counter
			if ($heartbeatCountdown < 1)
			{
				++$heartbeatCount;

				my $field = libgmsec_perl::I32Field->new("COUNTER", $heartbeatCount);
				$heartbeatMsg->addField($field);

				# Publish the heartbeat message
				$connection->publish($heartbeatMsg);

				# Output a heartbeat marker and reset the counter
				libgmsec_perl::LogInfo("Published Heartbeat");
				$heartbeatCountdown = $pubRate;
			}

			# Decrement the counters
			$heartbeatCountdown -= $updateRate;

			# Sleep
			Util::millisleep($updateRate * 1000);
		}

		# Display end time
		libgmsec_perl::TimeUtil::formatTime(libgmsec_perl::TimeUtil::getCurrentTime(), $theTime);
		libgmsec_perl::LogInfo("End Time: " . $theTime);

		# Stop the Auto Dispatcher
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
