#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_ConfigFileIterator < Test

	def initialize(args)
		super(args)
        @cfgFile    = nil
        @msgFactory = nil
	end


    def test()
        setup()

        test_iterator()
        test_reset()

        teardown()
	end


    def test_iterator()
        iter = @cfgFile.get_iterator()

        Libgmsec_ruby::Log::info("Test has_next_subscription() and next_subscription()")
        numSubscriptions = 0
        while iter.has_next_subscription() do
            entry = iter.next_subscription()

            numSubscriptions += 1

            if numSubscriptions == 1
                check("Unexpected subscription entry 1 name", entry.get_name() == "AllMessages")
                check("Unexpected subscription entry 1 pattern", entry.get_pattern() == "GMSEC.>")
            else
                check("Unexpected subscription entry 2 name", entry.get_name() == "LogMessages")
                check("Unexpected subscription entry 2 pattern", entry.get_pattern() == "GMSEC.*.*.MSG.LOG.+")
			end
		end

        check("Unexpected number of subscription entries", numSubscriptions == 2)


        Libgmsec_ruby::Log::info("Test has_next_config() and next_config()")
        numConfigs = 0
        while iter.has_next_config() do
            entry = iter.next_config()

            numConfigs += 1

            if numConfigs == 1
                check("Unexpected config entry 1 name", entry.get_name() == "ActiveMQ")
                check("Unexpected config entry mw-id value", entry.get_config().get_value("mw-id") == "activemq39")
                check("Unexpected config entry server value", entry.get_config().get_value("server") == "tcp://localhost:61616")
            else
                check("Unexpected config entry 2 name", entry.get_name() == "Bolt")
                check("Unexpected config entry mw-id value", entry.get_config().get_value("mw-id") == "bolt")
                check("Unexpected config entry server value", entry.get_config().get_value("server") == "localhost")
			end
		end

        check("Unexpected number of config entries", numConfigs == 2)


        Libgmsec_ruby::Log::info("Test has_next_message() and next_message()")
        numMessages = 0
        while iter.has_next_message() do
            entry = iter.next_message()

            numMessages += 1

            if numMessages == 1
                check("Unexpected message entry 1 name", entry.get_name() == "Heartbeat")
                check("Unexpected message entry 1", entry.get_message().to_xml() == @msgFactory.create_message("HB").to_xml())
            else
                check("Unexpected message entry 2 name", entry.get_name() == "Log")
                check("Unexpected message entry 2", entry.get_message().to_xml() == @msgFactory.create_message("LOG").to_xml())
			end
		end

        check("Unexpected number of message entries", numMessages == 2)
	end


    def test_reset()
        Libgmsec_ruby::Log::info("Test reset()")

        iter = @cfgFile.get_iterator()

        check("Expected to have subscriptions", iter.has_next_subscription())
        check("Expected to have configs", iter.has_next_config())
        check("Expected to have messages", iter.has_next_message())

        while iter.has_next_subscription() do
            iter.next_subscription()
		end
        while iter.has_next_config() do
            iter.next_config()
		end
        while iter.has_next_message() do
            iter.next_message()
		end

        check("Expected NOT to have additional subscriptions", iter.has_next_subscription() == false)
        check("Expected NOT to have additional configs", iter.has_next_config() == false)
        check("Expected NOT to have additional messages", iter.has_next_message() == false)

        iter.reset()

        check("Expected to have subscriptions", iter.has_next_subscription())
        check("Expected to have configs", iter.has_next_config())
        check("Expected to have messages", iter.has_next_message())
	end


    def setup()
        if @cfgFile == nil
            @cfgFile = Libgmsec_ruby::ConfigFile.new()
		end

        if @msgFactory == nil
            @msgFactory = Libgmsec_ruby::MessageFactory::create()
            set_standard_fields(@msgFactory)
		end

        @cfgFile.add_subscription_entry( Libgmsec_ruby::SubscriptionEntry.new("AllMessages", "GMSEC.>") )
        @cfgFile.add_subscription_entry( Libgmsec_ruby::SubscriptionEntry.new("LogMessages", "GMSEC.*.*.MSG.LOG.+") )

        @cfgFile.add_config("ActiveMQ", Libgmsec_ruby::Config.new("mw-id=activemq39 server=tcp://localhost:61616", Libgmsec_ruby::DataType_KEY_VALUE_DATA))
        @cfgFile.add_config("Bolt", Libgmsec_ruby::Config.new("mw-id=bolt server=localhost", Libgmsec_ruby::DataType_KEY_VALUE_DATA))

        @cfgFile.add_message("Heartbeat", @msgFactory.create_message("HB"))
        @cfgFile.add_message("Log", @msgFactory.create_message("LOG"))
	end


    def teardown()
        if @msgFactory != nil
            Libgmsec_ruby::MessageFactory::destroy(@msgFactory)
        end
    end
end


begin
    test = Test_ConfigFileIterator.new( ARGV )
    test.run
end
