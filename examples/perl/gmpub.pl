#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# gmpub.pl
# 
# A perl example demonstration of GMSEC publisher functionality.
# The associated example gmsub.pl example program will listen
# for the message published by this program.
# gmsub should be run before gmpub.
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

	print "\nusage: perl gmpub.pl connectiontype=<middleware> [ <parameter>=<value> ]" .
	"\n\n\tNote that the parameter 'connectiontype' is required." .
	"\n\tThe rest of other parameters are optional." .
	"\n\n\tserver=<server name> (required if middleware is not bolt/MB locally)" .
	"\n\tsubject=<subject name>" .
	"\n\titerations=<iterations>" .
	"\n\tinterval_ms=<interval (milliseconds)>" .
	"\n\tloglevel=<log level>" .
	"\n\tcfgfile=<config_filename.xml>" .
	"\n\nFor more information, see API User's Guide\n";

	return 1;
}


sub run
{
	my ($self, $config) = @_;

	my $subject = Util::getString($config, 'SUBJECT', 'GMSEC.TEST.PUBLISH');
	my $iterations = Util::getInteger($config, 'ITERATIONS', 1);
	my $interval_ms = Util::getInteger($config, 'INTERVAL_MS', 1000);

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

		# Output publisher-specific information
		libgmsec_perl::LogInfo("Using subject: " . $subject);
		libgmsec_perl::LogInfo("Publishing " . $iterations . " message(s)");

		if ($iterations > 1)
		{
			libgmsec_perl::LogInfo("Publish interval " . $interval_ms . " [ms]");
		}

		# Publish messages
		for (my $i = 0; $i < $iterations; ++$i)
		{
			$self->publish($config, $subject, $i);
			if ($i < $iterations -1)
			{
				Util::millisleep($interval_ms);
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


sub publish ()
{
	my ($self, $config, $subject, $iteration) = @_;

	my $connection = $self->{CONNECTION};

	my $value = $iteration + 1;

	my $message = libgmsec_perl::Message->new($subject, $libgmsec_perl::Message::PUBLISH);

	$self->{MESSAGE} = $message;

	# Add Fields
	my $field = libgmsec_perl::BooleanField->new("BOOL-FIELD-TRUE", 1);
	$message->addField($field);

	$field = libgmsec_perl::BooleanField->new("BOOL-FIELD-FALSE", 0);
	$message->addField($field);

	$field = libgmsec_perl::I8Field->new("I8-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::I16Field->new("I16-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::I32Field->new("I32-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::I64Field->new("I64-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::U8Field->new("U8-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::U16Field->new("U16-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::U32Field->new("U32-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::U64Field->new("U64-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::I32Field->new("COUNT", $value);
	$message->addField($field);

	$message->addField("STRING-FIELD", "This is a test");

	$field = libgmsec_perl::F32Field->new("F32-FIELD", $value);
	$message->addField($field);

	$field = libgmsec_perl::F64Field->new("F64-FIELD", $value);
	$message->addField($field);

	if ($config->getBooleanValue("ENCRYPT", 0))
	{
		$field = libgmsec_perl::BooleanField->new("SEC-ENCRYPT", 1);
		$message->addField($field);
	}

	# Publish Message
	$connection->publish($message);

	# Display XML representation of message
	libgmsec_perl::LogInfo("Published:\n" . $message->toXML());

	return 1;
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
