#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_Fields < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_BinaryField
        test_BooleanField
        test_CharField
        test_StringField

        test_F32Field
        test_F64Field

        test_I8Field
        test_I16Field
        test_I32Field
        test_I64Field

        test_U8Field
        test_U16Field
        test_U32Field
        test_U64Field

        test_Field
	end


    def test_BinaryField()
        Libgmsec_ruby::Log::info("Test BinaryField")

        # Nominal test
        data = []
        for i in 0..3 do
            data << (i % 256)
		end

        name  = "MY-FIELD"
		value = data.pack('C*').force_encoding('BINARY')

        # Constructor
        field1 = Libgmsec_ruby::BinaryField.new(name, value, value.length)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_BINARY)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_length() == value.length)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BIN\">00010203</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BIN\",\"VALUE\":\"00010203\"}")

        blob     = field1.get_value()
		blob_len = field1.get_length()

        for i in 0..(blob_len-1)
            if blob[i].ord != value[i].ord
                Libgmsec_ruby::Log::debug("BinaryField value at position #{i}" + " is wrong")
                check("BinaryField value is wrong", false)
			end
		end

        # Copy constructor
        field2 = Libgmsec_ruby::BinaryField.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_length() == field1.get_length())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        blob    = field2.get_value()
		blo_len = field2.get_length()

        for i in 0..(blob_len-1)
            if blob[i].ord != value[i].ord
                Libgmsec_ruby::Log::debug("BinaryField value at position #{i}" + " is wrong")
                check("BinaryField value is wrong", false)
			end
		end

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::BinaryField.new(nil, value, value.length)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::BinaryField.new("", value, value.length)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_BooleanField()
        Libgmsec_ruby::Log::info("Test BooleanField")

        # Nominal test
        name  = "MY-FIELD"
        value = true

        # Constructor
        field1 = Libgmsec_ruby::BooleanField.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_BOOL)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"BOOL\">TRUE</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"BOOL\",\"VALUE\":\"TRUE\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::BooleanField.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::BooleanField.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::BooleanField.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_CharField()
        Libgmsec_ruby::Log::info("Test CharField")

        # Nominal test
        name  = "MY-FIELD"
        value = 'f'

        # Constructor
        field1 = Libgmsec_ruby::CharField.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_CHAR)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"CHAR\">f</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"CHAR\",\"VALUE\":\"f\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::CharField.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json()== field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::CharField.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::CharField.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_StringField()
        Libgmsec_ruby::Log::info("Test StringField")

        # Nominal test
        name  = "MY-FIELD"
        value = "Test String"

        # Constructor
        field1 = Libgmsec_ruby::StringField.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_STRING)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"STRING\">Test String</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"STRING\",\"VALUE\":\"Test String\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::StringField.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field value
            Libgmsec_ruby::StringField.new(name, nil)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("StringField value cannot be NULL"))
		end

		begin
            # nil field name
            Libgmsec_ruby::StringField.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::StringField.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_F32Field()
        Libgmsec_ruby::Log::info("Test F32Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::F32Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_F32)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F32\" BITS=\"41200000\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F32\",\"BITS\":\"41200000\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::F32Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::F32Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::F32Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_F64Field()
        Libgmsec_ruby::Log::info("Test F64Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::F64Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_F64)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"F64\" BITS=\"4024000000000000\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"F64\",\"BITS\":\"4024000000000000\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::F64Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::F64Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::F64Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_I8Field()
        Libgmsec_ruby::Log::info("Test I8Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::I8Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_I8)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I8\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I8\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::I8Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::I8Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::I8Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_I16Field()
        Libgmsec_ruby::Log::info("Test I16Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::I16Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_I16)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I16\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I16\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::I16Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::I16Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::I16Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_I32Field()
        Libgmsec_ruby::Log::info("Test I32Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::I32Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_I32)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I32\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I32\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::I32Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::I32Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::I32Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_I64Field()
        Libgmsec_ruby::Log::info("Test I64Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::I64Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_I64)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"I64\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"I64\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::I64Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::I64Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::I64Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_U8Field()
        Libgmsec_ruby::Log::info("Test U8Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::U8Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_U8)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U8\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U8\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::U8Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::U8Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::U8Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_U16Field()
        Libgmsec_ruby::Log::info("Test U16Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::U16Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_U16)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U16\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U16\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::U16Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::U16Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::U16Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_U32Field()
        Libgmsec_ruby::Log::info("Test U32Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::U32Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_U32)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U32\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U32\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::U32Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::U32Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::U32Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_U64Field()
        Libgmsec_ruby::Log::info("Test U64Field")

        # Nominal test
        name  = "MY-FIELD"
        value = 10

        # Constructor
        field1 = Libgmsec_ruby::U64Field.new(name, value)
        check("Unexpected field type", field1.get_field_type() == Libgmsec_ruby::Field::Type_U64)
        check("Unexpected field name", field1.get_field_name() == name)
        check("Unexpected field value", field1.get_value() == value)
        check("Unexpected XML", field1.to_xml() == "<FIELD NAME=\"MY-FIELD\" TYPE=\"U64\">10</FIELD>")
        check("Unexpected JSON", field1.to_json() == "{\"NAME\":\"MY-FIELD\",\"TYPE\":\"U64\",\"VALUE\":\"10\"}")

        # Copy-constructor
        field2 = Libgmsec_ruby::U64Field.new(field1)
        check("Unexpected field type", field2.get_field_type() == field1.get_field_type())
        check("Unexpected field name", field2.get_field_name() == field1.get_field_name())
        check("Unexpected field value", field2.get_value() == field1.get_value())
        check("Unexpected XML", field2.to_xml() == field1.to_xml())
        check("Unexpected JSON", field2.to_json() == field1.to_json())

        # Off-Nominal test
		begin
            # nil field name
            Libgmsec_ruby::U64Field.new(nil, value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end

		begin
            # empty-string field name
            Libgmsec_ruby::U64Field.new("", value)
            check("Exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field name is not compliant"))
		end
	end


    def test_Field()
        # get_string_value()
        Libgmsec_ruby::Log::info("Test Field.get_string_value()")
        field1 = Libgmsec_ruby::I32Field.new("I32-FIELD", -1234)
        field2 = Libgmsec_ruby::U32Field.new("U32-FIELD", 1234)
        field3 = Libgmsec_ruby::F32Field.new("F32-FIELD", 1234.56)
        field4 = Libgmsec_ruby::StringField.new("STR-FIELD", "Hello World")

        check("Fail get_string_value 1", field1.get_string_value() == "-1234")
        check("Fail get_string_value 2", field2.get_string_value() == "1234")
        check("Fail get_string_value 3", field3.get_string_value() == "1234.56")
        check("Fail get_string_value 4", field4.get_string_value() == "Hello World")

        # get_boolean_value()
        Libgmsec_ruby::Log::info("Test Field.get_boolean_value()")
        field1 = Libgmsec_ruby::BooleanField.new("BOOL-FIELD", true)
        field2 = Libgmsec_ruby::U32Field.new("U32-FIELD1", 1234)
        field3 = Libgmsec_ruby::I32Field.new("I32-FIELD2", -1234)
        field4 = Libgmsec_ruby::F32Field.new("F32-FIELD", 0.0)
        field5 = Libgmsec_ruby::StringField.new("STR-FIELD1", "FALSE")
        field6 = Libgmsec_ruby::StringField.new("STR-FIELD2", "true")
        field7 = Libgmsec_ruby::StringField.new("STR-FIELD3", "Hello World")

        check("Fail getBooleanValue 1", field1.get_boolean_value() == true)
        check("Fail getBooleanValue 2", field2.get_boolean_value() == true)
        check("Fail getBooleanValue 3", field3.get_boolean_value() == true)
        check("Fail getBooleanValue 4", field4.get_boolean_value() == false)
        check("Fail getBooleanValue 5", field5.get_boolean_value() == false)
        check("Fail getBooleanValue 6", field6.get_boolean_value() == true)

        begin
            field7.get_boolean_value()
            check("Expected boolean conversion exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field cannot be represented as a boolean"))
        end

        # get_integer_value()
        Libgmsec_ruby::Log::info("Test Field.get_integer_value()")
        field1 = Libgmsec_ruby::I32Field.new("I32-FIELD", -1234)
        field2 = Libgmsec_ruby::U32Field.new("U32-FIELD", 1234)
        field3 = Libgmsec_ruby::F32Field.new("F32-FIELD", 1234.56)
        field4 = Libgmsec_ruby::StringField.new("STR-FIELD", "Hello World")

        check("Fail get_integer_value 1", field1.get_integer_value() == -1234)
        check("Fail get_integer_value 2", field2.get_integer_value() == 1234)
        check("Fail get_integer_value 3", field3.get_integer_value() == 1234)

		begin
            field4.get_integer_value()
            check("Fail get_integer_value 4", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field cannot be converted to a GMSEC_I64"))
		end

        # get_double_value()
        Libgmsec_ruby::Log::info("Test Field.get_double_value()")
        field1 = Libgmsec_ruby::I32Field.new("I32-FIELD", -1234)
        field2 = Libgmsec_ruby::U32Field.new("U32-FIELD", 1234)
        field3 = Libgmsec_ruby::F32Field.new("F32-FIELD", 1234.56)
        field4 = Libgmsec_ruby::StringField.new("STR-FIELD", "Hello World")

        check("Fail get_double_value 1", field1.get_double_value() == -1234.0)
        check("Fail get_double_value 2", field2.get_double_value() == 1234.0)
        check("Fail get_double_value 3", field3.get_double_value() == 1234.56)

		begin
            field4.get_double_value()
            check("Fail get_double_value 4", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Field cannot be converted to a GMSEC_F64"))
		end
	end
end


begin
    test = Test_Fields.new( ARGV )
    test.run
end
