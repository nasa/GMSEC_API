#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_Config(Test):

    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_constructor_3()
        self.test_copy_constructor()
        self.test_add_value()
        self.test_clear_value()
        self.test_get_value()
        self.test_get_boolean_value()
        self.test_get_integer_value()
        self.test_get_double_value()
        self.test_clear()
        self.test_merge()
        self.test_to_xml()
        self.test_from_xml()
        self.test_get_from_file()
        self.test_to_json()


    def test_constructor_1(self):
        lp.log_info("Test constructor (no-arg)")

        c1 = lp.Config()

        self.check("Unexpected XML content", c1.to_xml() == "<CONFIG>\n</CONFIG>")


    def test_constructor_2(self):
        lp.log_info("Test constructor (command line args)")

        args = [ "mw-id=bolt", "server=localhost" ]

        c1 = lp.Config(args)

        self.check("Unexpected XML content", c1.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")


    def test_constructor_3(self):
        lp.log_info("Test constructor (data args)")

        # Test: data constructor

        # Using string with options
        c1 = lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA)
        self.check("Unexpected XML content", c1.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using XML
        c2 = lp.Config(c1.to_xml(), lp.DataType_XML_DATA)
        self.check("Unexpected XML content", c2.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using JSON
        c3 = lp.Config(c1.to_json(), lp.DataType_JSON_DATA)
        self.check("Unexpected XML content", c3.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using lowercase XML
        lowerXML = c1.to_xml().lower()

        c4 = lp.Config(lowerXML, lp.DataType_XML_DATA)
        self.check("Unexpected XML content", c4.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using lowercase JSON
        lowerJSON = c1.to_json().lower()

        c5 = lp.Config(lowerJSON, lp.DataType_JSON_DATA)
        self.check("Unexpected XML content", c5.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")


    def test_copy_constructor(self):
        lp.log_info("Test copy-constructor")

        c1 = lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA)
        c2 = lp.Config(c1)

        self.check("Config is not the same as the original", c1.to_xml() == c2.to_xml())


    def test_add_value(self):
        lp.log_info("Test add_value()")

        c1 = lp.Config()

        self.check("Did not expect to find value", c1.get_value("mw-id") == None)

        c1.add_value("mw-id", "bolt")

        self.check("Expected to find value", c1.get_value("mw-id") == "bolt")


    def test_clear_value(self):
        lp.log_info("Test clear_value()")

        c1 = lp.Config("mw-id=bolt", lp.DataType_KEY_VALUE_DATA)

        self.check("Expected to find value", c1.get_value("mw-id") == "bolt")

        c1.clear_value("mw-id")

        self.check("Did not expect to find value", c1.get_value("mw-id") == None)


    def test_get_value(self):
        lp.log_info("Test get_value()")

        c1 = lp.Config("mw-id=bolt", lp.DataType_KEY_VALUE_DATA)

        self.check("Unexpected value", c1.get_value("mw-id") == "bolt")
        self.check("Unexpected value", c1.get_value("server") == None)

        lp.log_info("Test get_value() w/ default value")

        self.check("Unexpected default value", c1.get_value("connectionType", "mb") == "mb")
        self.check("Unexpected default value", c1.get_value(None, "mb") == "mb")


    def test_get_boolean_value(self):
        lp.log_info("Test get_boolean_value()")

        c1 = lp.Config("gmsec-rocks=True", lp.DataType_KEY_VALUE_DATA)
        c2 = lp.Config("gmsec-rocks=sure", lp.DataType_KEY_VALUE_DATA)

        self.check("Unexpected boolean value", c1.get_boolean_value("gmsec-rocks"))

        try:
            c1.get_boolean_value(None)
            self.check("Expected get_boolean_value() to throw exception", False)
        except Exception as e:
            self.check(str(e), True)

        try:
            c2.get_boolean_value("gmsec-rocks")
            self.check("Expected get_boolean_value() to throw exception", False)
        except Exception as e:
            self.check(str(e), True)

        lp.log_info("Test get_boolean_value() w/ default value")

        self.check("Expected get_boolean_value() to return default", c1.get_boolean_value("gmsec-missing", False) == False)
        self.check("Expected get_boolean_value() to return default", c1.get_boolean_value("gmsec-missing", True))
        self.check("Expected get_boolean_value() to return default", c2.get_boolean_value("gmsec-rocks", True))
        self.check("Expected get_boolean_value() to return default", c2.get_boolean_value("", True))
        self.check("Expected get_boolean_value() to return default", c2.get_boolean_value(None, False) == False)


    def test_get_integer_value(self):
        lp.log_info("Test get_integer_value()")

        c1 = lp.Config("gmsec-value-1=10", lp.DataType_KEY_VALUE_DATA)
        c2 = lp.Config("gmsec-value-2=NAN", lp.DataType_KEY_VALUE_DATA)

        self.check("Unexpected integer value", c1.get_integer_value("gmsec-value-1") == 10)

        try:
            c1.get_integer_value(None)
            self.check("Expected get_integer_value() to throw exception", False)
        except Exception as e:
            self.check(str(e), True)

        try:
            c2.get_integer_value("gmsec-value-2")
            self.check("Expected get_integer_value() to throw exception", False)
        except Exception as e:
            self.check(str(e), True)

        lp.log_info("Test get_integer_value() w/ default value")

        self.check("Expected get_integer_value() to return default", c1.get_integer_value("gmsec-missing", 20) == 20)
        self.check("Expected get_integer_value() to return default", c2.get_integer_value("gmsec-value-2", 30) == 30)
        self.check("Expected get_integer_value() to return default", c2.get_integer_value("", 10) == 10)
        self.check("Expected get_integer_value() to return default", c2.get_integer_value(None, 10) == 10)


    def test_get_double_value(self):
        lp.log_info("Test get_double_value()")

        c1 = lp.Config("gmsec-value-1=10.15", lp.DataType_KEY_VALUE_DATA)
        c2 = lp.Config("gmsec-value-2=NAN", lp.DataType_KEY_VALUE_DATA)

        self.check("Unexpected double value", c1.get_double_value("gmsec-value-1") == 10.15)

        try:
            c1.get_double_value(None)
            self.check("Expected get_double_value() to throw exception", False)
        except Exception as e:
            self.check(str(e), True)

        try:
            c2.get_double_value("gmsec-value-2")
            self.check("Expected get_double_value() to throw exception", False)
        except Exception as e:
            self.check(str(e), True)

        lp.log_info("Test get_double_value() w/ default value")

        self.check("Expected get_double_value() to return default", c1.get_double_value("gmsec-missing", 20.22) == 20.22)
        self.check("Expected get_double_value() to return default", c2.get_double_value("gmsec-value-2", 30.33) == 30.33)
        self.check("Expected get_double_value() to return default", c2.get_double_value("", 10.33) == 10.33)
        self.check("Expected get_double_value() to return default", c2.get_double_value(None, 10.33) == 10.33)


    def test_clear(self):
        lp.log_info("Test clear()")

        c1 = lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA)

        c1.clear()

        self.check("Unexpected XML content", c1.to_xml() == "<CONFIG>\n</CONFIG>")


    def test_merge(self):
        lp.log_info("Test merge()")

        c1 = lp.Config("mw-id=bolt", lp.DataType_KEY_VALUE_DATA)
        c2 = lp.Config("mw-id=activemq39", lp.DataType_KEY_VALUE_DATA)
        c3 = lp.Config("mw-id=mb server=localhost", lp.DataType_KEY_VALUE_DATA)

        # Overwrite existing config entries
        c1.merge(c3, True)

        self.check("Unexpected mw-id value", c1.get_value("mw-id") == "mb")
        self.check("Unexpected server value", c1.get_value("server") == "localhost")

        # Do not overwrite existing entries
        c2.merge(c3, False)

        self.check("Unexpected mw-id value", c2.get_value("mw-id") == "activemq39")
        self.check("Unexpected server value", c2.get_value("server") == "localhost")


    def test_to_xml(self):
        lp.log_info("Test to_xml()")

        xml  = "<CONFIG>\n"
        xml += "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
        xml += "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
        xml += "</CONFIG>"

        c1 = lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA)

        self.check("Unexpected XML content", c1.to_xml() == xml)


    def test_from_xml(self):
        lp.log_info("Test from_xml()")

        xml  = "<CONFIG>\n"
        xml += "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
        xml += "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
        xml += "</CONFIG>"

        c1 = lp.Config()

        c1.from_xml(xml)

        self.check("Unexpected XML content", c1.to_xml() == xml)


    def test_get_from_file(self):
        lp.log_info("Test get_from_file()")

        # Nominal test
        try:
            config1 = lp.Config.get_from_file( self.getDataFile("T002_Config_good.xml"), "Bolt" )
            self.check("Unexpected return value", config1 != None)
            self.check("Unexpected mw-id value", config1.get_value("mw-id") == "bolt")
            self.check("Unexpected server value", config1.get_value("server") == "localhost")

            config2 = lp.Config.get_from_file( self.getDataFile("T002_Config_good.xml"), "ActiveMQ" )
            self.check("Unexpected return value", config2 != None)
            self.check("Unexpected mw-id value", config2.get_value("mw-id") == "activemq39")
            self.check("Unexpected server value", config2.get_value("server") == "tcp://localhost:61616")

            config3 = lp.Config.get_from_file( self.getDataFile("T002_Config_good.xml"), "" )
            self.check("Unexpected return value", config3 != None)
            self.check("Unexpected mw-id value", config3.get_value("mw-id") == "bolt")
            self.check("Unexpected server value", config3.get_value("server") == "localhost")

        except Exception as e:
            self.require(str(e), False)

        # Off-nominal tests
        try:
            lp.Config.get_from_file( self.getDataFile("T002_Config_bad.xml"), "Bolt" )
            self.check("Unexpected result", False)
        except Exception as e:
            self.check(str(e), "Invalid XML format -- parse error" in str(e))

        try:
            lp.Config.get_from_file( "file-does-not-exist", "Bolt" )
            self.check("Unexpected result", False)
        except Exception as e:
            self.check(str(e), "Configuration file could not be found or opened" in str(e))


    def test_to_json(self):
        lp.log_info("Test to_json()")

        json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"server\",\"VALUE\":\"localhost\"}]}}"

        c1 = lp.Config("mw-id=bolt server=localhost", lp.DataType_KEY_VALUE_DATA)

        self.check("Unexpected JSON content", c1.to_json() == json)


Test.run('Test Config', Test_Config())
