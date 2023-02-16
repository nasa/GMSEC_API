#!/usr/bin/env python3


# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file message_field_iterator.py
 
 This file contains a simple example demonstrating the use of the
 MessageFieldIterator class to iterate over the Fields of a Message.

 This example program is intended to be run prior to executing any example
 program which publishes a GMSEC message (e.g. The publish example).
"""

import libgmsec_python3 as lp
import sys


class FieldIterationCallback(lp.Callback):
    def __init__(self):
        super().__init__()

    def on_message(self, connection, msg):
        try:
            lp.log_info("Received a message with subject: " + msg.get_subject())

            lp.log_info("Field Name (Field Type): Field Value")

            iter = msg.get_field_iterator()

            while iter.has_next():
                field = iter.next()

                # Extract the Field Name, Type, and Value
                lp.log_info(field.get_name() + " (" + self.type_to_string(field.get_type()) + "): " + field.get_string_value())

                # Field objects can also be converted to specific Field types prior
                # to retrieval of the value contained in the Field. This is useful
                # for ensuring that data types do not lose any level of precision.
                # 
                # See get_field_value() for an example of how a Field can be converted to a
                # specialized Field object.
                #lp.log_info(field.get_name() + " (" + self.type_to_string(field.get_type()) + "): " + str(self.get_field_value(field)))

            msg.acknowledge()

        except lp.GmsecError as e:
            lp.log_error(str(e))


    def type_to_string(self, fieldType):
        switcher = {
            lp.Field.Type_BOOL:    "BOOL",
            lp.Field.Type_BINARY:  "BIN",
            lp.Field.Type_CHAR:    "CHAR",
            lp.Field.Type_I16:     "I16",
            lp.Field.Type_I32:     "I32",
            lp.Field.Type_I64:     "I64",
            lp.Field.Type_I8:      "I8",
            lp.Field.Type_F32:     "F32",
            lp.Field.Type_F64:     "F64",
            lp.Field.Type_STRING:  "STRING",
            lp.Field.Type_U16:     "U16",
            lp.Field.Type_U32:     "U32",
            lp.Field.Type_U64:     "U64",
            lp.Field.Type_U8:      "U8"
        }

        return switcher.get(fieldType, "UNKNOWN")


    # This method is provided as an example of how to retrieve type-specific
    # values from individual Field objects.
    #
    def get_field_value(self, field):
        if field.get_type() == lp.Field.Type_BINARY:
            return lp.to_BinaryField(field).get_value()

        elif field.get_type() == lp.Field.Type_BOOL:
            return lp.to_BooleanField(field).get_value()

        elif field.get_type() == lp.Field.Type_CHAR:
            return lp.to_CharField(field).get_value()

        elif field.get_type() == lp.Field.Type_I16:
            return lp.to_I16Field(field).get_value()

        elif field.get_type() == lp.Field.Type_I32:
            return lp.to_I32Field(field).get_value()

        elif field.get_type() == lp.Field.Type_I64:
            return lp.to_I64Field(field).get_value()

        elif field.get_type() == lp.Field.Type_I8:
            return lp.to_I8Field(field).get_value()

        elif field.get_type() == lp.Field.Type_F32:
            return lp.to_F32Field(field).get_value()

        elif field.get_type() == lp.Field.Type_F64:
            return lp.to_F64Field(field).get_value()

        elif field.get_type() == lp.Field.Type_STRING:
            return lp.to_StringField(field).get_value()

        elif field.get_type() == lp.Field.Type_U16:
            return lp.to_U16Field(field).get_value()

        elif field.get_type() == lp.Field.Type_U32:
            return lp.to_U32Field(field).get_value()

        elif field.get_type() == lp.Field.Type_U64:
            return lp.to_U64Field(field).get_value()

        elif field.get_type() == lp.Field.Type_U8:
            return lp.to_U8Field(field).get_value()


class message_field_iterator:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Deduce subscription topic for the example program
        self.subscription_topic = ""
        level = self.config.get_integer_value("gmsec-schema-level", lp.Specification.SchemaLevel_LEVEL_0)

        if level == lp.Specification.SchemaLevel_LEVEL_0:
            self.subscription_topic = "C2MS.>"

        elif level == lp.Specification.SchemaLevel_LEVEL_1:
            self.subscription_topic = "C2MS-PIPE.>"

        else:
            self.subscription_topic = "GMSEC.>"


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance.
            conn = lp.Connection(self.config)

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Subscribe to receive messages using a callback
            cb = FieldIterationCallback()
            conn.subscribe(self.subscription_topic, cb)
            lp.log_info("Subscribed to topic: " + self.subscription_topic)

            # Start the AutoDispatcher to asynchronously receive messages
            conn.start_auto_dispatch()

            # Wait for user input to end the program
            lp.log_info("Listening for Messages indefinitely press <enter> to exit the program")
            input("")

            # Clean up
            conn.stop_auto_dispatch()
            conn.disconnect()

        except lp.GmsecError as e:
            lp.log_error(str(e))


# Main entry point
ex = message_field_iterator()
ex.run()
