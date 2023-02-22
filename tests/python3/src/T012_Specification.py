#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_Specification(Test):

    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_copy_constructor()
        self.test_get_schema_id_iterator()
        self.test_get_message_specifications()
        self.test_get_header_field_names()


    def test_constructor_1(self):
        lp.log_info("Test constructor (no args)")

        spec = lp.Specification()
        self.check("Unexpected specification version", spec.get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Unexpected specification schema level", spec.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_0)


    def test_constructor_2(self):
        lp.log_info("Test constructor w/ Config")

        # Nominal tests
        spec = lp.Specification( lp.Config() )
        self.check("Unexpected specification version", spec.get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Unexpected specification schema level", spec.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_0)

        spec0 = lp.Specification( lp.Config("gmsec-specification-version=201900 gmsec-schema-level=0", lp.DataType_KEY_VALUE_DATA) )
        spec1 = lp.Specification( lp.Config("gmsec-specification-version=201900 gmsec-schema-level=1", lp.DataType_KEY_VALUE_DATA) )
        spec2 = lp.Specification( lp.Config("gmsec-specification-version=201900 gmsec-schema-level=2", lp.DataType_KEY_VALUE_DATA) )

        self.check("Unexpected specification version", lp.GMSEC_MSG_SPEC_2019_00 == spec0.get_version())
        self.check("Unexpected specification schema level", lp.Specification.SchemaLevel_LEVEL_0 == spec0.get_schema_level())

        self.check("Unexpected specification version", lp.GMSEC_MSG_SPEC_2019_00 == spec1.get_version())
        self.check("Unexpected specification schema level", lp.Specification.SchemaLevel_LEVEL_1 == spec1.get_schema_level())

        self.check("Unexpected specification version", lp.GMSEC_MSG_SPEC_2019_00 == spec2.get_version())
        self.check("Unexpected specification schema level", lp.Specification.SchemaLevel_LEVEL_2 == spec2.get_schema_level())

        spec = lp.Specification( lp.Config("name=value", lp.DataType_KEY_VALUE_DATA) )
        self.check("Unexpected specification version", spec.get_version() == lp.GMSEC_MSG_SPEC_CURRENT)
        self.check("Unexpected specification schema level", spec.get_schema_level() == lp.Specification.SchemaLevel_LEVEL_0)

        # Off-nominal tests
        try:
            spec = lp.Specification( lp.Config("gmsec-specification-version=201300", lp.DataType_KEY_VALUE_DATA) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Could not list files in template directory" in str(e))

        try:
            spec = lp.Specification( lp.Config("gmsec-specification-version=ABC", lp.DataType_KEY_VALUE_DATA) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "\"GMSEC-SPECIFICATION-VERSION\" contains invalid value" in str(e))

        try:
            spec = lp.Specification( lp.Config("gmsec-schema-level=3", lp.DataType_KEY_VALUE_DATA) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "DIRECTORY is missing definition for LEVEL-3 HEADER" in str(e))

        try:
            spec = lp.Specification( lp.Config("gmsec-schema-level=10", lp.DataType_KEY_VALUE_DATA) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer" in str(e))

        try:
            spec = lp.Specification( lp.Config("gmsec-schema-level=ABC", lp.DataType_KEY_VALUE_DATA) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "\"GMSEC-SCHEMA-LEVEL\" contains invalid value" in str(e))

        try:
            spec = lp.Specification( lp.Config("gmsec-schema-path=here", lp.DataType_KEY_VALUE_DATA) )
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Could not list files in template directory" in str(e))


    def test_copy_constructor(self):
        lp.log_info("Test copy-constructor")

        spec1 = lp.Specification()
        spec2 = lp.Specification(spec1)

        self.check("Unexpected specification version", spec1.get_version() == spec2.get_version())
        self.check("Unexpected specification schema level", spec1.get_schema_level() == spec2.get_schema_level())
        self.check("Unexpected message specifications", spec1.get_message_specifications().size() == spec2.get_message_specifications().size())


    def test_get_schema_id_iterator(self):
        lp.log_info("Test get_schema_id_iterator()")

        spec = lp.Specification() 

        iter = spec.get_schema_id_iterator()

        self.check("Expected availability of schema IDs", iter.has_next())


    def test_get_message_specifications(self):
        lp.log_info("Test get_message_specifications()")

        spec = lp.Specification()

        msgSpecs = spec.get_message_specifications()

        self.check("Expected to have message specifications", msgSpecs.size() > 0)


    def test_get_header_field_names(self):
        lp.log_info("Test get_headers()")

        spec = lp.Specification()

        headers = spec.get_header_field_names()
        self.check("Expected to have headers", headers.size() == 26)

        headers = spec.get_header_field_names("")
        self.check("Expected to have headers", headers.size() == 26)

        defaultHeaders = spec.get_header_field_names("DEFAULT")
        self.check("Expected to have headers", defaultHeaders.size() == 26)

        C2MSHeaders = spec.get_header_field_names("C2MS")
        self.check("Expected to have headers", C2MSHeaders.size() == 26)


Test.run('Test Specification', Test_Specification())
