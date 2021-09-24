#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


""" 
 @file configfile.py
 
 A Python example demonstration of loading pre-defined subscriptions, Config
 objects, GMSEC Messages, and custom XML blocks from an xml file.  The
 configuration file used for this example program is "configfile.xml".
"""

import libgmsec_python3
import sys



def main(argv=None):

    if(len(sys.argv) <=1):        
        usageMessage = ("\nUsage: configfile <filename>\n")
        print(usageMessage)
        return -1


    try:
        initializeLogging()

        # load configuration file
        cfgFile = libgmsec_python3.ConfigFile()
        cfgFilename = sys.argv[1]
        cfgFile.load(cfgFilename)

        # retrieve config objects from the Config file
        c1 = cfgFile.lookup_config("config1")
        c2 = cfgFile.lookup_config("config2")

        # Display log of XML representation of Config objects
        libgmsec_python3.log_info("Config 1:\n" + c1.to_XML())
        libgmsec_python3.log_info("Config 2:\n" + c2.to_XML())


        # lookup subscription topic from configuration file, including excluded topics
        subEntry = cfgFile.lookup_subscription_entry("events")

        libgmsec_python3.log_info("Subscription pattern: " + subEntry.get_pattern())

        while (subEntry.has_next_excluded_pattern()):
            libgmsec_python3.log_info("Subscription excluded pattern: " + subEntry.next_excluded_pattern())


        # lookup a Message from the configuration file
        message = cfgFile.lookup_message("msg1")

        # Display XML representation of the message
        libgmsec_python3.log_info("Message:\n" + message.to_XML())

        # Obtain ConfigFile Iterator to examine all of the various
        # entries defined in the ConfigFile
        iterator = cfgFile.get_iterator()

        # Acquire and display all Config entries
        while (iterator.has_next_config()):
            entry = iterator.next_config()

            libgmsec_python3.log_info("\nConfig Name: " + entry.get_name() + "\nConfig:\n" + entry.get_config().to_XML())
            
        # Acquire and display all Message entries
        while (iterator.has_next_message()):
            entry = iterator.next_message()

            libgmsec_python3.log_info("\nMessage Name: " + entry.get_name() + "\nMessage:\n" + entry.get_message().to_XML())
                

        # Acquire and display all subscription entries
        while (iterator.has_next_subscription()):
            entry = iterator.next_subscription()

            libgmsec_python3.log_info("\nSubscription Name: " + entry.get_name() + "\nSubscription Topic: " + entry.get_pattern())
            
            while (entry.has_next_excluded_pattern()):
                libgmsec_python3.log_info("\nExcluded Pattern: " + entry.next_excluded_pattern())                

        # Acquire and display all custom XML entries
        while (iterator.has_next_custom_element()):
            element = iterator.next_custom_element()

            libgmsec_python3.log_info("\nCustom XML Element:\n" + element)
                
        
    except libgmsec_python3.GmsecError as e:
        libgmsec_python3.log_error(str(e))
        return -1
        

    return 0



def initializeLogging():
        
    # Note: When the LOGLEVEL and LOGFILE parameters are set in a Config
    # object, it affects the logging level and output destination for all
    # logging macros in the entire API, regardless of whether that Config
    # object is kept around or not. In this case, we are creating a local
    # Config object just for the sake of initializing logging.
    config = libgmsec_python3.Config()

    logLevel  = config.get_value("LOGLEVEL")
    logFile   = config.get_value("LOGFILE")

    if (not logLevel):
            config.add_value("LOGLEVEL", "INFO")
        
    if (not logFile):
            config.add_value("LOGFILE", "STDERR")
        


#
# Main entry point of script
#
if __name__=="__main__":
    sys.exit(main())

