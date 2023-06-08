#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_SchemaIdIterator < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_iterator
        test_reset
	end


    def test_iterator()
        Libgmsec_ruby::Log::info("Test has_next() and next()")

        test_schema_id_iterator_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0, 73)
        test_schema_id_iterator_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1, 103)
        test_schema_id_iterator_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2, 106)

        test_schema_id_iterator_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0, 73)
        test_schema_id_iterator_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1, 103)
        test_schema_id_iterator_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2, 106)
	end


    def test_reset()
        Libgmsec_ruby::Log::info("Test reset()")

        spec = Libgmsec_ruby::Specification.new()

        iter = spec.get_schema_id_iterator()

        while iter.has_next() do
            iter.next()
		end

        check("Expected no more schema IDs", iter.has_next() == false)

        iter.reset()

        check("Expected to have schema IDs", iter.has_next())
	end


    def test_schema_id_iterator_aux(specVersion, schemaLevel, expectedSchemaIds)
        config = Libgmsec_ruby::Config.new()

        config.add_value("gmsec-specification-version", "" + specVersion.to_s)
        config.add_value("gmsec-schema-level", "" + schemaLevel.to_s)

        spec = Libgmsec_ruby::Specification.new(config)

        iter = spec.get_schema_id_iterator()

        numSchemaIds = 0

        while iter.has_next() do
            id = iter.next()

            check("Schema ID is null", id != nil)

            numSchemaIds += 1
		end

        check("Unexpected number of schema IDs", expectedSchemaIds == numSchemaIds)
	end
end


begin
    test = Test_SchemaIdIterator.new( ARGV )
    test.run
end
