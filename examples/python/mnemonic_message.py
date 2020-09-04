#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file mnemonic_message.py
 
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
MVAL_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.MVAL.MNEMONIC_MESSAGE"


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>" 
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
        # to the GMSEC ISD.
        satIdField = libgmsec_python.StringField("SAT-ID-PHYSICAL", "SPACECRAFT")
        facilityField = libgmsec_python.StringField("FACILITY", "GMSEC Lab")
        componentField = libgmsec_python.StringField("COMPONENT", "mnemonic_message")

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

        # Populate the Mnemonic Sample(s)
        mSample = libgmsec_python.MnemonicSample("MS1", libgmsec_python.I32Field("MS1", 15))
        mSample.setEUValue(libgmsec_python.F32Field("My EU", 15.0))
        mSample.setFlags(1)
        mSample.setLimit(libgmsec_python.MnemonicSample.RED_HIGH)
        # Implicitly set limit enable/disable with setting of limit
        mSample.setQuality(True)
        mSample.setStalenessStatus(False)
        mSample.setTextValue("15")

        mnemonic_samples = libgmsec_python.MnemonicSampleList() 
        mnemonic_samples.append(mSample)

        # Add the Mnemonic values to a Mnemonic object
        mnemonic = libgmsec_python.Mnemonic("M1", mnemonic_samples)
        statusField = libgmsec_python.I16Field("status", 5)
        mnemonic.setStatus(statusField)
        mnemonic.setUnits("units")

        # Build up the Schema ID -- This is used to identify the
        # specific type of MVAL message to construct
                
        schemaId = GMSEC_SPEC_VERSION + ".GMSEC.MSG.MVAL"

        # Construct an MVAL Message and add the Mnemonic values to it
        mvalMessage = libgmsec_python.MnemonicMessage(MVAL_MESSAGE_SUBJECT, schemaId, connManager.getSpecification())
        mvalMessage.addMnemonic(mnemonic)

        # Add the header fields to the MVAL message
        connManager.addStandardFields(mvalMessage)

        libgmsec_python.logInfo("Publishing MVAL message:\n" + mvalMessage.toXML())
        connManager.publish(mvalMessage)
        
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
        
