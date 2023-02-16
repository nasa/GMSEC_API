#!/usr/bin/env perl

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file subscribe_async.pl
#
# @brief This example program provides a basic demonstration of subscribing
# to receive a message from the GMSEC Bus using a callback.
#


use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


# Define custom callback to receive messages
#
{
    package MyCallback;
    use base 'libgmsec_perl::Callback';

    sub new
    {
        my $class = shift;
        my $self = $class->SUPER::new();
        bless $self, $class;
        return $self;
    }

    sub onMessage
    {
        my ($self, $connection, $msg) = @_;
        libgmsec_perl::logInfo("MyCallback.onMessage -- \n" . $msg->toXML());
    }
}


# Define custom callback to receive events
# 
# Although not required, an event callback can assist with determining
# whether we have a connection to the GMSEC Bus, or in the case of this
# example program, whether the Auto-Dispatcher thread has terminated
# due to some error.
# 
{
    package MyEventCallback;
    use base 'libgmsec_perl::EventCallback';

    sub new
    {
        my $class = shift;
        my $self = $class->SUPER::new();
        bless $self, $class;

        $self->{ERROR} = 0;

        return $self;
    }

    sub onEvent
    {
        my ($self, $connection, $status, $event) = @_;

        # Check event type and/or error status of the Status object.
        if ($event == $libgmsec_perl::Connection::Event_DISPATCHER_ERROR_EVENT)
        {
            # Auto-dispatcher error has occurred we are done!
            libgmsec_perl::logError("MyEventCallback.onEvent -- " . $status->getReason());
            $self->{ERROR} = 1;
        }
        elsif ($status->hasError())
        {
            # Soft-error has occurred
            libgmsec_perl::logWarning("MyEventCallback.onEvent -- " . $status->getReason());
        }
        else
        {
            # Nominal event has occurred
            libgmsec_perl::logInfo("MyEventCallback.onEvent -- " . $status->getReason());
        }
    }

    sub haveError
    {
        my ($self) = @_;

        return $self->{ERROR};
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

    # Declared custom callback and custom event callback
    my $mcb = MyCallback->new();
    my $ecb = MyEventCallback->new();

    # Deduce subscription topic for the example program
    my $subscriptionTopic = "";
    my $level = $config->getIntegerValue("gmsec-schema-level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

    if ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0)
    {
        $subscriptionTopic = "C2MS.>";
    }
    elsif ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_1)
    {
        $subscriptionTopic = "C2MS-PIPE.>";
    }
    else
    {
        $subscriptionTopic = "GMSEC.>";
    }

    eval
    {
        # Create connection instance.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Register custom EventCallback
        $conn->registerEventCallback( $libgmsec_perl::Connection::Event_ALL_EVENTS, $ecb );

        # Establish connection to the GMSEC Bus.
        $conn->connect();

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Subscribe to receive message via a callback
        $conn->subscribe( $subscriptionTopic, $mcb );
        libgmsec_perl::logInfo("Subscribed to topic: " . $subscriptionTopic);

        # Attempt to receive a message
        my $rcvd = $conn->receive( $libgmsec_perl::GMSEC_WAIT_FOREVER );

        # It is good practice to check if a message was received, but in this particular
        # case, since GMSEC_WAIT_FOREVER is being used, we are either guaranteed to
        # receive a message or have some exception thrown due to a middleware issue.
        if (defined $rcvd)
        {
            libgmsec_perl::logInfo("Received a message; dispatching it...");
            $conn->dispatch( $rcvd );
            $rcvd->acknowledge();
            libgmsec_perl::Message::destroy( $rcvd );
        }
        else
        {
            libgmsec_perl::logWarning("Failed to receive a message");
        }

        # Disconnect from the GMSEC Bus, and terminate subscription.
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


# Main entry point
#
exit( main() );
