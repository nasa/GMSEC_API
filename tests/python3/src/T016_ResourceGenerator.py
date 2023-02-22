#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_ResourceGenerator(Test):

    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_start()
        self.test_stop()
        self.test_set_field()
        self.test_create_resource_message()


    def test_constructor_1(self):
        lp.log_info("Test constructor 1")

        # Nominal test
        rsrcgen = lp.ResourceGenerator(self.get_config(), 5, 1, 10)

        self.check("ResourceGenerator should not be running", rsrcgen.is_running() == False)


    def test_constructor_2(self):
        lp.log_info("Test constructor 2")

        emptyFieldList = lp.FieldList()
        standardFields = self.get_standard_fields()

        # Nominal test (empty, populated, and null list of fields)
        rsrcgen1 = lp.ResourceGenerator(self.get_config(), 5, 1, 10, emptyFieldList)
        rsrcgen2 = lp.ResourceGenerator(self.get_config(), 5, 1, 10, standardFields)

        self.check("ResourceGenerator should not be running", rsrcgen1.is_running() == False)
        self.check("ResourceGenerator should not be running", rsrcgen2.is_running() == False)


    def test_start(self):
        lp.log_info("Test start()")

        config  = self.get_config()
        pubRate = 1

        rsrcgen = lp.ResourceGenerator(config, pubRate, 1, 10, self.get_standard_fields())

        # To ensure this resource message is unique, we set the COMPONENT field
        rsrcgen.set_field( lp.StringField("COMPONENT", self.get_unique_component()) )

        rsrcgen.start()

        self.check("ResourceGenerator should be running", rsrcgen.is_running())

        self.verify_resource_message(config, pubRate)

        rsrcgen.stop()

        # Allow time for the RSRC-gen thread to stop
        lp.TimeUtil.millisleep(2000)


    def test_stop(self):
        lp.log_info("Test stop()")

        config  = self.get_config()
        pubRate = 1

        rsrcgen = lp.ResourceGenerator(config, pubRate, 1, 10, self.get_standard_fields())

        rsrcgen.start()

        self.check("ResourceGenerator should be running", rsrcgen.is_running())

        rsrcgen.stop()

        self.check("ResourceGenerator should not be running", rsrcgen.is_running() == False)

        # Allow time for the RSRC-gen thread to stop
        lp.TimeUtil.millisleep(2000)

        conn = lp.Connection(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + self.get_unique_component())

        for i in range(0,3):
            rsrcMsg = conn.receive(5000)

            if rsrcMsg != None:
                self.check("Unexpectedly received a Resource Message", False)
                lp.Message.destroy(rsrcMsg)


    def test_set_field(self):
        config = lp.Config( self.get_config() )     # make a copy

        standardFields = self.get_standard_fields()
        pubRate = 1

        # Nominal tests
        rsrcgen = lp.ResourceGenerator(config, pubRate, 1, 10, standardFields)

        # To ensure this resource message is unique, we set the COMPONENT field
        rsrcgen.set_field( lp.StringField("COMPONENT", self.get_unique_component()) )

        rsrcgen.start()

        lp.TimeUtil.millisleep(2000)

        lp.log_info("Test set_field() with a Field")
        newPubRate = 2
        rsrcgen.set_field( lp.U16Field("PUB-RATE", newPubRate) )
        self.verify_resource_message(config, newPubRate)

        rsrcgen.stop()

        # Allow time for the RSRC-gen thread to stop
        lp.TimeUtil.millisleep(2000)

        # Off-nominal tests
        config.add_value("gmsec-msg-content-validate", "true")

        rsrcgen2 = lp.ResourceGenerator(config, pubRate, 1, 10, standardFields)

        # Add bogus field using a Field
        try:
            rsrcgen2.set_field( lp.U16Field("BOGUS-FIELD", 2) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message Validation Failed" in str(e))


    def test_create_resource_message(self):
        lp.log_info("Test create_resource_message()")

        config = lp.Config( self.get_config() )     # make a copy

        factory = lp.MessageFactory(config)
        samInt  = 1
        avgInt  = 10

        factory.set_standard_fields( self.get_standard_fields() )

        rsrcMsg = lp.ResourceGenerator.create_resource_message(factory, samInt, avgInt)

        self.check("Unexpected MESSAGE-TYPE", rsrcMsg.get_string_value("MESSAGE-TYPE") == "MSG")
        self.check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.get_string_value("MESSAGE-SUBTYPE") == "RSRC")


    def verify_resource_message(self, config, expectedPubRate):
        conn = lp.Connection(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + self.get_unique_component() + ".+")

        t1 = 0
        t2 = 0

        for i in range(0,7):
            rsrcMsg = conn.receive(5000)

            # ignore the first few incoming messages (if any)
            if i < 3:
                if rsrcMsg != None:
                    lp.Message.destroy(rsrcMsg)
                continue

            if rsrcMsg != None:
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

                self.check("Unexpected MESSAGE-TYPE", rsrcMsg.get_string_value("MESSAGE-TYPE") == "MSG")
                self.check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.get_string_value("MESSAGE-SUBTYPE") == "RSRC")
                self.check("Unexpected PUB-RATE", rsrcMsg.get_integer_value("PUB-RATE") == expectedPubRate)

                lp.Message.destroy(rsrcMsg)
            else:
                self.check("Failed to received Resource Message", False)

        conn.disconnect()


Test.run('Test ResourceGenerator', Test_ResourceGenerator())
