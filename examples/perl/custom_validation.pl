#!/usr/bin/env perl

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#
# @file custom_validation.pl
#
# @brief This example program demonstrates how to implement additional
# message validation logic in addition to that which the GMSEC API
# provides.
#


use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


# Define a custom message validator
#
{
    package CustomMessageValidator;
    use base 'libgmsec_perl::MessageValidator';

    sub new
    {
        my $class = shift;
        my $self = $class->SUPER::new();
        bless $self, $class;
        return $self;
    }

    sub validateMessage
    {
        my ($self, $msg) = @_;

        my $status = libgmsec_perl::Status->new();

        # Check if we have a Heartbeat message, and if so, then check if
        # the PUB-RATE has an acceptable value.
        if ($msg->hasField("MESSAGE-SUBTYPE") && $msg->getStringValue("MESSAGE-SUBTYPE") eq "HB")
        {
            if ($msg->hasField("PUB-RATE"))
            {
                my $pubRate = $msg->getIntegerValue("PUB-RATE");

                if ($pubRate < 10 || $pubRate > 60)
                {
                    $status->set($libgmsec_perl::MSG_ERROR, $libgmsec_perl::VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value");
                }
            }
            else
            {
                $status->set($libgmsec_perl::MSG_ERROR, $libgmsec_perl::MISSING_REQUIRED_FIELD, "PUB-RATE field not found");
            }
        }
        else
        {
            $status->set($libgmsec_perl::MSG_ERROR, $libgmsec_perl::MISSING_REQUIRED_FIELD, "Unknown (i.e non-HB) message received");
        }

        return $status;
    }
}


sub main
{
    # Load the command-line input into a GMSEC Config object
    my $config = undef;
    if ($#ARGV ge 0)
    {
        $config = libgmsec_perl::Config::create(\@ARGV);
    }
    else
    {
        $config = libgmsec_perl::Config->new();
    }

    # Initialize log level for output
    initializeLogLevel($config);

    # Validate messages before they are sent to the bus
    $config->addValue("gmsec-msg-content-validate-send", "true");

    eval
    {
        # Create connection instance.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        setStandardFields( $conn->getMessageFactory(), "CUSTOM-VALIDATION");

        # Establish connection to the GMSEC Bus.
        $conn->connect();

        # Register custom message validator
        my $cmv = CustomMessageValidator->new();
        $conn->getMessageFactory()->registerMessageValidator($cmv);

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Create a Heartbeat message using the MessageFactory.
        # Set both required and optional fields within the message.
        my $hbMsg = $conn->getMessageFactory()->createMessage("HB");

        # Publish the message without PUB-RATE (expecting custom validation error)
        eval
        {
            libgmsec_perl::logInfo("Publishing Message:\n" . $hbMsg->toXML());
            $conn->publish($hbMsg);

            libgmsec_perl::logWarning("An error was expected!");
        };
        if (isa($@, 'libgmsec_perl::GmsecException'))
        {
            my $error = $@;
            libgmsec_perl::logInfo("This error is expected: " . $error->what());
        }

        # Publish the message with invalid PUB-RATE value (expecting custom validation error)
        eval
        {
            $hbMsg->setFieldValue("PUB-RATE", 5);

            libgmsec_perl::logInfo("Publishing Message:\n" . $hbMsg->toXML());
            $conn->publish($hbMsg);

            libgmsec_perl::logWarning("An error was expected!");
        };
        if (isa($@, 'libgmsec_perl::GmsecException'))
        {
            my $error = $@;
            libgmsec_perl::logInfo("This error is expected: " . $error->what());
        }

        # Publish the message with valid PUB-RATE value (expecting no validation error)
        eval
        {
            $hbMsg->setFieldValue("PUB-RATE", 10);

            libgmsec_perl::logInfo("Publishing Message:\n" . $hbMsg->toXML());
            $conn->publish($hbMsg);

            libgmsec_perl::logInfo("Message is valid!");
        };
        if (isa($@, 'libgmsec_perl::GmsecException'))
        {
            my $error = $@;
            libgmsec_perl::logInfo("Unexpected error: " . $error->what());
        }

        # Disconnect from the GMSEC Bus.
        $conn->disconnect();

        # Destroy the connection instance
        libgmsec_perl::Connection::destroy($conn);
    };
    if (isa($@, 'libgmsec_perl::GmsecException'))
    {
        # If here, the most likely case is the middleware could not be
        # deduced, or a failure occurred when attempting to connect to
        # the GMSEC Bus.
        my $error = $@;
        libgmsec_perl::logError("Exception: " . $error->what());
        return -1;
    }

    return 0;
}


sub initializeLogLevel
{
    my ($config) = @_;
    my $level = libgmsec_perl::Log::fromString( $config->getValue("loglevel", "info") );
    libgmsec_perl::Log::setReportingLevel($level);
}


# Convenience function for associating standard (i.e. common) fields
# with the MessageFactory which will be included automatically within
# all messages created using the factory.
#
sub setStandardFields
{
    my ($factory, $component) = @_;

    my $standardFields = libgmsec_perl::FieldArray->new();

    my $field = libgmsec_perl::StringField->new("MISSION-ID", "MY-MISSION", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "MY-SAT-ID", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("SAT-ID-LOGICAL", "MY-SAT-ID", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("CONSTELLATION-ID", "MY-CNST", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("FACILITY", "MY-FACILITY", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("DOMAIN1", "MY-DOMAIN-1", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("DOMAIN2", "MY-DOMAIN-2", 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $field = libgmsec_perl::StringField->new("COMPONENT", $component, 1);
    $standardFields->push( $field );
    $field->DISOWN();

    $factory->setStandardFields( $standardFields );
}


# Main entry point
#
exit( main() );
