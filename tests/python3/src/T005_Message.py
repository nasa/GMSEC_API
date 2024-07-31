#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_Message(Test):

    def test(self):
        self.test_constructor()
        self.test_copy_constructor()
        self.test_get_schema_id()
        self.test_get_version()
        self.test_get_schema_level()
        self.test_is_compliant()
        self.test_register_message_validator()
        self.test_set_field_value()
        self.test_set_config()
        self.test_set_subject()
        self.test_set_subject_element()
        self.test_get_subject()
        self.test_set_kind()
        self.test_get_kind()
        self.test_add_field()
        self.test_add_fields()
        self.test_clear_fields()
        self.test_clear_field()
        self.test_get_string_value()
        self.test_get_boolean_value()
        self.test_get_integer_value()
        self.test_get_double_value()
        self.test_has_field()
        self.test_get_field()
        self.test_get_field_type()
        self.test_get_binary_field()
        self.test_get_boolean_field()
        self.test_get_char_field()
        self.test_get_f32_field()
        self.test_get_f64_field()
        self.test_get_i8_field()
        self.test_get_i16_field()
        self.test_get_i32_field()
        self.test_get_i64_field()
        self.test_get_u8_field()
        self.test_get_u16_field()
        self.test_get_u32_field()
        self.test_get_u64_field()
        self.test_get_string_field()
        self.test_get_field_count()
        self.test_copy_fields()
        self.test_to_xml()
        self.test_to_json()
        self.test_get_size()
        self.test_get_field_iterator()


    def test_constructor(self):
        lp.log_info("Test constructor")

        msg = lp.Message()

        self.check("Unexpected subject", msg.get_subject() == "")
        self.check("Unexpected kind", msg.get_kind() == lp.Message.Kind_PUBLISH)
        self.check("Unexpected schema ID", msg.get_schema_id() == "")
        self.check("Unexpected version", msg.get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Unexpected schema level", msg.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_0)
        self.check("Unexpected compliancy", msg.is_compliant().has_error() == True)
        self.check("Unexpected field count", msg.get_field_count() == 0)
        self.check("Unexpected XML", msg.to_xml() != None)
        self.check("Unexpected JSON", msg.to_json() != None)


    def test_copy_constructor(self):
        lp.log_info("Test copy-constructor")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")

        # Nominal test
        msg2 = lp.Message(msg1)

        self.check("Unexpected subject", msg1.get_subject() == msg2.get_subject())
        self.check("Unexpected kind", msg1.get_kind() == msg2.get_kind())
        self.check("Unexpected schema ID", msg1.get_schema_id() == msg2.get_schema_id())
        self.check("Unexpected version", msg1.get_version() == msg2.get_version())
        self.check("Unexpected schema level", msg1.get_schema_level() == msg2.get_schema_level())
        self.check("Expected compliant message", msg1.is_compliant().has_error() == msg2.is_compliant().has_error())
        self.check("Unexpected field count", msg1.get_field_count() == msg2.get_field_count())
        self.check("Unexpected XML", msg1.to_xml() == msg2.to_xml())
        self.check("Unexpected JSON", msg1.to_json() == msg2.to_json())


    def test_get_schema_id(self):
        lp.log_info("Test get_schema_id()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = msgFactory.create_message("MVAL")
        msg3 = msgFactory.create_message("TLMFRAME")

        self.check("Message 1 has unexpected schema ID", msg1.get_schema_id() == "MSG.HB")
        self.check("Message 2 has unexpected schema ID", msg2.get_schema_id() == "MSG.MVAL")
        self.check("Message 3 has unexpected schema ID", msg3.get_schema_id() == "MSG.TLMFRAME")


    def test_get_version(self):
        lp.log_info("Test get_version()")

        msgFactory1 = lp.MessageFactory()
        msgFactory2 = lp.MessageFactory(lp.Config("gmsec-specification-version=201900", lp.DataType_KEY_VALUE_DATA))

        msg1 = msgFactory1.create_message("HB")
        msg2 = msgFactory2.create_message("HB")

        self.check("Message 1 has unexpected version", msg1.get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Message 2 has unexpected version", msg2.get_version() == lp.GMSEC_MSG_SPEC_2019_00)


    def test_get_schema_level(self):
        lp.log_info("Test get_schema_level()")

        msgFactory1 = lp.MessageFactory()
        msgFactory2 = lp.MessageFactory(lp.Config("gmsec-schema-level=2", lp.DataType_KEY_VALUE_DATA))

        msg1 = msgFactory1.create_message("HB")
        msg2 = msgFactory2.create_message("HB")

        self.check("Message 1 has unexpected version", msg1.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_0)
        self.check("Message 2 has unexpected version", msg2.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_2)


    def test_is_compliant(self):
        lp.log_info("Test is_compliant()")

        config = lp.Config( self.get_config() )   # make a copy
        config.add_value("gmsec-msg-content-validate", "True")

        msgFactory = lp.MessageFactory(config)

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        # Nominal test
        self.check("Expected compliant message", msg.is_compliant().has_error() == False)

        # Off-Nominal test
        msg.clear_field("DOMAIN1")
        self.check("Expected non-compliant message", msg.is_compliant().has_error() == True)


    def test_register_message_validator(self):
        lp.log_info("Test register_message_validator()")

        validator = T005_MessageValidator()

        config = lp.Config( self.get_config() )   # make a copy
        config.add_value("gmsec-msg-content-validate", "True")

        msgFactory = lp.MessageFactory(config)

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        msg.register_message_validator(validator)

        # Expecting no error
        status = msg.is_compliant()
        self.check("Expected compliant message", status.has_error() == False)

        # Expecting an error
        status = msg.is_compliant()
        self.check("Expected non-compliant message", status.has_error() == True)
        self.check("Expected non-compliant message", status.get_reason() == "Field is missing")

        # Expecting an error
        msg2 = lp.Message(msg)
        status = msg2.is_compliant()
        self.check("Expected non-compliant message", status.has_error() == True)
        self.check("Expected non-compliant message", status.get_reason() == "Field is missing")


    def test_set_field_value(self):
        lp.log_info("Test set_field_value()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        # Test with string value
        msg.set_field_value("PUB-RATE", "5")
        self.check("PUB-RATE has unexpected field type", msg.get_field("PUB-RATE").get_type() == lp.Field.Type_U16)
        self.check("PUB-RATE has unexpected field value", msg.get_integer_value("PUB-RATE") == 5)

        # Test with int value
        msg.set_field_value("PUB-RATE", 10)
        self.check("PUB-RATE has unexpected field type", msg.get_field("PUB-RATE").get_type() == lp.Field.Type_U16)
        self.check("PUB-RATE has unexpected field value", msg.get_integer_value("PUB-RATE") == 10)

        # Test with double value
        msg.set_field_value("PUB-RATE", 40.123)
        self.check("PUB-RATE has unexpected field type", msg.get_field("PUB-RATE").get_type() == lp.Field.Type_U16)
        self.check("PUB-RATE has unexpected field value", msg.get_integer_value("PUB-RATE") == 40)

        # Verify result returned is 'True' when replacing a field
        self.check("Expected return value of True", msg.set_field_value("PUB-RATE", 10) == True)

        # Verify result returned is 'False' when adding a new field
        self.check("Expected return value of False", msg.set_field_value("PUBLISH-RATE", 10) == False)


    def test_set_config(self):
        lp.log_info("Test set_config()")

        config = lp.Config("tracking=False", lp.DataType_KEY_VALUE_DATA)

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")

        msg.set_config(config)

        self.check("Message has unexpected Config content", config.to_xml() == msg.get_config().to_xml())


    def test_set_subject(self):
        lp.log_info("Test set_subject()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")

        msg.set_subject("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY")

        self.check("Message has unexpected subject", msg.get_subject() == "GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY")

        # Off-nominal tests
        try:
            # null subject
            tmp = lp.Message()
            tmp.set_subject(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Subject cannot be NULL, nor an empty string" in str(e))

        try:
            # zero-length subject
            tmp = lp.Message()
            tmp.set_subject("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Subject cannot be NULL, nor an empty string" in str(e))

        try:
            # invalid subject content
            tmp = lp.Message()
            tmp.set_subject("Subject.Cannot.Contain.Lowercase.Letters")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Invalid message subject" in str(e))

        try:
            # invalid subject content
            tmp = lp.Message()
            tmp.set_subject("HELLO%WORLD")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Invalid message subject" in str(e))


    def test_set_subject_element(self):
        lp.log_info("Test set_subject_element()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")

        msg.set_subject_element("ME4", "FOOEY")

        self.check("Message has unexpected subject", msg.get_subject() == "C2MS.MY-DOMAIN-1.MY-DOMAIN-2.MY-MISSION.MY-CONSTELLATION.MY-SAT-ID.MSG.TLMPROC.MY-COMPONENT.FILL.FILL.FOOEY")

        # Off-nominal tests
        try:
            msg.set_subject_element("ME9000", "FOOEY")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not have a subject element named ME9000" in str(e))
            
        try:
            tmp = lp.Message()
            tmp.set_subject_element(None, "FOOEY")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Subject element name cannot be NULL or empty string" in str(e))
            
        try:
            tmp = lp.Message()
            tmp.set_subject_element("", "FOOEY")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Subject element name cannot be NULL or empty string" in str(e))
            
        try:
            tmp = lp.Message()
            tmp.set_subject_element("ME4", "FOOEY")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not have a message template" in str(e))



    def test_get_subject(self):
        lp.log_info("Test get_subject()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("TLMPROC")
        msg.add_field( lp.StringField("STREAM-MODE", "RT") )  # add ME2 (stream mode)

        subject = msg.get_subject()
        subject += ".2"   # add ME2 (virtual channel ID)
        subject += ".1"   # add ME3 (AP ID)

        msg.set_subject(subject)

        self.check("Message has unexpected subject", msg.get_subject() == subject)


    def test_set_kind(self):
        lp.log_info("Test set_kind()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")
        self.check("Expected PUBLISH-kind", msg.get_kind() == lp.Message.Kind_PUBLISH)

        # Set to REQUEST-kind, and verify
        msg.set_kind(lp.Message.Kind_REQUEST)
        self.check("Expected REQUEST-kind", msg.get_kind() == lp.Message.Kind_REQUEST)

        # Set to REPLY-kind, and verify
        msg.set_kind(lp.Message.Kind_REPLY)
        self.check("Expected REPLY-kind", msg.get_kind() == lp.Message.Kind_REPLY)

        # Set to PUBLISH-kind, and verify
        msg.set_kind(lp.Message.Kind_PUBLISH)
        self.check("Expected PUBLISH-kind", msg.get_kind() == lp.Message.Kind_PUBLISH)


    def test_get_kind(self):
        lp.log_info("Test get_kind()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = msgFactory.create_message("REQ.DIR")
        msg3 = msgFactory.create_message("RESP.DIR")
        msg4 = msgFactory.create_message()
        msg5 = lp.Message()

        self.check("Message 1 has unexpected kind", msg1.get_kind() == lp.Message.Kind_PUBLISH)
        self.check("Message 2 has unexpected kind", msg2.get_kind() == lp.Message.Kind_REQUEST)
        self.check("Message 3 has unexpected kind", msg3.get_kind() == lp.Message.Kind_REPLY)
        self.check("Message 4 has unexpected kind", msg4.get_kind() == lp.Message.Kind_PUBLISH)
        self.check("Message 5 has unexpected kind", msg5.get_kind() == lp.Message.Kind_PUBLISH)


    def test_add_field(self):
        lp.log_info("Test add_field()")

        field = lp.StringField("SPECIFICATION", "C2MS")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("LOG")

        # replace existing field
        self.check("Expected field to be replaced", msg.add_field(field) == True)

        # test other overloaded methods
        blob = bytearray()
        for i in range(0,255):
            blob.append(i)

        self.check("Expected field to be added", msg.add_field( lp.BinaryField("BINARY", blob, len(blob)) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.BooleanField("BOOLEAN", True) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.CharField("CHAR", 'c') ) == False)
        self.check("Expected field to be added", msg.add_field( lp.F32Field("FLOAT", 1) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.F64Field("DOUBLE", 2.4) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.I8Field("BYTE", 5) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.I16Field("SHORT", 55) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.I32Field("INT", 555) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.I64Field("LONG", 5555) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.StringField("STRING", "The old fox") ) == False)
        self.check("Expected field to be added", msg.add_field( lp.U8Field("UBYTE", 5) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.U16Field("USHORT", 55) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.U32Field("UINT", 555) ) == False)
        self.check("Expected field to be added", msg.add_field( lp.U64Field("ULONG", 5555) ) == False)

        # validate field settings
        self.check("SPECIFICATION not found", msg.has_field("SPECIFICATION"))
        self.check("SPECIFICATION has wrong type", msg.get_field("SPECIFICATION").get_type() == lp.Field.Type_STRING)
        self.check("SPECIFICATION has wrong value", msg.get_string_value("SPECIFICATION") == "C2MS")

        self.check("BINARY not found", msg.has_field("BINARY"))
        self.check("BINARY has wrong type", msg.get_field("BINARY").get_type() == lp.Field.Type_BINARY)
        self.check("BINARY has wrong length", msg.get_binary_field("BINARY").get_length() == len(blob))

        self.check("BOOLEAN not found", msg.has_field("BOOLEAN"))
        self.check("BOOLEAN has wrong type", msg.get_field("BOOLEAN").get_type() == lp.Field.Type_BOOL)
        self.check("BOOLEAN has wrong value", msg.get_boolean_field("BOOLEAN").get_value() == True)

        self.check("CHAR not found", msg.has_field("CHAR"))
        self.check("CHAR has wrong type", msg.get_field("CHAR").get_type() == lp.Field.Type_CHAR)
        self.check("CHAR has wrong value", msg.get_char_field("CHAR").get_value() == 'c')

        self.check("FLOAT not found", msg.has_field("FLOAT"))
        self.check("FLOAT has wrong type", msg.get_field("FLOAT").get_type() == lp.Field.Type_F32)
        self.check("FLOAT has wrong value", msg.get_f32_field("FLOAT").get_value() == 1)

        self.check("DOUBLE not found", msg.has_field("DOUBLE"))
        self.check("DOUBLE has wrong type", msg.get_field("DOUBLE").get_type() == lp.Field.Type_F64)
        self.check("DOUBLE has wrong value", msg.get_f64_field("DOUBLE").get_value() == 2.4)

        self.check("BYTE not found", msg.has_field("BYTE"))
        self.check("BYTE has wrong type", msg.get_field("BYTE").get_type() == lp.Field.Type_I8)
        self.check("BYTE has wrong value", msg.get_i8_field("BYTE").get_value() == 5)

        self.check("SHORT not found", msg.has_field("SHORT"))
        self.check("SHORT has wrong type", msg.get_field("SHORT").get_type() == lp.Field.Type_I16)
        self.check("SHORT has wrong value", msg.get_i16_field("SHORT").get_value() == 55)

        self.check("INT not found", msg.has_field("INT"))
        self.check("INT has wrong type", msg.get_field("INT").get_type() == lp.Field.Type_I32)
        self.check("INT has wrong value", msg.get_i32_field("INT").get_value() == 555)

        self.check("LONG not found", msg.has_field("LONG"))
        self.check("LONG has wrong type", msg.get_field("LONG").get_type() == lp.Field.Type_I64)
        self.check("LONG has wrong value", msg.get_i64_field("LONG").get_value() == 5555)

        self.check("STRING not found", msg.has_field("STRING"))
        self.check("STRING has wrong type", msg.get_field("STRING").get_type() == lp.Field.Type_STRING)
        self.check("STRING has wrong value", msg.get_string_field("STRING").get_value() == "The old fox")

        self.check("UBYTE not found", msg.has_field("UBYTE"))
        self.check("UBYTE has wrong type", msg.get_field("UBYTE").get_type() == lp.Field.Type_U8)
        self.check("UBYTE has wrong value", msg.get_u8_field("UBYTE").get_value() == 5)

        self.check("USHORT not found", msg.has_field("USHORT"))
        self.check("USHORT has wrong type", msg.get_field("USHORT").get_type() == lp.Field.Type_U16)
        self.check("USHORT has wrong value", msg.get_u16_field("USHORT").get_value() == 55)

        self.check("UINT not found", msg.has_field("UINT"))
        self.check("UINT has wrong type", msg.get_field("UINT").get_type() == lp.Field.Type_U32)
        self.check("UINT has wrong value", msg.get_u32_field("UINT").get_value() == 555)

        self.check("ULONG not found", msg.has_field("ULONG"))
        self.check("ULONG has wrong type", msg.get_field("ULONG").get_type() == lp.Field.Type_U64)
        self.check("ULONG has wrong value", msg.get_u64_field("ULONG").get_value() == 5555)


    def test_add_fields(self):
        lp.log_info("Test add_fields()")

        field1 = lp.StringField("ONE", "uno")
        field2 = lp.StringField("TWO", "dos")
        field3 = lp.StringField("THREE", "tres")

        fields = lp.FieldList()
        fields.push_back(field1)
        fields.push_back(field2)
        fields.push_back(field3)

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("LOG")

        self.check("Expected only lp.fields to be added", False == msg.add_fields(fields))

        self.check("Expected to have field ONE", True == msg.has_field("ONE"))
        self.check("Unexpected field type for ONE", lp.Field.Type_STRING == msg.get_field("ONE").get_type())
        self.check("Unexpected field value for ONE", msg.get_string_value("ONE") == "uno")

        self.check("Expected to have field TWO", True == msg.has_field("TWO"))
        self.check("Unexpected field type for TWO", lp.Field.Type_STRING == msg.get_field("TWO").get_type())
        self.check("Unexpected field value for TWO", msg.get_string_value("TWO") == "dos")

        self.check("Expected to have field THREE", True == msg.has_field("THREE"))
        self.check("Unexpected field type for THREE", lp.Field.Type_STRING == msg.get_field("THREE").get_type())
        self.check("Unexpected field value for THREE", msg.get_string_value("THREE") == "tres")


    def test_clear_fields(self):
        lp.log_info("Test clear_fields()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        self.check("Message missing CONTENT-VERSION", True == msg.has_field("CONTENT-VERSION"))
        self.check("Message missing HEADER-VERSION", True == msg.has_field("HEADER-VERSION"))
        self.check("Message missing MESSAGE-SUBTYPE", True == msg.has_field("MESSAGE-SUBTYPE"))
        self.check("Message missing MESSAGE-TYPE", True == msg.has_field("MESSAGE-TYPE"))
        self.check("Message missing SPECIFICATION", True == msg.has_field("SPECIFICATION"))

        msg.clear_fields()

        self.check("Message has CONTENT-VERSION", False == msg.has_field("CONTENT-VERSION"))
        self.check("Message has HEADER-VERSION", False == msg.has_field("HEADER-VERSION"))
        self.check("Message has MESSAGE-SUBTYPE", False == msg.has_field("MESSAGE-SUBTYPE"))
        self.check("Message has MESSAGE-TYPE", False == msg.has_field("MESSAGE-TYPE"))
        self.check("Message has SPECIFICATION", False == msg.has_field("SPECIFICATION"))


    def test_clear_field(self):
        lp.log_info("Test clear_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        self.check("CONTENT-VERSION not found in message", msg.has_field("CONTENT-VERSION"))

        self.check("CONTENT-VERSION not cleared from message", msg.clear_field("CONTENT-VERSION") == True)
        self.check("FOO-BAR does not exist and cannot be cleared", msg.clear_field("FOO-BAR") == False)
        self.check("foo-bar does not exist and cannot be cleared", msg.clear_field("foo-bar") == False)

        self.check("CONTENT-VERSION should not exist in message", msg.has_field("CONTENT-VERSION") == False)


    def test_get_string_value(self):
        lp.log_info("Test get_string_value()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        msg.set_field_value("PUB-RATE", "30")

        self.check("MESSAGE-TYPE has unexpected value", msg.get_string_value("MESSAGE-TYPE") == "MSG")
        self.check("PUB-RATE has unexpected value", msg.get_string_value("PUB-RATE") == "30")


    def test_get_boolean_value(self):
        lp.log_info("Test get_boolean_value()")

        msg = lp.Message()

        msg.add_field( lp.I16Field("FIELD-1", 0) )
        msg.add_field( lp.I16Field("FIELD-2", 1) )
        msg.add_field( lp.StringField("FIELD-3", "10000") )
        msg.add_field( lp.I32Field("FIELD-4", 10000) )
        msg.add_field( lp.BooleanField("FIELD-5", True) )
        msg.add_field( lp.BooleanField("FIELD-6", False) )
        msg.add_field( lp.F64Field("FIELD-7", 2019.0) )
        msg.add_field( lp.StringField("FIELD-8", "True") )
        msg.add_field( lp.StringField("FIELD-9", "FALSE") )
        msg.add_field( lp.StringField("FIELD-0", "foo bar") )

        # Nominal tests
        self.check("1 Expected boolean value of False", msg.get_boolean_value("FIELD-1") == False)
        self.check("2 Expected boolean value of True",  msg.get_boolean_value("FIELD-2"))
        self.check("3 Expected boolean value of True",  msg.get_boolean_value("FIELD-3"))
        self.check("4 Expected boolean value of True",  msg.get_boolean_value("FIELD-4"))
        self.check("5 Expected boolean value of True",  msg.get_boolean_value("FIELD-5"))
        self.check("6 Expected boolean value of False", msg.get_boolean_value("FIELD-6") == False)
        self.check("7 Expected boolean value of True",  msg.get_boolean_value("FIELD-7"))
        self.check("8 Expected boolean value of True",  msg.get_boolean_value("FIELD-8"))
        self.check("9 Expected boolean value of False", msg.get_boolean_value("FIELD-9") == False)

        # Off-nominal tests
        # Field cannot be converted
        try:
            msg.get_boolean_value("FIELD-0")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field cannot be represented as a boolean" in str(e))

        # Unknown field
        try:
            msg.get_boolean_value("BOGUS")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain field with name: BOGUS" in str(e))

        # None field name
        try:
            msg.get_boolean_value(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # empty-string field name
        try:
            msg.get_boolean_value("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_integer_value(self):
        lp.log_info("Test get_integer_value()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        msg.set_field_value("PUB-RATE", "30")

        self.check("PUB-RATE has unexpected value", msg.get_integer_value("PUB-RATE") == 30)

        # Off-nominal test
        try:
            msg.get_integer_value("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field cannot be converted to a GMSEC_I64" in str(e))


    def test_get_double_value(self):
        lp.log_info("Test get_double_value()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        msg.set_field_value("PUB-RATE", "30")

        self.check("PUB-RATE has unexpected value", msg.get_double_value("PUB-RATE") == 30)

        # Off-nominal test
        try:
            msg.get_double_value("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field cannot be converted to a GMSEC_F64" in str(e))


    def test_has_field(self):
        lp.log_info("Test has_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        msg.set_field_value("PUB-RATE", "30")

        self.check("Message should have PUB-RATE", msg.has_field("PUB-RATE"))
        self.check("Message should not have PUBLISH-RATE", msg.has_field("PUBLISH-RATE") == False)
        self.check("Message should not have null field", msg.has_field(None) == False)
        self.check("Message should not have empty string field", msg.has_field("") == False)


    def test_get_field(self):
        lp.log_info("Test get_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        # Use valid field name
        self.check("Message should have MESSAGE-TYPE", msg.get_field("MESSAGE-TYPE") != None)

        # Use bogus field name
        self.check("Message should have MSG-TYPE", msg.get_field("MSG-TYPE") == None)

        # Use null field name
        try:
            msg.get_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_field_type(self):
        lp.log_info("Test get_field_type()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        self.check("MESSAGE-TYPE has unexpected field type", msg.get_field_type("MESSAGE-TYPE") == lp.Field.Type_STRING)
        self.check("CONTENT-VERSION has unexpected field type", msg.get_field_type("CONTENT-VERSION") == lp.Field.Type_F32)

        # Use bogus field name
        try:
            msg.get_field_type("MSG-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain field with name: MSG-TYPE" in str(e))

        # Use None field name
        try:
            msg.get_field_type(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_field_type("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_binary_field(self):
        lp.log_info("Test get_binary_field()")

        blob = bytearray()
        for i in range(0,255):
            blob.append(i)

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("MSG.TLMPKT")

        msg.add_field( lp.BinaryField("DATA", blob, len(blob)) )

        # Use valid field name
        field = msg.get_binary_field("DATA")
        self.require("Expected to get binary field", field != None)
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_BINARY)
        self.check("Unexpected field name", field.get_name() == "DATA")
        self.check("Unexpected data length", field.get_length() == len(blob))

        # Use non-binary field name
        try:
            msg.get_binary_field("CONTENT-VERSION")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain BinaryField with name: CONTENT-VERSION" in str(e))

        # Use null field name
        try:
            msg.get_binary_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_binary_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_boolean_field(self):
        lp.log_info("Test get_boolean_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.BooleanField("RESPONSE", True) )

        # Use valid field name
        field = msg.get_boolean_field("RESPONSE")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_BOOL)
        self.check("Unexpected field name", field.get_name() == "RESPONSE")
        self.check("Unexpected data value", field.get_value() == True)

        # Use non-boolean field name
        try:
            msg.get_boolean_field("CONTENT-VERSION")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain BooleanField with name: CONTENT-VERSION" in str(e))

        # Use null field name
        try:
            msg.get_boolean_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_boolean_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_char_field(self):
        lp.log_info("Test get_char_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.CharField("CHAR-FIELD", 'c') )

        # Use valid field name
        field = msg.get_char_field("CHAR-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_CHAR)
        self.check("Unexpected field name", field.get_name() == "CHAR-FIELD")
        self.check("Unexpected data value", field.get_value() == 'c')

        # Use non-char field name
        try:
            msg.get_char_field("CONTENT-VERSION")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain CharField with name: CONTENT-VERSION" in str(e))

        # Use null field name
        try:
            msg.get_char_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_char_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_f32_field(self):
        lp.log_info("Test get_f32_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        # Use valid field name
        field = msg.get_f32_field("CONTENT-VERSION")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_F32)
        self.check("Unexpected field name", field.get_name() == "CONTENT-VERSION")

        # Use non-F32 field name
        try:
            msg.get_f32_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain F32Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_f32_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_f32_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_f64_field(self):
        lp.log_info("Test get_f64_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.F64Field("F64-FIELD", 3.14) )

        # Use valid field name
        field = msg.get_f64_field("F64-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_F64)
        self.check("Unexpected field name", field.get_name() == "F64-FIELD")
        self.check("Unexpected field value", field.get_value() == 3.14)

        # Use non-F64 field name
        try:
            msg.get_f64_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain F64Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_f64_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_f64_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_i8_field(self):
        lp.log_info("Test get_i8_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.I8Field("I8-FIELD", 8) )

        # Use valid field name
        field = msg.get_i8_field("I8-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_I8)
        self.check("Unexpected field name", field.get_name() == "I8-FIELD")
        self.check("Unexpected field value", field.get_value() == 8)

        # Use non-I8 field name
        try:
            msg.get_i8_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain I8Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_i8_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_i8_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_i16_field(self):
        lp.log_info("Test get_i16_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.I16Field("I16-FIELD", 16) )

        # Use valid field name
        field = msg.get_i16_field("I16-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_I16)
        self.check("Unexpected field name", field.get_name() == "I16-FIELD")
        self.check("Unexpected field value", field.get_value() == 16)

        # Use non-I16 field name
        try:
            msg.get_i16_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain I16Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_i16_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_i16_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_i32_field(self):
        lp.log_info("Test get_i32_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.I32Field("I32-FIELD", 32) )

        # Use valid field name
        field = msg.get_i32_field("I32-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_I32)
        self.check("Unexpected field name", field.get_name() == "I32-FIELD")
        self.check("Unexpected field value", field.get_value() == 32)

        # Use non-I32 field name
        try:
            msg.get_i32_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain I32Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_i32_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_i32_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_i64_field(self):
        lp.log_info("Test get_i64_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.I64Field("I64-FIELD", 64) )

        # Use valid field name
        field = msg.get_i64_field("I64-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_I64)
        self.check("Unexpected field name", field.get_name() == "I64-FIELD")
        self.check("Unexpected field value", field.get_value() == 64)

        # Use non-I64 field name
        try:
            msg.get_i64_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain I64Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_i64_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_i64_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_u8_field(self):
        lp.log_info("Test get_u8_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.U8Field("U8-FIELD", 8) )

        # Use valid field name
        field = msg.get_u8_field("U8-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_U8)
        self.check("Unexpected field name", field.get_name() == "U8-FIELD")
        self.check("Unexpected field value", field.get_value() == 8)

        # Use non-U8 field name
        try:
            msg.get_u8_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain U8Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_u8_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_u8_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_u16_field(self):
        lp.log_info("Test get_u16_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.U16Field("U16-FIELD", 16) )

        # Use valid field name
        field = msg.get_u16_field("U16-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_U16)
        self.check("Unexpected field name", field.get_name() == "U16-FIELD")
        self.check("Unexpected field value", field.get_value() == 16)

        # Use non-U16 field name
        try:
            msg.get_u16_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain U16Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_u16_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_u16_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_u32_field(self):
        lp.log_info("Test get_u32_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.U32Field("U32-FIELD", 32) )

        # Use valid field name
        field = msg.get_u32_field("U32-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_U32)
        self.check("Unexpected field name", field.get_name() == "U32-FIELD")
        self.check("Unexpected field value", field.get_value() == 32)

        # Use non-U32 field name
        try:
            msg.get_u32_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain U32Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_u32_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_u32_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_u64_field(self):
        lp.log_info("Test get_u64_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        msg.add_field( lp.U64Field("U64-FIELD", 64) )

        # Use valid field name
        field = msg.get_u64_field("U64-FIELD")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_U64)
        self.check("Unexpected field name", field.get_name() == "U64-FIELD")
        self.check("Unexpected field value", field.get_value() == 64)

        # Use non-U64 field name
        try:
            msg.get_u64_field("MESSAGE-TYPE")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain U64Field with name: MESSAGE-TYPE" in str(e))

        # Use null field name
        try:
            msg.get_u64_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_u64_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_string_field(self):
        lp.log_info("Test get_string_field()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("REQ.DIR")

        # Use valid field name
        field = msg.get_string_field("MESSAGE-TYPE")
        self.check("Unexpected field type", field.get_type() == lp.Field.Type_STRING)
        self.check("Unexpected field name", field.get_name() == "MESSAGE-TYPE")
        self.check("Unexpected field value", field.get_value() == "REQ")

        # o Use non-string field name
        try:
            msg.get_string_field("CONTENT-VERSION")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Message does not contain StringField with name: CONTENT-VERSION" in str(e))

        # Use null field name
        try:
            msg.get_string_field(None)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))

        # Use empty-string field name
        try:
            msg.get_string_field("")
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name cannot be NULL, nor an empty string" in str(e))


    def test_get_field_count(self):
        lp.log_info("Test get_field_count()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg = msgFactory.create_message("HB")

        self.check("Message has unexpected field count", msg.get_field_count() == 13)

        msg.set_field_value("PUB-RATE", "30")

        self.check("Message has unexpected field count", msg.get_field_count() == 14)

        msg.clear_field("PUB-RATE")

        self.check("Message has unexpected field count", msg.get_field_count() == 13)

        msg.clear_fields()

        self.check("Message has unexpected field count", msg.get_field_count() == 0)


    def test_copy_fields(self):
        lp.log_info("Test copy_fields()")

        msgFactory = lp.MessageFactory()

        msg1 = msgFactory.create_message("HB")
        msg2 = lp.Message(msg1)

        msg1.set_field_value("PUB-RATE", "30")

        self.check("Message 2 should not have PUB-RATE", msg2.has_field("PUB-RATE") == False)

        msg1.copy_fields(msg2)

        self.check("Message 2 should have PUB-RATE", msg2.has_field("PUB-RATE"))


    def test_to_xml(self):
        lp.log_info("Test to_xml()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = lp.Message(msg1)

        self.check("Message 1 has NULL XML content", msg1.to_xml() != None)
        self.check("Message 2 has NULL XML content", msg2.to_xml() != None)
        self.check("Messages 1 and 2 do not have same XML content", msg1.to_xml() == msg2.to_xml())

        msg3 = msgFactory.from_data(msg1.to_xml(), lp.DataType_XML_DATA)
        self.check("Messages 1 and 3 do not have same XML content", msg1.to_xml() == msg3.to_xml())


    def test_to_json(self):
        lp.log_info("Test to_json()")

        msgFactory = lp.MessageFactory()

        self.set_standard_fields(msgFactory)

        msg1 = msgFactory.create_message("HB")
        msg2 = lp.Message(msg1)

        self.check("Message 1 has NULL JSON content", msg1.to_json() != None)
        self.check("Message 2 has NULL JSON content", msg2.to_json() != None)
        self.check("Messages 1 and 2 do not have same JSON content", msg1.to_json() == msg2.to_json())

        msg3 = msgFactory.from_data(msg1.to_json(), lp.DataType_JSON_DATA)
        self.check("Messages 1 and 3 do not have same JSON content", msg1.to_json() == msg3.to_json())


    def test_get_size(self):
        lp.log_info("Test get_size()")

        msgFactory = lp.MessageFactory()

        msg1 = msgFactory.create_message("HB")
        msg2 = lp.Message(msg1)

        self.check("Message 1 has unexpected size", msg1.get_size() > 0)
        self.check("Message 2 has unexpected size", msg2.get_size() > 0)
        self.check("Messages should have the same size", msg1.get_size() == msg2.get_size())


    def test_get_field_iterator(self):
        lp.log_info("Test get_field_iterator()")

        msgFactory = lp.MessageFactory()

        msg = msgFactory.create_message("HB")

        iter = msg.get_field_iterator()
        self.check("Got the field iterator", True)


class T005_MessageValidator(lp.MessageValidator):

    def __init__(self):
        super().__init__()
        self.iter = 0

    def validate_message(self, msg):
        status = lp.Status()

        if self.iter > 0:
            status = lp.Status(lp.MSG_ERROR, lp.MESSAGE_FAILED_VALIDATION, "Field is missing")

        self.iter += 1

        return status

    def reset(self):
        self.iter = 0


Test.run('Test Message', Test_Message())
