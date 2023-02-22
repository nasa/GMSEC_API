#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_FieldSpecification < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_field_specification()
	end


    def test_field_specification()
        Libgmsec_ruby::Log::info("Test FieldSpecification")

        test_field_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
        test_field_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1)
        test_field_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2)

        test_field_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_0)
        test_field_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_1)
        test_field_specification_aux(Libgmsec_ruby::GMSEC_MSG_SPEC_2019_00, Libgmsec_ruby::Specification::SchemaLevel_LEVEL_2)
	end


    def test_field_specification_aux(specVersion, schemaLevel)
        config = Libgmsec_ruby::Config.new()

        config.add_value("gmsec-specification-version", "" + specVersion.to_s)
        config.add_value("gmsec-schema-level", "" + schemaLevel.to_s)

        spec = Libgmsec_ruby::Specification.new(config)

        msgSpecs = spec.get_message_specifications()

        for msgSpec in msgSpecs do
            fieldSpecs = msgSpec.get_field_specifications()

            check("Expected to have at least 1 (one) field specification", fieldSpecs.size() > 0)

            for fieldSpec in fieldSpecs do
                check("FieldSpecification name is nil", fieldSpec.get_name() != nil)
                check("FieldSpecification type is nil", fieldSpec.get_field_type() != nil)
                check("FieldSpecification mode is nil", fieldSpec.get_mode() != nil)
                check("FieldSpecification classification is nil", fieldSpec.get_classification() != nil)
                check("FieldSpecification value is nil", fieldSpec.get_value() != nil)
                check("FieldSpecification description is nil", fieldSpec.get_description() != nil)
			end
		end
	end
end


begin
    test = Test_FieldSpecification.new( ARGV )
    test.run
end
