#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_SchemaIDIterator(Test):

    def test(self):
        self.test_iterator()
        self.test_reset()


    def test_iterator(self):
        lp.log_info("Test has_next() and next()")

        self.test_schema_id_iterator_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_0, 73)
        self.test_schema_id_iterator_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_1, 104)
        self.test_schema_id_iterator_aux(lp.GMSEC_MSG_SPEC_CURRENT, lp.Specification.SchemaLevel_LEVEL_2, 107)

        self.test_schema_id_iterator_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_0, 73)
        self.test_schema_id_iterator_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_1, 104)
        self.test_schema_id_iterator_aux(lp.GMSEC_MSG_SPEC_2019_00, lp.Specification.SchemaLevel_LEVEL_2, 107)


    def test_reset(self):
        lp.log_info("Test reset()")

        spec = lp.Specification()

        iter = spec.get_schema_id_iterator()

        while iter.has_next():
            iter.next()

        self.check("Expected no more schema IDs", iter.has_next() == False)

        iter.reset()

        self.check("Expected to have schema IDs", iter.has_next())


    def test_schema_id_iterator_aux(self, specVersion, schemaLevel, expectedSchemaIds):
        config = lp.Config()

        config.add_value("gmsec-specification-version", "" + str(specVersion))
        config.add_value("gmsec-schema-level", "" + str(schemaLevel))

        spec = lp.Specification(config)

        iter = spec.get_schema_id_iterator()

        numSchemaIds = 0

        while iter.has_next():
            id = iter.next()

            self.check("Schema ID is null", id != None)

            numSchemaIds += 1

        self.check("Unexpected number of schema IDs", expectedSchemaIds == numSchemaIds)


Test.run('Test SchemaIDIterator', Test_SchemaIDIterator())
