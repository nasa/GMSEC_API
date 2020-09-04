#!/usr/bin/env python3


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""

""" 
 @file validation_custom.py
 
 This file contains an example demonstrating how to implement additional
 Message validation logic in addition to that which the GMSEC API provides.

 Note: This example focuses on adding additional validation upon the receipt
 of a message.  It almost goes without saying that additional logic can be
 added to a program prior to invoking the publish() function without having
 to do anything special.
"""
import libgmsec_python3
import sys 

PROD_MESSAGE_SUBJECT = "GMSEC.MISSION.SATELLITE.MSG.PROD.PRODUCT_MESSAGE"


# Create a callback and define message content validation logic which will
# be used in combination with the GMSEC API validation.
class ValidationCallback(libgmsec_python3.ConnectionManagerCallback):

    def __init__(self):
        libgmsec_python3.ConnectionManagerCallback.__init__(self)

    def on_message(self, connMgr, message):
        try:
            # Run the message through the GMSEC API-supplied validation
            connMgr.get_specification().validate_message(message)

            # In this example scenario, we are expecting to receive a
            # GMSEC PROD message containing a URI to a location on the disk
            # where a product file has been placed for retrieval.  In this
            # case, we want to validate that the location on the disk is
            # in an area which we would expect (i.e. Something that the
            # team has agreed upon prior to operational deployment).
            #
            # By validating this URI, we ensure that no malicious users
            # have infiltrated the system and somehow modified the messages
            # to cause us to retrieve a file from an unknown location.

            # Start by checking to ensure that this is a PROD message
            if (isProdMsg(message)):
                prodMessage = libgmsec_python3.ProductFileMessage(connMgr.get_specification(), message.to_XML())

                # Get the iterator so that we can analyze each Product File within the message
                prodFileIter = prodMessage.get_product_file_iterator()

                # For each Product File available...
                while prodFileIter.has_next():

                    # Get the next Product File
                    prodFile = prodFileIter.next()

                    # Extract the URI from the Product File
                    prodUri = prodFile.get_URI()

                    # Check to ensure that the URI contains "//hostname/dir"; if not, report an error
                    if (prodUri.find("//hostname/dir") == -1):
                        errorMsg = "This error is expected; received an invalid PROD Message (bad URI):\n" + prodMessage.to_XML()
                        raise libgmsec_python3.GmsecError(libgmsec_python3.MIST_ERROR, libgmsec_python3.MESSAGE_FAILED_VALIDATION, errorMsg)
                             
                libgmsec_python3.log_info("Received a valid message:\n" + prodMessage.to_XML())
                        
        except libgmsec_python3.GmsecError as e:
            libgmsec_python3.log_error(str(e))


def main():

    if(len(sys.argv) <= 1):
        usageMessage = "usage: " + sys.argv[0] + " mw-id=<middleware ID>"
        print(usageMessage)
        return -1


    # Load the command-line input into a GMSEC Config object
    config = libgmsec_python3.Config(sys.argv)

    # If it was not specified in the command-line arguments, set LOGLEVEL
    # to 'INFO' and LOGFILE to 'stdout' to allow the program report output
    # on the terminal/command line
    initializeLogging(config);

    # Enable Message validation.  This parameter is "false" by default.
    config.add_value("GMSEC-MSG-CONTENT-VALIDATE", "true")

    libgmsec_python3.log_info(libgmsec_python3.ConnectionManager.get_API_version())

    try:
        connMgr = libgmsec_python3.ConnectionManager(config)

        libgmsec_python3.log_info("Opening the connection to the middleware server")
        connMgr.initialize()

        connMgr.get_library_version()

        # Set up the ValidationCallback and subscribe
        vc = ValidationCallback()
        connMgr.subscribe(PROD_MESSAGE_SUBJECT, vc)

        # Start the AutoDispatcher
        connMgr.start_auto_dispatch()

        # Create and publish a simple Product File Message
        setupStandardFields(connMgr)

        productMessage = createProductFileMessage(connMgr, "//hostname/dir/filename")

        # Publish the message to the middleware bus
        connMgr.publish(productMessage)

        productMessage = createProductFileMessage(connMgr, "//badhost/dir/filename")

        connMgr.publish(productMessage)

        # Disconnect from the middleware and clean up the Connection
        connMgr.cleanup()
        
    except libgmsec_python3.GmsecError as e:
        libgmsec_python3.log_error(str(e))
        return -1

    return 0


def initializeLogging(config):

    logLevel  = config.get_value("LOGLEVEL")
    logFile   = config.get_value("LOGFILE")

    if (not logLevel):
        config.add_value("LOGLEVEL", "INFO")

    if (not logFile):
        config.add_value("LOGFILE", "STDERR")


def setupStandardFields(connMgr):

    definedFields = libgmsec_python3.FieldList()

    missionField = libgmsec_python3.StringField("MISSION-ID", "MISSION")
    satIdField = libgmsec_python3.StringField("SAT-ID-PHYSICAL", "SPACECRAFT")
    facilityField = libgmsec_python3.StringField("FACILITY", "GMSEC Lab")
    componentField = libgmsec_python3.StringField("COMPONENT", "validation_custom")

    definedFields.append(missionField)
    definedFields.append(satIdField)
    definedFields.append(facilityField)
    definedFields.append(componentField)

    if (connMgr.get_specification().get_version() >= libgmsec_python3.GMSEC_ISD_2018_00):
        domain1 = libgmsec_python3.StringField("DOMAIN1", "MY-DOMAIN-1")
        domain2 = libgmsec_python3.StringField("DOMAIN2", "MY-DOMAIN-2")
        definedFields.append(domain1)
        definedFields.append(domain2)

    connMgr.set_standard_fields(definedFields)


def createProductFileMessage(connMgr, filePath):

    externalFile = libgmsec_python3.ProductFile("External File", "External File Description", "1.0.0", "TXT", filePath)

    if (connMgr.get_specification().get_version() <= libgmsec_python3.GMSEC_ISD_2016_00):
        productMessage = libgmsec_python3.ProductFileMessage(PROD_MESSAGE_SUBJECT, libgmsec_python3.ResponseStatus.SUCCESSFUL_COMPLETION, "MSG.PROD.AUTO", connMgr.get_specification())
    else:
        productMessage = libgmsec_python3.ProductFileMessage(PROD_MESSAGE_SUBJECT, libgmsec_python3.ResponseStatus.SUCCESSFUL_COMPLETION, "MSG.PROD", connMgr.get_specification())
        productMessage.add_field("PROD-TYPE", "AUTO");
        productMessage.add_field("PROD-SUBTYPE", "DM");

    productMessage.add_product_file(externalFile)

    connMgr.add_standard_fields(productMessage)

    return productMessage
    


def isProdMsg(message):

    result = (message.get_string_value("MESSAGE-TYPE") == "MSG" and message.get_string_value("MESSAGE-SUBTYPE") == "PROD") 
    return result



#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())


