#!/usr/bin/env ruby

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file subscribe_subject_filter.rb
# 
# This file contains a simple example outlining how to subscribe
# to and retrieve messages from the middleware bus while using subject
# exclusion filtering to reduce the scope of a wildcarded subject subscription.
#
# This example program is intended to be run before starting the
# 'publish_subject_filter' example program.
#


require 'libgmsec_ruby'
require './logger'


class Subscribe_Subject_Filter
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        # Deduce subscription topic for the example program
        level = @config.get_integer_value("gmsec-schema-level", Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)

        case level
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0
            @ALL_MESSAGES_TOPIC = "C2MS.>"
            @HB_MESSAGES_TOPIC  = "C2MS.*.*.*.*.*.MSG.HB.+"
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1
            @ALL_MESSAGES_TOPIC = "C2MS-PIPE.>"
            @HB_MESSAGES_TOPIC  = "C2MS-PIPE.*.*.*.*.*.MSG.HB.+"
        else
            @ALL_MESSAGES_TOPIC = "GMSEC.>"
            @HB_MESSAGES_TOPIC  = "GMSEC.*.*.MSG.HB.+"
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

        # Subscribe to receive all messages using the GMSEC specification
        Libgmsec_ruby::Log::info("Subscribe to: " + @ALL_MESSAGES_TOPIC)
        conn.subscribe( @ALL_MESSAGES_TOPIC )

        # Add specific message topic to the Connection's exclusion filter
        Libgmsec_ruby::Log::info("Filtering out: " + @HB_MESSAGES_TOPIC)
        conn.exclude_subject( @HB_MESSAGES_TOPIC )

        # Receive first message (LOG) sent by publisher
        Libgmsec_ruby::Log::info("Waiting to receive a message...")
        rcvdMsg = conn.receive()

        Libgmsec_ruby::Log::info("Received LOG message:\n" + rcvdMsg.to_xml())
        rcvdMsg.acknowledge()
        Libgmsec_ruby::Message::destroy(rcvdMsg)

        # Although a second message (HB) is sent by the publisher, it will not
        # be delivered to this Connection object
        Libgmsec_ruby::Log::info("Waiting to receive other message(s)...")
        rcvdMsg = conn.receive(5000)

        if rcvdMsg == nil
            Libgmsec_ruby::Log::info("As expected, a timeout occurred (i.e. no HB message received)")
        else
            Libgmsec_ruby::Log::Warning("Received unexpected message:\n" + rcvdMsg.to_xml())
            Libgmsec_ruby::Message::destroy(rcvdMsg)
        end

        # Disconnect from the GMSEC Bus.
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end
end


# Main entry point
begin
    ex = Subscribe_Subject_Filter.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
