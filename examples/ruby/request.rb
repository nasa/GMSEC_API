#!/usr/bin/env ruby

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file request.rb
#
# @brief This example program provides a basic demonstration of issuing
# a synchronous Request Directive message to the GMSEC Bus, and then
# handling the receipt of a Response Directive message.
#


require 'libgmsec_ruby'
require './logger'


class Request
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        # Validate messages before being sent to and as they are received from the bus
        @config.add_value("gmsec-msg-content-validate-all", "true")
    end


    def initialize_log_level()
        level = Libgmsec_ruby::Log::from_string( @config.get_value("loglevel", "info") )
        Libgmsec_ruby::Log::set_reporting_level(level)
    end


    def run()
        # Create connection instance.
        conn = Libgmsec_ruby::Connection::create( @config )

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        set_standard_fields(conn.get_message_factory(), "REQUEST")

        # Establish connection to the GMSEC Bus.
        conn.connect()

        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Create a Request Directive message using the MessageFactory.
        # Set both required and optional fields within the message.
        reqMsg = conn.get_message_factory().create_message("REQ.DIR")
        reqMsg.add_field( Libgmsec_ruby::StringField.new("DESTINATION-COMPONENT", "REPLY", true) )     # to whom is the request intended for
        reqMsg.add_field( Libgmsec_ruby::StringField.new("DIRECTIVE-STRING", "Turn on the lights") )   # operation to perform
        reqMsg.add_field( Libgmsec_ruby::BooleanField.new("RESPONSE", true) )                          # a response message is requested
        reqMsg.add_field( Libgmsec_ruby::U16Field.new("REQUEST-ID", 0) )

        # Output in XML what we intend to send
        Libgmsec_ruby::Log::info("Sending Request Message:\n" + reqMsg.to_xml())

        # Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
        # The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
        # prevent re-sending request message should a timeout occur.
        timeout   = 5000
        republish = 5000

        # Issue Request Directive.
        # Note, this is a blocking call (i.e. a synchronous operation).
        rspMsg = conn.request(reqMsg, timeout, republish)

        # We have returned from the previous blocking call check if we have a message.
        if rspMsg != nil
            Libgmsec_ruby::Log::info("Received Response Directive message:\n" + rspMsg.to_xml())
        else
            # This else-block is reached if a response message is not received.
            # If the republish period is set to a time period other than
            # GMSEC_REQUEST_REPUBLISH_NEVER, then this block will never be reached.
            Libgmsec_ruby::Log::warning("Time out no Response Directive message received")
        end

        # Disconnect from the GMSEC Bus, and terminate subscriptions.
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end


    # Convenience method for associating standard (i.e. common) fields
    # with the MessageFactory which will be included automatically within
    # all messages created using the factory.
    #
    def set_standard_fields(factory, component)
        field1 = Libgmsec_ruby::StringField.new("MISSION-ID", "MY-MISSION", true)
        field2 = Libgmsec_ruby::StringField.new("SAT-ID-PHYSICAL", "MY-SAT-ID", true)
        field3 = Libgmsec_ruby::StringField.new("SAT-ID-LOGICAL", "MY-SAT-ID", true)
        field4 = Libgmsec_ruby::StringField.new("CONSTELLATION-ID", "MY-CNST", true)
        field5 = Libgmsec_ruby::StringField.new("FACILITY", "MY-FACILITY", true)
        field6 = Libgmsec_ruby::StringField.new("DOMAIN1", "MY-DOMAIN-1", true)
        field7 = Libgmsec_ruby::StringField.new("DOMAIN2", "MY-DOMAIN-2", true)
        field8 = Libgmsec_ruby::StringField.new("COMPONENT", component, true)

        standardFields = Libgmsec_ruby::FieldArray.new()

        standardFields << field1
        standardFields << field2
        standardFields << field3
        standardFields << field4
        standardFields << field5
        standardFields << field6
        standardFields << field7
        standardFields << field8

        factory.set_standard_fields(standardFields)
    end
end


# Main entry point
begin
    ex = Request.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
