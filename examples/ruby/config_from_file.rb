#!/usr/bin/env ruby

# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file config_from_file.rb
# 
# @brief This example program provides a basic demonstration of creating
# a Config object using the XML contents of a file that represent a
# connection (or message) configuration.
#
# A configuration file should have contents using a format similar to:
#
# <CONFIG>
#     <PARAMETER NAME="mw-id">activemq39</PARAMETER>
#     <PARAMETER NAME="server">tcp://my-server:61616</PARAMETER>
# </CONFIG>
#
# See config.xml for a simple example.
#

require 'libgmsec_ruby'
require './logger'


class ConfigFromFile
    def initialize(args)
        initialize_log_level()

        # Initialize log handler
        @logHandler = CustomLogHandler.new
        Libgmsec_ruby::Log.register_handler(@logHandler)

        @config1 = Libgmsec_ruby::Config::get_from_file( "config.xml", "Bolt" )
        @config2 = Libgmsec_ruby::Config::get_from_file( "config.xml", "ActiveMQ" )
        @config3 = Libgmsec_ruby::Config::get_from_file( "config.xml", nil )
    end


    def initialize_log_level()
        Libgmsec_ruby::Log::set_reporting_level(Libgmsec_ruby::LogLevel_LogINFO)
    end


    def run()
        Libgmsec_ruby::Log::info("Config 1 is:\n" + @config1.to_xml());
        Libgmsec_ruby::Log::info("Config 2 is:\n" + @config2.to_xml());
        Libgmsec_ruby::Log::info("Config 3 is:\n" + @config3.to_xml());
    end
end


# Main entry point
begin
    ex = ConfigFromFile.new( ARGV )
    ex.run
rescue GmsecException => e
    Libgmsec_ruby::Log::error("Exception: #{e.message}")
end

