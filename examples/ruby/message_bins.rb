#!/usr/bin/env ruby

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file message_bins.py
#
# @brief This file contains an example describing how to aggregate multiple
# messages into a singular message sent to the middleware server. This
# functionality is useful for reducing the number of messages sent across
# the * middleware bus, if there is an application which is publishing an
# exorbitantly large number of (small) messages.
#
# It is recommended to execute the application GMSEC_API/bin/gmsub prior
# to running this program in order to see that the aggregated messages
# still appear on the receiving end as if they were individual messages.
#
# Please note that if an aggregated message is sent by the GMSEC API, any
# receivers will need to be running at least version 4.2 of the API in order
# to de-aggregate the messages. Older versions can receive the messages
# without crashing, but they will be unusable.
#


require 'libgmsec_ruby'
require './logger'


class Message_Bins
    def initialize(args)
        # Set up connection configuration options using values from the command line.
        @config = Libgmsec_ruby::Config.new( args )

        # Initialize log level for output
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        # Deduce subject and excluded subject for message(s) that will be binned and for those that will not
        level = @config.get_integer_value("gmsec-schema-level", Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)

        case level
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0
            @msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>"
            @msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>"
        when Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1
            @msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>"
            @msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>"
        else
            @msg_bin_subject         = "GMSEC.*.*.MSG.LOG.*.AAA.>";
            @msg_bin_exclude_subject = "GMSEC.*.*.MSG.LOG.*.TLM.>";
        end

        # Validate messages before they are sent to the bus
        @config.add_value("gmsec-msg-content-validate-send", "true")

        # Configure to enable message binning
        @config.add_value("gmsec-use-msg-bins", "true")

        # Specify the number of messages to be aggregated prior to publishing
        # the aggregate message to the middleware server. This applies to all
        # of the messages which match the subject(s) provided in the
        # GMSEC-MSG-BIN-SUBJECT-N configuration parameters
        # Note: The aggregate message will be sent to the middleware server
        # immediately upon this many messages being published, regardless of
        # the value supplied for GMSEC-MSG-BIN-TIMEOUT.
        @config.add_value("gmsec-msg-bin-size", "10")

        # Specify a timeout (in milliseconds) for the aggregate message to be
        # sent to the GMSEC Bus.
        # Note: The aggregate message will be sent to the GMSEC Bus after this
        # period of time if the message bin does not fill up (per the value
        # provided for gmsec-msg-bin-size) prior to this timeout
        @config.add_value("gmsec-msg-bin-timeout", "5000")

        # Specify the message subject(s) of messages that should be aggregated.
        # Subscription wildcard syntax can be used here.
        # Note: If this option is not specified, then ALL messages will be
        # aggregated (except those that are excluded).
        @config.add_value("gmsec-msg-bin-subject-1", @msg_bin_subject)

        # Specify any message subject(s) that should be excluded from being
        # aggregated.
        # This is useful if a wildcard subscription is provided in one of the
        # gmsec-msg-bin-subject-N parameters.
        @config.add_value("gmsec-msg-bin-exclude-subject-1", @msg_bin_exclude_subject)
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
        set_standard_fields(conn.get_message_factory(), "MESSAGE-BINS")

        # Establish connection to the GMSEC Bus.
        conn.connect()

        # Output information regarding the API version and connection
        Libgmsec_ruby::Log::info(Libgmsec_ruby::Connection::get_api_version())
        Libgmsec_ruby::Log::info("Middleware version = " + conn.get_library_version())

        # Create a Log messages using the MessageFactory.
        # Set both required and optional fields within the message.
        logMsg1 = conn.get_message_factory().create_message("LOG")
        logMsg2 = conn.get_message_factory().create_message("LOG")

        logMsg1.add_field( Libgmsec_ruby::StringField.new("SUBCLASS", "AAA") )
        logMsg1.add_field( Libgmsec_ruby::StringField.new("OCCURRENCE-TYPE", "PLOT") )
        logMsg1.add_field( Libgmsec_ruby::I16Field.new("SEVERITY", 1) )
        logMsg1.add_field( Libgmsec_ruby::StringField.new("MSG-TEXT", "Message will be binned") )

        logMsg2.add_field( Libgmsec_ruby::StringField.new("SUBCLASS", "TLM") )
        logMsg2.add_field( Libgmsec_ruby::StringField.new("OCCURRENCE-TYPE", "DATA") )
        logMsg2.add_field( Libgmsec_ruby::I16Field.new("SEVERITY", 1) )
        logMsg2.add_field( Libgmsec_ruby::StringField.new("MSG-TEXT", "Message will NOT be binned") )

        for i in 0..5 do
            theTime = Libgmsec_ruby::TimeUtil::format_time( Libgmsec_ruby::TimeUtil::get_current_time() )

            logMsg1.add_field( Libgmsec_ruby::StringField.new("EVENT-TIME", theTime) )
            logMsg2.add_field( Libgmsec_ruby::StringField.new("EVENT-TIME", theTime) )

            # Publish the first message it will be aggregated and published at a later time
            conn.publish(logMsg1)
            Libgmsec_ruby::Log::info("Publishing AAA Log Message #{i+1} (will be aggregated):\n" + logMsg1.to_xml())

            # Publish the second message it will NOT be aggregated and will be published immediately
            conn.publish(logMsg2)
            Libgmsec_ruby::Log::info("Publishing TLM Log Message #{i+1} (will NOT be aggregated):\n" + logMsg2.to_xml())

            # Add processing delay
            Libgmsec_ruby::TimeUtil::millisleep(100)
        end

        # Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
        Libgmsec_ruby::Log::info("Waiting for binned messages to be published...")
        Libgmsec_ruby::TimeUtil::millisleep(6000)

        # Disconnect from the GMSEC Bus, and terminate subscriptions.
        # Note: Any remaining messages in message bins will be published
        # before disconnecting from the GMSEC Bus, thus ensuring no message
        # loss occurs.
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end


    # Convenience function for associating standard (i.e. common) fields
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
    ex = Message_Bins.new( ARGV )
    ex.run
rescue GmsecException => e
    # If here, the most likely case is the middleware could not be
    # deduced, or a failure occurred when attempting to connect to
    # the GMSEC Bus.
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
