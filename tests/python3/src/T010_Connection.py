#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_Connection(Test):

    def test(self):
        self.test_constructor()
        self.test_get_api_version()
        self.test_connect()
        self.test_disconnect()
        self.test_get_library_name()
        self.test_get_library_version()
        self.test_get_config()
        self.test_get_message_factory()
        self.test_register_event_callback()
        self.test_subscribe()
        self.test_subscribe_with_callback()    # also calls on startAutoDispatch/stopAutoDispatch
        self.test_unsubscribe()
        self.test_start_auto_dispatch()
        self.test_stop_auto_dispatch()
        self.test_publish()                    # also calls on subscribe() and receive()
        self.test_publish_with_mwconfig()
        self.test_request_with_callback()      # also calls on reply()
        self.test_request()                    # also calls on reply()
        self.test_reply()
        self.test_dispatch()
        self.test_receive()
        self.test_exclude_subject()            # also tests remove_excluded_subject()
        self.test_remove_excluded_subject()
        self.test_get_set_name()
        self.test_get_id()
        self.test_get_mw_info()
        self.test_get_connection_endpoint()
        self.test_get_publish_queue_message_count()


    def test_constructor(self):
        lp.log_info("Test constructor")

        # Nominal test
        try:
            conn = lp.Connection( self.get_config() )
            self.check("Okay", True)
        except Exception as e:
            self.require(str(e), False)

        # Off-Nominal test
        try:
            # missing mw-id
            conn = lp.Connection( lp.Config() )
            self.check("Exception expected", False)
        except Exception as e:
            self.check(str(e), "mw-id in Config is not specified" in str(e))

        try:
            # bogus middleware
            conn = lp.Connection( lp.Config("mw-id=bogus", lp.DataType_KEY_VALUE_DATA) )
            self.check("Exception expected", false)
        except Exception as e:
            self.check(str(e), "Unable to load" in str(e))


    def test_get_api_version(self):
        lp.log_info("Test get_api_version()")
        version = "GMSEC API v" + lp.GMSEC_VERSION_NUMBER
        self.check("Expected to get API version info", version in lp.Connection.get_api_version())


    def test_connect(self):
        lp.log_info("Test connect()")

        # Nominal test
        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()
            self.check("Okay", True)
            conn.disconnect()
        except Exception as e:
            self.require(str(e), False)

        # Off-nominal test
        try:
            config = lp.Config( "mw-id=bolt server=gs580s-gc764mw1:1234", lp.DataType_KEY_VALUE_DATA )
            conn = lp.Connection(config)
            conn.connect()
            self.check("Expected error to indicate unable to connect", False)
        except Exception as e:
            result = (e.get_error_class() == lp.MIDDLEWARE_ERROR) and (e.get_error_code() == lp.CUSTOM_ERROR_CODE) and (e.get_custom_code() == 7)
            self.check(str(e), result);


    def test_disconnect(self):
        lp.log_info("Test disconnect()")

        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.disconnect()
            self.check("Okay", True)
        except Exception as e:
            self.check(str(e), False)


    def test_get_library_name(self):
        lp.log_info("Test get_library_name()")

        try:
            conn = lp.Connection( self.get_config() )
            self.check("Unexpected library name", self.get_config().get_value("mw-id", "unknown") in conn.get_library_name())
        except Exception as e:
            self.check(str(e), False)


    def test_get_library_version(self):
        lp.log_info("Test get_library_version()")

        try:
            conn = lp.Connection( self.get_config() )
            self.check("Expected to get a library version", conn.get_library_version() != None)
        except Exception as e:
            self.check(str(e), False)


    def test_get_config(self):
        lp.log_info("Test get_config()")

        try:
            conn = lp.Connection( self.get_config() )
            self.check("Expected to get a lconfig", conn.get_config() != None)
        except Exception as e:
            self.check(str(e), False)


    def test_get_message_factory(self):
        lp.log_info("Test get_message_factory()")

        try:
            conn = lp.Connection( self.get_config() )
            self.check("Expected to get a MessageFactory for the CURRENT specification", lp.GMSEC_MSG_SPEC_CURRENT == conn.get_message_factory().get_specification().get_version())
        except Exception as e:
            self.check(str(e), False)


    def test_register_event_callback(self):
        lp.log_info("Test register_event_callback()")

        mw = self.get_config().get_value("mw-id", "unknown")

        if mw == "unknown" or mw == "mb" or  mw == "generic_jms" or mw == "zeromq413" or mw == "zeromq432":
            # Test is being skipped because of lame middleware wrapper
            # which does not support event notification upon connect
            # and/or disconnect to/from the GMSEC Bus.
            return

        mec = T010_EventCallback()

        # Nominal case
        try:
            conn = lp.Connection( self.get_config() )
            conn.register_event_callback( lp.Connection.Event_ALL_EVENTS, mec )

            conn.connect()
            self.check("Expected EventCallback to be called", True == mec.was_called())
            self.check("Expected to be connected", lp.Connection.Event_CONNECTION_SUCCESSFUL_EVENT == mec.get_event())

            mec.reset()

            conn.disconnect()
            self.check("Expected EventCallback to be called", True == mec.was_called())
            self.check("Expected to be disconnected", lp.Connection.Event_CONNECTION_BROKEN_EVENT == mec.get_event())
        except Exception as e:
            self.check(str(e), False)

        # Off-nominal case
        try:
            conn = lp.Connection( self.get_config() )
            conn.register_event_callback( lp.Connection.Event_ALL_EVENTS, None )
        except Exception as e:
            self.check(str(e), "EventCallback cannot be NULL" in str(e))


    def test_subscribe(self):
        lp.log_info("Test subscribe()")

        conn = None 

        # Nominal test
        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe( self.get_subject("*.BAR") )
            conn.subscribe( self.get_subject("FOO.BAZ") )

            msg1 = conn.get_message_factory().create_message("HB")
            msg2 = conn.get_message_factory().create_message("LOG")
            msg3 = conn.get_message_factory().create_message("LOG")

            msg1.set_subject( self.get_subject("GIN.BAR") )
            msg2.set_subject( self.get_subject("FOO.BAZ") )
            msg3.set_subject( self.get_subject("FOO.GAZ") )

            conn.publish(msg1)

            msg = conn.receive(5000)
            self.require("Was expecting to receive a message", msg != None)
            self.check("Unexpected message subject", msg.get_subject() == self.get_subject("GIN.BAR"))
            self.verify_tracking_fields(msg, 11)
            lp.Message.destroy(msg)

            conn.publish(msg2)

            msg = conn.receive(5000)
            self.require("Was expecting to receive a message", msg != None)
            self.check("Unexpected message subject", msg.get_subject() == self.get_subject("FOO.BAZ"))
            self.verify_tracking_fields(msg, 7)
            lp.Message.destroy(msg)

            conn.publish(msg3)

            msg = conn.receive(5000)
            self.check("Was not expecting to receive a message", msg == None)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # duplicate subscription
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe("GMSEC.>")
            conn.subscribe("GMSEC.>")
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Duplicate subscription" in str(e))

        if conn != None:
            conn.disconnect()

        try:
            # subscribe() before connect()
            conn = lp.Connection( self.get_config() )
            conn.subscribe("GMSEC.>")
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # null subscription subject
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe(None)
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Subject is NULL or is empty string" in str(e))

        if conn != None:
            conn.disconnect()

        try:
            # illegal subscription subject
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe("GMSEC.MY MISSION.*.*.>")
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Subject is invalid" in str(e))

        if conn != None:
            conn.disconnect()


    def test_subscribe_with_callback(self):
        lp.log_info("Test subscribe() with Callback")

        mcb = T010_Callback()
        ecb = T010_EventCallback()

        conn = None

        # Nominal test (verify Callback is summoned)
        try:
            conn = lp.Connection( self.get_config() )

            conn.connect()
            conn.register_event_callback( lp.Connection.Event_ALL_EVENTS, ecb )
            conn.subscribe( self.get_subject(">"), mcb )

            conn.start_auto_dispatch()

            self.set_standard_fields(conn.get_message_factory())

            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( self.get_subject("HB") )

            conn.publish(msg)

            lp.TimeUtil.millisleep(5000)

            self.check("Expected Callback to be summoned", mcb.was_called())
            self.check("Expected ALL_EVENTS", ecb.get_event() == lp.Connection.Event_ALL_EVENTS)

            mcb.reset()
            ecb.reset()
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Nominal test (verify Callback is not summoned because of illegal message)
        try:
            config = lp.Config( self.get_config() )  # make copy!
            config.add_value("gmsec-msg-content-validate-recv", "True")

            conn = lp.Connection( config )

            conn.connect()
            conn.register_event_callback( lp.Connection.Event_ALL_EVENTS, ecb )
            conn.subscribe( self.get_subject(">"), mcb )

            conn.start_auto_dispatch()

            self.set_standard_fields(conn.get_message_factory())

            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( self.get_subject("HB") )

            conn.publish(msg)

            lp.TimeUtil.millisleep(5000)

            self.check("Did not expect Callback to be summoned", mcb.was_called() == False)
            self.check("Expected an INVALID_MESSAGE_EVENT", ecb.get_event() == lp.Connection.Event_INVALID_MESSAGE_EVENT)

            autoStopped = conn.stop_auto_dispatch()
            self.check("Expected the auto-dispatcher to stop", autoStopped)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        try:
            # subscriptions using same subject and unique callbacks
            mcb2 = T010_Callback()
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe("GMSEC.>", mcb)
            conn.subscribe("GMSEC.>", mcb2)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # duplicate subscription using the same callback
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe("GMSEC.>", mcb)
            conn.subscribe("GMSEC.>", mcb)
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Duplicate subscription" in str(e))

        if conn != None:
            conn.disconnect()

        try:
            # null subscription subject
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe(None, mcb)
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Subject is NULL or is empty string" in str(e))

        if conn != None:
            conn.disconnect()

        try:
            # illegal subscription subject
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.subscribe("GMSEC.MY MISSION.*.*.>", mcb)
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Subject is invalid" in str(e))

        if conn != None:
            conn.disconnect()


    def test_unsubscribe(self):
        lp.log_info("Test unsubscribe()")

        mcb = T010_Callback()

        config = lp.Config( self.get_config() )  # make copy!
        config.add_value("gmsec-msg-content-validate-recv", "True")

        conn = None

        # Nominal tests
        try:
            # Attempt to unsubscribe using NULL object
            conn = lp.Connection(config)
            conn.connect()
            info = conn.subscribe( self.get_subject("FOO.BAR") )
            conn.unsubscribe(info)

            hb = conn.get_message_factory().create_message("HB")
            hb.set_subject( self.get_subject("FOO.BAR") )

            conn.publish(hb)

            msg = conn.receive(5000)

            self.check("Did not expect to receive a message", msg == None)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # Attempt to unsubscribe using NULL object
            conn = lp.Connection(config)
            conn.connect()
            info = None
            conn.unsubscribe(info)
        except Exception as e:
            self.check(str(e), "Cannot unsubscribe using NULL SubscriptionInfo object" in str(e))

        if conn != None:
            conn.disconnect()

        conn2 = None
        try:
            # Attempt to unsubscribe using wrong Connection object
            conn  = lp.Connection(config)
            conn2 = lp.Connection(config)

            conn.connect()
            conn2.connect()

            info1 = conn.subscribe("GMSEC.FOO.BAR")

            conn2.unsubscribe(info1)
        except Exception as e:
            self.check(str(e), "SubscriptionInfo object is not associated with this Connection" in str(e))

        if conn != None:
            conn.disconnect()
        if conn2 != None:
            conn2.disconnect()


    def test_start_auto_dispatch(self):
        lp.log_info("Test start_auto_dispatch()")

        # start_auto_dispatcher() already tested within test_subscribe_callback()

        # Off-nominal test
        try:
            # Auto-dispatch cannot start until Connection is initialized
            conn = lp.Connection( self.get_config() )
            conn.start_auto_dispatch()
            self.check("Expected an exception", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))


    def test_stop_auto_dispatch(self):
        lp.log_info("Test stop_auto_dispatch()")

        # stop_auto_dispatcher() already tested within test_subscribe_callback()

        # Off-nominal test
        try:
            # Auto-dispatch cannot be stopped until Connection is initialized
            conn = lp.Connection( self.get_config() )
            conn.stop_auto_dispatch()
            self.check("Expected an exception", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))


    def test_publish(self):
        lp.log_info("Test publish()")

        # Nominal test
        conn = None

        try:
            conn = lp.Connection( self.get_config() )

            conn.connect()
            conn.subscribe( self.get_subject(">") )

            self.set_standard_fields(conn.get_message_factory())

            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( self.get_subject("HB") )

            conn.publish(msg)

            rcvd = conn.receive(5000)

            if rcvd != None:
                self.check("Unexpected subject", rcvd.get_subject() == self.get_subject("HB"))
                self.check("Unexpected message kind", rcvd.get_kind() == lp.Message.Kind_PUBLISH)

                lp.Message.destroy(rcvd)
            else:
                self.check("Failed to receive published message", False)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # Message validation fails (missing standard fields)
            config = lp.Config( self.get_config() )  # make a copy!
            config.add_value("gmsec-msg-content-validate", "True")

            conn = lp.Connection(config)
            msg = conn.get_message_factory().create_message("HB")
            conn.connect()
            conn.publish(msg)
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Message Validation Failed" in str(e))

        if conn != None:
            conn.disconnect()

        try:
            # Connection not initialized
            conn = lp.Connection( self.get_config() )
            msg = conn.get_message_factory().create_message("HB")
            conn.publish(msg)
            self.check("Was expecting an exception", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # Non-PUBLISH kind message
            conn = lp.Connection( self.get_config() )
            msg = conn.get_message_factory().create_message("REQ.DIR")
            conn.connect()
            conn.publish(msg)
            self.check("Was expecting an exception", false)
        except Exception as e:
            self.check(str(e), "Cannot publish message with non-PUBLISH message kind" in str(e))

        if conn != None:
            conn.disconnect()


    def test_publish_with_mwconfig(self):
        lp.log_info("Test publish() with mwConfig")

        conn = None

        try:
            conn = lp.Connection( self.get_config() )

            self.set_standard_fields(conn.get_message_factory())

            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( self.get_subject("MW-CONFIG") )

            mwConfig = lp.Config()

            conn.connect()

            conn.subscribe( self.get_subject("MW-CONFIG") )

            conn.publish( msg, mwConfig )

            rcv = conn.receive( 5000 )

            if rcv != None:
                self.check("Received a message", True)

                lp.Message.destroy(rcv)
            else:
                self.check("Failed to receive a message", False)

            conn.disconnect()

        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()


    def test_request_with_callback(self):
        lp.log_info("Test asynchronous request()")

        reqCallback = T010_RequestCallback(self)
        repCallback = T010_ReplyCallback()

        conn = None

        # Nominal case
        try:
            conn = lp.Connection( self.get_config() )

            conn.connect()

            self.set_standard_fields(conn.get_message_factory())

            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            reqMsg.set_subject( self.get_subject("REQ.DIR") )
            reqMsg.add_field( lp.StringField("DIRECTIVE-STRING", "Do something!") )
            reqMsg.add_field( lp.StringField("DESTINATION-COMPONENT", "RESPONDER") )

            # subscribe to receive the response message
            conn.subscribe( self.get_subject("RESP.DIR") )

            # subscribe to process the request message
            conn.subscribe(reqMsg.get_subject(), reqCallback)
            conn.start_auto_dispatch()

            # Verify response is returned
            reqMsg.add_field( lp.U16Field("REQUEST-ID", 0) )
            conn.request(reqMsg, 5000, repCallback, lp.GMSEC_REQUEST_REPUBLISH_NEVER)
            lp.TimeUtil.millisleep(2000)

            self.check("Was expecting a reply", repCallback.got_reply() == True)
            self.check("Was not expecting an event", repCallback.got_event() == False)

            repCallback.reset()

            # Verify None is returned on timeout
            reqCallback.provide_response(False)

            reqMsg.add_field( lp.U16Field("REQUEST-ID", 1) )
            conn.request(reqMsg, 5000, repCallback, lp.GMSEC_REQUEST_REPUBLISH_NEVER)
            lp.TimeUtil.millisleep(5500)

            self.check("Was not expecting a reply", repCallback.got_reply() == False)
            self.check("Was expecting an event", repCallback.got_event() == True)

            # Verify request is delivered multiple times until response delivered
            reqCallback.provide_response(False)

            reqMsg.add_field( lp.U16Field("REQUEST-ID", 2) )
            conn.request(reqMsg, 5000, repCallback, 5000)
            lp.TimeUtil.millisleep(5500*2)

            self.check("Was expecting a reply", repCallback.got_reply() == True)
            self.check("Was expecting an event", repCallback.got_event() == True)

            # Verify request is delivered multiple times until response delivered
            reqCallback.provide_response(False)

            reqMsg.add_field( lp.U16Field("REQUEST-ID", 3) )
            conn.request(reqMsg, -1, repCallback, 5000)
            lp.TimeUtil.millisleep(5500*2)

            self.check("Was expecting a reply", repCallback.got_reply() == True)
            self.check("Was expecting an event", repCallback.got_event() == True)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # Connection not initialized
            conn = lp.Connection( self.get_config() )
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            conn.request(reqMsg, 5000, repCallback, lp.GMSEC_REQUEST_REPUBLISH_NEVER)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # Request using non-REQUEST kind message
            conn = lp.Connection( self.get_config() )
            conn.connect()
            badMsg = conn.get_message_factory().create_message("RESP.DIR")
            conn.request(badMsg, 5000, repCallback, lp.GMSEC_REQUEST_REPUBLISH_NEVER)
            self.check("An exception was expected", False)
        except Exception as e:
            self.check(str(e), "Cannot issue request with non-REQUEST kind message" in str(e))

        if conn != None:
            conn.disconnect()


    def test_request(self):
        lp.log_info("Test synchronous request()")

        reqCallback = T010_RequestCallback(self)

        conn = None

        # Nominal case
        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()

            self.set_standard_fields(conn.get_message_factory())

            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            reqMsg.set_subject( self.get_subject("REQ.DIR") )
            reqMsg.add_field( lp.StringField("DIRECTIVE-STRING", "Do something!") )
            reqMsg.add_field( lp.StringField("DESTINATION-COMPONENT", "RESPONDER") )

            # subscribe to receive the response message
            conn.subscribe( self.get_subject("RESP.DIR.+") )

            # subscribe to process the request message
            conn.subscribe(reqMsg.get_subject(), reqCallback)
            conn.start_auto_dispatch()

            # Verify response is returned
            reqMsg.add_field( lp.U16Field("REQUEST-ID", 0) )
            repMsg = conn.request(reqMsg, 5000, lp.GMSEC_REQUEST_REPUBLISH_NEVER)

            if repMsg == None:
                self.check("Expected to receive response message", False)
            else:
                self.check("Unexpected response message kind", repMsg.get_kind() == lp.Message.Kind_REPLY)
                self.check("Unexpected response message subject", self.get_subject("RESP.DIR") in repMsg.get_subject())
                lp.Message.destroy(repMsg);

            # Verify None is returned on timeout
            reqCallback.provide_response(False)

            reqMsg.add_field( lp.U16Field("REQUEST-ID", 1) )
            repMsg = conn.request(reqMsg, 5000, lp.GMSEC_REQUEST_REPUBLISH_NEVER)

            if repMsg != None:
                self.check("Was not expecting to receive response message", False)
                lp.Message.destroy(repMsg);

            # Verify request is delivered multiple times until response delivered
            reqCallback.provide_response(False)

            reqMsg.add_field( lp.U16Field("REQUEST-ID", 2) )
            repMsg = conn.request(reqMsg, 5000, 1000)

            if repMsg == None:
                self.check("Expected to receive response message", False)
            else:
                lp.Message.destroy(repMsg)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # Connection not initialized
            conn = lp.Connection( self.get_config() )
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            conn.request(reqMsg, 5000, lp.GMSEC_REQUEST_REPUBLISH_NEVER)
            self.check("Connection not initialized", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # Request using non-REQUEST kind message
            conn = lp.Connection( self.get_config() )
            conn.connect()
            badMsg = conn.get_message_factory().create_message("RESP.DIR")
            conn.request(badMsg, 5000, lp.GMSEC_REQUEST_REPUBLISH_NEVER)
            self.check("Was expecting bad message error", False)
        except Exception as e:
            self.check(str(e), "Cannot issue request with non-REQUEST kind message" in str(e))

        if conn != None:
            conn.disconnect()


    def test_reply(self):
        lp.log_info("Test reply()")

        # Nominal tests for Connection.reply() tested earlier

        conn = None

        # Off-nominal tests
        try:
            # Connection not initialized
            conn = lp.Connection( self.get_config() )
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            repMsg = conn.get_message_factory().create_message("RESP.DIR")
            conn.reply(reqMsg, repMsg)
            self.check("Was expecting an exception since we ain't initialized", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # Bad reply message
            conn = lp.Connection( self.get_config() )
            conn.connect()
            reqMsg = conn.get_message_factory().create_message("REQ.DIR")
            badMsg = conn.get_message_factory().create_message("REQ.DIR")
            conn.reply(reqMsg, badMsg)
            self.check("Was expecting an exception since we have a bad reply message", False)
        except Exception as e:
            self.check(str(e), "Cannot issue reply with non-REPLY kind message" in str(e))

        if conn != None:
            conn.disconnect()


    def test_dispatch(self):
        lp.log_info("Test dispatch()")

        mcb1 = T010_Callback()
        mcb2 = T010_Callback()
        mcb3 = T010_Callback()

        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()

            self.set_standard_fields(conn.get_message_factory())

            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( self.get_subject("HB") )

            info1 = conn.subscribe( msg.get_subject(), mcb1 )
            info2 = conn.subscribe( msg.get_subject(), mcb2 )
            info3 = conn.subscribe( self.get_subject("FOO.BAR"),  mcb3 )

            conn.publish(msg)

            rcvd = conn.receive(5000)

            if rcvd != None:
                conn.dispatch(rcvd)

                lp.TimeUtil.millisleep(1000)

                self.check("Expected Callback 1 to receive a message", mcb1.was_called())
                self.check("Expected Callback 2 to receive a message", mcb2.was_called())
                self.check("Expected Callback 3 to not receive a message", mcb3.was_called() == False)

                lp.Message.destroy(rcvd)
            else:
                self.check("Failed to receive message", False)

            conn.unsubscribe(info1)
            conn.unsubscribe(info2)
            conn.unsubscribe(info3)

            conn.disconnect()

        except Exception as e:
            self.check(str(e), False)


    def test_receive(self):
        lp.log_info("Test receive()")

        conn = None

        # Nominal tests
        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()

            self.set_standard_fields( conn.get_message_factory() )

            # Timeout occurs (no subscription)
            rcvd = conn.receive(10)
            self.check("Received unexpected message", rcvd == None)

            # Receive message
            msg = conn.get_message_factory().create_message("HB")
            msg.set_subject( self.get_subject("HB") )

            conn.subscribe(msg.get_subject())
            conn.publish(msg)

            rcvd = conn.receive(5000)
            self.require("Did not receive expected message", rcvd != None)

            iter = rcvd.get_field_iterator( lp.MessageFieldIterator.Selector_TRACKING_FIELDS )
            numTrackingFields = 0
            while iter.has_next():
                field = iter.next()
                self.check("Expected a tracking field", field.is_tracking())
                numTrackingFields += 1
            self.check("Unexpected number of tracking fields", 10 == numTrackingFields)

        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # Connection not initialized
            conn = lp.Connection( self.get_config() )
            conn.receive(10)
            self.check("Expected exception since we're not initialized", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # Auto-dispatcher enabled
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.start_auto_dispatch()
            conn.receive(10)
            self.check("Expected exception since we've enabled the auto-dispatcher", False)
        except Exception as e:
            self.check(str(e), "This method cannot be called when the Auto-Dispatcher is in use" in str(e))

        if conn != None:
            conn.disconnect()


    def test_exclude_subject(self):
        lp.log_info("Test exclude_subject()")

        conn = None

        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()

            self.set_standard_fields(conn.get_message_factory())

            msg1 = conn.get_message_factory().create_message("HB")
            msg2 = conn.get_message_factory().create_message("LOG")

            msg1.set_subject( self.get_subject("HB") )
            msg2.set_subject( self.get_subject("LOG") )

            msg2.add_field( lp.StringField("SUBCLASS", "NO-CLASS") )
            msg2.add_field( lp.StringField("OCCURRENCE-TYPE", "FREQUENTLY") )
            msg2.add_field( lp.I16Field("SEVERITY", 1) )

            conn.subscribe( self.get_subject(">") )
            conn.exclude_subject( self.get_subject("LOG.+") )

            conn.publish(msg1)

            rcvd = conn.receive(5000)
            self.check("Was expecting to receive an HB message", rcvd != None)
            lp.Message.destroy(rcvd)

            conn.publish(msg2)
            rcvd = conn.receive(5000)
            self.check("Was not expecting to receive a LOG message", rcvd == None)

            conn.remove_excluded_subject( self.get_subject("LOG.+") )

            conn.publish(msg2)
            rcvd = conn.receive(5000)
            self.check("Was expecting to receive a LOG message", rcvd != None)
            lp.Message.destroy(rcvd)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()

        # Off-nominal tests
        try:
            # Connection not initialized
            conn = lp.Connection( self.get_config() )
            conn.exclude_subject("GMSEC.FOO.BAR")
            self.check("Expected exception since we're not initialized", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # Invalid subject
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.exclude_subject("GMSEC FOO BAR")
            self.check("Expected exception for invalid subject", False)
        except Exception as e:
            self.check(str(e), "Subject is invalid" in str(e))

        if conn != None:
            conn.disconnect()


    def test_remove_excluded_subject(self):
        lp.log_info("Test remove_excluded_subject()")

        # Nominal test for remove_excluded_subject() are conducted witin test_exclude_subject()

        conn = None

        # Off-nominal tests
        try:
            # Connection not initialized
            conn = lp.Connection( self.get_config() )
            conn.remove_excluded_subject("GMSEC.FOO.BAR")
            self.check("Expected exception since we're not initialized", False)
        except Exception as e:
            self.check(str(e), "Connection has not been initialized" in str(e))

        try:
            # Invalid subject
            conn = lp.Connection( self.get_config() )
            conn.connect()
            conn.remove_excluded_subject("GMSEC FOO BAR")
            self.check("Expected exception for invalid subject", False)
        except Exception as e:
            self.check(str(e), "Subject is invalid" in str(e))

        if conn != None:
            conn.disconnect()


    def test_get_set_name(self):
        lp.log_info("Test get_name() and set_name()")

        try:
            conn = lp.Connection( self.get_config() )
            self.check("Expected a connection name", conn.get_name() != None)

            conn.set_name("FOOBAR")
            self.check("Expected a connection name of FOOBAR", conn.get_name() == "FOOBAR")

            conn.set_name(None)
            self.check("Expected a connection name of FOOBAR", conn.get_name() == "FOOBAR")
        except Exception as e:
            self.check(str(e), False)



    def test_get_id(self):
        lp.log_info("Test get_id()")

        conn = None

        try:
            conn = lp.Connection( self.get_config() )
            conn.connect()
            self.check("Expected a connection ID", conn.get_id() != None)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()


    def test_get_mw_info(self):
        lp.log_info("Test get_mw_info()")

        conn = None

        try:
            conn = lp.Connection( self.get_config() )
            self.check("Expected mwInfo to contain mw-id", self.get_config().get_value("mw-id") in conn.get_mw_info())
        except Exception as e:
            self.check(str(e), False)


    def test_get_connection_endpoint(self):
        lp.log_info("Test get_connection_endpoint()")

        conn = lp.Connection( self.get_config() )
        conn.connect()
        self.check("Unexpected connection endpoint", conn.get_connection_endpoint() != None)
        conn.disconnect()


    def test_get_publish_queue_message_count(self):
        lp.log_info("Test get_publish_queue_message_count()")

        config = lp.Config( self.get_config() )  # make a copy!
        config.add_value("gmsec-async-publish", "True")
        config.add_value("gmsec-async-publish-queue-depth", "20")
        config.add_value("gmsec-async-publish-teardown-wait", "2000")

        conn = None

        try:
            conn = lp.Connection(config)
            conn.connect()
            self.set_standard_fields(conn.get_message_factory())

            msg = conn.get_message_factory().create_message("LOG")

            msg.add_field( lp.StringField("SUBCLASS", "TEST") )
            msg.add_field( lp.StringField("OCCURRENCE-TYPE", "FREQUENT") )
            msg.add_field( lp.I16Field("SEVERITY", 1) )

            for i in range(0,10):
                conn.publish(msg)
                self.check("Unexpected message count < 0", conn.get_publish_queue_message_count() >= 0)

            lp.TimeUtil.millisleep(2000)

            self.check("Unexpected message count != 0", conn.get_publish_queue_message_count() == 0)
        except Exception as e:
            self.check(str(e), False)

        if conn != None:
            conn.disconnect()


    def verify_tracking_fields(self, msg, expected):
        numTrackingFields = 0

        iter = msg.get_field_iterator(lp.MessageFieldIterator.Selector_TRACKING_FIELDS)

        while iter.has_next():
            field = iter.next()

            self.check("Expected a tracking field", field.is_tracking())

            if field.is_tracking():
                numTrackingFields += 1

        self.check("Unexpected number of tracking fields", numTrackingFields == expected)


class T010_EventCallback(lp.EventCallback):
    def __init__(self):
        super().__init__()
        self.m_called = False
        self.m_event  = lp.Connection.Event_ALL_EVENTS

    def on_event(self, conn, status, event):
        self.m_called = True
        self.m_event  = event

    def was_called(self):
        return self.m_called

    def get_event(self):
        return self.m_event

    def reset(self):
        self.m_called = False
        self.m_event  = lp.Connection.Event_ALL_EVENTS


class T010_Callback(lp.Callback):
    def __init__(self):
        super().__init__()
        self.m_called = False

    def on_message(self, conn, msg):
        self.m_called = True

    def was_called(self):
        return self.m_called

    def reset(self):
        self.m_called = False


class T010_RequestCallback(lp.Callback):
    def __init__(self, test):
        super().__init__()
        self.m_test = test
        self.m_provideResponse = True

    def on_message(self, conn, msg):
        try:
            if self.m_provideResponse:
                repMsg = conn.get_message_factory().create_message("RESP.DIR")
                repMsg.set_subject( self.m_test.get_subject("RESP.DIR") )

                repMsg.set_field_value("RESPONSE-STATUS", lp.Message.ResponseStatus_SUCCESSFUL_COMPLETION)

                if msg.has_field("COMPONENT"):
                    repMsg.add_field( lp.StringField("DESTINATION-COMPONENT", msg.get_string_value("COMPONENT")) )

                if msg.has_field("DESTINATION-COMPONENT"):
                    repMsg.add_field( lp.StringField("COMPONENT", msg.get_string_value("DESTINATION-COMPONENT")) )
                else:
                    repMsg.add_field( lp.StringField("COMPONENT", "RESPONDER") )

                if msg.has_field("REQUEST-ID"):
                    repMsg.add_field( msg.get_field("REQUEST-ID") )

                conn.reply(msg, repMsg)
            else:
                self.m_provideResponse = True
        except Exception as e:
            lp.log_error(str(e), False)

    def provide_response(self, flag):
        self.m_provideResponse = flag


class T010_ReplyCallback(lp.ReplyCallback):
    def __init__(self):
        super().__init__()
        self.m_gotReply = False
        self.m_gotEvent = False

    def on_reply(self, conn, request, reply):
        self.m_gotReply = True

    def on_event(self, conn, status, event):
        self.m_gotEvent = (event == lp.Connection.Event_REQUEST_TIMEOUT_EVENT)

    def got_reply(self):
        return self.m_gotReply

    def got_event(self):
        return self.m_gotEvent

    def reset(self):
        self.m_gotReply = False
        self.m_gotEvent = False


Test.run('Connection', Test_Connection())
