#!/usr/bin/env ruby

# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file message_field_iterator.py
# 
# This file contains a simple example demonstrating the use of the
# MessageFieldIterator class to iterate over the Fields of a Message.
#
# This example program is intended to be run prior to executing any example
# program which publishes a GMSEC message (e.g. The publish example).
#


require 'libgmsec_ruby'
require './logger'


class FieldIterationCallback < Libgmsec_ruby::Callback
    def initialize()
        super()
    end

    def on_message(connection, msg)
        begin
            Libgmsec_ruby::Log::info("Received a message with subject: " + msg.get_subject())

            Libgmsec_ruby::Log::info("Field Name (Field Type): Field Value")

            iter = msg.get_field_iterator()

            while iter.has_next() do
                field = iter.next()

                # Extract the Field Name, Type, and Value
                name  = field.get_field_name()
                type  = type_to_string(field.get_field_type())
                value = field.get_string_value()

                # Field objects can also be converted to specific Field types prior
                # to retrieval of the value contained in the Field. This is useful
                # for ensuring that data types do not lose any level of precision.
                # 
                # See get_field_value() for an example of how a Field can be converted to a
                # specialized Field object.
                #value = get_field_value(field)

                Libgmsec_ruby::Log::info("#{name} (#{type}): #{value}")
            end

            msg.acknowledge()

        rescue GmsecException => e
            Libgmsec_ruby::Log::error(e.message)
        end
    end


    def type_to_string(fieldType)
        case fieldType
        when Libgmsec_ruby::Field::Type_BINARY
            return "BIN"
        when Libgmsec_ruby::Field::Type_BOOL
            return "BOOL"
        when Libgmsec_ruby::Field::Type_CHAR
            return "CHAR"
        when Libgmsec_ruby::Field::Type_I16
            return "I16"
        when Libgmsec_ruby::Field::Type_I32
            return "I32"
        when Libgmsec_ruby::Field::Type_I64
            return "I64"
        when Libgmsec_ruby::Field::Type_I8
            return "I8"
        when Libgmsec_ruby::Field::Type_F32
            return "F32"
        when Libgmsec_ruby::Field::Type_F64
            return "F64"
        when Libgmsec_ruby::Field::Type_STRING
            return "STRING"
        when Libgmsec_ruby::Field::Type_U16
            return "U16"
        when Libgmsec_ruby::Field::Type_U32
            return "U32"
        when Libgmsec_ruby::Field::Type_U64
            return "U64"
        when Libgmsec_ruby::Field::Type_U8
            return "U8"
        end
    end


    # This method is provided as an example of how to retrieve type-specific
    # values from individual Field objects.
    #
    def get_field_value(field)
        case field.get_field_type()
        when Libgmsec_ruby::Field::Type_BINARY
            return Libgmsec_ruby::Field::toBinaryField(field).get_value()
        when Libgmsec_ruby::Field::Type_BOOL
            return Libgmsec_ruby::Field::toBooleanField(field).get_value()
        when Libgmsec_ruby::Field::Type_CHAR
            return Libgmsec_ruby::Field::toCharField(field).get_value()
        when Libgmsec_ruby::Field::Type_I16
            return Libgmsec_ruby::Field::toI16Field(field).get_value()
        when Libgmsec_ruby::Field::Type_I32
            return Libgmsec_ruby::Field::toI32Field(field).get_value()
        when Libgmsec_ruby::Field::Type_I64
            return Libgmsec_ruby::Field::toI64Field(field).get_value()
        when Libgmsec_ruby::Field::Type_I8
            return Libgmsec_ruby::Field::toI8Field(field).get_value()
        when Libgmsec_ruby::Field::Type_F32
            return Libgmsec_ruby::Field::toF32Field(field).get_value()
        when Libgmsec_ruby::Field::Type_F64
            return Libgmsec_ruby::Field::toF64Field(field).get_value()
        when Libgmsec_ruby::Field::Type_STRING
            return Libgmsec_ruby::Field::toStringField(field).get_value()
        when Libgmsec_ruby::Field::Type_U16
            return Libgmsec_ruby::Field::toU16Field(field).get_value()
        when Libgmsec_ruby::Field::Type_U32
            return Libgmsec_ruby::Field::toU32Field(field).get_value()
        when Libgmsec_ruby::Field::Type_U64
            return Libgmsec_ruby::Field::toU64Field(field).get_value()
        when Libgmsec_ruby::Field::Type_U8
            return Libgmsec_ruby::Field::toU8Field(field).get_value()
        end
    end
end


class Message_Field_Iterator
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

        # Subscribe to receive messages using a callback
        cb = FieldIterationCallback.new()
        conn.subscribe(@subscription_topic, cb)
        Libgmsec_ruby::Log::info("Subscribed to topic: #{@subscription_topic}")

        # Attempt to receive a message
        rcvd = conn.receive(Libgmsec_ruby::GMSEC_WAIT_FOREVER)

        Libgmsec_ruby::Log::info("Dispatching message")
        conn.dispatch(rcvd)
        rcvd.acknowledge()
        Libgmsec_ruby::Message::destroy(rcvd)

        # Clean up
        conn.disconnect()

        # Destroy the connection instance
        Libgmsec_ruby::Connection::destroy(conn)
    end
end


# Main entry point
begin
    ex = Message_Field_Iterator.new( ARGV )
    ex.run
rescue GmsecException => e
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end
