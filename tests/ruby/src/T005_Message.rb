#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_Message < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_constructor
        test_copy_constructor
        test_get_schema_id
        test_get_version
        test_get_schema_level
        test_is_compliant
        test_register_message_validator
        test_set_field_value
        test_set_config
        test_set_subject
        test_set_subject_element
        test_get_subject
        test_set_kind
        test_get_kind
        test_add_field
        test_add_fields
        test_clear_fields
        test_clear_field
        test_get_string_value
        test_get_boolean_value
        test_get_integer_value
        test_get_double_value
        test_has_field
        test_get_field
        test_get_field_type
        test_get_binary_field
        test_get_boolean_field
        test_get_char_field
        test_get_f32_field
        test_get_f64_field
        test_get_i8_field
        test_get_i16_field
        test_get_i32_field
        test_get_i64_field
        test_get_u8_field
        test_get_u16_field
        test_get_u32_field
        test_get_u64_field
        test_get_string_field
        test_get_field_count
        test_copy_fields
        test_to_xml
        test_to_json
        test_get_size
        test_get_field_iterator
	end


    def test_constructor()
        Libgmsec_ruby::Log::info("Test constructor")

        msg = Libgmsec_ruby::Message.new()

        check("Unexpected subject", msg.get_subject() == "")
        check("Unexpected kind", msg.get_kind() == Libgmsec_ruby::Message::Kind_PUBLISH)
        check("Unexpected schema ID", msg.get_schema_id() == "")
        check("Unexpected version", msg.get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Unexpected schema level", msg.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
        check("Unexpected compliancy", msg.is_compliant().has_error() == true)
        check("Unexpected field count", msg.get_field_count() == 0)
        check("Unexpected XML", msg.to_xml() != nil)
        check("Unexpected JSON", msg.to_json() != nil)
	end


    def test_copy_constructor()
        Libgmsec_ruby::Log::info("Test copy-constructor")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Nominal test
        msg2 = Libgmsec_ruby::Message.new(msg1)

        check("Unexpected subject", msg1.get_subject() == msg2.get_subject())
        check("Unexpected kind", msg1.get_kind() == msg2.get_kind())
        check("Unexpected schema ID", msg1.get_schema_id() == msg2.get_schema_id())
        check("Unexpected version", msg1.get_version() == msg2.get_version())
        check("Unexpected schema level", msg1.get_schema_level() == msg2.get_schema_level())
        check("Expected compliant message", msg1.is_compliant().has_error() == msg2.is_compliant().has_error())
        check("Unexpected field count", msg1.get_field_count() == msg2.get_field_count())
        check("Unexpected XML", msg1.to_xml() == msg2.to_xml())
        check("Unexpected JSON", msg1.to_json() == msg2.to_json())
	end


    def test_get_schema_id()
        Libgmsec_ruby::Log::info("Test get_schema_id()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = msgFactory.create_message("MVAL")
        msg3 = msgFactory.create_message("TLMFRAME")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("Message 1 has unexpected schema ID", msg1.get_schema_id() == "MSG.HB")
        check("Message 2 has unexpected schema ID", msg2.get_schema_id() == "MSG.MVAL")
        check("Message 3 has unexpected schema ID", msg3.get_schema_id() == "MSG.TLMFRAME")
	end


    def test_get_version()
        Libgmsec_ruby::Log::info("Test get_version()")

        msgFactory1 = Libgmsec_ruby::MessageFactory::create()
        msgFactory2 = Libgmsec_ruby::MessageFactory::create( Libgmsec_ruby::Config.new("gmsec-specification-version=201900", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )

        msg1 = msgFactory1.create_message("HB")
        msg2 = msgFactory2.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory1)
        Libgmsec_ruby::MessageFactory::destroy(msgFactory2)

        check("Message 1 has unexpected version", msg1.get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Message 2 has unexpected version", msg2.get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00)
	end


    def test_get_schema_level()
        Libgmsec_ruby::Log::info("Test get_schema_level()")

        msgFactory1 = Libgmsec_ruby::MessageFactory::create()
        msgFactory2 = Libgmsec_ruby::MessageFactory::create( Libgmsec_ruby::Config.new("gmsec-schema-level=2", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )

        msg1 = msgFactory1.create_message("HB")
        msg2 = msgFactory2.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory1)
        Libgmsec_ruby::MessageFactory::destroy(msgFactory2)

        check("Message 1 has unexpected version", msg1.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
        check("Message 2 has unexpected version", msg2.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2)
	end


    def test_is_compliant()
        Libgmsec_ruby::Log::info("Test is_compliant()")

        config = Libgmsec_ruby::Config.new( get_config() )   # make a copy
        config.add_value("gmsec-msg-content-validate", "true")

        msgFactory = Libgmsec_ruby::MessageFactory::create(config)

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Nominal test
        check("Expected compliant message", msg.is_compliant().has_error() == false)

        # Off-Nominal test
        msg.clear_field("DOMAIN1")
        check("Expected non-compliant message", msg.is_compliant().has_error() == true)
	end


    def test_register_message_validator()
        Libgmsec_ruby::Log::info("Test register_message_validator()")

        validator = T005_MessageValidator.new()

        config = Libgmsec_ruby::Config.new( get_config() )   # make a copy
        config.add_value("gmsec-msg-content-validate", "true")

        msgFactory = Libgmsec_ruby::MessageFactory::create(config)

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.register_message_validator(validator)

        # Expecting no error
        status = msg.is_compliant()
        check("Expected compliant message", status.has_error() == false)

        # Expecting an error
        status = msg.is_compliant()
        check("Expected non-compliant message", status.has_error() == true)
        check("Expected non-compliant message", status.get_reason() == "Field is missing")

        # Expecting an error
        msg2 = Libgmsec_ruby::Message.new(msg)
        status = msg2.is_compliant()
        check("Expected non-compliant message", status.has_error() == true)
        check("Expected non-compliant message", status.get_reason() == "Field is missing")
	end


    def test_set_field_value()
        Libgmsec_ruby::Log::info("Test set_field_value()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Test with string value
        msg.set_field_value("PUB-RATE", "5")
        check("PUB-RATE has unexpected field type", msg.get_field("PUB-RATE").get_field_type() == Libgmsec_ruby::Field::Type_U16)
        check("PUB-RATE has unexpected field value", msg.get_integer_value("PUB-RATE") == 5)

        # Test with int value
        msg.set_field_value("PUB-RATE", 10)
        check("PUB-RATE has unexpected field type", msg.get_field("PUB-RATE").get_field_type() == Libgmsec_ruby::Field::Type_U16)
        check("PUB-RATE has unexpected field value", msg.get_integer_value("PUB-RATE") == 10)

        # Test with double value
        msg.set_field_value("PUB-RATE", 40.123)
        check("PUB-RATE has unexpected field type", msg.get_field("PUB-RATE").get_field_type() == Libgmsec_ruby::Field::Type_U16)
        check("PUB-RATE has unexpected field value", msg.get_integer_value("PUB-RATE") == 40)

        # Verify result returned is 'true' when replacing a field
        check("Expected return value of true", msg.set_field_value("PUB-RATE", 10) == true)

        # Verify result returned is 'false' when adding a new field
        check("Expected return value of false", msg.set_field_value("PUBLISH-RATE", 10) == false)
	end


    def test_set_config()
        Libgmsec_ruby::Log::info("Test set_config()")

        config = Libgmsec_ruby::Config.new("tracking=false", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.set_config(config)

        check("Message has unexpected Config content", config.to_xml() == msg.get_config().to_xml())
	end


    def test_set_subject()
        Libgmsec_ruby::Log::info("Test set_subject()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.set_subject("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY")

        check("Message has unexpected subject", msg.get_subject() == "GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY")

        # Off-nominal tests
		begin
            # null subject
            tmp = Libgmsec_ruby::Message.new()
            tmp.set_subject(nil)
            check("An exception was expected", false)
		rescue GmsecException => e
            check(e.message, e.message.include?("Subject cannot be NULL, nor an empty string"))
		end

		begin
            # zero-length subject
            tmp = Libgmsec_ruby::Message.new()
            tmp.set_subject("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Subject cannot be NULL, nor an empty string"))
		end

		begin
            # invalid subject content
            tmp = Libgmsec_ruby::Message.new()
            tmp.set_subject("Subject.Cannot.Contain.Lowercase.Letters")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid message subject"))
		end

		begin
            # invalid subject content
            tmp = Libgmsec_ruby::Message.new()
            tmp.set_subject("HELLO%WORLD")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid message subject"))
		end
	end


    def test_set_subject_element()
        Libgmsec_ruby::Log::info("Test set_subject_element()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.set_subject_element("ME4", "FOOEY")

        check("Message has unexpected subject", msg.get_subject() == "C2MS.MY-DOMAIN-1.MY-DOMAIN-2.MY-MISSION.MY-CONSTELLATION-ID.MY-SAT-ID.MSG.TLMPROC.MY-COMPONENT.FILL.FILL.FOOEY")

        # Off-nominal tests
		begin
            msg.set_subject_element("ME9000", "FOOEY")
            check("An exception was expected", false)
		rescue GmsecException => e
            check(e.message, e.message.include?("Message does not have a subject element named ME9000"))
		end
		
		begin
            tmp = Libgmsec_ruby::Message.new()
            tmp.set_subject_element(nil, "FOOEY")
            check("An exception was expected", false)
		rescue GmsecException => e
            check(e.message, e.message.include?("Subject element name cannot be NULL or empty string"))
		end
		
		begin
            tmp = Libgmsec_ruby::Message.new()
            tmp.set_subject_element("", "FOOEY")
            check("An exception was expected", false)
		rescue GmsecException => e
            check(e.message, e.message.include?("Subject element name cannot be NULL or empty string"))
		end
		
		begin
            tmp = Libgmsec_ruby::Message.new()
            tmp.set_subject_element("ME4", "FOOEY")
            check("An exception was expected", false)
		rescue GmsecException => e
            check(e.message, e.message.include?("Message does not have a message template"))
		end
	end


    def test_get_subject()
        Libgmsec_ruby::Log::info("Test get_subject()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")
        msg.add_field( Libgmsec_ruby::StringField.new("STREAM-MODE", "RT") )  # add ME2 (stream mode)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        subject = msg.get_subject()
        subject += ".2"   # add ME2 (virtual channel ID)
        subject += ".1"   # add ME3 (AP ID)

        msg.set_subject(subject)

        check("Message has unexpected subject", msg.get_subject() == subject)
	end


    def test_set_kind()
        Libgmsec_ruby::Log::info("Test set_kind()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")
        check("Expected PUBLISH-kind", msg.get_kind() == Libgmsec_ruby::Message::Kind_PUBLISH)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Set to REQUEST-kind, and verify
        msg.set_kind(Libgmsec_ruby::Message::Kind_REQUEST)
        check("Expected REQUEST-kind", msg.get_kind() == Libgmsec_ruby::Message::Kind_REQUEST)

        # Set to REPLY-kind, and verify
        msg.set_kind(Libgmsec_ruby::Message::Kind_REPLY)
        check("Expected REPLY-kind", msg.get_kind() == Libgmsec_ruby::Message::Kind_REPLY)

        # Set to PUBLISH-kind, and verify
        msg.set_kind(Libgmsec_ruby::Message::Kind_PUBLISH);
        check("Expected PUBLISH-kind", msg.get_kind() == Libgmsec_ruby::Message::Kind_PUBLISH)
    end


    def test_get_kind()
        Libgmsec_ruby::Log::info("Test get_kind()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = msgFactory.create_message("REQ.DIR")
        msg3 = msgFactory.create_message("RESP.DIR")
        msg4 = msgFactory.create_message()
        msg5 = Libgmsec_ruby::Message.new()

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("Message 1 has unexpected kind", msg1.get_kind() == Libgmsec_ruby::Message::Kind_PUBLISH)
        check("Message 2 has unexpected kind", msg2.get_kind() == Libgmsec_ruby::Message::Kind_REQUEST)
        check("Message 3 has unexpected kind", msg3.get_kind() == Libgmsec_ruby::Message::Kind_REPLY)
        check("Message 4 has unexpected kind", msg4.get_kind() == Libgmsec_ruby::Message::Kind_PUBLISH)
        check("Message 5 has unexpected kind", msg5.get_kind() == Libgmsec_ruby::Message::Kind_PUBLISH)
	end


    def test_add_field()
        Libgmsec_ruby::Log::info("Test add_field()")

        field = Libgmsec_ruby::StringField.new("SPECIFICATION", "C2MS")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("LOG")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # replace existing field
        check("Expected field to be replaced", msg.add_field(field) == true)

        # test other overloaded methods
		data = []
		for i in 0..255 do
			data << (i % 256)
		end
        blob = data.pack('C*').force_encoding('BINARY')

        check("Expected field to be added", msg.add_field( Libgmsec_ruby::BinaryField.new("BINARY", blob, blob.length) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::BooleanField.new("BOOLEAN", true) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::CharField.new("CHAR", 'c') ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::F32Field.new("FLOAT", 1) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::F64Field.new("DOUBLE", 2.4) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::I8Field.new("BYTE", 5) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::I16Field.new("SHORT", 55) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::I32Field.new("INT", 555) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::I64Field.new("LONG", 5555) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::StringField.new("STRING", "The old fox") ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::U8Field.new("UBYTE", 5) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::U16Field.new("USHORT", 55) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::U32Field.new("UINT", 555) ) == false)
        check("Expected field to be added", msg.add_field( Libgmsec_ruby::U64Field.new("ULONG", 5555) ) == false)

        # validate field settings
        check("SPECIFICATION not found", msg.has_field("SPECIFICATION"))
        check("SPECIFICATION has wrong type", msg.get_field("SPECIFICATION").get_field_type() == Libgmsec_ruby::Field::Type_STRING)
        check("SPECIFICATION has wrong value", msg.get_string_value("SPECIFICATION") == "C2MS")

        check("BINARY not found", msg.has_field("BINARY"))
        check("BINARY has wrong type", msg.get_field("BINARY").get_field_type() == Libgmsec_ruby::Field::Type_BINARY)
        check("BINARY has wrong length", msg.get_binary_field("BINARY").get_length() == blob.length)

        check("BOOLEAN not found", msg.has_field("BOOLEAN"))
        check("BOOLEAN has wrong type", msg.get_field("BOOLEAN").get_field_type() == Libgmsec_ruby::Field::Type_BOOL)
        check("BOOLEAN has wrong value", msg.get_boolean_field("BOOLEAN").get_value() == true)

        check("CHAR not found", msg.has_field("CHAR"))
        check("CHAR has wrong type", msg.get_field("CHAR").get_field_type() == Libgmsec_ruby::Field::Type_CHAR)
        check("CHAR has wrong value", msg.get_char_field("CHAR").get_value() == 'c')

        check("FLOAT not found", msg.has_field("FLOAT"))
        check("FLOAT has wrong type", msg.get_field("FLOAT").get_field_type() == Libgmsec_ruby::Field::Type_F32)
        check("FLOAT has wrong value", msg.get_f32_field("FLOAT").get_value() == 1)

        check("DOUBLE not found", msg.has_field("DOUBLE"))
        check("DOUBLE has wrong type", msg.get_field("DOUBLE").get_field_type() == Libgmsec_ruby::Field::Type_F64)
        check("DOUBLE has wrong value", msg.get_f64_field("DOUBLE").get_value() == 2.4)

        check("BYTE not found", msg.has_field("BYTE"))
        check("BYTE has wrong type", msg.get_field("BYTE").get_field_type() == Libgmsec_ruby::Field::Type_I8)
        check("BYTE has wrong value", msg.get_i8_field("BYTE").get_value() == 5)

        check("SHORT not found", msg.has_field("SHORT"))
        check("SHORT has wrong type", msg.get_field("SHORT").get_field_type() == Libgmsec_ruby::Field::Type_I16)
        check("SHORT has wrong value", msg.get_i16_field("SHORT").get_value() == 55)

        check("INT not found", msg.has_field("INT"))
        check("INT has wrong type", msg.get_field("INT").get_field_type() == Libgmsec_ruby::Field::Type_I32)
        check("INT has wrong value", msg.get_i32_field("INT").get_value() == 555)

        check("LONG not found", msg.has_field("LONG"))
        check("LONG has wrong type", msg.get_field("LONG").get_field_type() == Libgmsec_ruby::Field::Type_I64)
        check("LONG has wrong value", msg.get_i64_field("LONG").get_value() == 5555)

        check("STRING not found", msg.has_field("STRING"))
        check("STRING has wrong type", msg.get_field("STRING").get_field_type() == Libgmsec_ruby::Field::Type_STRING)
        check("STRING has wrong value", msg.get_string_field("STRING").get_value() == "The old fox")

        check("UBYTE not found", msg.has_field("UBYTE"))
        check("UBYTE has wrong type", msg.get_field("UBYTE").get_field_type() == Libgmsec_ruby::Field::Type_U8)
        check("UBYTE has wrong value", msg.get_u8_field("UBYTE").get_value() == 5)

        check("USHORT not found", msg.has_field("USHORT"))
        check("USHORT has wrong type", msg.get_field("USHORT").get_field_type() == Libgmsec_ruby::Field::Type_U16)
        check("USHORT has wrong value", msg.get_u16_field("USHORT").get_value() == 55)

        check("UINT not found", msg.has_field("UINT"))
        check("UINT has wrong type", msg.get_field("UINT").get_field_type() == Libgmsec_ruby::Field::Type_U32)
        check("UINT has wrong value", msg.get_u32_field("UINT").get_value() == 555)

        check("ULONG not found", msg.has_field("ULONG"))
        check("ULONG has wrong type", msg.get_field("ULONG").get_field_type() == Libgmsec_ruby::Field::Type_U64)
        check("ULONG has wrong value", msg.get_u64_field("ULONG").get_value() == 5555)
	end


    def test_add_fields()
        Libgmsec_ruby::Log::info("Test add_fields()")

        field1 = Libgmsec_ruby::StringField.new("ONE", "uno")
        field2 = Libgmsec_ruby::StringField.new("TWO", "dos")
        field3 = Libgmsec_ruby::StringField.new("THREE", "tres")

        fields = Libgmsec_ruby::FieldArray.new()
        fields << field1
        fields << field2
        fields << field3

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("LOG")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("Expected only Libgmsec_ruby::fields to be added", false == msg.add_fields(fields))

        check("Expected to have field ONE", true == msg.has_field("ONE"))
        check("Unexpected field type for ONE", Libgmsec_ruby::Field::Type_STRING == msg.get_field("ONE").get_field_type())
        check("Unexpected field value for ONE", msg.get_string_value("ONE") == "uno")

        check("Expected to have field TWO", true == msg.has_field("TWO"))
        check("Unexpected field type for TWO", Libgmsec_ruby::Field::Type_STRING == msg.get_field("TWO").get_field_type())
        check("Unexpected field value for TWO", msg.get_string_value("TWO") == "dos")

        check("Expected to have field THREE", true == msg.has_field("THREE"))
        check("Unexpected field type for THREE", Libgmsec_ruby::Field::Type_STRING == msg.get_field("THREE").get_field_type())
        check("Unexpected field value for THREE", msg.get_string_value("THREE") == "tres")
	end


    def test_clear_fields()
        Libgmsec_ruby::Log::info("Test clear_fields()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("Message missing CONTENT-VERSION", true == msg.has_field("CONTENT-VERSION"))
        check("Message missing HEADER-VERSION", true == msg.has_field("HEADER-VERSION"))
        check("Message missing MESSAGE-SUBTYPE", true == msg.has_field("MESSAGE-SUBTYPE"))
        check("Message missing MESSAGE-TYPE", true == msg.has_field("MESSAGE-TYPE"))
        check("Message missing SPECIFICATION", true == msg.has_field("SPECIFICATION"))

        msg.clear_fields()

        check("Message has CONTENT-VERSION", false == msg.has_field("CONTENT-VERSION"))
        check("Message has HEADER-VERSION", false == msg.has_field("HEADER-VERSION"))
        check("Message has MESSAGE-SUBTYPE", false == msg.has_field("MESSAGE-SUBTYPE"))
        check("Message has MESSAGE-TYPE", false == msg.has_field("MESSAGE-TYPE"))
        check("Message has SPECIFICATION", false == msg.has_field("SPECIFICATION"))
	end


    def test_clear_field()
        Libgmsec_ruby::Log::info("Test clear_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("CONTENT-VERSION not found in message", msg.has_field("CONTENT-VERSION"))

        check("CONTENT-VERSION not cleared from message", msg.clear_field("CONTENT-VERSION") == true)
        check("FOO-BAR does not exist and cannot be cleared", msg.clear_field("FOO-BAR") == false)
        check("foo-bar does not exist and cannot be cleared", msg.clear_field("foo-bar") == false)

        check("CONTENT-VERSION should not exist in message", msg.has_field("CONTENT-VERSION") == false)
	end


    def test_get_string_value()
        Libgmsec_ruby::Log::info("Test get_string_value()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.set_field_value("PUB-RATE", "30")

        check("MESSAGE-TYPE has unexpected value", msg.get_string_value("MESSAGE-TYPE") == "MSG")
        check("PUB-RATE has unexpected value", msg.get_string_value("PUB-RATE") == "30")
	end


    def test_get_boolean_value()
        Libgmsec_ruby::Log::info("Test get_boolean_value()")

        msg = Libgmsec_ruby::Message.new()

        msg.add_field( Libgmsec_ruby::I16Field.new("FIELD-1", 0) )
        msg.add_field( Libgmsec_ruby::I16Field.new("FIELD-2", 1) )
        msg.add_field( Libgmsec_ruby::StringField.new("FIELD-3", "10000") )
        msg.add_field( Libgmsec_ruby::I32Field.new("FIELD-4", 10000) )
        msg.add_field( Libgmsec_ruby::BooleanField.new("FIELD-5", true) )
        msg.add_field( Libgmsec_ruby::BooleanField.new("FIELD-6", false) )
        msg.add_field( Libgmsec_ruby::F64Field.new("FIELD-7", 2019.0) )
        msg.add_field( Libgmsec_ruby::StringField.new("FIELD-8", "true") )
        msg.add_field( Libgmsec_ruby::StringField.new("FIELD-9", "FALSE") )
        msg.add_field( Libgmsec_ruby::StringField.new("FIELD-0", "foo bar") )

        # Nominal tests
        check("1 Expected boolean value of false", msg.get_boolean_value("FIELD-1") == false)
        check("2 Expected boolean value of true",  msg.get_boolean_value("FIELD-2"))
        check("3 Expected boolean value of true",  msg.get_boolean_value("FIELD-3"))
        check("4 Expected boolean value of true",  msg.get_boolean_value("FIELD-4"))
        check("5 Expected boolean value of true",  msg.get_boolean_value("FIELD-5"))
        check("6 Expected boolean value of false", msg.get_boolean_value("FIELD-6") == false)
        check("7 Expected boolean value of true",  msg.get_boolean_value("FIELD-7"))
        check("8 Expected boolean value of true",  msg.get_boolean_value("FIELD-8"))
        check("9 Expected boolean value of false", msg.get_boolean_value("FIELD-9") == false)

        # Off-nominal tests
        # Field cannot be converted
		begin
            msg.get_boolean_value("FIELD-0")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field cannot be represented as a boolean"))
		end

        # Unknown field
		begin
            msg.get_boolean_value("BOGUS")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain field with name: BOGUS"))
		end

        # nil field name
		begin
            msg.get_boolean_value(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # empty-string field name
		begin
            msg.get_boolean_value("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_integer_value()
        Libgmsec_ruby::Log::info("Test get_integer_value()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.set_field_value("PUB-RATE", "30")

        check("PUB-RATE has unexpected value", msg.get_integer_value("PUB-RATE") == 30)

        # Off-nominal test
		begin
            msg.get_integer_value("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field cannot be converted to a GMSEC_I64"))
		end
	end


    def test_get_double_value()
        Libgmsec_ruby::Log::info("Test get_double_value()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.set_field_value("PUB-RATE", "30")

        check("PUB-RATE has unexpected value", msg.get_double_value("PUB-RATE") == 30)

        # Off-nominal test
		begin
            msg.get_double_value("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field cannot be converted to a GMSEC_F64"))
		end
	end


    def test_has_field()
        Libgmsec_ruby::Log::info("Test has_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.set_field_value("PUB-RATE", "30")

        check("Message should have PUB-RATE", msg.has_field("PUB-RATE"))
        check("Message should not have PUBLISH-RATE", msg.has_field("PUBLISH-RATE") == false)
        check("Message should not have null field", msg.has_field(nil) == false)
        check("Message should not have empty string field", msg.has_field("") == false)
	end


    def test_get_field()
        Libgmsec_ruby::Log::info("Test get_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Use valid field name
        check("Message should have MESSAGE-TYPE", msg.get_field("MESSAGE-TYPE") != nil)

        # Use bogus field name
        check("Message should have MSG-TYPE", msg.get_field("MSG-TYPE") == nil)

        # Use null field name
		begin
            msg.get_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_field_type()
        Libgmsec_ruby::Log::info("Test get_field_type()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("MESSAGE-TYPE has unexpected field type", msg.get_field_type("MESSAGE-TYPE") == Libgmsec_ruby::Field::Type_STRING)
        check("CONTENT-VERSION has unexpected field type", msg.get_field_type("CONTENT-VERSION") == Libgmsec_ruby::Field::Type_F32)

        # Use bogus field name
		begin
            msg.get_field_type("MSG-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain field with name: MSG-TYPE"))
		end

        # Use nil field name
		begin
            msg.get_field_type(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_field_type("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_binary_field()
        Libgmsec_ruby::Log::info("Test get_binary_field()")

		data = []
		for i in 0..255 do
			data << (i % 256)
		end
        blob = data.pack('C*').force_encoding('BINARY')

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("MSG.TLMPKT")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::BinaryField.new("DATA", blob, blob.length) )

        # Use valid field name
        field = msg.get_binary_field("DATA")
        require("Expected to get binary field", field != nil)
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_BINARY)
        check("Unexpected field name", field.get_field_name() == "DATA")
        check("Unexpected data length", field.get_length() == blob.length)

        # Use non-binary field name
		begin
            msg.get_binary_field("CONTENT-VERSION")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain BinaryField with name: CONTENT-VERSION"))
		end

        # Use null field name
		begin
            msg.get_binary_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_binary_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_boolean_field()
        Libgmsec_ruby::Log::info("Test get_boolean_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::BooleanField.new("RESPONSE", true) )

        # Use valid field name
        field = msg.get_boolean_field("RESPONSE")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_BOOL)
        check("Unexpected field name", field.get_field_name() == "RESPONSE")
        check("Unexpected data value", field.get_value() == true)

        # Use non-boolean field name
		begin
            msg.get_boolean_field("CONTENT-VERSION")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain BooleanField with name: CONTENT-VERSION"))
		end

        # Use null field name
		begin
            msg.get_boolean_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_boolean_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_char_field()
        Libgmsec_ruby::Log::info("Test get_char_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::CharField.new("CHAR-FIELD", 'c') )

        # Use valid field name
        field = msg.get_char_field("CHAR-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_CHAR)
        check("Unexpected field name", field.get_field_name() == "CHAR-FIELD")
        check("Unexpected data value", field.get_value() == 'c')

        # Use non-char field name
		begin
            msg.get_char_field("CONTENT-VERSION")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain CharField with name: CONTENT-VERSION"))
		end

        # Use null field name
		begin
            msg.get_char_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_char_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_f32_field()
        Libgmsec_ruby::Log::info("Test get_f32_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Use valid field name
        field = msg.get_f32_field("CONTENT-VERSION")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_F32)
        check("Unexpected field name", field.get_field_name() == "CONTENT-VERSION")

        # Use non-F32 field name
		begin
            msg.get_f32_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain F32Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_f32_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_f32_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_f64_field()
        Libgmsec_ruby::Log::info("Test get_f64_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::F64Field.new("F64-FIELD", 3.14) )

        # Use valid field name
        field = msg.get_f64_field("F64-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_F64)
        check("Unexpected field name", field.get_field_name() == "F64-FIELD")
        check("Unexpected field value", field.get_value() == 3.14)

        # Use non-F64 field name
		begin
            msg.get_f64_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain F64Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_f64_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_f64_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_i8_field()
        Libgmsec_ruby::Log::info("Test get_i8_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::I8Field.new("I8-FIELD", 8) )

        # Use valid field name
        field = msg.get_i8_field("I8-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_I8)
        check("Unexpected field name", field.get_field_name() == "I8-FIELD")
        check("Unexpected field value", field.get_value() == 8)

        # Use non-I8 field name
		begin
            msg.get_i8_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain I8Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_i8_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_i8_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_i16_field()
        Libgmsec_ruby::Log::info("Test get_i16_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::I16Field.new("I16-FIELD", 16) )

        # Use valid field name
        field = msg.get_i16_field("I16-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_I16)
        check("Unexpected field name", field.get_field_name() == "I16-FIELD")
        check("Unexpected field value", field.get_value() == 16)

        # Use non-I16 field name
		begin
            msg.get_i16_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain I16Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_i16_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_i16_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_i32_field()
        Libgmsec_ruby::Log::info("Test get_i32_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::I32Field.new("I32-FIELD", 32) )

        # Use valid field name
        field = msg.get_i32_field("I32-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_I32)
        check("Unexpected field name", field.get_field_name() == "I32-FIELD")
        check("Unexpected field value", field.get_value() == 32)

        # Use non-I32 field name
		begin
            msg.get_i32_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain I32Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_i32_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_i32_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_i64_field()
        Libgmsec_ruby::Log::info("Test get_i64_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::I64Field.new("I64-FIELD", 64) )

        # Use valid field name
        field = msg.get_i64_field("I64-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_I64)
        check("Unexpected field name", field.get_field_name() == "I64-FIELD")
        check("Unexpected field value", field.get_value() == 64)

        # Use non-I64 field name
		begin
            msg.get_i64_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain I64Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_i64_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_i64_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_u8_field()
        Libgmsec_ruby::Log::info("Test get_u8_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::U8Field.new("U8-FIELD", 8) )

        # Use valid field name
        field = msg.get_u8_field("U8-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_U8)
        check("Unexpected field name", field.get_field_name() == "U8-FIELD")
        check("Unexpected field value", field.get_value() == 8)

        # Use non-U8 field name
		begin
            msg.get_u8_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain U8Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_u8_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_u8_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_u16_field()
        Libgmsec_ruby::Log::info("Test get_u16_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::U16Field.new("U16-FIELD", 16) )

        # Use valid field name
        field = msg.get_u16_field("U16-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_U16)
        check("Unexpected field name", field.get_field_name() == "U16-FIELD")
        check("Unexpected field value", field.get_value() == 16)

        # Use non-U16 field name
		begin
            msg.get_u16_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain U16Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_u16_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_u16_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_u32_field()
        Libgmsec_ruby::Log::info("Test get_u32_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::U32Field.new("U32-FIELD", 32) )

        # Use valid field name
        field = msg.get_u32_field("U32-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_U32)
        check("Unexpected field name", field.get_field_name() == "U32-FIELD")
        check("Unexpected field value", field.get_value() == 32)

        # Use non-U32 field name
		begin
            msg.get_u32_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain U32Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_u32_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_u32_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_u64_field()
        Libgmsec_ruby::Log::info("Test get_i64_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg.add_field( Libgmsec_ruby::U64Field.new("U64-FIELD", 64) )

        # Use valid field name
        field = msg.get_u64_field("U64-FIELD")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_U64)
        check("Unexpected field name", field.get_field_name() == "U64-FIELD")
        check("Unexpected field value", field.get_value() == 64)

        # Use non-U64 field name
		begin
            msg.get_u64_field("MESSAGE-TYPE")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain U64Field with name: MESSAGE-TYPE"))
		end

        # Use null field name
		begin
            msg.get_u64_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_u64_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_string_field()
        Libgmsec_ruby::Log::info("Test get_string_field()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("REQ.DIR")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        # Use valid field name
        field = msg.get_string_field("MESSAGE-TYPE")
        check("Unexpected field type", field.get_field_type() == Libgmsec_ruby::Field::Type_STRING)
        check("Unexpected field name", field.get_field_name() == "MESSAGE-TYPE")
        check("Unexpected field value", field.get_value() == "REQ")

        # Use non-string field name
		begin
            msg.get_string_field("CONTENT-VERSION")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message does not contain StringField with name: CONTENT-VERSION"))
		end

        # Use null field name
		begin
            msg.get_string_field(nil)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end

        # Use empty-string field name
		begin
            msg.get_string_field("")
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name cannot be NULL, nor an empty string"))
		end
	end


    def test_get_field_count()
        Libgmsec_ruby::Log::info("Test get_field_count()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("Message has unexpected field count", msg.get_field_count() == 13)

        msg.set_field_value("PUB-RATE", "30")

        check("Message has unexpected field count", msg.get_field_count() == 14)

        msg.clear_field("PUB-RATE")

        check("Message has unexpected field count", msg.get_field_count() == 13)

        msg.clear_fields()

        check("Message has unexpected field count", msg.get_field_count() == 0)
	end


    def test_copy_fields()
        Libgmsec_ruby::Log::info("Test copy_fields()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg1 = msgFactory.create_message("HB")
        msg2 = Libgmsec_ruby::Message.new(msg1)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        msg1.set_field_value("PUB-RATE", "30")

        check("Message 2 should not have PUB-RATE", msg2.has_field("PUB-RATE") == false)

        msg1.copy_fields(msg2)

        check("Message 2 should have PUB-RATE", msg2.has_field("PUB-RATE"))
	end


    def test_to_xml()
        Libgmsec_ruby::Log::info("Test to_xml()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = Libgmsec_ruby::Message.new(msg1)

        check("Message 1 has NULL XML content", msg1.to_xml() != nil)
        check("Message 2 has NULL XML content", msg2.to_xml() != nil)
        check("Messages 1 and 2 do not have same XML content", msg1.to_xml() == msg2.to_xml())

        msg3 = msgFactory.from_data(msg1.to_xml(), Libgmsec_ruby::DataType_XML_DATA)
        check("Messages 1 and 3 do not have same XML content", msg1.to_xml() == msg3.to_xml())

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_to_json()
        Libgmsec_ruby::Log::info("Test to_json()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = Libgmsec_ruby::Message.new(msg1)

        check("Message 1 has NULL JSON content", msg1.to_json() != nil)
        check("Message 2 has NULL JSON content", msg2.to_json() != nil)
        check("Messages 1 and 2 do not have same JSON content", msg1.to_json() == msg2.to_json())

        msg3 = msgFactory.from_data(msg1.to_json(), Libgmsec_ruby::DataType_JSON_DATA)
        check("Messages 1 and 3 do not have same JSON content", msg1.to_json() == msg3.to_json())

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)
	end


    def test_get_size()
        Libgmsec_ruby::Log::info("Test get_size()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg1 = msgFactory.create_message("HB")
        msg2 = Libgmsec_ruby::Message.new(msg1)

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        check("Message 1 has unexpected size", msg1.get_size() > 0)
        check("Message 2 has unexpected size", msg2.get_size() > 0)
        check("Messages should have the same size", msg1.get_size() == msg2.get_size())
	end


    def test_get_field_iterator()
        Libgmsec_ruby::Log::info("Test get_field_iterator()")

        msgFactory = Libgmsec_ruby::MessageFactory::create()

        msg = msgFactory.create_message("HB")

        Libgmsec_ruby::MessageFactory::destroy(msgFactory)

        iter = msg.get_field_iterator()
        check("Got the field iterator", true)
	end
end


class T005_MessageValidator < Libgmsec_ruby::MessageValidator

    def initialize
        super()
        @iter = 0
	end

    def validate_message(msg)
        status = Libgmsec_ruby::Status.new()

        if @iter > 0
            status = Libgmsec_ruby::Status.new(Libgmsec_ruby::MSG_ERROR, Libgmsec_ruby::MESSAGE_FAILED_VALIDATION, "Field is missing")
		end

        @iter += 1

        return status
	end

    def reset
        @iter = 0
	end
end


begin
    test = Test_Message.new( ARGV )
    test.run
end
