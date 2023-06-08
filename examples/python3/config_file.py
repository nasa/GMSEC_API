#!/usr/bin/env python3

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

import libgmsec_python3
import sys


# 
#  Demonstrate reading the contents of a ConfigFile object from a config file xml.
#  
class config_file(object):

    # Constructor, provide the file name of interest. Open the test file. A GmsecError is possible.
    #       
    # @param configFilename the filename.  All filename checking is done by the API.
    #      
    def __init__(self, configFilename):
        self.configFileName = configFilename

        self.configFile = libgmsec_python3.ConfigFile()

        self.configFile.load(self.configFileName)
        print("Using config file --> " + configFilename)
       

    # Print the named subscription entry contents or print that it does not exist, or there is an error. A GmsecError is possible.
    # @param subscriptionName a String
    #      
    def showSubscription(self, subscriptionName):
        sub = self.configFile.lookup_subscription_entry(subscriptionName);
            
        print("Subscription <" + subscriptionName + ">:")
        print("  -> name: " + sub.get_name() + " -> pattern: " + sub.get_pattern())

        #  if a subscription has an exclude pattern, it can be iterated through like this
        while sub.has_next_excluded_pattern():
            exclude = sub.next_excluded_pattern()
            print("     -> exclude: " + exclude)

        print() 
       

  
    # Print the named config contents or print that it does not exist, or there is an error. A GmsecError is possible.
    # @param configName a String
    #      
    def showConfig(self, configName):
        config = self.configFile.lookup_config(configName);
            
        print("Config <" + configName + ">:")
            
        mwid1 = config.get_value("mw-id")
        print(mwid1);
        
        configPair = libgmsec_python3.ConfigPair();
        result = config.getFirst(configPair);
        while (result):
            print("  -> key: " + configPair.get_name() + " -> value: " + configPair.get_value())
            result = config.getNext(configPair)

        #  individual values may be found also
        print()
        mwid = config.get_value("mw-id");
        if mwid != None:
            print("  -> find 'mw-id': " + config.get_value("mw-id"))
        else:
            print("  -> 'mw-id'is not defined in this config.")

        #  or the entire thing can be printed as XML (or JSON)
        print() 
        print(config.to_xml())
        print() 


    # Print the named message contents or print that it does not exist, or there is an error. A GmsecError is possible.
    #      
    # @param configName a String
    #      
    def showMessage(self, messageName):
        #  the Message is a core gmsec type or class
        msg = self.configFile.lookup_message(messageName);
        print("Message <" + messageName + ">:")

        #  there are number getters such as this one
        print("  -> find 'STRING-FIELD': " + msg.get_string_field("STRING-FIELD").get_string_value())
        print() 

        #  or the entire thing can be printed as XML (or JSON)
        print(msg.to_xml())
        print() 
      

#
# Main entry point
#
if __name__ == "__main__":
    n = len(sys.argv)
    if n != 2:
        print("Usage: Provide an example XML file of interest.", file=sys.stderr)
        exit(1)

    try:
        demo = config_file(sys.argv[1])

        demo.showSubscription("all-messages")
        demo.showSubscription("custom1")

        demo.showConfig("config1")
        demo.showConfig("config2")

        demo.showMessage("msg1")
        demo.showMessage("msg2")
    except libgmsec_python3.GmsecError as e:
        print("config_file failed [" + sys.argv[0] + "] error -- " + str(e))
        exit(1)
