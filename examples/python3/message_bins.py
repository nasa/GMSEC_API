#!/usr/bin/env python3

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file message_bins.py

 @brief This file contains an example describing how to aggregate multiple
 messages into a singular message sent to the middleware server. This
 functionality is useful for reducing the number of messages sent across
 the * middleware bus, if there is an application which is publishing an
 exorbitantly large number of (small) messages.

 It is recommended to execute the application GMSEC_API/bin/gmsub prior
 to running this program in order to see that the aggregated messages
 still appear on the receiving end as if they were individual messages.

 Please note that if an aggregated message is sent by the GMSEC API, any
 receivers will need to be running at least version 4.2 of the API in order
 to de-aggregate the messages. Older versions can receive the messages
 without crashing, but they will be unusable.
"""

import libgmsec_python3 as lp
import sys


class message_bins:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Initialize log level for output
        self.initialize_log_level()

        # Deduce subject and excluded subject for message(s) that will be binned and for those that will not
        level = self.config.get_integer_value("gmsec-schema-level", lp.Specification.SchemaLevel_LEVEL_0)

        if level == lp.Specification.SchemaLevel_LEVEL_0:
            self.msg_bin_subject         = "C2MS.*.*.*.*.*.MSG.LOG.*.AAA.>"
            self.msg_bin_exclude_subject = "C2MS.*.*.*.*.*.MSG.LOG.*.TLM.>"

        elif level == lp.Specification.SchemaLevel_LEVEL_1:
            self.msg_bin_subject         = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.AAA.>"
            self.msg_bin_exclude_subject = "C2MS-PIPE.*.*.*.*.*.MSG.LOG.*.TLM.>"

        else:
            self.msg_bin_subject         = "GMSEC.*.*.MSG.LOG.*.AAA.>"
            self.msg_bin_exclude_subject = "GMSEC.*.*.MSG.LOG.*.TLM.>"

        # Validate messages before being sent to the bus
        self.config.add_value("gmsec-msg-content-validate-send", "true")

        # Configure to enable message binning
        self.config.add_value("gmsec-use-msg-bins", "true")

        # Specify the number of messages to be aggregated prior to publishing
        # the aggregate message to the middleware server. This applies to all
        # of the messages which match the subject(s) provided in the
        # GMSEC-MSG-BIN-SUBJECT-N configuration parameters
        # Note: The aggregate message will be sent to the middleware server
        # immediately upon this many messages being published, regardless of
        # the value supplied for GMSEC-MSG-BIN-TIMEOUT.
        self.config.add_value("gmsec-msg-bin-size", "10")

        # Specify a timeout (in milliseconds) for the aggregate message to be
        # sent to the GMSEC Bus.
        # Note: The aggregate message will be sent to the GMSEC Bus after this
        # period of time if the message bin does not fill up (per the value
        # provided for gmsec-msg-bin-size) prior to this timeout
        self.config.add_value("gmsec-msg-bin-timeout", "5000")

        # Specify the message subject(s) of messages that should be aggregated.
        # Subscription wildcard syntax can be used here.
        # Note: If this option is not specified, then ALL messages will be
        # aggregated (except those that are excluded).
        self.config.add_value("gmsec-msg-bin-subject-1", self.msg_bin_subject)

        # Specify any message subject(s) that should be excluded from being
        # aggregated.
        # This is useful if a wildcard subscription is provided in one of the
        # gmsec-msg-bin-subject-N parameters.
        self.config.add_value("gmsec-msg-bin-exclude-subject-1", self.msg_bin_exclude_subject)


    def initialize_log_level(self):
        level = lp.Log.from_string( self.config.get_value("loglevel", "info") )
        lp.Log.set_reporting_level(level)


    def run(self):
        try:
            # Create connection instance.
            conn = lp.Connection(self.config)

            # Set up standard fields within the MessageFactory associated
            # with the connection object.
            self.set_standard_fields(conn.get_message_factory(), "MESSAGE-BINS")

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Create a Log messages using the MessageFactory.
            # Set both required and optional fields within the message.
            logMsg1 = conn.get_message_factory().create_message("LOG")
            logMsg2 = conn.get_message_factory().create_message("LOG")

            logMsg1.add_field( lp.StringField("SUBCLASS", "AAA") )
            logMsg1.add_field( lp.StringField("OCCURRENCE-TYPE", "PLOT") )
            logMsg1.add_field( lp.I16Field("SEVERITY", 1) )
            logMsg1.add_field( lp.StringField("MSG-TEXT", "Message will be binned") )

            logMsg2.add_field( lp.StringField("SUBCLASS", "TLM") )
            logMsg2.add_field( lp.StringField("OCCURRENCE-TYPE", "DATA") )
            logMsg2.add_field( lp.I16Field("SEVERITY", 1) )
            logMsg2.add_field( lp.StringField("MSG-TEXT", "Message will NOT be binned") )

            for i in range(0,5):
                theTime = lp.TimeUtil.format_time( lp.TimeUtil.get_current_time() )

                logMsg1.add_field( lp.StringField("EVENT-TIME", theTime) )
                logMsg2.add_field( lp.StringField("EVENT-TIME", theTime) )

                # Publish the first message it will be aggregated and published at a later time
                conn.publish(logMsg1)
                lp.log_info("Publishing AAA Log Message " + str(i+1) + " (will be aggregated):\n" + logMsg1.to_xml())

                # Publish the second message it will NOT be aggregated and will be published immediately
                conn.publish(logMsg2)
                lp.log_info("Publishing TLM Log Message " + str(i+1) + " (will NOT be aggregated):\n" + logMsg2.to_xml())

                # Add processing delay
                lp.TimeUtil.millisleep(100)

            # Delay to demonstrate message bin is flushed after a period of time (e.g. 5 seconds)
            lp.log_info("Waiting for binned messages to be published...")
            lp.TimeUtil.millisleep(6000)

            # Disconnect from the GMSEC Bus, and terminate subscriptions.
            # Note: Any remaining messages in message bins will be published
            # before disconnecting from the GMSEC Bus, thus ensuring no message
            # loss occurs.
            conn.disconnect()

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))



    # Convenience function for associating standard (i.e. common) fields
    # with the MessageFactory which will be included automatically within
    # all messages created using the factory.
    def set_standard_fields(self, factory, component):
        self.field1 = lp.StringField("MISSION-ID", "MY-MISSION", True)
        self.field2 = lp.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", True)
        self.field3 = lp.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", True)
        self.field4 = lp.StringField("CONSTELLATION-ID", "MY-CNST", True)
        self.field5 = lp.StringField("FACILITY", "MY-FACILITY", True)
        self.field6 = lp.StringField("DOMAIN1", "MY-DOMAIN-1", True)
        self.field7 = lp.StringField("DOMAIN2", "MY-DOMAIN-2", True)
        self.field8 = lp.StringField("COMPONENT", component, True)

        standardFields = lp.FieldList()

        standardFields.push_back( self.field1 )
        standardFields.push_back( self.field2 )
        standardFields.push_back( self.field3 )
        standardFields.push_back( self.field4 )
        standardFields.push_back( self.field5 )
        standardFields.push_back( self.field6 )
        standardFields.push_back( self.field7 )
        standardFields.push_back( self.field8 )

        factory.set_standard_fields(standardFields)


# Main entry point
ex = message_bins()
ex.run()
