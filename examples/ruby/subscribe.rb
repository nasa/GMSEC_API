#!/usr/bin/env ruby

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file subscribe.rb
#
# @brief This example program provides a basic demonstration of subscribing
# to receive a message from the GMSEC Bus.
#


require 'libgmsec_ruby'
require './logger'


class Subscribe
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        # Validate messages as they are received from the bus
        @config.add_value("gmsec-msg-content-validate-recv", "true")

        # Deduce subscription topic for the example program
        level = @config.get_integer_value("gmsec-schema-level", Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)

        case level
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0
            @subscription_topic = "C2MS.>"
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1
            @subscription_topic = "C2MS-PIPE.>"
        else
            @subscription_topic = "GMSEC.>"
        end
    end


    def initialize_log_level()
        level = Libgmsec_ruby::Log::from_string( @config.get_value("loglevel", "info") )
        Libgmsec_ruby::Log::set_reporting_level(level)
    end


    def run()
        # Create connection instance.
        conn = Libgmsec_ruby::Connection::create( @config )

        # Establish connection to the GMSEC Bus.
        conn.connect()

        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Subscribe to receive message
        conn.subscribe( @subscription_topic )
        Libgmsec_ruby::Log::info("Subscribed to topic: " + @subscription_topic)

        # Define timeout period, or use Gmsec.WAIT_FOREVER to block
        # indefinitely to receive a message.
        timeout = 5000   # 5 seconds

        # Attempt to receive a message
        msg = conn.receive(timeout)

        if msg != nil
            # Received a message!
            Libgmsec_ruby::Log::info("Received Message:\n" + msg.to_xml())

            # Acknowledge processing the message
            msg.acknowledge()

            # Release received message
            Libgmsec_ruby::Message::destroy(msg)
        else
            # Time out!
            Libgmsec_ruby::Log::warning("Time out no message was received")
        end

        # Disconnect from the GMSEC Bus, and terminate subscriptions.
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end
end


# Main entry point
begin
    ex = Subscribe.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
