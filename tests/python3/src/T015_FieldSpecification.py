#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_FieldSpecification(Test):

    def test(self):
        self.test_field_specification()


    def test_field_specification(self):
        lp.log_info("Test FieldSpecification")

        self.test_field_specification_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_0)
        self.test_field_specification_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_1)
        self.test_field_specification_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_2)

        self.test_field_specification_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_0)
        self.test_field_specification_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_1)
        self.test_field_specification_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_2)


    def test_field_specification_aux(self, specVersion, schemaLevel):
        config = lp.Config()

        config.add_value("gmsec-specification-version", "" + str(specVersion))
        config.add_value("gmsec-schema-level", "" + str(schemaLevel))

        spec = lp.Specification(config)

        msgSpecs = spec.get_message_specifications()

        for msgSpec in msgSpecs:
            fieldSpecs = msgSpec.get_field_specifications()

            self.check("Expected to have at least 1 (one) field specification", fieldSpecs.size() > 0)

            for fieldSpec in fieldSpecs:
                self.check("FieldSpecification name is None", fieldSpec.get_name() != None)
                self.check("FieldSpecification type is None", fieldSpec.get_type() != None)
                self.check("FieldSpecification mode is None", fieldSpec.get_mode() != None)
                self.check("FieldSpecification classification is None", fieldSpec.get_classification() != None)
                self.check("FieldSpecification value is None", fieldSpec.get_value() != None)
                self.check("FieldSpecification description is None", fieldSpec.get_description() != None)


Test.run('Test FieldSpecification', Test_FieldSpecification())
