#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_Fields(Test):

    def test(self):
        self.test_BinaryField()
        self.test_BooleanField()
        self.test_CharField()
        self.test_StringField()

        self.test_F32Field()
        self.test_F64Field()

        self.test_I8Field()
        self.test_I16Field()
        self.test_I32Field()
        self.test_I64Field()

        self.test_U8Field()
        self.test_U16Field()
        self.test_U32Field()
        self.test_U64Field()

        self.test_Field()


    def test_BinaryField(self):
        lp.log_info("Test BinaryField")

        # Nominal test
        name  = "MY-FIELD"
        value = bytearray()
        for i in range(0,4):
            value.append(i)

        # Constructor
        field1 = lp.BinaryField(name, value, len(value))
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_BINARY)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_length() == len(value))
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BIN\">00010203</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00010203\"}")

        blob = field1.get_value()
        for i in range(0, field1.get_length()):
            if not blob[i] == value[i]:
                lp.log_debug("BinaryField value at position " + str(i) + " is wrong")
                self.check("BinaryField value is wrong", False)

        # Copy constructor
        field2 = lp.BinaryField(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_length() == field1.get_length())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        blob = field2.get_value()
        for i in range(0, field2.get_length()):
            if not blob[i] == value[i]:
                lp.log_debug("BinaryField value at position " + str(i) + " is wrong")
                self.check("BinaryField value is wrong", False)

        # Off-Nominal test
        try:
            # None field name
            lp.BinaryField(None, value, len(value))
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.BinaryField("", value, len(value))
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_BooleanField(self):
        lp.log_info("Test BooleanField")

        # Nominal test
        name  = "MY-FIELD"
        value = True

        # Constructor
        field1 = lp.BooleanField(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_BOOL)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BOOL\",\"VALUE\":\"TRUE\"}")

        # Copy-constructor
        field2 = lp.BooleanField(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.BooleanField(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.BooleanField("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_CharField(self):
        lp.log_info("Test CharField")

        # Nominal test
        name  = "MY-FIELD"
        value = 'f'

        # Constructor
        field1 = lp.CharField(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_CHAR)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"CHAR\">f</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"CHAR\",\"VALUE\":\"f\"}")

        # Copy-constructor
        field2 = lp.CharField(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json()== field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.CharField(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.CharField("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_StringField(self):
        lp.log_info("Test StringField")

        # Nominal test
        name  = "MY-FIELD"
        value = "Test String"

        # Constructor
        field1 = lp.StringField(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_STRING)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"STRING\">Test String</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Test String\"}")


        # Copy-constructor
        field2 = lp.StringField(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field value
            lp.StringField(name, None)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "StringField value cannot be NULL" in str(e))

        try:
            # None field name
            lp.StringField(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.StringField("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_F32Field(self):
        lp.log_info("Test F32Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.F32Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_F32)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F32\" BITS=\"41200000\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F32\",\"BITS\":\"41200000\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.F32Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.F32Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.F32Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_F64Field(self):
        lp.log_info("Test F64Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.F64Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_F64)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F64\" BITS=\"4024000000000000\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F64\",\"BITS\":\"4024000000000000\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.F64Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.F64Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.F64Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_I8Field(self):
        lp.log_info("Test I8Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.I8Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_I8)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I8\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I8\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.I8Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.I8Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.I8Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_I16Field(self):
        lp.log_info("Test I16Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.I16Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_I16)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I16\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I16\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.I16Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.I16Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.I16Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_I32Field(self):
        lp.log_info("Test I32Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.I32Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_I32)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I32\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I32\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.I32Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.I32Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.I32Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_I64Field(self):
        lp.log_info("Test I64Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.I64Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_I64)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I64\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I64\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.I64Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.I64Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.I64Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_U8Field(self):
        lp.log_info("Test U8Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.U8Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_U8)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U8\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U8\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.U8Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.U8Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.U8Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_U16Field(self):
        lp.log_info("Test U16Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.U16Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_U16)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U16\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U16\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.U16Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.U16Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.U16Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_U32Field(self):
        lp.log_info("Test U32Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.U32Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_U32)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U32\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U32\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.U32Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.U32Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.U32Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_U64Field(self):
        lp.log_info("Test U64Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = lp.U64Field(name, value)
        self.check("Unexpected field type", field1.get_type() == lp.Field.Type_U64)
        self.check("Unexpected field name", field1.get_name() == name)
        self.check("Unexpected field value", field1.get_value() == value)
        self.check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U64\">10</FIELD>")
        self.check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U64\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = lp.U64Field(field1)
        self.check("Unexpected field type", field2.get_type() == field1.get_type())
        self.check("Unexpected field name", field2.get_name() == field1.get_name())
        self.check("Unexpected field value", field2.get_value() == field1.get_value())
        self.check("Unexpected XML", field2.to_xml() == field1.to_xml())
        self.check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
        try:
            # None field name
            lp.U64Field(None, value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))

        try:
            # empty-string field name
            lp.U64Field("", value)
            self.check("Exception was expected", False)
        except Exception as e:
            self.check(str(e), "Field name is not compliant" in str(e))


    def test_Field(self):
        field1 = lp.I32Field("I32-FIELD", -1234)
        field2 = lp.U32Field("U32-FIELD", 1234)
        field3 = lp.F32Field("F32-FIELD", 1234.56)
        field4 = lp.StringField("STR-FIELD", "Hello World")
        field5 = lp.StringField("STR-FIELD1", "FALSE")
        field6 = lp.StringField("STR-FIELD2", "true")
        field7 = lp.BooleanField("BOOL-FIELD", True)

        # get_string_value()
        lp.log_info("Test Field.get_string_value()")
        self.check("Fail get_string_value 1", field1.get_string_value() == "-1234")
        self.check("Fail get_string_value 2", field2.get_string_value() == "1234")
        self.check("Fail get_string_value 3", field3.get_string_value() == "1234.56")
        self.check("Fail get_string_value 4", field4.get_string_value() == "Hello World")

        # get_boolean_value()
        lp.log_info("Test Field.get_boolean_value()")
        self.check("Fail getBooleanValue 1", field1.get_boolean_value() == True)
        self.check("Fail getBooleanValue 2", field2.get_boolean_value() == True)
        self.check("Fail getBooleanValue 3", field3.get_boolean_value() == True)
        self.check("Fail getBooleanValue 5", field5.get_boolean_value() == False)
        self.check("Fail getBooleanValue 6", field6.get_boolean_value() == True)
        self.check("Fail getBooleanValue 7", field7.get_boolean_value() == True)

        try:
            field4.get_boolean_value()
            self.check("Expected boolean conversion exception", False)
        except Exception as e:
            self.check(str(e), "Field cannot be represented as a boolean" in str(e))

        # get_integer_value()
        lp.log_info("Test Field.get_integer_value()")
        self.check("Fail get_integer_value 1", field1.get_integer_value() == -1234)
        self.check("Fail get_integer_value 2", field2.get_integer_value() == 1234)
        self.check("Fail get_integer_value 3", field3.get_integer_value() == 1234)

        try:
            field4.get_integer_value()
            self.check("Fail get_integer_value 4", False)
        except Exception as e:
            self.check(str(e), "Field cannot be converted to a GMSEC_I64" in str(e))

        # get_double_value()
        lp.log_info("Test Field.get_double_value()")
        self.check("Fail get_double_value 1", field1.get_double_value() == -1234.0)
        self.check("Fail get_double_value 2", field2.get_double_value() == 1234.0)
        self.check("Fail get_double_value 3", field3.get_double_value() == 1234.56)

        try:
            field4.get_double_value()
            self.check("Fail get_double_value 4", False)
        except Exception  as e:
            self.check(str(e), "Field cannot be converted to a GMSEC_F64" in str(e))


Test.run('Test Fields', Test_Fields())
