#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_HeartbeatGenerator(Test):

    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_start()
        self.test_stop()
        self.test_change_publish_rate()
        self.test_set_field()


    def test_constructor_1(self):
        lp.log_info("Test constructor 1")

        # Nominal test
        hbgen = lp.HeartbeatGenerator(self.get_config(), 5)
        self.check("HeartbeatGenerator should not be running", hbgen.is_running() == False)

        # Off-nominal test(s)
        try:
            config = lp.Config(self.get_config())
            config.add_value("mw-id", "bogus-mw")
            lp.HeartbeatGenerator(config, 5)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Unable to load" in str(e))


    def test_constructor_2(self):
        lp.log_info("Test constructor 2")

        emptyFieldList = lp.FieldList()
        standardFields = self.get_standard_fields()

        # Nominal test
        hbgen1 = lp.HeartbeatGenerator(self.get_config(), 5, emptyFieldList)
        self.check("HeartbeatGenerator should not be running", hbgen1.is_running() == False)

        hbgen2 = lp.HeartbeatGenerator(self.get_config(), 5, standardFields)
        self.check("HeartbeatGenerator should not be running", hbgen2.is_running() == False)

        # Off-nominal test(s)
        try:
            config = lp.Config(self.get_config())
            config.add_value("mw-id", "bogus-mw")
            lp.HeartbeatGenerator(config, 5, standardFields)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Unable to load" in str(e))


    def test_start(self):
        lp.log_info("Test start()")

        config = lp.Config(self.get_config())

        hbgen = lp.HeartbeatGenerator(config, 1, self.get_standard_fields())

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( lp.StringField("COMPONENT", self.get_unique_component()) )

        hbgen.start()

        self.check("HeartbeatGenerator should be running", hbgen.is_running())

        self.verify_heartbeat_message(config, 1)

        hbgen.stop()

        # Allow time for the HB-gen thread to stop
        lp.TimeUtil.millisleep(2000)

        # Off-nominal tests
        config.add_value("gmsec-msg-content-validate", "true")

        hbgen2 = lp.HeartbeatGenerator(config, 1, self.get_standard_fields())

        # Add bogus field using a Field
        try:
            hbgen2.set_field( lp.U16Field("BOGUS-FIELD", 2) )
            hbgen2.start()
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message Validation Failed" in str(e))

        # Add bogus field using a long value
        try:
            hbgen2.set_field("BOGUS-FIELD", 2)
            hbgen2.start()
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message Validation Failed" in str(e))

        # Add bogus field using a double value
        try:
            hbgen2.set_field("BOGUS-FIELD", 2.0)
            hbgen2.start()
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message Validation Failed" in str(e))

        # Add bogus field using a string value
        try:
            hbgen2.set_field("BOGUS-FIELD", "2")
            hbgen2.start()
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message Validation Failed" in str(e))


    def test_stop(self):
        lp.log_info("Test stop()")

        config = self.get_config()

        hbgen = lp.HeartbeatGenerator(config, 1, self.get_standard_fields())

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( lp.StringField("COMPONENT", self.get_unique_component()) )

        hbgen.start()

        self.check("HeartbeatGenerator should be running", hbgen.is_running())

        hbgen.stop()

        self.check("HeartbeatGenerator should not be running", hbgen.is_running() == False)

        # Allow time for the HB-gen thread to stop
        lp.TimeUtil.millisleep(2000)

        conn = lp.Connection(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + self.get_unique_component())

        for i in range(0,3):
            hbmsg = conn.receive(5000)

            if hbmsg != None:
                self.check("Unexpectedly received a Heartbeat Message", False)
                lp.Message.destroy(hbmsg)


    def test_change_publish_rate(self):
        lp.log_info("Test change_publish_rate()")

        config = self.get_config()

        hbgen = lp.HeartbeatGenerator(config, 1, self.get_standard_fields())

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( lp.StringField("COMPONENT", self.get_unique_component()) )

        hbgen.start()

        hbgen.change_publish_rate(2)

        self.verify_heartbeat_message(config, 2)

        conn = lp.Connection(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + self.get_unique_component())

        lp.TimeUtil.millisleep(1000)

        hbgen.change_publish_rate(0)

        lp.TimeUtil.millisleep(1000)

        lastMessage = False

        for i in range(0,3):
            hbmsg = conn.receive(5000)

            if hbmsg == None:
                self.check("Failed to receive Heartbeat Message", False)

            elif hbmsg.get_integer_value("PUB-RATE") != 0:
                lp.Message.destroy(hbmsg)

            else:
                lastMessage = True
                lp.Message.destroy(hbmsg)
                break

        self.check("Failed to receive message with PUB-RATE of 0", lastMessage)

        hbgen.stop()

        # Allow time for the HB-gen thread to stop
        lp.TimeUtil.millisleep(2000)


    def test_set_field(self):
        config = lp.Config( self.get_config() )     # make a copy

        standardFields = self.get_standard_fields()

        # Nominal tests
        hbgen = lp.HeartbeatGenerator(config, 1, standardFields)

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( lp.StringField("COMPONENT", self.get_unique_component()) )

        hbgen.start()

        lp.TimeUtil.millisleep(2000)

        lp.log_info("Test set_field() with a Field")
        hbgen.set_field( lp.U16Field("PUB-RATE", 2) )
        self.verify_heartbeat_message(config, 2)

        lp.log_info("Test set_field() with a Field Name and long value")
        hbgen.set_field("PUB-RATE", 3)
        self.verify_heartbeat_message(config, 3)

        lp.log_info("Test set_field() with a Field Name and double value")
        hbgen.set_field("PUB-RATE", 2.0)
        self.verify_heartbeat_message(config, 2)

        lp.log_info("Test set_field() with a Field Name and string value")
        hbgen.set_field("PUB-RATE", "3")
        self.verify_heartbeat_message(config, 3)

        hbgen.stop()

        # Allow time for the HB-gen thread to stop
        lp.TimeUtil.millisleep(2000)


    def verify_heartbeat_message(self, config, expectedPubRate):
        conn = lp.Connection(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + self.get_unique_component() + ".+")

        t1 = 0
        t2 = 0

        for i in range(0,7):
            hbmsg = conn.receive(5000)

            # ignore the first few incoming messages (if any)
            if i < 3:
                if hbmsg != None:
                    lp.Message.destroy(hbmsg)
                continue

            if hbmsg != None:
                if t1 == 0:
                    t1 = lp.TimeUtil.get_current_time_s()
                else:
                    t2 = lp.TimeUtil.get_current_time_s()

                    delta = t2 - t1
                    if delta < expectedPubRate:
                        delta = ((t2 - t1) * 10.0 + 0.5) / 10.0

                    lp.log_info("Expected rate is: " + str(expectedPubRate) + ", delta is: " + str(delta))

                    self.check("Unexpected publish rate", int(expectedPubRate) == int(delta))

                    t1 = t2

                self.check("Unexpected MESSAGE-TYPE", hbmsg.get_string_value("MESSAGE-TYPE") == "MSG")
                self.check("Unexpected MESSAGE-SUBTYPE", hbmsg.get_string_value("MESSAGE-SUBTYPE") == "HB")
                self.check("Unexpected PUB-RATE", hbmsg.get_integer_value("PUB-RATE") == expectedPubRate)

                lp.Message.destroy(hbmsg)
            else:
                self.check("Failed to received Heartbeat Message", False)

        conn.disconnect()


Test.run('Test HeartbeatGenerator', Test_HeartbeatGenerator())
