#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test

import os


class Test_ConfigFile(Test):

    def __init__(self):
        super().__init__()

        self.GOOD_CONFIG_FILE  = "good_config_file.xml"
        self.BAD_CONFIG_FILE_1 = "bad_config_file1.xml"
        self.BAD_CONFIG_FILE_2 = "bad_config_file2.xml"

        self.XML  = "<DEFINITIONS>\n"
        self.XML += "    <SUBSCRIPTION NAME=\"sub1\" PATTERN=\"GMSEC.&amp;gt\">\n"
        self.XML += "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.HB.+\"/>\n"
        self.XML += "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.LOG.+\"/>\n"
        self.XML += "    </SUBSCRIPTION>\n"
        self.XML += "    <CONFIG NAME=\"Bolt\">\n"
        self.XML += "        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
        self.XML += "        <PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
        self.XML += "    </CONFIG>\n"
        self.XML += "    <MESSAGE NAME=\"Msg1\">\n"
        self.XML += "        <CONFIG NAME=\"msg_config\">\n"
        self.XML += "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-SIZE\">15</PARAMETER>\n"
        self.XML += "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-TYPE\">tree</PARAMETER>\n"
        self.XML += "        </CONFIG>\n"
        self.XML += "        <FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n"
        self.XML += "        <FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n"
        self.XML += "        <FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MY-MISSION</FIELD>\n"
        self.XML += "        <FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">MY-CONSTELLATION</FIELD>\n"
        self.XML += "    </MESSAGE>\n"
        self.XML += "</DEFINITIONS>"


    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_load()
        self.test_save()
        self.test_from_xml()
        self.test_to_xml()
        self.test_is_loaded()
        self.test_add_config()
        self.test_lookup_config()
        self.test_remove_config()
        self.test_add_message()
        self.test_lookup_message()
        self.test_remove_message()
        self.test_add_subscription_entry()
        self.test_lookup_subscription_entry()
        self.test_remove_subscription()
        self.test_get_iterator()


    def test_constructor_1(self):
        lp.log_info("Test constructor (no args)")

        cfgFile = lp.ConfigFile()
        self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)


    def test_constructor_2(self):
        lp.log_info("Test constructor (Config)")

        try:
            config  = lp.Config()
            cfgFile = lp.ConfigFile( config )
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

            cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )
            self.check("Expected is_loaded to report False", cfgFile.is_loaded())

            msg = cfgFile.lookup_message("Msg1")
            self.check("Expected C2MS specification", msg.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_0)
        except Exception as e:
            self.check(str(e), False)

        try:
            config  = lp.Config("gmsec-schema-level=2", lp.DataType_KEY_VALUE_DATA)
            cfgFile = lp.ConfigFile( config )
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

            cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )
            self.check("Expected is_loaded to report False", cfgFile.is_loaded())

            msg = cfgFile.lookup_message("Msg1")
            self.check("Expected GMSEC specification", msg.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_2)
        except Exception as e:
            self.check(str(e), False)


    def test_load(self):
        lp.log_info("Test load()")

        cfgFile = lp.ConfigFile()

        # Nominal test
        cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )
        self.check("Expected is_loaded to report False", cfgFile.is_loaded())

        # Off-nominal test
        try:
            cfgFile.load(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Invalid or missing config file path" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

        try:
            cfgFile.load("nonexistent.xml")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Error: File not found" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

        try:
            cfgFile.load( self.getDataFile(self.BAD_CONFIG_FILE_1) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Invalid XML document - missing DEFINITIONS" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

        try:
            cfgFile.load( self.getDataFile(self.BAD_CONFIG_FILE_2) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Error: Mismatched element" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)


    def test_save(self):
        lp.log_info("Test save()")

        cfgFile = lp.ConfigFile()
        cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )

        cfgFile.save("new_config_file.xml")

        cfgFile2 = lp.ConfigFile()
        cfgFile2.load("new_config_file.xml")

        self.check("Unexpected XML content", cfgFile.to_xml() == cfgFile2.to_xml())

        os.remove("new_config_file.xml")


    def test_from_xml(self):
        lp.log_info("Test from_xml()")

        cfgFile = lp.ConfigFile()

        # Nominal case
        cfgFile.from_xml(self.XML)
        self.check("Expected is_loaded() to report true", cfgFile.is_loaded())

        # Off-nominal cases

        # None content
        try:
            cfgFile.from_xml(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Invalid XML string" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

        # empty-string content
        try:
            cfgFile.from_xml("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Invalid XML string" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

        # invalid content
        try:
            cfgFile.from_xml("<FOOBAR/>")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Invalid XML document - missing DEFINITIONS" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

        # incomplete content
        try:
            cfgFile.from_xml("<DEFINITIONS><SUBSCRIPTION NAME=\"bad xml\" PATTERN=\"GMSEC.>\">")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Unable to parse XML string" in str(e))
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)


    def test_to_xml(self):
        lp.log_info("Test to_xml()")

        cfgFile = lp.ConfigFile()

        cfgFile.from_xml(self.XML)

        self.check("Unexpected XML content from config file", cfgFile.to_xml() == self.XML)


    def test_is_loaded(self):
        lp.log_info("Test is_loaded()")

        cfgFile = lp.ConfigFile()
        self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)

        cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )
        self.check("Expected is_loaded() to report true", cfgFile.is_loaded())

        try:
            cfgFile.load( self.getDataFile(self.BAD_CONFIG_FILE_1) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check("Expected is_loaded() to report False", cfgFile.is_loaded() == False)


    def test_add_config(self):
        lp.log_info("Test add_config()")

        cfgFile = lp.ConfigFile()

        # Nominal test
        config = lp.Config("name1=value1", lp.DataType_KEY_VALUE_DATA)

        cfgFile.add_config("Cfg1", config)

        tmp = cfgFile.lookup_config("Cfg1")

        self.check("Unexpected config value", config.get_value("name1") == tmp.get_value("name1"))

        # Off-nominal tests

        # null config name
        try:
            cfgFile.add_config(None, config)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Configuration name cannot be NULL, nor an empty string" in str(e))

        # empty-string config name
        try:
            cfgFile.add_config("", config)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Configuration name cannot be NULL, nor an empty string" in str(e))


    def test_lookup_config(self):
        lp.log_info("Test lookup_config()")

        cfgFile = lp.ConfigFile()

        # Nominal case
        cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )

        cfg = cfgFile.lookup_config("Bolt")
        self.check("Unexpected mw-id value", cfg.get_value("mw-id") == "bolt")
        self.check("Unexpected server value", cfg.get_value("server") == "localhost")

        cfg = cfgFile.lookup_config("ActiveMQ")
        self.check("Unexpected mw-id value", cfg.get_value("mw-id") == "activemq39")
        self.check("Unexpected server value", cfg.get_value("server") == "tcp://localhost:61616")

        # Off-nominal cases

        # bogus config name
        try:
            cfgFile.lookup_config("bogus")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Configuration does not exist for the name bogus" in str(e))

        # None config name
        try:
            cfgFile.lookup_config(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Configuration name is NULL, or is an empty string" in str(e))

        # empty-string config name
        try:
            cfgFile.lookup_config("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Configuration name is NULL, or is an empty string" in str(e))


    def test_remove_config(self):
        lp.log_info("Test remove_config()")

        cfgFile = lp.ConfigFile()

        cfgFile.add_config("Bolt", lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA))

        self.check("Expected to remove existing config", cfgFile.remove_config("Bolt"))
        self.check("Did not expect to remove config", cfgFile.remove_config("Bolt") == False)
        self.check("Did not expect to remove config", cfgFile.remove_config("bogus") == False)
        self.check("Did not expect to remove config", cfgFile.remove_config(None) == False)
        self.check("Did not expect to remove config", cfgFile.remove_config("") == False)


    def test_add_message(self):
        lp.log_info("Test add_message()")

        cfgFile = lp.ConfigFile()
        factory = lp.MessageFactory()

        self.set_standard_fields(factory)

        msg = factory.create_message("HB")
        msg.add_field( lp.StringField("FIELD1", "ONE") )
        msg.add_field( lp.StringField("FIELD2", "TWO") )

        # Nominal test
        cfgFile.add_message("my-msg", msg)

        tmp = cfgFile.lookup_message("my-msg")

        self.check("Unexpected message subject", tmp.get_subject() == msg.get_subject())
        self.check("Unexpected message kind", tmp.get_kind() == msg.get_kind())
        self.check("Unexpected message field count", tmp.get_field_count() == msg.get_field_count())
        self.check("Unexpected field value", tmp.get_string_value("FIELD1") == "ONE")
        self.check("Unexpected field value", tmp.get_string_value("FIELD2") == "TWO")
        self.check("Unexpected XML content", tmp.to_xml() == msg.to_xml())
        self.check("Unexpected JSON content", tmp.to_json() == msg.to_json())

        # Off-nominal tests

        # None message name
        try:
            cfgFile.add_message(None, msg)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message name cannot be NULL, nor an empty string" in str(e))

        # empty-string message name
        try:
            cfgFile.add_message("", msg)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message name cannot be NULL, nor an empty string" in str(e))


    def test_lookup_message(self):
        lp.log_info("Test lookup_message()")

        cfgFile = lp.ConfigFile()

        # Nominal case
        cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )

        msg = cfgFile.lookup_message("Msg1")
        self.check("Unexpected MESSAGE-TYPE", msg.get_string_value("MESSAGE-TYPE") == "MSG")
        self.check("Unexpected MESSAGE-SUBTYPE", msg.get_string_value("MESSAGE-SUBTYPE") == "HB")

        # Off-nominal cases

        # bogus message name
        try:
            cfgFile.lookup_message("bogus")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not exist for the name bogus" in str(e))

        # None message name
        try:
            cfgFile.lookup_message(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message name cannot be NULL, nor an empty string" in str(e))

        # empty-string message name
        try:
            cfgFile.lookup_message("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message name cannot be NULL, nor an empty string" in str(e))


    def test_remove_message(self):
        lp.log_info("Test remove_message()")

        cfgFile = lp.ConfigFile()

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        cfgFile.add_message("Heartbeat", msgFactory.create_message("HB"))

        self.check("Expected to remove existing message", cfgFile.remove_message("Heartbeat"))
        self.check("Did not expect to remove message", cfgFile.remove_message("Heartbeat") == False)
        self.check("Did not expect to remove message", cfgFile.remove_message("bogus") == False)
        self.check("Did not expect to remove message", cfgFile.remove_message(None) == False)
        self.check("Did not expect to remove message", cfgFile.remove_message("") == False)


    def test_add_subscription_entry(self):
        lp.log_info("Test add_subscription_entry()")

        cfgFile = lp.ConfigFile()

        # Nominal case
        entry = lp.SubscriptionEntry("Sub1", "GMSEC.>")
        entry.add_excluded_pattern("GMSEC.*.*.MSG.HB.>")

        cfgFile.add_subscription_entry(entry)

        entry = cfgFile.lookup_subscription_entry("Sub1")

        self.check("Unexpected subscription entry name", entry.get_name() == "Sub1")
        self.check("Unexpected subscription entry pattern", entry.get_pattern() == "GMSEC.>")
        self.check("Expected an excluded pattern", entry.has_next_excluded_pattern() == True)
        self.check("Unexpected excluded pattern", entry.next_excluded_pattern() == "GMSEC.*.*.MSG.HB.>")
        self.check("Expected no additional excluded patterns", entry.has_next_excluded_pattern() == False)


    def test_lookup_subscription_entry(self):
        lp.log_info("Test lookup_subscription_entry()")

        cfgFile = lp.ConfigFile()

        # Nominal case
        cfgFile.load( self.getDataFile(self.GOOD_CONFIG_FILE) )

        entry = cfgFile.lookup_subscription_entry("Sub1")

        self.check("Unexpected subscription entry name", entry.get_name() == "Sub1")
        self.check("Unexpected subscription entry pattern", entry.get_pattern() == "GMSEC.>")

        # Off-nominal cases

        # bogus subscription name
        try:
            cfgFile.lookup_subscription_entry("bogus")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "SubscriptionEntry does not exist for the name bogus" in str(e))

        # None subscription name
        try:
            cfgFile.lookup_subscription_entry(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "SubscriptionEntry name cannot be NULL, nor an empty string" in str(e))

        # empty-string subscription name
        try:
            cfgFile.lookup_subscription_entry("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "SubscriptionEntry name cannot be NULL, nor an empty string" in str(e))


    def test_remove_subscription(self):
        lp.log_info("Test remove_subscription()")

        cfgFile = lp.ConfigFile()

        cfgFile.add_subscription_entry( lp.SubscriptionEntry("AllMessages", "GMSEC.>") )

        self.check("Expected to remove existing subscription", cfgFile.remove_subscription_entry("AllMessages"))
        self.check("Did not expect to remove subscription", cfgFile.remove_subscription_entry("AllMessages") == False)
        self.check("Did not expect to remove subscription", cfgFile.remove_subscription_entry("bogus") == False)
        self.check("Did not expect to remove subscription", cfgFile.remove_subscription_entry(None) == False)
        self.check("Did not expect to remove subscription", cfgFile.remove_subscription_entry("") == False)


    def test_get_iterator(self):
        lp.log_info("Test get_iterator()")

        cfgFile = lp.ConfigFile()

        cfgFile.add_subscription_entry( lp.SubscriptionEntry("AllMessages", "GMSEC.>") )
        cfgFile.add_subscription_entry( lp.SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") )

        cfgFile.add_config("Bolt", lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA))
        cfgFile.add_config("ActiveMQ", lp.Config("mw-id=activemq39 server=tcp://localhost:61616", lp.DataType_KEY_VALUE_DATA))

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        cfgFile.add_message("Heartbeat", msgFactory.create_message("HB"))
        cfgFile.add_message("Log", msgFactory.create_message("LOG"))

        iter = cfgFile.get_iterator()

        numSubscriptions = 0
        numConfigs = 0
        numMessages = 0

        while iter.has_next_subscription():
            iter.next_subscription()
            numSubscriptions += 1

        while iter.has_next_config():
            iter.next_config()
            numConfigs += 1

        while iter.has_next_message():
            iter.next_message()
            numMessages += 1

        self.check("Unexpected number of subscriptions", numSubscriptions == 2)
        self.check("Unexpected number of configs", numConfigs == 2)
        self.check("Unexpected number of messages", numMessages == 2)


Test.run('Test ConfigFile', Test_ConfigFile())
