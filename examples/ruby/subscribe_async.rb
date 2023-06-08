#!/usr/bin/env ruby

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file subscribe_async.rb
#
# @brief This example program provides a basic demonstration of subscribing
# to receive a message from the GMSEC Bus using a callback.
#


require 'libgmsec_ruby'
require './logger'


# Define custom callback to receive messages
#
class MyCallback < Libgmsec_ruby::Callback
    def initialize()
        super()
    end

    def on_message(conn, msg)
        Libgmsec_ruby::Log::info("MyCallback.on_message -- \n" + msg.to_xml())
        msg.acknowledge()
    end
end


# Define custom callback to receive events
# 
# Although not required, an event callback can assist with determining
# whether we have a connection to the GMSEC Bus, or in the case of this
# example program, whether the Auto-Dispatcher thread has terminated
# due to some error.
# 
class MyEventCallback < Libgmsec_ruby::EventCallback
    def initialize()
        super()
        @error = false
    end

    def on_event(conn, status, event)
        # Check event type and/or error status of the Status object.

        if event == Libgmsec_ruby::Connection::Event_DISPATCHER_ERROR_EVENT
            # Auto-dispatcher error has occurred we are done!
            Libgmsec_ruby::Log::error("MyEventCallback.on_event -- " + status.get_reason())
            self.error = True

        elsif status.has_error()
            # Soft-error has occurred
            Libgmsec_ruby::Log::warning("MyEventCallback.on_event -- " + status.get_reason())

        else
            # Nominal event has occurred
            Libgmsec_ruby::Log::info("MyEventCallback.on_event -- " + status.get_reason())
        end
    end

    def have_error()
        @error
    end
end


class Subscribe_Async
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        # Declared custom callback and custom event callback
        @mcb = MyCallback.new()
        @ecb = MyEventCallback.new()

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

        # Register custom EventCallback
        conn.register_event_callback(Libgmsec_ruby::Connection::Event_ALL_EVENTS, @ecb)

        # Establish connection to the GMSEC Bus.
        conn.connect()

        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Subscribe to receive message via a callback
        conn.subscribe(@subscription_topic, @mcb)
        Libgmsec_ruby::Log::info("Subscribed to topic: " + @subscription_topic)

        # Attempt to receive a message
        rcvd = conn.receive(Libgmsec_ruby::GMSEC_WAIT_FOREVER)

        # It is good practice to check if a message was received, but in this particular
        # case, since GMSEC_WAIT_FOREVER is being used, we are either guaranteed to
        # receive a message or have some exception thrown due to a middleware issue.
        if rcvd == nil
            Libgmsec_ruby::Log::warning("Failed to receive a message")
        else
            Libgmsec_ruby::Log::info("Received a message; dispatching it...")
            conn.dispatch(rcvd)
            rcvd.acknowledge()
            Libgmsec_ruby::Message::destroy(rcvd)
        end

        # Disconnect from the GMSEC Bus, and terminate subscription.
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end
end


# Main entry point
begin
    ex = Subscribe_Async.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
