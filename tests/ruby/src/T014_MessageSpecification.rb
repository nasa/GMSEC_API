#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_MessageSpecification < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_message_specification
	end


    def test_message_specification()
        Libgmsec_ruby::Log::info("Test MessageSpecification")

        test_message_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
        test_message_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1)
        test_message_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2)

        test_message_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
        test_message_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1)
        test_message_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2)
	end


    def test_message_specification_aux(specVersion, schemaLevel)
        config = Libgmsec_ruby::Config.new()

        config.add_value("gmsec-specification-version", "" + specVersion.to_s)
        config.add_value("gmsec-schema-level", "" + schemaLevel.to_s)

        spec = Libgmsec_ruby::Specification.new(config)

        msgSpecs = spec.get_message_specifications()

        check("Expected at least 1 (one) message specification", msgSpecs.size() > 0)

        for msgSpec in msgSpecs do
            check("Schema ID is NULL", msgSpec.get_schema_id() != nil)
            check("Subject Template is NULL", msgSpec.get_subject_template() != nil)

            fieldSpecs = msgSpec.get_field_specifications()

            check("Expected at least 1 (one) field specification", fieldSpecs.size() > 0)
		end
	end
end


begin
    test = Test_MessageSpecification.new( ARGV )
    test.run
end
