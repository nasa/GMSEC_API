#!/usr/bin/env ruby

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file specification_addendum.rb
# 
# This file contains an example demonstrating the extensibility of the GMSEC
# API Message validation engine through use of additional XML template files
# represending an additional layer of Message definitions to be validated.
#


require 'libgmsec_ruby'
require './logger'


class Specification_Addendum
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        # Validate messages before they are sent to the bus
        @config.add_value("GMSEC-MSG-CONTENT-VALIDATE-SEND", "true")

        # Tell the API that there is an additional layer of message schema to
        # validate (The 'EXAMPLE' message definitions). By default, this value
        # is set to 0 (only 'C2MS' specification validation).
        # 
        # Note: These levels for validation are determined by the "LEVEL-X"
        # attributes defined in the DIRECTORY.xml file contained in the XML
        # templates directory. In the case of this example, Level-0 implies
        # C2MS, and Level-1 is our example extension.
        # 
        # Note: The GMSEC team envisions using message specifications in a
        # layered hierarchical fashion. For example, the "C2MS" message
        # specification would be 'layer 0', followed by an organization-level
        # message specification at 'layer 1' which builds upon the message
        # specification outlined in C2MS.  This would be followed by a mission
        # level message specification at 'layer 2' (e.g. GMSEC) and so on.
        @config.add_value("GMSEC-SCHEMA-LEVEL", "1")

        # Tell the API where to find all of the schema files.
        # 
        # Note: This example only demonstrates a simple usage case of this
        # functionality. When using this functionality, if the intent is
        # to use any of the GMSEC message definitions, then ALL of the XML
        # template files must be contained in the same directory.
        # e.g. GMSEC_API/templates/2019.00 (or the location defined in
        # GMSEC-SCHEMA-PATH)
        @config.add_value("GMSEC-SCHEMA-PATH", "templates")

        # Since this example relies on the 2019.00 version of the templates,
        # we indicate such within the configuration object.
        @config.add_value("GMSEC-SPECIFICATION-VERSION", "201900")
    end


    def initialize_log_level()
        level = Libgmsec_ruby::Log::from_string( @config.get_value("loglevel", "info") )
        Libgmsec_ruby::Log::set_reporting_level(level)
    end


    def run()
        # Create connection instance using custom configuration.
        conn = Libgmsec_ruby::Connection::create( @config )

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        set_standard_fields(conn.get_message_factory(), "SPECIFICATION-ADDENDUM")

        # Establish connection to the GMSEC Bus.
        conn.connect()

        # Output information regarding the API version and connection.
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Create a LOG Message using a subject defined in the XML template
        # outlining our example addendum message definitions.
        logMsg = conn.get_message_factory().create_message("LOG")

        # Add all of the required fields to the LOG message.
        logMsg.add_field( Libgmsec_ruby::U16Field.new("NUM-OF-EVENTS", 2) )
        logMsg.add_field( Libgmsec_ruby::StringField.new("EVENT.1.DESC", add_time_to_string("AOS occurred at: ")) )
        logMsg.add_field( Libgmsec_ruby::StringField.new("EVENT.2.DESC", add_time_to_string("Telemetry downlink began at: ")) )

        # Publish the LOG message to the GMSEC Bus.
        conn.publish(logMsg)

        # Display the XML string representation of the LOG message just published.
        Libgmsec_ruby::Log::info("Published custom LOG Message:\n" + logMsg.to_xml())

        # Setup a new LOG message without the NUM-OF-EVENTS required field and
        # attempt to publish it (i.e. trigger a validation failure)
        badLogMsg = conn.get_message_factory().create_message("LOG")

        begin
            conn.publish(badLogMsg)
            Libgmsec_ruby::Log::warning("Surprisingly was able to publish an invalid message:\n" + badLogMsg.to_xml())
        rescue GmsecException => e
            Libgmsec_ruby::Log::info("This validation error is expected for the incomplete LOG message:\n#{e.message}")
        end

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


    def add_time_to_string(preface)
        return preface + Libgmsec_ruby::TimeUtil::format_time( Libgmsec_ruby::TimeUtil::get_current_time() )
    end
end


# Main entry point
begin
    ex = Specification_Addendum.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
