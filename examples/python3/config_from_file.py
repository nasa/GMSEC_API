#!/usr/bin/env python3

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

"""
 @file config_from_file.py
 
 @brief This example program provides a basic demonstration of creating
 a Config object using the XML contents of a file that represent a
 connection (or message) configuration.

 A configuration file should have contents using a format similar to:

 <CONFIG>
     <PARAMETER NAME="mw-id">activemq39</PARAMETER>
     <PARAMETER NAME="server">tcp://my-server:61616</PARAMETER>
 </CONFIG>

 See config.xml for a simple example.
"""


import libgmsec_python3 as lp
import sys


class config_from_file:
    def __init__(self):
        lp.Log.set_reporting_level(lp.LogLevel_logINFO)
        self.config1 = lp.Config.get_from_file( "config.xml", "Bolt" )
        self.config2 = lp.Config.get_from_file( "config.xml", "ActiveMQ" )
        self.config3 = lp.Config.get_from_file( "config.xml", None )

    def run(self):
        lp.log_info("Config 1 is:\n" + self.config1.to_xml())
        lp.log_info("Config 2 is:\n" + self.config2.to_xml())
        lp.log_info("Config 3 is:\n" + self.config3.to_xml())


# Main entry point
#
try:
    ex = config_from_file()
    ex.run()
except lp.GmsecError as e:
    lp.log_error("Exception: " + str(e));
