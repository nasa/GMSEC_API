#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""


""" 
 @file subscribe_async.py
 
 This file contains a simple example outlining how to asynchronously subscribe
 to and retrieve messages from the middleware bus. Asynchronous receipt
 operations essentially mean that a Callback subclass is implemented, that the
 corresponding Callback object is registered to a subscription, and that
 messages are received asynchronously after starting the AutoDispatcher.

 This example program is intended to be run before starting the 'publish'
 example program.
"""

import libgmsec_python
import sys

DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.TEST.PUBLISH"

# Globals
receivedMessage = False

# Create a subclass of Callback and implement the onMessage function
# The onMessage function will determine what action(s) the program takes
# when receiving messages asynchronously
class ExampleCallback(libgmsec_python.ConnectionManagerCallback):

    def __init__(self):
        libgmsec_python.ConnectionManagerCallback.__init__(self)
        self.receivedMessage = False

    def onMessage(self, connection, message):
        libgmsec_python.logInfo("[ExampleCallback.onMessage] Received:\n" + message.toXML())
        self.receivedMessage = True
     

def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " +  sys.argv[0] + " mw-id=<middleware ID>"
        print usageMessage
        return -1


    # Load the command-line input into a GMSEC Config object
    # A Config object is basically a key-value pair map which is used to
    # pass configuration options into objects such as Connections,
    # ConnectionManagers, Subscribe and Publish function calls, Messages,
    # etc.
    config = libgmsec_python.Config()

    for arg in sys.argv[1:]:
        value = arg.split('=')
        config.addValue(value[0], value[1])


    initializeLogging(config)

    #Print the GMSEC API version number using the GMSEC Logging
    # interface
    # This is useful for determining which version of the API is
    # configured within the environment
    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    try:
        # Create a ConnectionManager object
        # This is the linchpin for all communications between the
        # GMSEC API and the middleware server
        connMgr = libgmsec_python.ConnectionManager(config)

        # Open the connection to the middleware
        libgmsec_python.logInfo("Opening the connection to the middleware server")
        connMgr.initialize()

        # Output middleware client library version
        libgmsec_python.logInfo(connMgr.getLibraryVersion())

        # Set up a subscription to listen for Messages with the topic
        # "GMSEC.TEST.PUBLISH" which are published to the middleware
        # bus and register the callback object to the subscription
        # Subscription subject wildcard syntax:
        # * - Matches any one token separated by periods in a subject
        # > - Reading left to right, matches everything up to and ONE
        #     or more tokens in a subject
        # + - Reading left to right, matches everything up to and ZERO 
        #     or more tokens in a subject
        # For more information on wildcards, please see the GMSEC API
        # User's Guide
        libgmsec_python.logInfo("Subscribing to the topic: " + DEFAULT_SUBSCRIPTION_SUBJECT)
        cb = ExampleCallback()
        connMgr.subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, cb)

        # Start the AutoDispatcher to begin asynchronously processing
        # messages
        connMgr.startAutoDispatch()

        # Because the handling of messages is occurring asynchronously
        # in another thread, we will need to 'busy wait' here until a
        # message is received
        while (cb.receivedMessage == 0):
            libgmsec_python.TimeUtil.millisleep(100)
                
        # Clean up
        connMgr.stopAutoDispatch()
        connMgr.cleanup()
        
    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())
        return -1
        
    return 0


def initializeLogging(config):

    # If it was not specified in the command-line arguments, set
    # LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the 
    # program report output on the terminal/command line
    logLevel  = config.getValue("LOGLEVEL")
    logFile   = config.getValue("LOGFILE")

    if (not logLevel):
        
        config.addValue("LOGLEVEL", "INFO")
        
    if (not logFile):
        
        config.addValue("LOGFILE", "STDERR")
       
#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())
 
