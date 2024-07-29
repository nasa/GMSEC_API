#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_ResourceGenerator < Test

    def initialize(args)
        super(args)

        @@stdFields = get_standard_fields()
    end


    def test()
        test_create_1
        test_create_2
        test_start
        test_stop
        test_set_field
    end


    def test_create_1()
        Libgmsec_ruby::Log::info("Test create 1")

        # Nominal test
        rsrcgen = Libgmsec_ruby::ResourceGenerator::create(get_config(), 5, 1, 10)
        check("ResourceGenerator should not be running", rsrcgen.is_running() == false)
        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen)

        # Off-nominal test(s)
        begin
            config = Libgmsec_ruby::Config::new(get_config())
            config.add_value("mw-id", "bogus-mw")
            Libgmsec_ruby::ResourceGenerator::create(config, 5, 1, 10)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Unable to load"))
        end
    end


    def test_create_2()
        Libgmsec_ruby::Log::info("Test create 2")

        emptyFieldList = Libgmsec_ruby::FieldArray.new()

        # Nominal test (empty, populated, and null list of fields)
        rsrcgen1 = Libgmsec_ruby::ResourceGenerator::create(get_config(), 5, 1, 10, emptyFieldList)
        check("ResourceGenerator should not be running", rsrcgen1.is_running() == false)
        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen1)

        rsrcgen2 = Libgmsec_ruby::ResourceGenerator::create(get_config(), 5, 1, 10, @@stdFields)
        check("ResourceGenerator should not be running", rsrcgen2.is_running() == false)
        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen2)

        # Off-nominal test(s)
        begin
            config = Libgmsec_ruby::Config::new(get_config())
            config.add_value("mw-id", "bogus-mw")
            Libgmsec_ruby::ResourceGenerator::create(config, 5, 1, 10, @@stdFields)
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Unable to load"))
        end
    end


    def test_start()
        Libgmsec_ruby::Log::info("Test start()")

        config = Libgmsec_ruby::Config::new(get_config())
        pubRate = 1

        rsrcgen = Libgmsec_ruby::ResourceGenerator::create( config, pubRate, 1, 10, @@stdFields )

        # To ensure this resource message is unique, we set the COMPONENT field
        rsrcgen.set_field( Libgmsec_ruby::StringField.new("COMPONENT", get_unique_component()) )

        rsrcgen.start()

        check("ResourceGenerator should be running", rsrcgen.is_running())

        verify_resource_message(config, pubRate)

        rsrcgen.stop()

        # Allow time for the RSRC-gen thread to stop
        sleep(4)

        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen)

        # Off-nominal tests
        Libgmsec_ruby::Log::info("Off-nominal cases...")
        config.add_value("gmsec-msg-content-validate", "true")

        rsrcgen2 = Libgmsec_ruby::ResourceGenerator::create(config, 1, 1, 10, get_standard_fields)

        # Add bogus field using a Field
        begin
            rsrcgen2.set_field( Libgmsec_ruby::U16Field.new("BOGUS-FIELD", 2) )
            rsrcgen2.start()
            check("An exception was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message Validation Failed"))
        end
        Libgmsec_ruby::Log::info("Cleanup...")
        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen2)
    end


    def test_stop()
        Libgmsec_ruby::Log::info("Test stop()")

        config = get_config()
        pubRate = 1

        rsrcgen = Libgmsec_ruby::ResourceGenerator::create( config, pubRate, 1, 10, @@stdFields )

        # To ensure this resource message is unique, we set the COMPONENT field
        rsrcgen.set_field( Libgmsec_ruby::StringField.new("COMPONENT", get_unique_component()) )

        rsrcgen.start()

        check("ResourceGenerator should be running", rsrcgen.is_running())

        rsrcgen.stop()

        check("ResourceGenerator should not be running", rsrcgen.is_running() == false)

        # Allow time for the RSRC-gen thread to stop
        sleep(4)

        conn = Libgmsec_ruby::Connection::create(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + get_unique_component())

        sleep(4)

        for i in 0..3 do
            rsrcMsg = conn.receive(5000)

            if rsrcMsg != nil
                check("Unexpectedly received a Resource Message", false)
                Libgmsec_ruby::Message::destroy(rsrcMsg)
            end
        end

        conn.disconnect()

        Libgmsec_ruby::Connection::destroy(conn)
        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen)
    end


    def test_set_field()
        Libgmsec_ruby::Log::info("Test set_field()")

        config = Libgmsec_ruby::Config.new( get_config() )     # make a copy
        pubRate = 1

        # Nominal tests
        rsrcgen = Libgmsec_ruby::ResourceGenerator::create( config, pubRate, 1, 10, @@stdFields )

        # To ensure this resource message is unique, we set the COMPONENT field
        rsrcgen.set_field( Libgmsec_ruby::StringField.new("COMPONENT", get_unique_component()) )

        rsrcgen.start()

        sleep(4)

        Libgmsec_ruby::Log::info("Test set_field() with a Field")
        newPubRate = 2
        rsrcgen.set_field( Libgmsec_ruby::U16Field.new("PUB-RATE", newPubRate) )

        verify_resource_message(config, newPubRate)

        rsrcgen.stop()

        # Allow time for the RSRC-gen thread to stop
        sleep(4)

        Libgmsec_ruby::Log::info("Cleanup...")
        Libgmsec_ruby::ResourceGenerator::destroy(rsrcgen)
    end


    def verify_resource_message(config, expectedPubRate)
        begin
            conn = Libgmsec_ruby::Connection::create(config)
            conn.connect()
            conn.subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + get_unique_component() + ".+")

            sleep(4)

            rsrcMsg = conn.receive(5000)

            if rsrcMsg != nil
                check("Unexpected MESSAGE-TYPE", rsrcMsg.get_string_value("MESSAGE-TYPE") == "MSG")
                check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.get_string_value("MESSAGE-SUBTYPE") == "RSRC")
                check("Unexpected PUB-RATE", rsrcMsg.get_integer_value("PUB-RATE") == expectedPubRate)

                Libgmsec_ruby::Message::destroy(rsrcMsg)
            else
                check("Failed to received Resource Message", false)
            end

            conn.disconnect()
            Libgmsec_ruby::Connection::destroy(conn)
        rescue GmsecException => e
            require("verify_resource_message: " + e.message, false);
        end
    end
end


begin
    test = Test_ResourceGenerator.new( ARGV )
    test.run
end
