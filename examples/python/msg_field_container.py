#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file msg_field_container.py
 
 
 This file contains an example demonstrating the creation of GMSEC Messages
 using the two available container types for Field objects: Binary Tree Map
 (STL map) or Hash Map (unordered map).
"""

import libgmsec_python
import sys

def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " <config-file>" + "\nPlease see the GMSEC_API/example/cpp/message_field_container directory for an example xml configuration file." 
        print usageMessage
        return -1


    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging()

    try:
        # Demonstration of setting the configuration options via
        # configuration file
        createMsgUsingConfigFile(sys.argv[1])

        # Demonstration of hard-coding the configuration options
        # using a Config object
        createMsgUsingConfigObject()
        
    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())
        return -1
        
    return 0


def createMsgUsingConfigFile(filename):
    
    # Load and parse configuration file
    cfgFile = libgmsec_python.ConfigFile()
    cfgFile.load(filename)

    # Retrieve config from file
    cfg = cfgFile.lookupConfig("config")

    # Loop through three different examples outlined in the configuration
    # file provided with this example
    for i in range(0,3):
        
        # Form message name (e.g. msg1) to reference.
        msgName = "msg" + str(i+1)

        # Parse the message from the configuration file and load it
        # into a GMSEC Message
        msg = cfgFile.lookupMessage(msgName)
                
        if(i+1 == 1):
            description = "Expecting TREE map (at size limit of 8 fields):"
        elif(i+1 == 2):
            description = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):"
        elif(i+1 == 3):
            description = "Expecting HASH map (size limit is ignored):"
                
        displayMessage(msg, description)
        

def createMsgUsingConfigObject():
    
    cfg1 = libgmsec_python.Config()
    cfg2 = libgmsec_python.Config()
    cfg3 = libgmsec_python.Config()
    empty = libgmsec_python.Config()

    cfg1.addValue("gmsec-msgfld-store-type", "tree")
    # If the Message object is using a binary tree map to store fields,
    # the 'gmsec-msgfield-store-size' configuration option indicates how
    # many fields should be allowed to be stored before the Message
    # object converts to using a hash map (Default is 50).
    cfg1.addValue("gmsec-msgfld-store-size", "10")

    cfg2.addValue("gmsec-msgfld-store-type", "hash")

    cfg3.addValue("gmsec-msgfld-store-type", "tree")    # container type
    cfg3.addValue("gmsec-msgfld-store-size", "0")      # never rollover

    msg1 = buildMessage(cfg1,   9)   # uses bin-tree container map (under size limit)
    msg2 = buildMessage(cfg1,  10)   # uses bin-tree container map (at size limit)
    msg3 = buildMessage(cfg1,  11)   # uses hash container map (exceeds size limit)
    msg4 = buildMessage(cfg2,   5)   # default to using hash container map (size limit ignored)
    msg5 = buildMessage(cfg3,  55)   # always uses bin-tree container map
    msg6 = buildMessage(empty, 50)   # uses bin-tree container map (default size limit not exceeded)
    msg7 = buildMessage(empty, 51)   # uses hash container map (exceeds default size limit)

    displayMessage(msg1, "Message using TREE container map (under limit):")
    displayMessage(msg2, "Message using TREE container map (at max limit):")
    displayMessage(msg3, "Message was using TREE container map, but rolls over to HASH map (limit exceeded):")
    displayMessage(msg4, "Message using HASH container map (limit ignored):")
    displayMessage(msg5, "Message using TREE container map (limit ignored):")
    displayMessage(msg6, "Message using TREE container map (at limit):")
    displayMessage(msg7, "Message using HASH container map (limit exceeded):")



def buildMessage(config, numFields):

    msg = libgmsec_python.Message("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", libgmsec_python.Message.PUBLISH, config)

    if (numFields > 0):
        
        # Add Fields
        for i in range (0, numFields):
                
            fieldName = "FIELD-"

            if i <10:
                fieldName+="00"
            elif i < 100:
                fieldName+="0" 
            else:
                fieldName+=""

            msg.addField(fieldName, i)
            

        return msg



def displayMessage(msg, description):

    xml = msg.toXML()
    libgmsec_python.logInfo(description + "\n" + xml)


def initializeLogging():

    # Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
    # object, it affects the logging level and output destination for all
    # logging macros in the entire API, regardless of whether that Config
    # object is kept around or not. In this case, we are creating a local
    # Config object just for the sake of initializing logging.
    config = libgmsec_python.Config()

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

