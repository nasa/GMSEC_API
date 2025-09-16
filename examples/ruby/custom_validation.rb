#!/usr/bin/env ruby

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#
# @file custom_validation.rb
#
# @brief This example program demonstrates how to implement additional
# message validation logic in addition to that which the GMSEC API
# provides.
#


require 'libgmsec_ruby'
require './logger'


# Define a custom message validator
#
class CustomMessageValidator < Libgmsec_ruby::MessageValidator
    def initialize()
        super()
    end

    def validate_message(msg)
        status = Libgmsec_ruby::Status.new()

        # Check if we have a Heartbeat message, and if so, then check if
        # the PUB-RATE has an acceptable value.
        begin
            if msg.get_string_value("MESSAGE-SUBTYPE") == "HB"
                if msg.has_field("PUB-RATE")
                    pubRate = msg.get_integer_value("PUB-RATE")

                    if pubRate < 10 or pubRate > 60
                        status.set(Libgmsec_ruby::MSG_ERROR, Libgmsec_ruby::VALUE_OUT_OF_RANGE, "PUB-RATE field does not have a valid value")
                    end
                else
                    status.set(Libgmsec_ruby::MSG_ERROR, Libgmsec_ruby::MISSING_REQUIRED_FIELD, "PUB-RATE field not found")
                end
            end
        rescue GmsecException => e
            status = Libgmsec_ruby::Status.new(e)
        end

        return status
    end
end


class Custom_Validation
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
        # Create connection instance.
        conn = Libgmsec_ruby::Connection::create( @config )

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        set_standard_fields(conn.get_message_factory(), "CUSTOM-VALIDATION")

        # Establish connection to the GMSEC Bus.
        conn.connect()

        # Register custom message validator
        cmv = CustomMessageValidator.new()
        conn.get_message_factory().register_message_validator(cmv)

        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Create a Heartbeat message using the MessageFactory.
        # Set both required and optional fields within the message.
        hbMsg = conn.get_message_factory().create_message("HB")

        # Publish the message without PUB-RATE (expecting custom validation error)
        begin
            Libgmsec_ruby::Log::info("Publishing Message:\n" + hbMsg.to_xml())
            conn.publish(hbMsg)

            Libgmsec_ruby::Log::warning("An error was expected!")
        rescue GmsecException => e
            Libgmsec_ruby::Log::info("This error is expected: #{e.message}")
        end

        # Publish the message with invalid PUB-RATE value (expecting custom validation error)
        begin
            hbMsg.set_field_value("PUB-RATE", 5)

            Libgmsec_ruby::Log::info("Publishing Message:\n" + hbMsg.to_xml())
            conn.publish(hbMsg)

            Libgmsec_ruby::Log::warning("An error was expected!")
        rescue GmsecException => e
            Libgmsec_ruby::Log::info("This error is expected: #{e.message}")
        end

        # Publish the message with valid PUB-RATE value (expecting no validation error)
        begin
            hbMsg.set_field_value("PUB-RATE", 10)

            Libgmsec_ruby::Log::info("Publishing Message:\n" + hbMsg.to_xml())
            conn.publish(hbMsg)

            Libgmsec_ruby::Log::info("Message is valid!")
        rescue GmsecException => e
            Libgmsec_ruby::Log::error("Unexpected error: #{e.message}")
        end

        # Disconnect from the GMSEC Bus, and terminate subscriptions.
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end


    # Convenience function for associating standard (i.e. common) fields
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
    ex = Custom_Validation.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
