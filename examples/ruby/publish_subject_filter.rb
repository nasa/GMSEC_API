#!/usr/bin/env ruby

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file publish_subject_filter.rb
# 
# This file contains an example publishing a few messages with different
# subjects in order to demonstrate the subject filtering functionality of the
# API. 
#
# This example program is intended to be run after starting the
# 'subscribe_subject_filter' example program.
#


require 'libgmsec_ruby'
require './logger'


class Publish_Subject_Filter
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)
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
        set_standard_fields(conn.get_message_factory(), "PUBLISH-SUBJECT-FILTER")

        # Establish connection to the GMSEC Bus.
        conn.connect()
		
        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Create a valid Log message using the MessageFactory.
        logMsg = conn.get_message_factory().create_message("LOG")

        # Add required fields
        logMsg.add_field( Libgmsec_ruby::StringField.new("SUBCLASS", "INFO") )
        logMsg.add_field( Libgmsec_ruby::StringField.new("OCCURRENCE-TYPE", "TEST") )
        logMsg.add_field( Libgmsec_ruby::I16Field.new("SEVERITY", 1) )
        logMsg.add_field( Libgmsec_ruby::StringField.new("MSG-TEXT", "Hello subscriber!") )

        # And the current (event) time
        logMsg.add_field( Libgmsec_ruby::StringField.new("EVENT-TIME", Libgmsec_ruby::TimeUtil::format_time( Libgmsec_ruby::TimeUtil::get_current_time() )) )

        # Create a valid Heartbeat message using the MessageFactory.
        hbMsg = conn.get_message_factory().create_message("HB")

        Libgmsec_ruby::Log::info("Publishing two messages -- Log message will be received by the subscriber," +
                                 " the Heartbeat message will be filtered out")

        # Publish the Log message
        conn.publish(logMsg)
        Libgmsec_ruby::Log::info("Published LOG message:\n" + logMsg.to_xml())

        # Delay a bit
        Libgmsec_ruby::TimeUtil::millisleep(2000)

        # Publish the Heartbeat message
        conn.publish(hbMsg)
        Libgmsec_ruby::Log::info("Published HB message:\n" + hbMsg.to_xml())

        # Disconnect from the GMSEC Bus.
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
    ex = Publish_Subject_Filter.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
