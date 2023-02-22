#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_ConfigFile < Test

	def initialize(args)
		super(args)

        @GOOD_CONFIG_FILE  = "good_config_file.xml"
        @BAD_CONFIG_FILE_1 = "bad_config_file1.xml"
        @BAD_CONFIG_FILE_2 = "bad_config_file2.xml"

        @XML  = "<DEFINITIONS>\n"
        @XML += "    <SUBSCRIPTION NAME=\"sub1\" PATTERN=\"GMSEC.&amp;gt\">\n"
        @XML += "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.HB.+\"/>\n"
        @XML += "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.LOG.+\"/>\n"
        @XML += "    </SUBSCRIPTION>\n"
        @XML += "    <CONFIG NAME=\"Bolt\">\n"
        @XML += "        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
        @XML += "        <PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
        @XML += "    </CONFIG>\n"
        @XML += "    <MESSAGE NAME=\"Msg1\">\n"
        @XML += "        <CONFIG NAME=\"msg_config\">\n"
        @XML += "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-SIZE\">15</PARAMETER>\n"
        @XML += "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-TYPE\">tree</PARAMETER>\n"
        @XML += "        </CONFIG>\n"
        @XML += "        <FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n"
        @XML += "        <FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n"
        @XML += "        <FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MY-MISSION</FIELD>\n"
        @XML += "        <FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">MY-CONSTELLATION</FIELD>\n"
        @XML += "    </MESSAGE>\n"
        @XML += "</DEFINITIONS>"
	end


    def test()
        test_constructor_1()
        test_constructor_2()
        test_load()
        test_save()
        test_from_xml()
        test_to_xml()
        test_is_loaded()
        test_add_config()
        test_lookup_config()
        test_remove_config()
        test_add_message()
        test_lookup_message()
        test_remove_message()
        test_add_subscription_entry()
        test_lookup_subscription_entry()
        test_remove_subscription()
        test_get_iterator()
	end


    def test_constructor_1()
        Libgmsec_ruby::Log::info("Test constructor (no args)")

        cfgFile = Libgmsec_ruby::ConfigFile.new()
        check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
	end


    def test_constructor_2()
        Libgmsec_ruby::Log::info("Test constructor (Config)")

        begin
            cfgFile = Libgmsec_ruby::ConfigFile.new( Libgmsec_ruby::Config.new() )
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)

            cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )
            check("Expected is_loaded to report false", cfgFile.is_loaded())

            msg = cfgFile.lookup_message("Msg1")
            check("Expected C2MS specification", msg.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
        rescue GmsecException => e
            check(e.message, false)
		end

        begin
            cfgFile = Libgmsec_ruby::ConfigFile.new( Libgmsec_ruby::Config.new("gmsec-schema-level=2", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)

            cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )
            check("Expected is_loaded to report false", cfgFile.is_loaded())

            msg = cfgFile.lookup_message("Msg1")
            check("Expected GMSEC specification", msg.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2)
        rescue GmsecException => e
            check(e.message, false)
		end
	end


    def test_load()
        Libgmsec_ruby::Log::info("Test load()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        # Nominal test
        cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )
        check("Expected is_loaded to report false", cfgFile.is_loaded())

        # Off-nominal test
		begin
            cfgFile.load(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid or missing config file path"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end

		begin
            cfgFile.load("nonexistent.xml")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Error: File not found"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end

		begin
            cfgFile.load( get_data_file(@BAD_CONFIG_FILE_1) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid XML document - missing DEFINITIONS"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end

		begin
            cfgFile.load( get_data_file(@BAD_CONFIG_FILE_2) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Error: Mismatched element"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end
	end


    def test_save()
        Libgmsec_ruby::Log::info("Test save()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()
        cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )

        cfgFile.save("new_config_file.xml")

        cfgFile2 = Libgmsec_ruby::ConfigFile.new()
        cfgFile2.load("new_config_file.xml")

        check("Unexpected XML content", cfgFile.to_xml() == cfgFile2.to_xml())

		begin
        	File.delete("new_config_file.xml")
		rescue Errno::ENOENT
		end
	end


    def test_from_xml()
        Libgmsec_ruby::Log::info("Test from_xml()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        # Nominal case
        cfgFile.from_xml(@XML)
        check("Expected is_loaded() to report true", cfgFile.is_loaded())

        # Off-nominal cases

        # nil content
		begin
            cfgFile.from_xml(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid XML string"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end

        # empty-string content
		begin
            cfgFile.from_xml("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid XML string"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end

        # invalid content
		begin
            cfgFile.from_xml("<FOOBAR/>")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid XML document - missing DEFINITIONS"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end

        # incomplete content
		begin
            cfgFile.from_xml("<DEFINITIONS><SUBSCRIPTION NAME=\"bad xml\" PATTERN=\"GMSEC.>\">")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Unable to parse XML string"))
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end
	end


    def test_to_xml()
        Libgmsec_ruby::Log::info("Test to_xml()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        cfgFile.from_xml(@XML)

        check("Unexpected XML content from config file", cfgFile.to_xml() == @XML)
	end


    def test_is_loaded()
        Libgmsec_ruby::Log::info("Test is_loaded()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()
        check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)

        cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )
        check("Expected is_loaded() to report true", cfgFile.is_loaded())

		begin
            cfgFile.load( get_data_file(@BAD_CONFIG_FILE_1) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check("Expected is_loaded() to report false", cfgFile.is_loaded() == false)
		end
	end


    def test_add_config()
        Libgmsec_ruby::Log::info("Test add_config()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        # Nominal test
        config = Libgmsec_ruby::Config.new("name1=value1", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        cfgFile.add_config("Cfg1", config)

        tmp = cfgFile.lookup_config("Cfg1")

        check("Unexpected config value", config.get_value("name1") == tmp.get_value("name1"))

        # Off-nominal tests

        # null config name
		begin
            cfgFile.add_config(nil, config)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Configuration name cannot be NULL, nor an empty string"))
		end

        # empty-string config name
		begin
            cfgFile.add_config("", config)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Configuration name cannot be NULL, nor an empty string"))
		end
	end


    def test_lookup_config()
        Libgmsec_ruby::Log::info("Test lookup_config()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        # Nominal case
        cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )

        cfg = cfgFile.lookup_config("Bolt")
        check("Unexpected mw-id value", cfg.get_value("mw-id") == "bolt")
        check("Unexpected server value", cfg.get_value("server") == "localhost")

        cfg = cfgFile.lookup_config("ActiveMQ")
        check("Unexpected mw-id value", cfg.get_value("mw-id") == "activemq39")
        check("Unexpected server value", cfg.get_value("server") == "tcp://localhost:61616")

        # Off-nominal cases

        # bogus config name
		begin
            cfgFile.lookup_config("bogus")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Configuration does not exist for the name bogus"))
		end

        # nil config name
		begin
            cfgFile.lookup_config(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Configuration name is NULL, or is an empty string"))
		end

        # empty-string config name
		begin
            cfgFile.lookup_config("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Configuration name is NULL, or is an empty string"))
		end
	end


    def test_remove_config()
        Libgmsec_ruby::Log::info("Test remove_config()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        cfgFile.add_config("Bolt", Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA))

        check("Expected to remove existing config", cfgFile.remove_config("Bolt"))
        check("Did not expect to remove config", cfgFile.remove_config("Bolt") == false)
        check("Did not expect to remove config", cfgFile.remove_config("bogus") == false)
        check("Did not expect to remove config", cfgFile.remove_config(nil) == false)
        check("Did not expect to remove config", cfgFile.remove_config("") == false)
	end


    def test_add_message()
        Libgmsec_ruby::Log::info("Test add_message()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()
        factory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(factory)

        msg = factory.create_message("HB")
        msg.add_field( Libgmsec_ruby::StringField.new("FIELD1", "ONE") )
        msg.add_field( Libgmsec_ruby::StringField.new("FIELD2", "TWO") )

        Libgmsec_ruby::MessageFactory::destroy(factory)

        # Nominal test
        cfgFile.add_message("my-msg", msg)

        tmp = cfgFile.lookup_message("my-msg")

        check("Unexpected message subject", tmp.get_subject() == msg.get_subject())
        check("Unexpected message kind", tmp.get_kind() == msg.get_kind())
        check("Unexpected message field count", tmp.get_field_count() == msg.get_field_count())
        check("Unexpected field value", tmp.get_string_value("FIELD1") == "ONE")
        check("Unexpected field value", tmp.get_string_value("FIELD2") == "TWO")
        check("Unexpected XML content", tmp.to_xml() == msg.to_xml())
        check("Unexpected JSON content", tmp.to_json() == msg.to_json())

        # Off-nominal tests

        # nil message name
		begin
            cfgFile.add_message(nil, msg)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message name cannot be NULL, nor an empty string"))
		end

        # empty-string message name
		begin
            cfgFile.add_message("", msg)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message name cannot be NULL, nor an empty string"))
		end
	end


    def test_lookup_message()
        Libgmsec_ruby::Log::info("Test lookup_message()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        # Nominal case
        cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )

        msg = cfgFile.lookup_message("Msg1")
        check("Unexpected MESSAGE-TYPE", msg.get_string_value("MESSAGE-TYPE") == "MSG")
        check("Unexpected MESSAGE-SUBTYPE", msg.get_string_value("MESSAGE-SUBTYPE") == "HB")

        # Off-nominal cases

        # bogus message name
		begin
            cfgFile.lookup_message("bogus")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not exist for the name bogus"))
		end

        # nil message name
		begin
            cfgFile.lookup_message(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message name cannot be NULL, nor an empty string"))
		end

        # empty-string message name
		begin
            cfgFile.lookup_message("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message name cannot be NULL, nor an empty string"))
		end
	end


    def test_remove_message()
        Libgmsec_ruby::Log::info("Test remove_message()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        cfgFile.add_message("Heartbeat", msgFactory.create_message("HB"))

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("Expected to remove existing message", cfgFile.remove_message("Heartbeat"))
        check("Did not expect to remove message", cfgFile.remove_message("Heartbeat") == false)
        check("Did not expect to remove message", cfgFile.remove_message("bogus") == false)
        check("Did not expect to remove message", cfgFile.remove_message(nil) == false)
        check("Did not expect to remove message", cfgFile.remove_message("") == false)
	end


    def test_add_subscription_entry()
        Libgmsec_ruby::Log::info("Test add_subscription_entry()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        # Nominal case
        entry = Libgmsec_ruby::SubscriptionEntry.new("Sub1", "GMSEC.>")
        entry.add_excluded_pattern("GMSEC.*.*.MSG.HB.>")

        cfgFile.add_subscription_entry(entry)

        entry = cfgFile.lookup_subscription_entry("Sub1")

        check("Unexpected subscription entry name", entry.get_name() == "Sub1")
        check("Unexpected subscription entry pattern", entry.get_pattern() == "GMSEC.>")
        check("Expected an excluded pattern", entry.has_next_excluded_pattern() == true)
        check("Unexpected excluded pattern", entry.next_excluded_pattern() == "GMSEC.*.*.MSG.HB.>")
        check("Expected no additional excluded patterns", entry.has_next_excluded_pattern() == false)
	end


    def test_lookup_subscription_entry()
        Libgmsec_ruby::Log::info("Test lookup_subscription_entry()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        # Nominal case
        cfgFile.load( get_data_file(@GOOD_CONFIG_FILE) )

        entry = cfgFile.lookup_subscription_entry("Sub1")

        check("Unexpected subscription entry name", entry.get_name() == "Sub1")
        check("Unexpected subscription entry pattern", entry.get_pattern() == "GMSEC.>")

        # Off-nominal cases

        # bogus subscription name
		begin
            cfgFile.lookup_subscription_entry("bogus")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("SubscriptionEntry does not exist for the name bogus"))
		end

        # nil subscription name
		begin
            cfgFile.lookup_subscription_entry(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("SubscriptionEntry name cannot be NULL, nor an empty string"))
		end

        # empty-string subscription name
		begin
            cfgFile.lookup_subscription_entry("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("SubscriptionEntry name cannot be NULL, nor an empty string"))
		end
	end


    def test_remove_subscription()
        Libgmsec_ruby::Log::info("Test remove_subscription()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        cfgFile.add_subscription_entry( Libgmsec_ruby::SubscriptionEntry.new("AllMessages", "GMSEC.>") )

        check("Expected to remove existing subscription", cfgFile.remove_subscription_entry("AllMessages"))
        check("Did not expect to remove subscription", cfgFile.remove_subscription_entry("AllMessages") == false)
        check("Did not expect to remove subscription", cfgFile.remove_subscription_entry("bogus") == false)
        check("Did not expect to remove subscription", cfgFile.remove_subscription_entry(nil) == false)
        check("Did not expect to remove subscription", cfgFile.remove_subscription_entry("") == false)
	end


    def test_get_iterator()
        Libgmsec_ruby::Log::info("Test get_iterator()")

        cfgFile = Libgmsec_ruby::ConfigFile.new()

        cfgFile.add_subscription_entry( Libgmsec_ruby::SubscriptionEntry.new("AllMessages", "GMSEC.>") )
        cfgFile.add_subscription_entry( Libgmsec_ruby::SubscriptionEntry.new("LogMessages", "GMSEC.*.*.MSG.LOG.+") )

        cfgFile.add_config("Bolt", Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA))
        cfgFile.add_config("ActiveMQ", Libgmsec_ruby::Config.new("mw-id=activemq39 server=tcp://localhost:61616", Libgmsec_ruby::DataType_KEY_VALUE_DATA))

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        cfgFile.add_message("Heartbeat", msgFactory.create_message("HB"))
        cfgFile.add_message("Log", msgFactory.create_message("LOG"))

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        iter = cfgFile.get_iterator()

        numSubscriptions = 0
        numConfigs = 0
        numMessages = 0

        while iter.has_next_subscription()
            iter.next_subscription()
            numSubscriptions += 1
		end

        while iter.has_next_config()
            iter.next_config()
            numConfigs += 1
		end

        while iter.has_next_message()
            iter.next_message()
            numMessages += 1
		end

        check("Unexpected number of subscriptions", numSubscriptions == 2)
        check("Unexpected number of configs", numConfigs == 2)
        check("Unexpected number of messages", numMessages == 2)
	end
end


begin
    test = Test_ConfigFile.new( ARGV )
    test.run
end
