#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_MessageFactory(Test):

    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_set_standard_fields()
        self.test_clear_standard_fields()
        self.test_set_message_config()
        self.test_create_message_1()
        self.test_create_message_2()
        self.test_from_data()
        self.test_get_specification()
        self.test_register_message_validator()


    def test_constructor_1(self):
        lp.log_info("Test constructor (no args)")

        msgFactory = lp.MessageFactory()

        self.check("Unexpected specification version", msgFactory.get_specification().get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Unexpected specification schema level", msgFactory.get_specification().get_schema_level() == lp.Specification.SchemaLevel_LEVEL_0)


    def test_constructor_2(self):
        lp.log_info("Test constructor w/ Config")

        config = lp.Config("gmsec-schema-level=2", lp.DataType_KEY_VALUE_DATA)

        msgFactory = lp.MessageFactory(config)

        self.check("Unexpected specification version", msgFactory.get_specification().get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Unexpected specification schema level", msgFactory.get_specification().get_schema_level() == lp.Specification.SchemaLevel_LEVEL_2)


    def test_set_standard_fields(self):
        lp.log_info("Test set_standard_fields()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message()

        self.verify_standard_fields(msg)

        msgFactory.clear_standard_fields()


    def test_clear_standard_fields(self):
        lp.log_info("Test clear_standard_fields()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message()

        self.verify_standard_fields(msg1)

        msgFactory.clear_standard_fields()

        msg2 = msgFactory.create_message()

        self.check("Expected message field count to be 0 (zero)", msg2.get_field_count() == 0)


    def test_set_message_config(self):
        lp.log_info("Test set_message_config()")

        msgFactory = lp.MessageFactory()

        msgConfig = lp.Config("tracking=false", lp.DataType_KEY_VALUE_DATA)

        msgFactory.set_message_config(msgConfig)

        msg = msgFactory.create_message()

        self.check("Unexpected message config", msg.get_config().to_xml() == msgConfig.to_xml())


    def test_create_message_1(self):
        lp.log_info("Test create_message() w/ no args")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message()

        self.check("Expected no fields in the message", msg.get_field_count() == 0)


    def test_create_message_2(self):
        lp.log_info("Test create_message()")

        msgFactory = lp.MessageFactory()

        # Nominal case (valid schema ID)
        msg = msgFactory.create_message("HB")

        self.check("Unexpected MESSAGE-TYPE", msg.get_string_value("MESSAGE-TYPE") == "MSG")
        self.check("Unexpected MESSAGE-SUBTYPE", msg.get_string_value("MESSAGE-SUBTYPE") == "HB")

        # Off-nominal case (invalid schema ID)
        try:
            msg = msgFactory.create_message("BOGUS")
            self.check("Expected an exception", False)
        except Exception as e:
            self.check(str(e), "SchemaID \"BOGUS\" could not be found in list of available schema" in str(e))

        # Off-nominal case (empty-string schema ID)
        try:
            msg = msgFactory.create_message("")
            self.check("Expected an exception", False)
        except Exception as e:
            self.check(str(e), "SchemaID cannot be NULL, nor be an empty string" in str(e))

        # Off-nominal case (null schema ID)
        try:
            msg = msgFactory.create_message(None)
            self.check("Expected an exception", False)
        except Exception as e:
            self.check(str(e), "SchemaID cannot be NULL, nor be an empty string" in str(e))


    def test_from_data(self):
        lp.log_info("Test from_data()")

        # Nominal tests
        xml  = ""
        xml += "<MESSAGE SUBJECT=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" KIND=\"PUBLISH\">\n"
        xml += "\t<FIELD NAME=\"COMPONENT\" TYPE=\"STRING\">GMPUB</FIELD>\n"
        xml += "\t<FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">CNS1</FIELD>\n"
        xml += "\t<FIELD NAME=\"CONTENT-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
        xml += "\t<FIELD NAME=\"COUNTER\" TYPE=\"U16\">1</FIELD>\n"
        xml += "\t<FIELD NAME=\"DOMAIN1\" TYPE=\"STRING\">DOM1</FIELD>\n"
        xml += "\t<FIELD NAME=\"DOMAIN2\" TYPE=\"STRING\">DOM2</FIELD>\n"
        xml += "\t<FIELD NAME=\"FACILITY\" TYPE=\"STRING\">MY-FACILITY</FIELD>\n"
        xml += "\t<FIELD NAME=\"HEADER-VERSION\" TYPE=\"F32\" BITS=\"44FC6000\">2019</FIELD>\n"
        xml += "\t<FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n"
        xml += "\t<FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n"
        xml += "\t<FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MSSN</FIELD>\n"
        xml += "\t<FIELD NAME=\"PUB-RATE\" TYPE=\"U16\">1</FIELD>\n"
        xml += "\t<FIELD NAME=\"SAT-ID-LOGICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
        xml += "\t<FIELD NAME=\"SAT-ID-PHYSICAL\" TYPE=\"STRING\">SAT1</FIELD>\n"
        xml += "\t<FIELD NAME=\"SPECIFICATION\" TYPE=\"STRING\">GMSEC</FIELD>\n"
        xml += "</MESSAGE>"

        xml_lower  = ""
        xml_lower += "<message subject=\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\" kind=\"publish\">\n"
        xml_lower += "\t<field name=\"COMPONENT\" type=\"STRING\">GMPUB</field>\n"
        xml_lower += "\t<field name=\"CONSTELLATION-ID\" type=\"STRING\">CNS1</field>\n"
        xml_lower += "\t<field name=\"CONTENT-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n"
        xml_lower += "\t<field name=\"COUNTER\" type=\"U16\">1</field>\n"
        xml_lower += "\t<field name=\"DOMAIN1\" type=\"STRING\">DOM1</field>\n"
        xml_lower += "\t<field name=\"DOMAIN2\" type=\"STRING\">DOM2</field>\n"
        xml_lower += "\t<field name=\"FACILITY\" type=\"STRING\">MY-FACILITY</field>\n"
        xml_lower += "\t<field name=\"HEADER-VERSION\" type=\"F32\" bits=\"44FC6000\">2019</field>\n"
        xml_lower += "\t<field name=\"MESSAGE-SUBTYPE\" type=\"STRING\">HB</field>\n"
        xml_lower += "\t<field name=\"MESSAGE-TYPE\" type=\"STRING\">MSG</field>\n"
        xml_lower += "\t<field name=\"MISSION-ID\" type=\"STRING\">MSSN</field>\n"
        xml_lower += "\t<field name=\"PUB-RATE\" type=\"U16\">1</field>\n"
        xml_lower += "\t<field name=\"SAT-ID-LOGICAL\" type=\"STRING\">SAT1</field>\n"
        xml_lower += "\t<field name=\"SAT-ID-PHYSICAL\" type=\"STRING\">SAT1</field>\n"
        xml_lower += "\t<field name=\"SPECIFICATION\" type=\"STRING\">GMSEC</field>\n"
        xml_lower += "</message>"

        json  = ""
        json += "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"KIND\":\"PUBLISH\","
        json += "\"FIELD\":["
        json += "{\"NAME\":\"COMPONENT\",\"TYPE\":\"STRING\",\"VALUE\":\"GMPUB\"},"
        json += "{\"NAME\":\"CONSTELLATION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"CNS1\"},"
        json += "{\"NAME\":\"CONTENT-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},"
        json += "{\"NAME\":\"COUNTER\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},"
        json += "{\"NAME\":\"DOMAIN1\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM1\"},"
        json += "{\"NAME\":\"DOMAIN2\",\"TYPE\":\"STRING\",\"VALUE\":\"DOM2\"},"
        json += "{\"NAME\":\"FACILITY\",\"TYPE\":\"STRING\",\"VALUE\":\"MY-FACILITY\"},"
        json += "{\"NAME\":\"HEADER-VERSION\",\"TYPE\":\"F32\",\"BITS\":\"44FC6000\",\"VALUE\":\"2019\"},"
        json += "{\"NAME\":\"MESSAGE-SUBTYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"HB\"},"
        json += "{\"NAME\":\"MESSAGE-TYPE\",\"TYPE\":\"STRING\",\"VALUE\":\"MSG\"},"
        json += "{\"NAME\":\"MISSION-ID\",\"TYPE\":\"STRING\",\"VALUE\":\"MSSN\"},"
        json += "{\"NAME\":\"PUB-RATE\",\"TYPE\":\"U16\",\"VALUE\":\"1\"},"
        json += "{\"NAME\":\"SAT-ID-LOGICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},"
        json += "{\"NAME\":\"SAT-ID-PHYSICAL\",\"TYPE\":\"STRING\",\"VALUE\":\"SAT1\"},"
        json += "{\"NAME\":\"SPECIFICATION\",\"TYPE\":\"STRING\",\"VALUE\":\"GMSEC\"}"
        json += "]}}"

        json_lower  = ""
        json_lower += "{\"message\":{\"subject\":\"GMSEC.MSSN.SAT1.MSG.HB.GMPUB\",\"kind\":\"publish\","
        json_lower += "\"field\":["
        json_lower += "{\"name\":\"COMPONENT\",\"type\":\"string\",\"value\":\"GMPUB\"},"
        json_lower += "{\"name\":\"CONSTELLATION-ID\",\"type\":\"string\",\"value\":\"CNS1\"},"
        json_lower += "{\"name\":\"CONTENT-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},"
        json_lower += "{\"name\":\"COUNTER\",\"type\":\"u16\",\"value\":\"1\"},"
        json_lower += "{\"name\":\"DOMAIN1\",\"type\":\"string\",\"value\":\"DOM1\"},"
        json_lower += "{\"name\":\"DOMAIN2\",\"type\":\"string\",\"value\":\"DOM2\"},"
        json_lower += "{\"name\":\"FACILITY\",\"type\":\"string\",\"value\":\"MY-FACILITY\"},"
        json_lower += "{\"name\":\"HEADER-VERSION\",\"type\":\"f32\",\"bits\":\"44FC6000\",\"value\":\"2019\"},"
        json_lower += "{\"name\":\"MESSAGE-SUBTYPE\",\"type\":\"string\",\"value\":\"HB\"},"
        json_lower += "{\"name\":\"MESSAGE-TYPE\",\"type\":\"string\",\"value\":\"MSG\"},"
        json_lower += "{\"name\":\"MISSION-ID\",\"type\":\"string\",\"value\":\"MSSN\"},"
        json_lower += "{\"name\":\"PUB-RATE\",\"type\":\"u16\",\"value\":\"1\"},"
        json_lower += "{\"name\":\"SAT-ID-LOGICAL\",\"type\":\"string\",\"value\":\"SAT1\"},"
        json_lower += "{\"name\":\"SAT-ID-PHYSICAL\",\"type\":\"string\",\"value\":\"SAT1\"},"
        json_lower += "{\"name\":\"SPECIFICATION\",\"type\":\"string\",\"value\":\"GMSEC\"}"
        json_lower += "]}}"

        msgFactory = lp.MessageFactory( lp.Config("gmsec-schema-level=2", lp.DataType_KEY_VALUE_DATA) )

        msg1 = self.from_data(msgFactory, xml, lp.DataType_XML_DATA, None)
        msg2 = self.from_data(msgFactory, json, lp.DataType_JSON_DATA, None)
        msg3 = self.from_data(msgFactory, xml_lower, lp.DataType_XML_DATA, None)
        msg4 = self.from_data(msgFactory, json_lower, lp.DataType_JSON_DATA, None)

        self.check("Unexpected XML",  msg1.to_xml() == xml)
        self.check("Unexpected JSON", msg2.to_json() == json)
        self.check("Unexpected XML",  msg3.to_xml() == xml)
        self.check("Unexpected JSON", msg4.to_json() == json)

        self.check("Message 1 missing CONTENT-VERSION", msg1.has_field("CONTENT-VERSION"))
        self.check("Message 1 missing HEADER-VERSION", msg1.has_field("HEADER-VERSION"))
        self.check("Message 1 missing MESSAGE-SUBTYPE", msg1.has_field("MESSAGE-SUBTYPE"))
        self.check("Message 1 missing MESSAGE-TYPE", msg1.has_field("MESSAGE-TYPE"))
        self.check("Message 1 missing PUB-RATE", msg1.has_field("PUB-RATE"))

        self.check("Message 2 missing CONTENT-VERSION", msg2.has_field("CONTENT-VERSION"))
        self.check("Message 2 missing HEADER-VERSION", msg2.has_field("HEADER-VERSION"))
        self.check("Message 2 missing MESSAGE-SUBTYPE", msg2.has_field("MESSAGE-SUBTYPE"))
        self.check("Message 2 missing MESSAGE-TYPE", msg2.has_field("MESSAGE-TYPE"))
        self.check("Message 2 missing PUB-RATE", msg2.has_field("PUB-RATE"))

        self.check("Message 3 missing CONTENT-VERSION", msg3.has_field("CONTENT-VERSION"))
        self.check("Message 3 missing HEADER-VERSION", msg3.has_field("HEADER-VERSION"))
        self.check("Message 3 missing MESSAGE-SUBTYPE", msg3.has_field("MESSAGE-SUBTYPE"))
        self.check("Message 3 missing MESSAGE-TYPE", msg3.has_field("MESSAGE-TYPE"))
        self.check("Message 3 missing PUB-RATE", msg3.has_field("PUB-RATE"))

        self.check("Message 4 missing CONTENT-VERSION", msg4.has_field("CONTENT-VERSION"))
        self.check("Message 4 missing HEADER-VERSION", msg4.has_field("HEADER-VERSION"))
        self.check("Message 4 missing MESSAGE-SUBTYPE", msg4.has_field("MESSAGE-SUBTYPE"))
        self.check("Message 4 missing MESSAGE-TYPE", msg4.has_field("MESSAGE-TYPE"))
        self.check("Message 4 missing PUB-RATE", msg4.has_field("PUB-RATE"))

        # Off-nominal tests
        msgFactory = lp.MessageFactory()

        invalid_msg_subject = "<MESSAGE SUBJECT=\"GMSEC FOO BAR\"/>"
        invalid_xml         = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\">"
        invalid_json        = "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.FOO.BAR\""
        empty_str           = ""
        none_str            = None

        self.from_data(msgFactory, invalid_msg_subject, lp.DataType_XML_DATA, "Invalid message subject")
        self.from_data(msgFactory, invalid_xml, lp.DataType_XML_DATA, "Mismatched element")
        self.from_data(msgFactory, invalid_json, lp.DataType_JSON_DATA, "Invalid JSON message format -- invalid JSON string data")
        self.from_data(msgFactory, empty_str, lp.DataType_XML_DATA, "Data string cannot be NULL, nor empty");
        self.from_data(msgFactory, none_str, lp.DataType_JSON_DATA, "Data string cannot be NULL, nor empty");


    def test_get_specification(self):
        lp.log_info("Test get_specification()")

        msgFactory1 = lp.MessageFactory()
        msgFactory2 = lp.MessageFactory( lp.Config("gmsec-specification-version=201900", lp.DataType_KEY_VALUE_DATA) )

        self.check("Unexpected specification version", msgFactory1.get_specification().get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Unexpected specification version", msgFactory2.get_specification().get_version() == lp.GMSEC_MSG_SPEC_2019_00)


    def test_register_message_validator(self):
        lp.log_info("Test register_message_validator()")

        val1 = MyValidator(True)
        val2 = MyValidator(False)

        # Nominal case
        msgFactory1 = lp.MessageFactory()
        self.set_standard_fields(msgFactory1)
        msgFactory1.register_message_validator(val1)
        msg1 = msgFactory1.create_message("HB")
        status1 = msg1.is_compliant()
        self.check("Expected message to be compliant", status1.has_error() == False)

        msgFactory2 = lp.MessageFactory()
        msgFactory2.register_message_validator(val2)
        msg2 = msgFactory2.create_message("HB")
        status2 = msg2.is_compliant()
        self.check("Expected message to be non-compliant", status2.has_error())
        self.check(status2.get_reason(), "MyValidator is not happy with the message" in status2.get_reason())

        # Off-nominal case
        try:
            msgFactory = lp.MessageFactory()
            msgFactory.register_message_validator(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "MessageValidator cannot be null" in str(e))


    def from_data(self, msgFactory, data, dataType, expectedError):
        msg = None

        try:
            msg = msgFactory.from_data(data, dataType)

            if expectedError != None:
                self.check("Expected an error: " + expectedError)
        except Exception as e:
            if expectedError != None:
                self.check(str(e), expectedError in str(e))
            else:
                self.check(str(e), False)

        return msg


class MyValidator(lp.MessageValidator):

    def __init__(self, nominal):
        super().__init__()
        self.m_nominal = nominal
        self.m_used    = False

    def was_used(self):
        return self.m_used

    def reset(self):
        self.m_used = False

    def validate_message(self, msg):
        status = lp.Status()

        if self.m_nominal == False:
            status.set(lp.MSG_ERROR, lp.MESSAGE_FAILED_VALIDATION, "MyValidator is not happy with the message")

        return status


Test.run('Test MessageFactory', Test_MessageFactory())
