#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_HeartbeatGenerator < Test

    def initialize(args)
        super(args)
    end


    def test()
        test_create_1
        test_create_2
        test_start
        test_stop
        test_change_publish_rate
        test_set_field
    end


    def test_create_1()
        Libgmsec_ruby::Log::info("Test create 1")

        # Nominal test
        hbgen = Libgmsec_ruby::HeartbeatGenerator::create(get_config(), 5)
        check("HeartbeatGenerator should not be running", hbgen.is_running() == false)
        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen)
    end


    def test_create_2()
        Libgmsec_ruby::Log::info("Test create 2")

        emptyFieldList = Libgmsec_ruby::FieldArray.new()
        standardFields = get_standard_fields()

        # Nominal test
        hbgen1 = Libgmsec_ruby::HeartbeatGenerator::create(get_config(), 5, emptyFieldList)
        hbgen2 = Libgmsec_ruby::HeartbeatGenerator::create(get_config(), 5, standardFields)

        check("HeartbeatGenerator should not be running", hbgen1.is_running() == false)
        check("HeartbeatGenerator should not be running", hbgen2.is_running() == false)

        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen1)
        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen2)
    end


    def test_start()
        Libgmsec_ruby::Log::info("Test start()")

        config = get_config()

        hbgen = Libgmsec_ruby::HeartbeatGenerator::create(config, 1, get_standard_fields())

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( Libgmsec_ruby::StringField.new("COMPONENT", get_unique_component()) )

        hbgen.start()

        check("HeartbeatGenerator should be running", hbgen.is_running())

        verify_heartbeat_message(config, 1)

        hbgen.stop()

        # Allow time for the HB-gen thread to stop
        Libgmsec_ruby::TimeUtil::millisleep(2000)

        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen)
    end


    def test_stop()
        Libgmsec_ruby::Log::info("Test stop()")

        config = get_config()

        hbgen = Libgmsec_ruby::HeartbeatGenerator::create(config, 1, get_standard_fields())

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( Libgmsec_ruby::StringField.new("COMPONENT", get_unique_component()) )

        hbgen.start()

        check("HeartbeatGenerator should be running", hbgen.is_running())

        hbgen.stop()

        check("HeartbeatGenerator should not be running", hbgen.is_running() == false)

        # Allow time for the HB-gen thread to stop
        Libgmsec_ruby::TimeUtil::millisleep(2000)

        conn = Libgmsec_ruby::Connection::create(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + get_unique_component())

        for i in 0..3 do
            hbmsg = conn.receive(5000)

            if hbmsg != nil
                check("Unexpectedly received a Heartbeat Message", false)
                Libgmsec_ruby::Message::destroy(hbmsg)
            end
        end

        conn.disconnect()

        Libgmsec_ruby::Connection::destroy(conn)
        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen)
    end


    def test_change_publish_rate()
        Libgmsec_ruby::Log::info("Test change_publish_rate()")

        config = get_config()

        hbgen = Libgmsec_ruby::HeartbeatGenerator::create(config, 1, get_standard_fields())

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( Libgmsec_ruby::StringField.new("COMPONENT", get_unique_component()) )

        hbgen.start()

        hbgen.change_publish_rate(2)

        Libgmsec_ruby::TimeUtil::millisleep(2000);

        verify_heartbeat_message(config, 2)

        conn = Libgmsec_ruby::Connection::create(config)
        conn.connect()
        conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + get_unique_component())

        Libgmsec_ruby::TimeUtil::millisleep(1000);

        hbgen.change_publish_rate(0)

        Libgmsec_ruby::TimeUtil::millisleep(1000);

        lastMessage = false

        for i in 0..2 do
            hbmsg = conn.receive(5000)

            if hbmsg == nil
                check("Failed to receive Heartbeat Message", false)
            elsif hbmsg.get_integer_value("PUB-RATE") != 0
                Libgmsec_ruby::Message::destroy(hbmsg)
            else
                lastMessage = true 
                Libgmsec_ruby::Message::destroy(hbmsg)
                break
            end
        end

        check("Failed to receive message with PUB-RATE of 0", lastMessage)

        conn.disconnect()
        Libgmsec_ruby::Connection::destroy(conn)

        hbgen.stop()

        # Allow time for the HB-gen thread to stop
        Libgmsec_ruby::TimeUtil::millisleep(2000)

        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen)
    end


    def test_set_field()
        config = Libgmsec_ruby::Config.new( get_config() )     # make a copy

        # Nominal tests
        hbgen = Libgmsec_ruby::HeartbeatGenerator::create(config, 1, get_standard_fields)

        # To ensure this heartbeat message is unique, we set the COMPONENT field
        hbgen.set_field( Libgmsec_ruby::StringField.new("COMPONENT", get_unique_component()) )

        hbgen.start()

        Libgmsec_ruby::TimeUtil::millisleep(2000)

        Libgmsec_ruby::Log::info("Test set_field() with a Field")
        hbgen.set_field( Libgmsec_ruby::U16Field.new("PUB-RATE", 2) )
        verify_heartbeat_message(config, 2)

        Libgmsec_ruby::Log::info("Test set_field() with a Field Name and long value")
        hbgen.set_field("PUB-RATE", 3)
        verify_heartbeat_message(config, 3)

        Libgmsec_ruby::Log::info("Test set_field() with a Field Name and double value")
        hbgen.set_field("PUB-RATE", 2.0)
        verify_heartbeat_message(config, 2)

        Libgmsec_ruby::Log::info("Test set_field() with a Field Name and string value")
        hbgen.set_field("PUB-RATE", "3")
        verify_heartbeat_message(config, 3)

        hbgen.stop()

        # Allow time for the HB-gen thread to stop
        Libgmsec_ruby::TimeUtil::millisleep(2000)

        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen)

        # Off-nominal tests
        Libgmsec_ruby::Log::info("Off-nominal cases...")
        config.add_value("gmsec-msg-content-validate", "true")

        hbgen2 = Libgmsec_ruby::HeartbeatGenerator::create(config, 1, get_standard_fields)

        # Add bogus field using a Field
        begin
            hbgen2.set_field( Libgmsec_ruby::U16Field.new("BOGUS-FIELD", 2) )
            check("An expection was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message Validation Failed"))
        end

        # Add bogus field using a long value
        begin
            hbgen2.set_field("BOGUS-FIELD", 2)
            check("An expection was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message Validation Failed"))
        end

        # Add bogus field using a double value
        begin
            hbgen2.set_field("BOGUS-FIELD", 2.0)
            check("An expection was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message Validation Failed"))
        end

        # Add bogus field using a string value
        begin
            hbgen2.set_field("BOGUS-FIELD", "2")
            check("An expection was expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message Validation Failed"))
        end

        Libgmsec_ruby::Log::info("Cleanup...")
        Libgmsec_ruby::HeartbeatGenerator::destroy(hbgen2)
    end


    def verify_heartbeat_message(config, expectedPubRate)
        begin
            conn = Libgmsec_ruby::Connection::create(config)
            conn.connect()
            conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + get_unique_component() + ".+")

            t1 = 0
            t2 = 0

            for i in 0..8 do
                hbmsg = conn.receive(5000)

                # ignore the first few incoming messages (if any)
                if i < 3
                    if hbmsg != nil
                        Libgmsec_ruby::Message::destroy(hbmsg)
                    end
                    next
                end

                if hbmsg != nil
                    if t1 == 0
                        t1 = Libgmsec_ruby::TimeUtil.get_current_time_s()
                    else
                        t2 = Libgmsec_ruby::TimeUtil.get_current_time_s()

                        delta = t2 - t1
                        if delta < expectedPubRate
                            delta = ((t2 - t1) * 10.0 + 0.5) / 10.0
                        end

                        Libgmsec_ruby::Log::info("Expected rate is: " + expectedPubRate.to_s + ", delta is: " + delta.to_s)

                        check("Unexpected publish rate", expectedPubRate == delta.to_i)

                        t1 = t2
                    end

                    check("Unexpected MESSAGE-TYPE", hbmsg.get_string_value("MESSAGE-TYPE") == "MSG")
                    check("Unexpected MESSAGE-SUBTYPE", hbmsg.get_string_value("MESSAGE-SUBTYPE") == "HB")
                    check("Unexpected PUB-RATE", hbmsg.get_integer_value("PUB-RATE") == expectedPubRate)

                    Libgmsec_ruby::Message::destroy(hbmsg)
                else
                    check("Failed to received Heartbeat Message", false)
                end
            end

            conn.disconnect()
            Libgmsec_ruby::Connection::destroy(conn)
        rescue GmsecException => e
            require("verify_heartbeat_message: " + e.message, false)
        end
    end
end


begin
    test = Test_HeartbeatGenerator.new( ARGV )
    test.run
end
