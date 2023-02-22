#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_CreateResourceMessage < Test

    def initialize(args)
        super(args)

        @@stdFields = get_standard_fields()
    end


    def test()
        test_create_resource_message()
    end


    def test_create_resource_message()
        Libgmsec_ruby::Log::info("Test create_resource_message()")

        factory = Libgmsec_ruby::MessageFactory::create( get_config() )
        samInt  = 1
        avgInt  = 10

        factory.set_standard_fields( @@stdFields )

        rsrcMsg = Libgmsec_ruby::ResourceGenerator::create_resource_message( factory, samInt, avgInt )

        check("Unexpected MESSAGE-TYPE", rsrcMsg.get_string_value("MESSAGE-TYPE") == "MSG")
        check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.get_string_value("MESSAGE-SUBTYPE") == "RSRC")

        Libgmsec_ruby::MessageFactory::destroy( factory )
    end
end


begin
    test = Test_CreateResourceMessage.new( ARGV )
    test.run
end
