#!/usr/bin/env ruby

# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# Demonstrate reading the contents of a ConfigFile object from an XML configuration file.
#
require 'libgmsec_ruby'

class ConfigFileEx

    #
    # Constructor, provide the file name of interest. Open the test file. A GmsecException may result.
    #
    # param configFilename the filename.  All filename checking is done by the API.
    #
    def initialize(config_filename)
        @config_filename = config_filename

        @config_file = Libgmsec_ruby::ConfigFile.new
        @config_file.load(@config_filename)
        puts "Using config file --> #{config_filename}"
    end


    #
    # Print the named subscription entry contents or print that it does not exist, or there is an error.
    # @param subscriptionName a String
    #
    def show_subscription(subscription_name)
        sub = @config_file.lookup_subscription_entry(subscription_name)
        puts "Subscription <" + subscription_name + ">:"
        puts "  -> name: " + sub.get_name() + " -> pattern: " + sub.get_pattern()

        # if a subscription has an exclude pattern, it can be iterated through like this
        while sub.has_next_excluded_pattern()
            exclude = sub.next_excluded_pattern()
            puts "     -> exclude: " + exclude
        end
        puts
    end


    #
    # Print the named config contents or print that it does not exist, or there is an error.
    # @param configName a String
    #
    def show_config(config_name)
        config = @config_file.lookup_config(config_name)
        puts "Config <" + config_name + ">:"

        # the way to get the guts of the config is using getFirst/getNext
        cp = Libgmsec_ruby::ConfigPair.new
        result = config.get_first(cp)
        while result == true
            puts "  -> key: " + cp.get_name() + " -> value: " + cp.get_value()
            result = config.get_next(cp)
        end

        # but individual values may be found also
        puts
        mwid = config.get_value("mw-id")
        if mwid != nil
            puts "  -> find 'mw-id': " + config.get_value("mw-id")
        else
            puts "  -> 'mw-id'is not defined in this config."
        end

        # or the entire thing can be printed as XML (or JSON)
        puts
        puts config.to_xml()
        puts
    end


    #
    # Print the named message contents or print that it does not exist, or there is an error.
    # param configName a String
    #
    def show_message(message_name)
        msg = @config_file.lookup_message(message_name)
        puts "Message <" + message_name + ">:"

        # there are number getters such as this one
        puts "  -> find 'STRING-FIELD': " + msg.get_string_field("STRING-FIELD").get_string_value()
        puts

        # or the entire thing can be printed as XML (or JSON)
        puts msg.to_xml()
        puts
    end
end


#
# main
#
if ARGV.length < 1
    puts "Usage: Provide an example XML file of interest.\n";
    exit 1
end

begin
    demo = ConfigFileEx.new ARGV[0]

    demo.show_subscription "all-messages"
    demo.show_subscription "custom1"

    demo.show_config "config1"
    demo.show_config "config2"

    demo.show_message "msg1"
    demo.show_message "msg2"
rescue GmsecException => e
    puts "ConfigFileEx failed [" + @config_filename + "] error -- " + e.message
    exit 1
end
