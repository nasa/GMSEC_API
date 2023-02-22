#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_MessageSpecification(Test):

    def test(self):
        self.test_message_specification()


    def test_message_specification(self):
        lp.log_info("Test MessageSpecification")

        self.test_message_specification_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_0)
        self.test_message_specification_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_1)
        self.test_message_specification_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_2)

        self.test_message_specification_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_0)
        self.test_message_specification_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_1)
        self.test_message_specification_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_2)


    def test_message_specification_aux(self, specVersion, schemaLevel):
        config = lp.Config()

        config.add_value("gmsec-specification-version", "" + str(specVersion))
        config.add_value("gmsec-schema-level", "" + str(schemaLevel))

        spec = lp.Specification(config)

        msgSpecs = spec.get_message_specifications()

        self.check("Expected at least 1 (one) message specification", msgSpecs.size() > 0)

        for msgSpec in msgSpecs:
            self.check("Schema ID is NULL", msgSpec.get_schema_id() != None)
            self.check("Subject Template is NULL", msgSpec.get_subject_template() != None)

            fieldSpecs = msgSpec.get_field_specifications()

            self.check("Expected at least 1 (one) field specification", fieldSpecs.size() > 0)


Test.run('Test MessageSpecification', Test_MessageSpecification())
