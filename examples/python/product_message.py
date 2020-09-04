#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file product_message.py
 
 This file contains an example outlining how to create and publish a
 GMSEC-compliant Mnemonic Data Value message to the middleware bus using the
 Messaging Interface Standardization Toolkit (MIST) namespace
 ConnectionManager class function calls available for such operations.

 It is also recommended that you run a subscriber application
 (i.e. GMSEC_API/bin/gmsub) to see the LOG Messages which are published to the
 bus.
"""

import libgmsec_python
import sys

GMSEC_SPEC_VERSION   = "2016.00"
PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE"


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


    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging(config)

    # Print the GMSEC API version number using the GMSEC Logging
    # interface
    # This is useful for determining which version of the API is
    # configured within the environment
    libgmsec_python.logInfo(libgmsec_python.ConnectionManager.getAPIVersion())

    try:
        # Create a ConnectionManager object
        connManager = libgmsec_python.ConnectionManager(config)

        # Open the connection to the middleware
        libgmsec_python.logInfo("Opening the connection to the middleware server")
        connManager.initialize()

        # Output middleware client library version
        libgmsec_python.logInfo(connManager.getLibraryVersion())

        # Create all of the GMSEC Message header Fields which will
        # be used by all GMSEC Messages
                
        # Note: Since these Fields contain variable values which are
        # based on the context in which they are used, they cannot be
        # automatically populated using MistMessage.
        definedFields = libgmsec_python.FieldList() 

        missionField = libgmsec_python.StringField("MISSION-ID", "MISSION")
        # Note: SAT-ID-PHYSICAL is an optional header Field, according

        satIdField = libgmsec_python.StringField("SAT-ID-PHYSICAL", "SPACECRAFT")
        facilityField = libgmsec_python.StringField("FACILITY", "GMSEC Lab")
        componentField = libgmsec_python.StringField("COMPONENT", "product_message")

        definedFields.append(missionField)
        definedFields.append(satIdField)
        definedFields.append(facilityField)
        definedFields.append(componentField)

        # Use setStandardFields to define a set of header fields for
        # all messages which are created or published on the
        # ConnectionManager using the following functions:
        # createLogMessage, publishLog, createHeartbeatMessage,
        # startHeartbeatService, createResourceMessage, 
        # publishResourceMessage, or startResourceMessageService
        connManager.setStandardFields(definedFields)

        # Create a ProductFile object with the product name,
        # description, version, file format, and the URI
        externalFile = libgmsec_python.ProductFile("External File", "External File Description", "1.0.0", "TXT", "//hostname/dir/filename")

        filePayload = bytearray()
    
        for i in range(0, 256):
            filePayload.append(i)

        # Create a ProductFile object with the product name,
        # description, version, format, binary array, and file size
        binaryFile = libgmsec_python.ProductFile("File as Binary", "Binary File Description", "1.0.0", "BIN", filePayload, len(filePayload))

        # Create a Product File Message with the subject,
        # RESPONSE-STATUS Field value, Message type (publish, request,
        # or reply), PROD-TYPE Field value, PROD-SUBTYPE Field value,
        # and pass it the Specification object from the Connection
        # Manager
        productMessage = libgmsec_python.ProductFileMessage(PROD_MESSAGE_SUBJECT, libgmsec_python.ResponseStatus.SUCCESSFUL_COMPLETION, libgmsec_python.Message.PUBLISH, "AUTO", "DM", connManager.getSpecification())
        productMessage.addProductFile(externalFile)
        productMessage.addProductFile(binaryFile)

        connManager.addStandardFields(productMessage)

        libgmsec_python.logInfo("Publishing DEV message:\n" + productMessage.toXML())
        connManager.publish(productMessage)
        
    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())
        return -1

    return 0



def initializeLogging(config):

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
       
