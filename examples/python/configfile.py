#!/usr/bin/env python


"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file configfile.py
 
 A Python example demonstration of loading pre-defined subscriptions, Config
 objects, GMSEC Messages, and custom XML blocks from an xml file.  The
 configuration file used for this example program is "configfile.xml".
"""

import libgmsec_python
import sys



def main(argv=None):

    if(len(sys.argv) <=1):        
        usageMessage = ("\nUsage: configfile <filename>\n")
        print usageMessage
        return -1


    try:
        initializeLogging()

        # load configuration file
        cfgFile = libgmsec_python.ConfigFile()
        cfgFilename = sys.argv[1]
        cfgFile.load(cfgFilename)

        # retrieve config objects from the Config file
        c1 = cfgFile.lookupConfig("config1")
        c2 = cfgFile.lookupConfig("config2")

        # Display log of XML representation of Config objects
        libgmsec_python.logInfo("Config 1:\n" + c1.toXML())
        libgmsec_python.logInfo("Config 2:\n" + c2.toXML())


        # lookup subscription topic from configuration file, including excluded topics
        subEntry = cfgFile.lookupSubscriptionEntry("events")

        libgmsec_python.logInfo("Subscription pattern: " + subEntry.getPattern())

        while (subEntry.hasNextExcludedPattern()):
            libgmsec_python.logInfo("Subscription excluded pattern: " + subEntry.nextExcludedPattern())


        # lookup a Message from the configuration file
        message = cfgFile.lookupMessage("msg1")

        # Display XML representation of the message
        libgmsec_python.logInfo("Message:\n" + message.toXML())

        # Obtain ConfigFile Iterator to examine all of the various
        # entries defined in the ConfigFile
        iterator = cfgFile.getIterator()

        # Acquire and display all Config entries
        while (iterator.hasNextConfig()):
            entry = iterator.nextConfig()

            libgmsec_python.logInfo("\nConfig Name: " + entry.getName() + "\nConfig:\n" + entry.getConfig().toXML())
            
        # Acquire and display all Message entries
        while (iterator.hasNextMessage()):
            entry = iterator.nextMessage()

            libgmsec_python.logInfo("\nMessage Name: " + entry.getName() + "\nMessage:\n" + entry.getMessage().toXML())
                

        # Acquire and display all subscription entries
        while (iterator.hasNextSubscription()):
            entry = iterator.nextSubscription()

            libgmsec_python.logInfo("\nSubscription Name: " + entry.getName() + "\nSubscription Topic: " + entry.getPattern())
            
            while (entry.hasNextExcludedPattern()):
                libgmsec_python.logInfo("\nExcluded Pattern: " + entry.nextExcludedPattern())                

        # Acquire and display all custom XML entries
        while (iterator.hasNextCustomElement()):
            element = iterator.nextCustomElement()

            libgmsec_python.logInfo("\nCustom XML Element:\n" + element)
                
        
    except libgmsec_python.Exception as e:
        libgmsec_python.logError(e.what())
        return -1
        

    return 0



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

