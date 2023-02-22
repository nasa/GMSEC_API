#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_ConfigFileIterator(Test):

    def __init__(self):
        super().__init__()
        self.cfgFile    = None
        self.msgFactory = None


    def test(self):
        self.setup()

        self.test_iterator()
        self.test_reset()


    def test_iterator(self):
        iter = self.cfgFile.get_iterator()

        lp.log_info("Test has_next_subscription() and next_subscription()")
        numSubscriptions = 0
        while iter.has_next_subscription():
            entry = iter.next_subscription()

            numSubscriptions += 1

            if numSubscriptions == 1:
                self.check("Unexpected subscription entry 1 name", entry.get_name() == "AllMessages")
                self.check("Unexpected subscription entry 1 pattern", entry.get_pattern() == "GMSEC.>")
            else:
                self.check("Unexpected subscription entry 2 name", entry.get_name() == "LogMessages")
                self.check("Unexpected subscription entry 2 pattern", entry.get_pattern() == "GMSEC.*.*.MSG.LOG.+")

        self.check("Unexpected number of subscription entries", numSubscriptions == 2)


        lp.log_info("Test has_next_config() and next_config()")
        numConfigs = 0
        while iter.has_next_config():
            entry = iter.next_config()

            numConfigs += 1

            if numConfigs == 1:
                self.check("Unexpected config entry 1 name", entry.get_name() == "ActiveMQ")
                self.check("Unexpected config entry mw-id value", entry.get_config().get_value("mw-id") == "activemq39")
                self.check("Unexpected config entry server value", entry.get_config().get_value("server") == "tcp://localhost:61616")
            else:
                self.check("Unexpected config entry 2 name", entry.get_name() == "Bolt")
                self.check("Unexpected config entry mw-id value", entry.get_config().get_value("mw-id") == "bolt")
                self.check("Unexpected config entry server value", entry.get_config().get_value("server") == "localhost")

        self.check("Unexpected number of config entries", numConfigs == 2)


        lp.log_info("Test has_next_message() and next_message()")
        numMessages = 0
        while iter.has_next_message():
            entry = iter.next_message()

            numMessages += 1

            if numMessages == 1:
                self.check("Unexpected message entry 1 name", entry.get_name() == "Heartbeat")
                self.check("Unexpected message entry 1", entry.get_message().to_xml() == self.msgFactory.create_message("HB").to_xml())
            else:
                self.check("Unexpected message entry 2 name", entry.get_name() == "Log")
                self.check("Unexpected message entry 2", entry.get_message().to_xml() == self.msgFactory.create_message("LOG").to_xml())

        self.check("Unexpected number of message entries", numMessages == 2)


    def test_reset(self):
        lp.log_info("Test reset()")

        iter = self.cfgFile.get_iterator()

        self.check("Expected to have subscriptions", iter.has_next_subscription())
        self.check("Expected to have configs", iter.has_next_config())
        self.check("Expected to have messages", iter.has_next_message())

        while iter.has_next_subscription():
            iter.next_subscription()
        while iter.has_next_config():
            iter.next_config()
        while iter.has_next_message():
            iter.next_message()

        self.check("Expected NOT to have additional subscriptions", iter.has_next_subscription() == False)
        self.check("Expected NOT to have additional configs", iter.has_next_config() == False)
        self.check("Expected NOT to have additional messages", iter.has_next_message() == False)

        iter.reset()

        self.check("Expected to have subscriptions", iter.has_next_subscription())
        self.check("Expected to have configs", iter.has_next_config())
        self.check("Expected to have messages", iter.has_next_message())


    def setup(self):
        if self.cfgFile == None:
            self.cfgFile = lp.ConfigFile()

        if self.msgFactory == None:
            self.msgFactory = lp.MessageFactory()
            self.set_standard_fields(self.msgFactory)

        self.cfgFile.add_subscription_entry( lp.SubscriptionEntry("AllMessages", "GMSEC.>") )
        self.cfgFile.add_subscription_entry( lp.SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+") )

        self.cfgFile.add_config("ActiveMQ", lp.Config("mw-id=activemq39 server=tcp://localhost:61616", lp.DataType_KEY_VALUE_DATA))
        self.cfgFile.add_config("Bolt", lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA))

        self.cfgFile.add_message("Heartbeat", self.msgFactory.create_message("HB"))
        self.cfgFile.add_message("Log", self.msgFactory.create_message("LOG"))


Test.run('Test ConfigFileIterator', Test_ConfigFileIterator())
