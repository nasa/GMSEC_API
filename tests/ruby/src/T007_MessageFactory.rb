#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_MessageFactory < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_create
        test_set_standard_fields
        test_clear_standard_fields
        test_set_message_config
        test_create_message_1
        test_create_message_2
        test_from_data
        test_get_specification
        test_register_message_validator
	end


    def test_create()
        Libgmsec_ruby::Log::info("Test create and destroy")

        # No config
        msgFactory = Libgmsec_ruby::MessageFactory::create()

        check("Unexpected specification version", msgFactory.get_specification().get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Unexpected specification schema level", msgFactory.get_specification().get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)


        # With config
        config = Libgmsec_ruby::Config.new("gmsec-schema-level=2", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        msgFactory = Libgmsec_ruby::MessageFactory::create(config)

        check("Unexpected specification version", msgFactory.get_specification().get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Unexpected specification schema level", msgFactory.get_specification().get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_set_standard_fields()
        Libgmsec_ruby::Log::info("Test set_standard_fields()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message()

        verify_standard_fields(msg)

        msgFactory.clear_standard_fields()

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_clear_standard_fields()
        Libgmsec_ruby::Log::info("Test clear_standard_fields()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message()

        verify_standard_fields(msg1)

        msgFactory.clear_standard_fields()

        msg2 = msgFactory.create_message()

        check("Expected message field count to be 0 (zero)", msg2.get_field_count() == 0)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_set_message_config()
        Libgmsec_ruby::Log::info("Test set_message_config()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msgConfig = Libgmsec_ruby::Config.new("tracking=false", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        msgFactory.set_message_config(msgConfig)

        msg = msgFactory.create_message()

        check("Unexpected message config", msg.get_config().to_xml() == msgConfig.to_xml())

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_create_message_1()
        Libgmsec_ruby::Log::info("Test create_message() w/ no args")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message()

        check("Expected no fields in the message", msg.get_field_count() == 0)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_create_message_2()
        Libgmsec_ruby::Log::info("Test create_message()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        # Nominal case (valid schema ID)
        msg = msgFactory.create_message("HB")

        check("Unexpected MESSAGE-TYPE", msg.get_string_value("MESSAGE-TYPE") == "MSG")
        check("Unexpected MESSAGE-SUBTYPE", msg.get_string_value("MESSAGE-SUBTYPE") == "HB")

        # Off-nominal case (invalid schema ID)
		begin
            msg = msgFactory.create_message("BOGUS")
            check("Expected an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("SchemaID \"BOGUS\" could not be found in list of available schema"))
		end

        # Off-nominal case (empty-string schema ID)
		begin
            msg = msgFactory.create_message("")
            check("Expected an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("SchemaID cannot be NULL, nor be an empty string"))
		end

        # Off-nominal case (null schema ID)
		begin
            msg = msgFactory.create_message(nil)
            check("Expected an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("SchemaID cannot be NULL, nor be an empty string"))
		end

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_from_data()
        Libgmsec_ruby::Log::info("Test from_data()")

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

        msgFactory = Libgmsec_ruby::MessageFactory::create( Libgmsec_ruby::Config.new("gmsec-schema-level=2", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )

        msg1 = from_data(msgFactory, xml, Libgmsec_ruby::DataType_XML_DATA, nil)
        msg2 = from_data(msgFactory, json, Libgmsec_ruby::DataType_JSON_DATA, nil)
        msg3 = from_data(msgFactory, xml_lower, Libgmsec_ruby::DataType_XML_DATA, nil)
        msg4 = from_data(msgFactory, json_lower, Libgmsec_ruby::DataType_JSON_DATA, nil)

        check("Unexpected XML",  msg1.to_xml() == xml)
        check("Unexpected JSON", msg2.to_json() == json)
        check("Unexpected XML",  msg3.to_xml() == xml)
        check("Unexpected JSON", msg4.to_json() == json)

        check("Message 1 missing CONTENT-VERSION", msg1.has_field("CONTENT-VERSION"))
        check("Message 1 missing HEADER-VERSION", msg1.has_field("HEADER-VERSION"))
        check("Message 1 missing MESSAGE-SUBTYPE", msg1.has_field("MESSAGE-SUBTYPE"))
        check("Message 1 missing MESSAGE-TYPE", msg1.has_field("MESSAGE-TYPE"))
        check("Message 1 missing PUB-RATE", msg1.has_field("PUB-RATE"))

        check("Message 2 missing CONTENT-VERSION", msg2.has_field("CONTENT-VERSION"))
        check("Message 2 missing HEADER-VERSION", msg2.has_field("HEADER-VERSION"))
        check("Message 2 missing MESSAGE-SUBTYPE", msg2.has_field("MESSAGE-SUBTYPE"))
        check("Message 2 missing MESSAGE-TYPE", msg2.has_field("MESSAGE-TYPE"))
        check("Message 2 missing PUB-RATE", msg2.has_field("PUB-RATE"))

        check("Message 3 missing CONTENT-VERSION", msg3.has_field("CONTENT-VERSION"))
        check("Message 3 missing HEADER-VERSION", msg3.has_field("HEADER-VERSION"))
        check("Message 3 missing MESSAGE-SUBTYPE", msg3.has_field("MESSAGE-SUBTYPE"))
        check("Message 3 missing MESSAGE-TYPE", msg3.has_field("MESSAGE-TYPE"))
        check("Message 3 missing PUB-RATE", msg3.has_field("PUB-RATE"))

        check("Message 4 missing CONTENT-VERSION", msg4.has_field("CONTENT-VERSION"))
        check("Message 4 missing HEADER-VERSION", msg4.has_field("HEADER-VERSION"))
        check("Message 4 missing MESSAGE-SUBTYPE", msg4.has_field("MESSAGE-SUBTYPE"))
        check("Message 4 missing MESSAGE-TYPE", msg4.has_field("MESSAGE-TYPE"))
        check("Message 4 missing PUB-RATE", msg4.has_field("PUB-RATE"))

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Off-nominal tests
        msgFactory = Libgmsec_ruby::MessageFactory::create()

        invalid_msg_subject = "<MESSAGE SUBJECT=\"GMSEC FOO BAR\"/>"
        invalid_xml         = "<MESSAGE SUBJECT=\"GMSEC.FOO.BAR\">"
        invalid_json        = "{\"MESSAGE\":{\"SUBJECT\":\"GMSEC.FOO.BAR\""
        empty_str           = ""
        none_str            = nil

        from_data(msgFactory, invalid_msg_subject, Libgmsec_ruby::DataType_XML_DATA, "Invalid message subject")
        from_data(msgFactory, invalid_xml, Libgmsec_ruby::DataType_XML_DATA, "Mismatched element")
        from_data(msgFactory, invalid_json, Libgmsec_ruby::DataType_JSON_DATA, "Invalid JSON message format -- invalid JSON string data")
        from_data(msgFactory, empty_str, Libgmsec_ruby::DataType_XML_DATA, "Data string cannot be NULL, nor empty");
        from_data(msgFactory, none_str, Libgmsec_ruby::DataType_JSON_DATA, "Data string cannot be NULL, nor empty");

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_get_specification()
        Libgmsec_ruby::Log::info("Test get_specification()")

        msgFactory1 = Libgmsec_ruby::MessageFactory::create()
        msgFactory2 = Libgmsec_ruby::MessageFactory::create( Libgmsec_ruby::Config.new("gmsec-specification-version=201900", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )

        check("Unexpected specification version", msgFactory1.get_specification().get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Unexpected specification version", msgFactory2.get_specification().get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory1)
        Libgmsec_ruby::MessageFactory::destroy(msgFactory2)
	end


    def test_register_message_validator()
        Libgmsec_ruby::Log::info("Test register_message_validator()")

        val1 = MyValidator.new(true)
        val2 = MyValidator.new(false)

        # Nominal case
        msgFactory1 = Libgmsec_ruby::MessageFactory::create()
        set_standard_fields(msgFactory1)
        msgFactory1.register_message_validator(val1)
        msg1 = msgFactory1.create_message("HB")
        status1 = msg1.is_compliant()
        check("Expected message to be compliant", status1.has_error() == false)
        Libgmsec_ruby::MessageFactory::destroy(msgFactory1)

        msgFactory2 = Libgmsec_ruby::MessageFactory::create()
        msgFactory2.register_message_validator(val2)
        msg2 = msgFactory2.create_message("HB")
        status2 = msg2.is_compliant()
        check("Expected message to be non-compliant", status2.has_error())
        check(status2.get_reason(), status2.get_reason().include?("MyValidator is not happy with the message"))
        Libgmsec_ruby::MessageFactory::destroy(msgFactory2)

        # Off-nominal case
        msgFactory = nil
		begin
            msgFactory = Libgmsec_ruby::MessageFactory::create()
            msgFactory.register_message_validator(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("MessageValidator cannot be null"))
		end
        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def from_data(msgFactory, data, type, expectedError)
        msg = nil

		begin
            msg = msgFactory.from_data(data, type)
            if expectedError != nil
                check("Expected an error: " + expectedError)
            end
        rescue GmsecException => e
            if expectedError != nil
                check(e.message, e.message.include?(expectedError))
            else
                check(e.message, false)
			end
		end

        return msg
	end
end


class MyValidator < Libgmsec_ruby::MessageValidator
    def initialize(nominal)
        super()
        @nominal = nominal
        @used    = false
	end

    def was_used()
        @used
	end

    def reset()
        @used = false
	end

    def validate_message(msg)
        status = Libgmsec_ruby::Status.new()

        if @nominal == false
            status.set(Libgmsec_ruby::MSG_ERROR, Libgmsec_ruby::MESSAGE_FAILED_VALIDATION, "MyValidator is not happy with the message")
		end

        return status
	end
end


begin
    test = Test_MessageFactory.new( ARGV )
    test.run
end
