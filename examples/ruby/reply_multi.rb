#!/usr/bin/env ruby

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file reply_multi.rb
#
# @brief This example program provides a basic demonstration of synchronously
# receiving a Request Directive message from the GMSEC Bus, and then issuing
# multiple Response Directive messages (should one be requested).
#


require 'libgmsec_ruby'
require './logger'


class Reply_Multi
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

        # Define subscription topic for receiving request message(s).
        level = @config.get_integer_value("gmsec-schema-level", Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)

        case level
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0
            @subscription_topic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY-MULTI"
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1
            @subscription_topic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY-MULTI"
        else
            @subscription_topic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY-MULTI"
        end
    end


    def initialize_log_level()
        level = Libgmsec_ruby::Log::from_string( @config.get_value("loglevel", "info") )
        Libgmsec_ruby::Log::set_reporting_level(level)
    end


    def run()
        # Create connection instance
        conn = Libgmsec_ruby::Connection::create( @config )

        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Set up standard fields within the MessageFactory associated
        # with the connection object.
        set_standard_fields(conn.get_message_factory(), "REPLY-MULTI")

        # Establish connection to the GMSEC Bus.
        conn.connect()

        # Set up our subscription for receiving a request message.
        conn.subscribe( @subscription_topic )
        Libgmsec_ruby::Log::info("Subscribed to topic: #{@subscription_topic}")

        # Timeout was arbitrarily chosen as Libgmsec_ruby::GMSEC_WAIT_FOREVER so that
        # we block indefinitely waiting to receive a request message.
        # It can be set to other time values (e.g. 1000 is 1 second) so
        # the application does not block indefinitely waiting for a
        # request message.
        timeout = Libgmsec_ruby::GMSEC_WAIT_FOREVER

        # Block for a Request Directive message to arrive.
        reqMsg = conn.receive(timeout)

        # Check if we have received a message.
        if reqMsg != nil
            # Output in XML the received message.
            Libgmsec_ruby::Log::info("Received Request Message:\n" + reqMsg.to_xml())

            # Check if a response message should be returned.
            if reqMsg.has_field("RESPONSE") and reqMsg.get_boolean_value("RESPONSE")
                # Create a Response Directive message using the MessageFactory.
                rspMsg = conn.get_message_factory().create_message("RESP.DIR")

                # Insert required and optional fields into the Response Directive
                # using values derived from the incoming Request Directive.
                if reqMsg.has_field("COMPONENT")
                    rspMsg.add_field( Libgmsec_ruby::StringField.new("DESTINATION-COMPONENT", reqMsg.get_string_value("COMPONENT"), true) )
                end

                if reqMsg.has_field("REQUEST-ID")
                    rspMsg.add_field( reqMsg.get_field("REQUEST-ID") )
                end

                # Send multiple responses
                for i in 0..3 do
                    if i == 0
                        responseStatus = Libgmsec_ruby::Message::ResponseStatus_ACKNOWLEDGEMENT
                    elsif i == 3
                        responseStatus = Libgmsec_ruby::Message::ResponseStatus_SUCCESSFUL_COMPLETION
                    else
                        responseStatus = Libgmsec_ruby::Message::ResponseStatus_WORKING_KEEP_ALIVE
                    end

                    rspMsg.set_field_value("RESPONSE-STATUS", responseStatus)

                    # Send the Response Directive message.
                    conn.reply(reqMsg, rspMsg)

                    # Output in XML the message that has been sent.
                    Libgmsec_ruby::Log::info("Sent Response Message:\n" + rspMsg.to_xml())

                    # Arbitrary delay to mimic processing of task
                    Libgmsec_ruby::TimeUtil::millisleep(1000)
                end
            end

            # Acknowledge processing the request message
            reqMsg.acknowledge()

            # Release received request message
            Libgmsec_ruby::Message::destroy(reqMsg)

        else
            # This else-block is reached if a reuqest message is not received.
            # When the timeout period is set to Gmsec.WAIT_FOREVER, then this
            # block would never be reached.
            Libgmsec_ruby::Log::warning("Time out no Request Directive message received")
        end

        # Disconnect from the GMSEC Bus, and terminate subscriptions.
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end


    # Convenience method for associating standard (i.e. common) fields
    # with the MessageFactory which will be included automatically within
    # all messages created using the factory.
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
    ex = Reply_Multi.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect toi
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
