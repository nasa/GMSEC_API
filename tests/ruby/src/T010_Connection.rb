#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_Connection < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_create
        test_get_api_version
        test_connect
        test_disconnect
        test_get_library_name
        test_get_library_version
        test_get_message_factory
        test_register_event_callback
        test_subscribe
        test_unsubscribe
        test_publish                    # also calls on subscribe() and receive()
        test_publish_with_mwconfig
        test_request                    # also calls on reply()
        test_reply
        test_dispatch
        test_receive
        test_exclude_subject            # also tests remove_excluded_subject()
        test_remove_excluded_subject
        test_get_set_name
        test_get_id
        test_get_mw_info
        test_get_connection_endpoint
	end


    def test_create()
        Libgmsec_ruby::Log::info("Test create")

        # Nominal test
		begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            check("Okay", true)
            Libgmsec_ruby::Connection::destroy( conn )
        rescue GmsecException => e
            require(e.message, false)
		end

        # Off-Nominal test
		begin
            # missing mw-id
            conn = Libgmsec_ruby::Connection::create( Libgmsec_ruby::Config.new() )
            check("Exception expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("mw-id in Config is not specified"))
		end

        begin
            # bogus middleware
            conn = Libgmsec_ruby::Connection::create( Libgmsec_ruby::Config.new("mw-id=bogus", Libgmsec_ruby::DataType_KEY_VALUE_DATA) )
            check("Exception expected", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Unable to load"))
		end
	end


    def test_get_api_version()
        Libgmsec_ruby::Log::info("Test get_api_version()")
        version = "GMSEC API v" + Libgmsec_ruby::GMSEC_VERSION_NUMBER
        check("Expected to get API version info", Libgmsec_ruby::Connection::get_api_version().include?(version))
	end


    def test_connect()
        Libgmsec_ruby::Log::info("Test connect()")

        conn = nil

        # Nominal test
        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            check("Okay", true)
        rescue GmsecException => e
            require(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        # Off-nominal test
        begin
            config = Libgmsec_ruby::Config.new( "mw-id=bolt server=gs580s-gc764mw1:1234", Libgmsec_ruby::DataType_KEY_VALUE_DATA )
            conn = Libgmsec_ruby::Connection::create(config)
            conn.connect()
            check("Expected error to indicate unable to connect", false)
        rescue GmsecException => e
            e1 = Libgmsec_ruby::GmsecException.new( e.message )
            result = (e1.get_error_class() == Libgmsec_ruby::MIDDLEWARE_ERROR)  &&
                     (e1.get_error_code()  == Libgmsec_ruby::CUSTOM_ERROR_CODE) &&
                     (e1.get_custom_code() == 7)
            check(e.message, result)
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_disconnect()
        Libgmsec_ruby::Log::info("Test disconnect()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
                check("Okay", true)
            end
		end
	end


    def test_get_library_name()
        Libgmsec_ruby::Log::info("Test get_library_name()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            check("Unexpected library name", conn.get_library_name().include?(get_config().get_value("mw-id", "unknown")))
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_get_library_version()
        Libgmsec_ruby::Log::info("Test get_library_version()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            check("Expected to get a library version", conn.get_library_version() != nil)
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_get_message_factory()
        Libgmsec_ruby::Log::info("Test get_message_factory()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            check("Expected to get a MessageFactory for the CURRENT specification", Libgmsec_ruby::GMSEC_MSG_SPEC_CURRENT == conn.get_message_factory().get_specification().get_version())
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_register_event_callback()
        Libgmsec_ruby::Log::info("Test register_event_callback()")

        mw = get_config().get_value("mw-id", "unknown")

        if (mw.include?("unknown") || mw.include?("mb") || mw.include?("generic_jms") || mw.include?("zeromq413") || mw.include?("zeromq432"))
            # Test is being skipped because of lame middleware wrapper
            # which does not support event notification upon connect
            # and/or disconnect to/from the GMSEC Bus.
            return 0
        end

        mec  = T010_EventCallback.new()
        conn = nil

        # Nominal case
        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.register_event_callback( Libgmsec_ruby::Connection::Event_ALL_EVENTS, mec )

            conn.connect()
            check("Expected EventCallback to be called", true == mec.was_called())
            check("Expected to be connected", Libgmsec_ruby::Connection::Event_CONNECTION_SUCCESSFUL_EVENT == mec.get_event())

            mec.reset()

            conn.disconnect()
            check("Expected EventCallback to be called", true == mec.was_called())
            check("Expected to be disconnected", Libgmsec_ruby::Connection::Event_CONNECTION_BROKEN_EVENT == mec.get_event())
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        # Off-nominal case
        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.register_event_callback( Libgmsec_ruby::Connection::Event_ALL_EVENTS, nil )
        rescue GmsecException => e
            check(e.message, e.message.include?("EventCallback cannot be NULL"))
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_subscribe()
        Libgmsec_ruby::Log::info("Test subscribe()")

        conn = nil 

        # Nominal test
        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            conn.subscribe( get_subject("*.BAR") )
            conn.subscribe( get_subject("FOO.BAZ") )

            msg1 = conn.get_message_factory().create_message("LOG")
            msg2 = conn.get_message_factory().create_message("LOG")
            msg3 = conn.get_message_factory().create_message("LOG")

            msg1.set_subject( get_subject("GIN.BAR") )
            msg2.set_subject( get_subject("FOO.BAZ") )
            msg3.set_subject( get_subject("FOO.GAZ") )

            conn.publish(msg1)

            msg = conn.receive(5000)
            require("Was expecting to receive a message", msg != nil)
            check("Unexpected message subject", msg.get_subject() == get_subject("GIN.BAR"))
            Libgmsec_ruby::Message::destroy(msg)

            conn.publish(msg2)

            msg = conn.receive(5000)
            require("Was expecting to receive a message", msg != nil)
            check("Unexpected message subject", msg.get_subject() == get_subject("FOO.BAZ"))
            Libgmsec_ruby::Message::destroy(msg)

            conn.publish(msg3)

            msg = conn.receive(5000)
            check("Was not expecting to receive a message", msg == nil)
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        # Off-nominal tests
        begin
            # duplicate subscription
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            conn.subscribe("GMSEC.>")
            conn.subscribe("GMSEC.>")
            check("Was expecting an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Duplicate subscription"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # subscribe() before connect()
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.subscribe("GMSEC.>")
            check("Was expecting an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Connection has not been initialized"))
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # null subscription subject
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            conn.subscribe(nil)
            check("Was expecting an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Subject is NULL or is empty string"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # illegal subscription subject
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            conn.subscribe("GMSEC.MY MISSION.*.*.>")
            check("Was expecting an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Subject is invalid"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_unsubscribe()
        Libgmsec_ruby::Log::info("Test unsubscribe()")

        mcb = T010_Callback.new()

        config = Libgmsec_ruby::Config.new( get_config() )  # make copy!
        config.add_value("gmsec-msg-content-validate-recv", "true")

        conn = nil

        # Nominal tests
        begin
            conn = Libgmsec_ruby::Connection::create(config)
            conn.connect()
            info = conn.subscribe( get_subject("FOO.BAR") )
            conn.unsubscribe(info)

            hb = conn.get_message_factory().create_message("HB")
            hb.set_subject( get_subject("FOO.BAR") )

            conn.publish(hb)

            msg = conn.receive(5000)

            check("Did not expect to receive a message", msg == nil)
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        # Off-nominal tests
        begin
            # Attempt to unsubscribe using NULL object
            conn = Libgmsec_ruby::Connection::create(config)
            conn.connect()
            info = nil
            conn.unsubscribe(info)
        rescue GmsecException => e
            check(e.message, e.message.include?("Cannot unsubscribe using NULL SubscriptionInfo object"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        conn2 = nil
        begin
            # Attempt to unsubscribe using wrong Connection object
            conn  = Libgmsec_ruby::Connection::create(config)
            conn2 = Libgmsec_ruby::Connection::create(config)

            conn.connect()
            conn2.connect()

            info1 = conn.subscribe("GMSEC.FOO.BAR")

            conn2.unsubscribe(info1)
        rescue GmsecException => e
            check(e.message, e.message.include?("SubscriptionInfo object is not associated with this Connection"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
            if conn2 != nil
                conn2.disconnect()
                Libgmsec_ruby::Connection::destroy( conn2 )
                conn2 = nil
            end
		end
	end


    def test_publish()
        Libgmsec_ruby::Log::info("Test publish()")

        # Nominal test
        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )

            conn.connect()
            conn.subscribe( get_subject(">") )

            set_standard_fields(conn.get_message_factory())

            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( get_subject("HB") )

            conn.publish(msg)

            rcvd = conn.receive(5000)

            if rcvd != nil
                check("Unexpected subject", rcvd.get_subject() == get_subject("HB"))
                check("Unexpected message kind", rcvd.get_kind() == Libgmsec_ruby::Message::Kind_PUBLISH)

                Libgmsec_ruby::Message::destroy(rcvd)
            else
                check("Failed to receive published message", false)
			end
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        # Off-nominal tests
        begin
            # Message validation fails (missing standard fields)
            config = Libgmsec_ruby::Config.new( get_config() )  # make a copy!
            config.add_value("gmsec-msg-content-validate", "true")

            conn = Libgmsec_ruby::Connection::create(config)
            msg = conn.get_message_factory().create_message("HB")
            conn.connect()
            conn.publish(msg)
            check("Was expecting an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Message Validation Failed"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # Connection not initialized
            conn = Libgmsec_ruby::Connection::create( get_config() )
            msg = conn.get_message_factory().create_message("HB")
            conn.publish(msg)
            check("Was expecting an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Connection has not been initialized"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # Non-PUBLISH kind message
            conn = Libgmsec_ruby::Connection::create( get_config() )
            msg = conn.get_message_factory().create_message("REQ.DIR")
            conn.connect()
            conn.publish(msg)
            check("Was expecting an exception", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Cannot publish message with non-PUBLISH message kind"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_publish_with_mwconfig()
        Libgmsec_ruby::Log::info("Test publish() with mwConfig")

        conn    = nil
        subject = get_subject("MW-CONFIG")

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )

            set_standard_fields( conn.get_message_factory() )

            # Create message
            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( subject )

            mwConfig = Libgmsec_ruby::Config::new()

            conn.connect()

            conn.subscribe( subject )

            conn.publish( msg, mwConfig )

            rcv = conn.receive( 5000 )

            if rcv != nil
                check("Received a message", true)

                Libgmsec_ruby::Message::destroy( rcv )
            else
                check("Failed to received a message", false)
            end

            conn.disconnect()

        rescue GmsecException => e
            check(e.message, false)

        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_request()
        Libgmsec_ruby::Log::info("Test synchronous request()")

        conn = nil

        # Nominal case
        begin
            response_subject = get_subject("RESP.DIR")
            request_subject = get_subject("REQ.DIR")

            # Define args for sub-process that will issue response messages
            #
            args = ""
            cp = Libgmsec_ruby::ConfigPair.new()
            has_pair = get_config().get_first(cp)
            while has_pair do
                args += "#{cp.get_name}=#{cp.get_value} "

                has_pair = get_config().get_next(cp)
            end

            args += "req-subject=#{request_subject} rep-subject=#{response_subject} loglevel=none"
            responder = get_data_file("T010_Responder.rb")

            # Start the sub-process (i.e. responder)
            ruby_cmd = "ruby"
            if not OS.windows?
                ruby_cmd = "/usr/bin/ruby"
            end

            Libgmsec_ruby::Log::info("Spawning... #{ruby_cmd} #{responder} #{args}")

            spawn("#{ruby_cmd} #{responder} #{args}")

            # Allow time for the responder process to start
            Libgmsec_ruby::TimeUtil::millisleep(5000)

            conn = Libgmsec_ruby::Connection::create( get_config() )

            set_standard_fields( conn.get_message_factory() )

            conn.connect()

            # Subscribe to receive the response message
            conn.subscribe(response_subject)

            # Create request message
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            reqMsg.set_subject(request_subject)

            reqMsg.add_field( Libgmsec_ruby::StringField.new("DIRECTIVE-STRING", "Do something!") )
            reqMsg.add_field( Libgmsec_ruby::StringField.new("DESTINATION-COMPONENT", "RESPONDER") )

            # Connect to the bus (main process)
            # Verify response is returned
            reqMsg.add_field( Libgmsec_ruby::U16Field.new("REQUEST-ID", 0) )
            repMsg = conn.request(reqMsg, 5000, Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)

            if repMsg == nil
                check("Expected to receive response message", false)
            else
                check("Unexpected response message kind", repMsg.get_kind() == Libgmsec_ruby::Message::Kind_REPLY)
                check("Unexpected response message subject", repMsg.get_subject().include?( get_subject("RESP.DIR")))
                Libgmsec_ruby::Message::destroy(repMsg)
            end

            reqMsg.add_field( Libgmsec_ruby::U16Field.new("REQUEST-ID", 1) )
            repMsg = conn.request(reqMsg, 5000, Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)

            if repMsg != nil
                check("Was not expecting to receive response message", false)
                Libgmsec_ruby::Message::destroy(repMsg);
            end

            # TODO
            # If the spawned task is not running, then this test has the potential
            # to 'fail' in the sense that it will never receive a response message.
            # The API will continue reissuing the request message indefinitely, and
            # this will give the appearance that the unit test is hung.
            # This is NOT good. For now, the test is commented out.
            #
            #reqMsg.add_field( Libgmsec_ruby::U16Field.new("REQUEST-ID", 2) )
            #repMsg = conn.request(reqMsg, 5000, 1000)

            #if repMsg == nil
            #    check("Expected to receive response message", false)
            #else
            #    Libgmsec_ruby::Message::destroy(repMsg)
            #end
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
        end

        # Off-nominal tests
        begin
            # Connection not initialized
            conn = Libgmsec_ruby::Connection::create( get_config() )
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            conn.request(reqMsg, 5000, Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)
            check("Connection not initialized", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Connection has not been initialized"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
        end

        begin
            # Request using non-REQUEST kind message
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            badMsg = conn.get_message_factory().create_message("RESP.DIR")
            conn.request(badMsg, 5000, Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)
            check("Was expecting bad message error", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Cannot issue request with non-REQUEST kind message"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
        end
    end


    def test_reply()
        Libgmsec_ruby::Log::info("Test reply()")

        # Nominal tests for Connection.reply() tested earlier

        conn = nil

        # Off-nominal tests
        begin
            # Connection not initialized
            conn = Libgmsec_ruby::Connection::create( get_config() )
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            repMsg = conn.get_message_factory().create_message("RESP.DIR")
            conn.reply(reqMsg, repMsg)
            check("Was expecting an exception since we ain't initialized", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Connection has not been initialized"))
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # Bad reply message
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            badMsg = conn.get_message_factory().create_message("REQ.DIR")
            conn.reply(reqMsg, badMsg)
            check("Was expecting an exception since we have a bad reply message", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Cannot issue reply with non-REPLY kind message"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_dispatch()
        Libgmsec_ruby::Log::info("Test dispatch()")

        mcb1 = T010_Callback.new()
        mcb2 = T010_Callback.new()
        mcb3 = T010_Callback.new()

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()

            set_standard_fields( conn.get_message_factory() )

            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( get_subject("HB") )

            info1 = conn.subscribe( msg.get_subject(), mcb1 )
            info2 = conn.subscribe( msg.get_subject(), mcb2 )
            info3 = conn.subscribe( get_subject("FOO.BAR"),  mcb3 )

            conn.publish(msg)

            rcvd = conn.receive(5000)

            if rcvd != nil
                conn.dispatch(rcvd)

                Libgmsec_ruby::TimeUtil.millisleep(1000)

                check("Expected Callback 1 to receive a message", mcb1.was_called())
                check("Expected Callback 2 to receive a message", mcb2.was_called())
                check("Expected Callback 3 to not receive a message", mcb3.was_called() == false)

                Libgmsec_ruby::Message::destroy(rcvd)
            else
                check("Failed to receive message", false)
			end

            conn.unsubscribe(info1)
            conn.unsubscribe(info2)
            conn.unsubscribe(info3)

        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_receive()
        Libgmsec_ruby::Log::info("Test receive()")

        conn = nil

        # Nominal tests
        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()

            set_standard_fields( conn.get_message_factory() )

            # Timeout occurs (no subscription)
            rcvd = conn.receive(10)
            check("Received unexpected message", rcvd == nil)

            # Receive message
            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( get_subject("HB") )

            conn.subscribe(msg.get_subject())
            conn.publish(msg)

            rcvd = conn.receive(5000)
            check("Did not receive expected message", rcvd != nil)
			Libgmsec_ruby::Message::destroy(rcvd)
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        # Off-nominal tests
        begin
            # Connection not initialized
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.receive(10)
            check("Expected exception since we're not initialized", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Connection has not been initialized"))
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_exclude_subject()
        Libgmsec_ruby::Log::info("Test exclude_subject()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()

            set_standard_fields( conn.get_message_factory() )

            msg1 = conn.get_message_factory().create_message("HB")
            msg2 = conn.get_message_factory().create_message("LOG")

            msg1.set_subject( get_subject("HB") )
            msg2.set_subject( get_subject("LOG") )

            msg2.add_field( Libgmsec_ruby::StringField.new("SUBCLASS", "NO-CLASS") )
            msg2.add_field( Libgmsec_ruby::StringField.new("OCCURRENCE-TYPE", "FREQUENTLY") )
            msg2.add_field( Libgmsec_ruby::I16Field.new("SEVERITY", 1) )

            conn.subscribe( get_subject(">") )
            conn.exclude_subject( get_subject("LOG.+") )

            conn.publish(msg1)

            rcvd = conn.receive(5000)
            check("Was expecting to receive an HB message", rcvd != nil)
            Libgmsec_ruby::Message::destroy(rcvd)

            conn.publish(msg2)
            rcvd = conn.receive(5000)
            check("Was not expecting to receive a LOG message", rcvd == nil)

            conn.remove_excluded_subject( get_subject("LOG.+") )

            conn.publish(msg2)
            rcvd = conn.receive(5000)
            check("Was expecting to receive a LOG message", rcvd != nil)
            Libgmsec_ruby::Message::destroy(rcvd)
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        # Off-nominal tests
        begin
            # Connection not initialized
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.exclude_subject("GMSEC.FOO.BAR")
            check("Expected exception since we're not initialized", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Connection has not been initialized"))
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # Invalid subject
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            conn.exclude_subject("GMSEC FOO BAR")
            check("Expected exception for invalid subject", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Subject is invalid"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_remove_excluded_subject()
        Libgmsec_ruby::Log::info("Test remove_excluded_subject()")

        # Nominal test for remove_excluded_subject() are conducted witin test_exclude_subject()

        conn = nil

        # Off-nominal tests
        begin
            # Connection not initialized
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.remove_excluded_subject("GMSEC.FOO.BAR")
            check("Expected exception since we're not initialized", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Connection has not been initialized"))
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end

        begin
            # Invalid subject
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            conn.remove_excluded_subject("GMSEC FOO BAR")
            check("Expected exception for invalid subject", false)
        rescue GmsecException => e
            check(e.message, e.message.include?("Subject is invalid"))
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_get_set_name()
        Libgmsec_ruby::Log::info("Test get_name() and set_name()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            check("Expected a connection name", conn.get_name() != nil)

            conn.set_name("FOOBAR")

            check("Expected a connection name of FOOBAR", conn.get_name() == "FOOBAR")
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_get_id()
        Libgmsec_ruby::Log::info("Test get_id()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            check("Expected a connection ID", conn.get_id() != nil)
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_get_mw_info()
        Libgmsec_ruby::Log::info("Test get_mw_info()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            check("Expected mwInfo to contain mw-id", conn.get_mw_info().include?(get_config().get_value("mw-id")))
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
		end
	end


    def test_get_connection_endpoint()
        Libgmsec_ruby::Log::info("Test get_connection_endpoint()")

        conn = nil

        begin
            conn = Libgmsec_ruby::Connection::create( get_config() )
            conn.connect()
            check("Unexpected connection endpoint", conn.get_connection_endpoint() != nil)
        rescue GmsecException => e
            check(e.message, false)
        ensure
            if conn != nil
                conn.disconnect()
                Libgmsec_ruby::Connection::destroy( conn )
                conn = nil
            end
        end
	end
end


class T010_EventCallback < Libgmsec_ruby::EventCallback
    def initialize()
        super()
        @m_called = false
        @m_event  = Libgmsec_ruby::Connection::Event_ALL_EVENTS
	end

    def on_event(conn, status, event)
        @m_called = true
        @m_event  = event
	end

    def was_called()
        @m_called
	end

    def get_event()
        @m_event
	end

    def reset()
        @m_called = false
        @m_event  = Libgmsec_ruby::Connection::Event_ALL_EVENTS
	end
end


class T010_Callback < Libgmsec_ruby::Callback
    def initialize()
        super()
        @m_called = false
	end

    def on_message(conn, msg)
        @m_called = true
	end

    def was_called()
        @m_called
	end

    def reset()
        @m_called = false
	end
end


class T010_RequestCallback < Libgmsec_ruby::Callback
    def initialize()
        super()
        @m_provideResponse = true
	end

    def on_message(conn, msg)
        begin
            if @m_provideResponse
                repMsg = conn.get_message_factory().create_message("RESP.DIR")

                repMsg.set_field_value("RESPONSE-STATUS", Libgmsec_ruby::Message::SUCCESSFUL_COMPLETION)

                if msg.has_field("COMPONENT")
                    repMsg.add_field( Libgmsec_ruby::StringField.new("DESTINATION-COMPONENT", msg.get_string_value("COMPONENT")) )
				end

                if msg.has_field("DESTINATION-COMPONENT")
                    repMsg.add_field( Libgmsec_ruby::StringField.new("COMPONENT", msg.get_string_value("DESTINATION-COMPONENT")) )
                else
                    repMsg.add_field( Libgmsec_ruby::StringField.new("COMPONENT", "RESPONDER") )
				end

                if msg.has_field("REQUEST-ID")
                    repMsg.add_field( msg.get_field("REQUEST-ID") )
				end

                conn.reply(msg, repMsg)
            else
                @m_provideResponse = true
			end
        rescue GmsecException => e
            Libgmsec_ruby::Log::error(e.message, false)
		end
	end

    def provide_response(flag)
        @m_provideResponse = flag
	end
end


begin
    test = Test_Connection.new( ARGV )
    test.run
end
