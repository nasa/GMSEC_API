#!/usr/bin/env ruby

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file resource_generator.rb
#
# @brief This example program provides a basic demonstration of using
# the ResourceGenerator to publish resource messages onto the GMSEC
# Bus.
#


require 'libgmsec_ruby'
require './logger'


class Resource_Generator
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        # Validate messages before they are sent to the bus
        @config.add_value("gmsec-msg-content-validate-send", "true")
    end


    def initialize_log_level()
        level = Libgmsec_ruby::Log::from_string( @config.get_value("loglevel", "info") )
        Libgmsec_ruby::Log::set_reporting_level(level)
    end


    def run()
        # Define the resource publish rate and how often resource are collected
        publishRate     = 5  # in seconds
        sampleInterval  = 1  # in seconds
        averageInterval = 10 # in seconds

        # Acquire standard fields to be included with the heartbeat messages
        standardFields = get_standard_fields()

        # Create ResourceGenerator instance.
        rsrcgen = Libgmsec_ruby::ResourceGenerator::create(@config, publishRate, sampleInterval, averageInterval, standardFields)

        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("ResourceGenerator publish rate is: #{publishRate} seconds")

        # Start the resource generator
        rsrcgen.start()

        Libgmsec_ruby::Log::info("ResourceGenerator is running use gmsub or other utility to monitor messages")

        Libgmsec_ruby::Log::info("Press <enter> to stop the ResourceGenerator")
        $stdin.gets

        # Stop the ResourceGenerator
        rsrcgen.stop()
        Libgmsec_ruby::Log::info("ResourceGenerator has been stopped")

        # Destroy the ResourceGenerator instance
        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen)
    end


    def get_standard_fields()
        field1 = Libgmsec_ruby::StringField.new("MISSION-ID", "MY-MISSION", true)
        field2 = Libgmsec_ruby::StringField.new("SAT-ID-PHYSICAL", "MY-SAT-ID", true)
        field3 = Libgmsec_ruby::StringField.new("SAT-ID-LOGICAL", "MY-SAT-ID", true)
        field4 = Libgmsec_ruby::StringField.new("CONSTELLATION-ID", "MY-CNST", true)
        field5 = Libgmsec_ruby::StringField.new("FACILITY", "MY-FACILITY", true)
        field6 = Libgmsec_ruby::StringField.new("DOMAIN1", "MY-DOMAIN-1", true)
        field7 = Libgmsec_ruby::StringField.new("DOMAIN2", "MY-DOMAIN-2", true)
        field8 = Libgmsec_ruby::StringField.new("COMPONENT", "RSRC-GEN", true)

        standardFields = Libgmsec_ruby::FieldArray.new()

        standardFields << field1
        standardFields << field2
        standardFields << field3
        standardFields << field4
        standardFields << field5
        standardFields << field6
        standardFields << field7
        standardFields << field8

        return standardFields
    end
end


# Main entry point
begin
    ex = Resource_Generator.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
