#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_MessageFieldIterator < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_all_fields
        test_header_fields
        test_non_header_fields
        test_reset
	end


    def test_all_fields()
        Libgmsec_ruby::Log::info("Test ALL_FIELDS")

        msg = TestMessage::instance().get_message()

        iter = msg.get_field_iterator( Libgmsec_ruby::MessageFieldIterator::Selector_ALL_FIELDS )

        fieldsFound = 0

        while iter.has_next() do
            field = iter.next()

            fieldsFound += 1

            if fieldsFound == 1
                check("Unexpected field type 1", field.get_field_type() == Libgmsec_ruby::Field::Type_STRING)
                check("Unexpected field name 1", field.get_field_name() == "FIELD-1")
                check("Unexpected field value 1", field.get_string_value() == "1")

            elsif fieldsFound == 2
                check("Unexpected field type 2", field.get_field_type() == Libgmsec_ruby::Field::Type_STRING)
                check("Unexpected field name 2", field.get_field_name() == "FIELD-2")
                check("Unexpected field value 2", field.get_string_value() == "2")

            else
                check("Unexpected field type 3", field.get_field_type() == Libgmsec_ruby::Field::Type_I32)
                check("Unexpected field name 3", field.get_field_name() == "FIELD-3")
                check("Unexpected field value 3", field.get_integer_value() == 3)
			end
		end

        check("Unexpected field count", fieldsFound == 3)
	end


    def test_header_fields()
        Libgmsec_ruby::Log::info("Test HEADER_FIELDS")

        msg = TestMessage::instance().get_message()

        iter = msg.get_field_iterator( Libgmsec_ruby::MessageFieldIterator::Selector_HEADER_FIELDS )

        fieldsFound = 0

        while iter.has_next() do
            field = iter.next()

            fieldsFound += 1

            if fieldsFound == 1
                check("Unexpected field type 1", field.get_field_type() == Libgmsec_ruby::Field::Type_STRING)
                check("Unexpected field name 1", field.get_field_name() == "FIELD-1")
                check("Unexpected field value 1", field.get_string_value() == "1")

            elsif fieldsFound == 2
                check("Unexpected field type 2", field.get_field_type() == Libgmsec_ruby::Field::Type_STRING)
                check("Unexpected field name 2", field.get_field_name() == "FIELD-2")
                check("Unexpected field value 2", field.get_string_value() == "2")

            else
                check("Unexpected field", false)
			end
		end

        check("Unexpected field count", fieldsFound == 2)
	end


    def test_non_header_fields()
        Libgmsec_ruby::Log::info("Test NON_HEADER_FIELDS")

        msg = TestMessage::instance().get_message()

        iter = msg.get_field_iterator( Libgmsec_ruby::MessageFieldIterator::Selector_NON_HEADER_FIELDS )

        fieldsFound = 0

        while iter.has_next() do
            field = iter.next()

            fieldsFound += 1

            if fieldsFound == 1
                check("Unexpected field type 3", field.get_field_type() == Libgmsec_ruby::Field::Type_I32)
                check("Unexpected field name 3", field.get_field_name() == "FIELD-3")
                check("Unexpected field value 3", field.get_integer_value() == 3)

            else
                check("Unexpected field", false)
			end
		end

        check("Unexpected field count", fieldsFound == 1)
	end


    def test_reset()
        Libgmsec_ruby::Log::info("Test reset()")

        msg = TestMessage::instance().get_message()

        iter = msg.get_field_iterator()

        fieldsFound = 0

        while iter.has_next() do
            iter.next()
            fieldsFound += 1
		end

        check("Unexpected field count", fieldsFound == 3)

        check("Should no more field(s) to iterate over", iter.has_next() == false)

        iter.reset()

        check("Should have field(s) to iterator over", iter.has_next())

        fieldsFound = 0

        while iter.has_next() do
            iter.next()
            fieldsFound += 1
		end

        check("Unexpected field count", fieldsFound == 3)
	end
end


class TestMessage
	def self.instance
		return TestMessage.new
	end

    def get_message
		@@msg
	end

private
	def initialize
        @@msg = Libgmsec_ruby::Message.new()

        field1 = Libgmsec_ruby::StringField.new("FIELD-1", "1", true)
        field2 = Libgmsec_ruby::StringField.new("FIELD-2", "2", true)
        field3 = Libgmsec_ruby::I32Field.new("FIELD-3", 3)

        @@msg.add_field( field1 )
        @@msg.add_field( field2 )
        @@msg.add_field( field3 )
	end
end


begin
    test = Test_MessageFieldIterator.new( ARGV )
    test.run
end
