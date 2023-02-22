#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_MessageFieldIterator(Test):

    def test(self):
        self.test_all_fields()
        self.test_header_fields()
        self.test_non_header_fields()
        self.test_reset()


    def test_all_fields(self):
        lp.log_info("Test ALL_FIELDS")

        msg = TestMessage.instance().get_message()

        iter = msg.get_field_iterator( lp.MessageFieldIterator.Selector_ALL_FIELDS )

        fieldsFound = 0

        while iter.has_next():
            field = iter.next()

            fieldsFound += 1

            if fieldsFound == 1:
                self.check("Unexpected field type 1", field.get_type() == lp.Field.Type_STRING)
                self.check("Unexpected field name 1", field.get_name() == "FIELD-1")
                self.check("Unexpected field value 1", field.get_string_value() == "1")

            elif fieldsFound == 2:
                self.check("Unexpected field type 2", field.get_type() == lp.Field.Type_STRING)
                self.check("Unexpected field name 2", field.get_name() == "FIELD-2")
                self.check("Unexpected field value 2", field.get_string_value() == "2")

            else:
                self.check("Unexpected field type 3", field.get_type() == lp.Field.Type_I32)
                self.check("Unexpected field name 3", field.get_name() == "FIELD-3")
                self.check("Unexpected field value 3", field.get_integer_value() == 3)

        self.check("Unexpected field count", fieldsFound == 3)


    def test_header_fields(self):
        lp.log_info("Test HEADER_FIELDS")

        msg = TestMessage.instance().get_message()

        iter = msg.get_field_iterator( lp.MessageFieldIterator.Selector_HEADER_FIELDS )

        fieldsFound = 0

        while iter.has_next():
            field = iter.next()

            fieldsFound += 1

            if fieldsFound == 1:
                self.check("Unexpected field type 1", field.get_type() == lp.Field.Type_STRING)
                self.check("Unexpected field name 1", field.get_name() == "FIELD-1")
                self.check("Unexpected field value 1", field.get_string_value() == "1")

            elif fieldsFound == 2:
                self.check("Unexpected field type 2", field.get_type() == lp.Field.Type_STRING)
                self.check("Unexpected field name 2", field.get_name() == "FIELD-2")
                self.check("Unexpected field value 2", field.get_string_value() == "2")

            else:
                self.check("Unexpected field", False)

        self.check("Unexpected field count", fieldsFound == 2)


    def test_non_header_fields(self):
        lp.log_info("Test NON_HEADER_FIELDS")

        msg = TestMessage.instance().get_message()

        iter = msg.get_field_iterator( lp.MessageFieldIterator.Selector_NON_HEADER_FIELDS )

        fieldsFound = 0

        while iter.has_next():
            field = iter.next()

            fieldsFound += 1

            if fieldsFound == 1:
                self.check("Unexpected field type 3", field.get_type() == lp.Field.Type_I32)
                self.check("Unexpected field name 3", field.get_name() == "FIELD-3")
                self.check("Unexpected field value 3", field.get_integer_value() == 3)

            else:
                self.check("Unexpected field", False)

        self.check("Unexpected field count", fieldsFound == 1)


    def test_reset(self):
        lp.log_info("Test reset()")

        msg = TestMessage.instance().get_message()

        iter = msg.get_field_iterator()

        fieldsFound = 0

        while iter.has_next():
            iter.next()
            fieldsFound += 1

        self.check("Unexpected field count", fieldsFound == 3)

        self.check("Should no more field(s) to iterate over", iter.has_next() == False)

        iter.reset()

        self.check("Should have field(s) to iterator over", iter.has_next())

        fieldsFound = 0

        while iter.has_next():
            iter.next()
            fieldsFound += 1

        self.check("Unexpected field count", fieldsFound == 3)


class TestMessage:

    s_instance = None

    @staticmethod
    def instance():
        if TestMessage.s_instance == None:
            TestMessage()
        return TestMessage.s_instance


    def __init__(self):
        TestMessage.s_instance = self
        self.m_msg = lp.Message()

        field1 = lp.StringField("FIELD-1", "1", True)
        field2 = lp.StringField("FIELD-2", "2", True)
        field3 = lp.I32Field("FIELD-3", 3)

        self.m_msg.add_field( field1 )
        self.m_msg.add_field( field2 )
        self.m_msg.add_field( field3 )


    def get_message(self):
        return self.m_msg


Test.run('MessageFieldIterator', Test_MessageFieldIterator())
