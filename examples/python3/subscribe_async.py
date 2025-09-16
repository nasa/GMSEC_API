#!/usr/bin/env python3

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


"""
 @file subscribe_async.py

 @brief This example program provides a basic demonstration of subscribing
 to receive a message from the GMSEC Bus using an asynchronous callback.
"""

import libgmsec_python3 as lp
import sys


# Define custom callback to receive messages
#
class MyCallback(lp.Callback):
    def __init__(self):
        super().__init__()
        self.gotMsg = False

    def on_message(self, conn, msg):
        self.gotMsg = True
        lp.log_info("MyCallback.on_message -- \n" + msg.to_xml())
        msg.acknowledge()

    def got_message(self):
        return self.gotMsg


# Define custom callback to receive events
# 
# Although not required, an event callback can assist with determining
# whether we have a connection to the GMSEC Bus, or in the case of this
# example program, whether the Auto-Dispatcher thread has terminated
# due to some error.
# 
class MyEventCallback(lp.EventCallback):
    def __init__(self):
        super().__init__()
        self.error = False

    def on_event(self, conn, status, event):
        # Check event type and/or error status of the Status object.

        if event == lp.Connection.Event_DISPATCHER_ERROR_EVENT:
            # Auto-dispatcher error has occurred we are done!
            lp.log_error("MyEventCallback.on_event -- " + status.get_reason())
            self.error = True

        elif status.has_error():
            # Soft-error has occurred
            lp.log_warning("MyEventCallback.on_event -- " + status.get_reason())

        else:
            # Nominal event has occurred
            lp.log_info("MyEventCallback.on_event -- " + status.get_reason())

    def have_error(self):
        return self.error


class subscribe_async:
    def __init__(self):
        # Set up connection configuration options using values from the command line.
        self.config = lp.Config(sys.argv)

        # Validate incoming messages
        self.config.add_value("gmsec-msg-content-validate-recv", "true")

        # Initialize log level for output
        self.initialize_log_level()

        # Declared custom callback and custom event callback
        self.mcb = MyCallback()
        self.ecb = MyEventCallback()

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

            # Register custom EventCallback
            conn.register_event_callback(lp.Connection.Event_ALL_EVENTS, self.ecb)

            # Establish connection to the GMSEC Bus.
            conn.connect()

            # Output information regarding the API version and connection
            lp.log_info(lp.Connection.get_api_version())
            lp.log_info("Middleware version = " + conn.get_library_version())

            # Subscribe to receive message via a callback
            conn.subscribe(self.subscription_topic, self.mcb)
            lp.log_info("Subscribed to: " + self.subscription_topic)

            # Start the auto-dispatcher so we can receive messages via the callback
            conn.start_auto_dispatch()

            # Normally an application would have something important to do
            # here we just idle until a message arrives. Should an error
            # occur, we bail.
            lp.log_info("Waiting to receive a message...")
            while self.mcb.got_message() == False and self.ecb.have_error() == False:
                lp.TimeUtil.millisleep(1000)

            # Disconnect from the GMSEC Bus, and terminate subscriptions.
            conn.disconnect()

            # Destroy the Connection
            del conn

        except lp.GmsecError as e:
            # If here, the most likely case is the middleware could not be
            # deduced, or a failure occurred when attempting to connect to
            # the GMSEC Bus.
            lp.log_error("Exception: " + str(e))


# Main entry point
ex = subscribe_async()
ex.run()
