#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_Specification < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_constructor_1
        test_constructor_2
        test_copy_constructor
        test_get_schema_id_iterator
        test_get_message_specifications
        test_get_header_field_names
	end


    def test_constructor_1()
        Libgmsec_ruby::Log::info("Test constructor (no args)")

        spec = Libgmsec_ruby::Specification.new()
        check("Unexpected specification version", spec.get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Unexpected specification schema level", spec.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
	end


    def test_constructor_2()
        Libgmsec_ruby::Log::info("Test constructor w/ Config")

        # Nominal tests
        spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new() )
        check("Unexpected specification version", spec.get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Unexpected specification schema level", spec.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)

        spec0 = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-specification-version=201900 gmsec-schema-level=0", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
        spec1 = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-specification-version=201900 gmsec-schema-level=1", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
        spec2 = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-specification-version=201900 gmsec-schema-level=2", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )

        check("Unexpected specification version", Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00 == spec0.get_version())
        check("Unexpected specification schema level", Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0 == spec0.get_schema_level())

        check("Unexpected specification version", Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00 == spec1.get_version())
        check("Unexpected specification schema level", Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1 == spec1.get_schema_level())

        check("Unexpected specification version", Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00 == spec2.get_version())
        check("Unexpected specification schema level", Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2 == spec2.get_schema_level())

        spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("name=value", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
        check("Unexpected specification version", spec.get_version() == Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT)
        check("Unexpected specification schema level", spec.get_schema_level() == Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)

        # Off-nominal tests
		begin
            spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-specification-version=201300", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Could not list files in template directory"))
		end

		begin
            spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-specification-version=ABC", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("\"GMSEC-SPECIFICATION-VERSION\" contains invalid value"))
		end

		begin
            spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-schema-level=3", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("DIRECTORY is missing definition for LEVEL-3 HEADER"))
		end

		begin
            spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-schema-level=10", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer"))
		end

		begin
            spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-schema-level=ABC", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("\"GMSEC-SCHEMA-LEVEL\" contains invalid value"))
		end

		begin
            spec = Libgmsec_ruby::Specification.new( Libgmsec_ruby::Config.new("gmsec-schema-path=here", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Could not list files in template directory"))
		end
	end


    def test_copy_constructor()
        Libgmsec_ruby::Log::info("Test copy-constructor")

        spec1 = Libgmsec_ruby::Specification.new()
        spec2 = Libgmsec_ruby::Specification.new(spec1)

        check("Unexpected specification version", spec1.get_version() == spec2.get_version())
        check("Unexpected specification schema level", spec1.get_schema_level() == spec2.get_schema_level())
        check("Unexpected message specifications", spec1.get_message_specifications().size() == spec2.get_message_specifications().size())
	end


    def test_get_schema_id_iterator()
        Libgmsec_ruby::Log::info("Test get_schema_id_iterator()")

        spec = Libgmsec_ruby::Specification.new() 

        iter = spec.get_schema_id_iterator()

        check("Expected availability of schema IDs", iter.has_next())
	end


    def test_get_message_specifications()
        Libgmsec_ruby::Log::info("Test get_message_specifications()")

        spec = Libgmsec_ruby::Specification.new()

        msgSpecs = spec.get_message_specifications()

        check("Expected to have message specifications", msgSpecs.size() > 0)
	end


    def test_get_header_field_names()
        Libgmsec_ruby::Log::info("Test get_headers()")

        spec = Libgmsec_ruby::Specification.new()

        headers = spec.get_header_field_names()
        check("Expected to have message specifications", headers.size() == 26)

        headers = spec.get_header_field_names("")
        check("Expected to have message specifications", headers.size() == 26)

        headers = spec.get_header_field_names("DEFAULT")
        check("Expected to have message specifications", headers.size() == 26)

        headers = spec.get_header_field_names("C2MS")
        check("Expected to have message specifications", headers.size() == 26)
	end
end


begin
    test = Test_Specification.new( ARGV )
    test.run
end
