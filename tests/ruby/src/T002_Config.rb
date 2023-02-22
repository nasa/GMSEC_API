#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_Config < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_constructor_1
        test_constructor_2
        test_constructor_3
        test_copy_constructor
        test_add_value
        test_clear_value
        test_get_value
        test_get_boolean_value
        test_get_integer_value
        test_get_double_value
        test_clear
        test_merge
        test_to_xml
        test_from_xml
        test_get_from_file
        test_to_json
	end


    def test_constructor_1()
        Libgmsec_ruby::Log::info("Test constructor (no-arg)")

        c1 = Libgmsec_ruby::Config.new()

        check("Unexpected XML content", c1.to_xml() == "<CONFIG>\n</CONFIG>")
	end


    def test_constructor_2()
        Libgmsec_ruby::Log::info("Test constructor (command line args)")

        args1 = [ "mw-id=bolt", "server=localhost" ]
        c1 = Libgmsec_ruby::Config.new(args1)
        check("Unexpected XML content for Config 1", c1.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Off-nominal; empty array given
        args2 = [ ]
        c2 = Libgmsec_ruby::Config.new(args2)
        check("Unexpected XML content for Config 2", c2.to_xml() == "<CONFIG>\n</CONFIG>");
	end


    def test_constructor_3()
        Libgmsec_ruby::Log::info("Test constructor (data args)")

        # Test: data constructor

        # Using string with options
        c1 = Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA)
        check("Unexpected XML content", c1.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using XML
        c2 = Libgmsec_ruby::Config.new(c1.to_xml(), Libgmsec_ruby::DataType_XML_DATA)
        check("Unexpected XML content", c2.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using JSON
        c3 = Libgmsec_ruby::Config.new(c1.to_json(), Libgmsec_ruby::DataType_JSON_DATA)
        check("Unexpected XML content", c3.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using lowercase XML
        lowerXML = c1.to_xml().downcase()

        c4 = Libgmsec_ruby::Config.new(lowerXML, Libgmsec_ruby::DataType_XML_DATA)
        check("Unexpected XML content", c4.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")

        # Using lowercase JSON
        lowerJSON = c1.to_json().downcase()

        c5 = Libgmsec_ruby::Config.new(lowerJSON, Libgmsec_ruby::DataType_JSON_DATA)
        check("Unexpected XML content", c5.to_xml() == "<CONFIG>\n\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n</CONFIG>")
	end


    def test_copy_constructor()
        Libgmsec_ruby::Log::info("Test copy-constructor")

        c1 = Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA)
        c2 = Libgmsec_ruby::Config.new(c1)

        check("Config is not the same as the original", c1.to_xml() == c2.to_xml())
	end


    def test_add_value()
        Libgmsec_ruby::Log::info("Test add_value()")

        c1 = Libgmsec_ruby::Config.new()

        check("Did not expect to find value", c1.get_value("mw-id") == nil)

        c1.add_value("mw-id", "bolt")

        check("Expected to find value", c1.get_value("mw-id") == "bolt")
	end


    def test_clear_value()
        Libgmsec_ruby::Log::info("Test clear_value()")

        c1 = Libgmsec_ruby::Config.new("mw-id=bolt", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        check("Expected to find value", c1.get_value("mw-id") == "bolt")

        c1.clear_value("mw-id")

        check("Did not expect to find value", c1.get_value("mw-id") == nil)
	end


    def test_get_value()
        Libgmsec_ruby::Log::info("Test get_value()")

        c1 = Libgmsec_ruby::Config.new("mw-id=bolt", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        check("Unexpected value", c1.get_value("mw-id") == "bolt")
        check("Unexpected value", c1.get_value("server") == nil)

        Libgmsec_ruby::Log::info("Test get_value() w/ default value")

        check("Unexpected default value", c1.get_value("connectionType", "mb") == "mb")
        check("Unexpected default value", c1.get_value(nil, "mb") == "mb")
	end


    def test_get_boolean_value()
        Libgmsec_ruby::Log::info("Test get_boolean_value()")

        c1 = Libgmsec_ruby::Config.new("gmsec-rocks=true", Libgmsec_ruby::DataType_KEY_VALUE_DATA)
        c2 = Libgmsec_ruby::Config.new("gmsec-rocks=sure", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        check("Unexpected boolean value", c1.get_boolean_value("gmsec-rocks"))

		begin
            c1.get_boolean_value(nil)
            check("Expected get_boolean_value() to throw exception", false)
        rescue GmsecException => e
            check(e.message, true)
		end

        begin
            c2.get_boolean_value("gmsec-rocks")
            check("Expected get_boolean_value() to throw exception", false)
        rescue GmsecException => e
            check(e.message, true)
		end

        Libgmsec_ruby::Log::info("Test get_boolean_value() w/ default value")

        check("Expected get_boolean_value() to return default", c1.get_boolean_value("gmsec-missing", false) == false)
        check("Expected get_boolean_value() to return default", c1.get_boolean_value("gmsec-missing", true))
        check("Expected get_boolean_value() to return default", c2.get_boolean_value("gmsec-rocks", true))
        check("Expected get_boolean_value() to return default", c2.get_boolean_value("", true))
        check("Expected get_boolean_value() to return default", c2.get_boolean_value(nil, false) == false)
	end


    def test_get_integer_value()
        Libgmsec_ruby::Log::info("Test get_integer_value()")

        c1 = Libgmsec_ruby::Config.new("gmsec-value-1=10", Libgmsec_ruby::DataType_KEY_VALUE_DATA)
        c2 = Libgmsec_ruby::Config.new("gmsec-value-2=NAN", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        check("Unexpected integer value", c1.get_integer_value("gmsec-value-1") == 10)

		begin
            c1.get_integer_value(nil)
            check("Expected get_integer_value() to throw exception", false)
        rescue GmsecException => e
            check(e.message, true)
		end

		begin
            c2.get_integer_value("gmsec-value-2")
            check("Expected get_integer_value() to throw exception", false)
        rescue GmsecException => e
            check(e.message, true)
		end

        Libgmsec_ruby::Log::info("Test get_integer_value() w/ default value")

        check("Expected get_integer_value() to return default", c1.get_integer_value("gmsec-missing", 20) == 20)
        check("Expected get_integer_value() to return default", c2.get_integer_value("gmsec-value-2", 30) == 30)
        check("Expected get_integer_value() to return default", c2.get_integer_value("", 10) == 10)
        check("Expected get_integer_value() to return default", c2.get_integer_value(nil, 10) == 10)
	end


    def test_get_double_value()
        Libgmsec_ruby::Log::info("Test get_double_value()")

        c1 = Libgmsec_ruby::Config.new("gmsec-value-1=10.15", Libgmsec_ruby::DataType_KEY_VALUE_DATA)
        c2 = Libgmsec_ruby::Config.new("gmsec-value-2=NAN", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        check("Unexpected double value", c1.get_double_value("gmsec-value-1") == 10.15)

		begin
            c1.get_double_value(nil)
            check("Expected get_double_value() to throw exception", false)
        rescue GmsecException => e
            check(e.message, true)
		end

		begin
            c2.get_double_value("gmsec-value-2")
            check("Expected get_double_value() to throw exception", false)
        rescue GmsecException => e
            check(e.message, true)
		end

        Libgmsec_ruby::Log::info("Test get_double_value() w/ default value")

        check("Expected get_double_value() to return default", c1.get_double_value("gmsec-missing", 20.22) == 20.22)
        check("Expected get_double_value() to return default", c2.get_double_value("gmsec-value-2", 30.33) == 30.33)
        check("Expected get_double_value() to return default", c2.get_double_value("", 10.33) == 10.33)
        check("Expected get_double_value() to return default", c2.get_double_value(nil, 10.33) == 10.33)
	end


    def test_clear()
        Libgmsec_ruby::Log::info("Test clear()")

        c1 = Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        c1.clear()

        check("Unexpected XML content", c1.to_xml() == "<CONFIG>\n</CONFIG>")
	end


    def test_merge()
        Libgmsec_ruby::Log::info("Test merge()")

        c1 = Libgmsec_ruby::Config.new("mw-id=bolt", Libgmsec_ruby::DataType_KEY_VALUE_DATA)
        c2 = Libgmsec_ruby::Config.new("mw-id=activemq39", Libgmsec_ruby::DataType_KEY_VALUE_DATA)
        c3 = Libgmsec_ruby::Config.new("mw-id=mb server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        # Overwrite existing config entries
        c1.merge(c3, true)

        check("Unexpected mw-id value", c1.get_value("mw-id") == "mb")
        check("Unexpected server value", c1.get_value("server") == "localhost")

        # Do not overwrite existing entries
        c2.merge(c3, false)

        check("Unexpected mw-id value", c2.get_value("mw-id") == "activemq39")
        check("Unexpected server value", c2.get_value("server") == "localhost")
	end


    def test_to_xml()
        Libgmsec_ruby::Log::info("Test to_xml()")

        xml  = "<CONFIG>\n"
        xml += "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
        xml += "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
        xml += "</CONFIG>"

        c1 = Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        check("Unexpected XML content", c1.to_xml() == xml)
	end


    def test_from_xml()
        Libgmsec_ruby::Log::info("Test from_xml()")

        xml  = "<CONFIG>\n"
        xml += "\t<PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n"
        xml += "\t<PARAMETER NAME=\"server\">localhost</PARAMETER>\n"
        xml += "</CONFIG>"

        c1 = Libgmsec_ruby::Config.new()

        c1.from_xml(xml)

        check("Unexpected XML content", c1.to_xml() == xml)
	end


    def test_get_from_file()
        Libgmsec_ruby::Log::info("Test get_from_file()")

        # Nominal test
        begin
            config1 = Libgmsec_ruby::Config::get_from_file( get_data_file("T002_Config_good.xml"), "Bolt" )
            check("Unexpected return value", config1 != nil)
            check("Unexpected mw-id value", config1.get_value("mw-id") == "bolt")
            check("Unexpected server value", config1.get_value("server") == "localhost")

            config2 = Libgmsec_ruby::Config::get_from_file( get_data_file("T002_Config_good.xml"), "ActiveMQ" )
            check("Unexpected return value", config2 != nil)
            check("Unexpected mw-id value", config2.get_value("mw-id") == "activemq39")
            check("Unexpected server value", config2.get_value("server") == "tcp://localhost:61616")

            config3 = Libgmsec_ruby::Config::get_from_file( get_data_file("T002_Config_good.xml"), "" )
            check("Unexpected return value", config3 != nil)
            check("Unexpected mw-id value", config3.get_value("mw-id") == "bolt")
            check("Unexpected server value", config3.get_value("server") == "localhost")

        rescue GmsecException => e
            require(e.message, false)
        end

        # Off-nominal tests
        begin
            Libgmsec_ruby::Config::get_from_file( get_data_file("T002_Config_bad.xml"), "Bolt" )
            check("Unexpected result", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Invalid XML format -- parse error"))
        end

        begin
            Libgmsec_ruby::Config::get_from_file( "file-does-not-exist", "Bolt" )
            check("Unexpected result", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Configuration file could not be found or opened"))
        end
    end


    def test_to_json()
        Libgmsec_ruby::Log::info("Test to_json()")

        json = "{\"CONFIG\":{\"PARAMETER\":[{\"NAME\":\"mw-id\",\"VALUE\":\"bolt\"},{\"NAME\":\"server\",\"VALUE\":\"localhost\"}]}}"

        c1 = Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA)

        check("Unexpected JSON content", c1.to_json() == json)
	end
end


begin
    test = Test_Config.new( ARGV )
    test.run
end
