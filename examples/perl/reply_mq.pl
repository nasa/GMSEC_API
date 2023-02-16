#!/usr/bin/env perl

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file reply_mq.pl
#
# @brief This example program provides a basic demonstration of synchronously
# receiving a Request Directive message from the GMSEC Bus using an Apache
# Artemis Message Queue, and then issuing a Response Directive message (should
# one be requested).
#
# This example is intended to be used with the request example program.
#


use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


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

    # Validate messages before being sent to and as they are received from the bus
    $config->addValue("gmsec-msg-content-validate-all", "true");

    # Check user has specified an mw-id of artemis
    my $mw = $config->getValue("mw-id", "");
    if ($mw ne "artemis")
    {
        libgmsec_perl::logError("This example program only works with Apache Artemis; use mw-id=artemis and run again");
        return;
    }

    # Define subscription topic for receiving request message(s).
    # The topic includes the syntax for a Fully Qualified Queue Name (FQQN).
    my $subscriptionTopic = "";
    my $level = $config->getIntegerValue("gmsec-schema-level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

    if ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0)
    {
        $subscriptionTopic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY::MY-MSG-QUEUE";
    }
    elsif ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_1)
    {
        $subscriptionTopic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY::MY-MSG-QUEUE";
    }
    else
    {
        $subscriptionTopic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY::MY-MSG-QUEUE";
    }

    eval
    {
        # Create connection instance
        my $conn = libgmsec_perl::Connection::create( $config );

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        setStandardFields( $conn->getMessageFactory(), "REPLY" );

        # Establish connection to the GMSEC Bus.
        $conn->connect();

        # Set up our subscription for receiving a request message.
        $conn->subscribe( $subscriptionTopic );
        libgmsec_perl::logInfo("Subscribed to topic: " . $subscriptionTopic);

        # Timeout was arbitrarily chosen as libgmsec_perl::GMSEC_WAIT_FOREVER so that
        # we block indefinitely waiting to receive a request message.
        # It can be set to other time values (e.g. 1000 is 1 second) so
        # the application does not block indefinitely waiting for a
        # request message.
        my $timeout = $libgmsec_perl::GMSEC_WAIT_FOREVER;

        libgmsec_perl::logInfo("Responder is waiting to receive message(s)...");
        libgmsec_perl::logInfo("Consider running a second instance of the example program");
        libgmsec_perl::logInfo("Use the 'request' example program to send a message");

        # Block for a Request Directive message to arrive.
        my $reqMsg = $conn->receive( $timeout );

        # Check if we have received a message.
        if (defined $reqMsg)
        {
            # Output in XML the received message.
            libgmsec_perl::logInfo("Received Request Message:\n" . $reqMsg->toXML());

            # Check if a response message should be returned.
            if ($reqMsg->hasField("RESPONSE") && $reqMsg->getBooleanValue("RESPONSE"))
            {
                # Create a Response Directive message using the MessageFactory.
                my $rspMsg = $conn->getMessageFactory()->createMessage("RESP.DIR");

                # Insert required and optional fields into the Response Directive
                # using values derived from the incoming Request Directive.
                if ($reqMsg->hasField("COMPONENT"))
                {
                    $rspMsg->addField( libgmsec_perl::StringField->new("DESTINATION-COMPONENT", $reqMsg->getStringValue("COMPONENT"), 1) );
                }

                if ($reqMsg->hasField("REQUEST-ID"))
                {
                    $rspMsg->addField( $reqMsg->getField("REQUEST-ID") );
                }

                # Set the response status for the request.
                $rspMsg->setFieldValue("RESPONSE-STATUS", $libgmsec_perl::Message::ResponseStatus_SUCCESSFUL_COMPLETION);

                # Send the Response Directive message.
                $conn->reply($reqMsg, $rspMsg);

                # Output in XML the message that has been sent.
                libgmsec_perl::logInfo("Sent Response Message:\n" . $rspMsg->toXML());
            }

            # Acknowledge processing the request message so that it is not delivered to other clients.
            $reqMsg->acknowledge();

            # Release received message
            libgmsec_perl::Message::destroy($reqMsg);
        }
        else
        {
            # This else-block is reached if a request message is not received.
            # When the timeout period is set to libgmsec_perl::GMSEC_WAIT_FOREVER, then this
            # block would never be reached.
            libgmsec_perl::logWarning("Time out no Request Directive message received");
        }

        # Disconnect from the GMSEC Bus, and terminate subscriptions.
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


# Convenience method for associating standard (i.e. common) fields
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
