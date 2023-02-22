#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class T043_Callback(lp.Callback):
    def __init__(self, test):
        super().__init__()
        self.test = test
        self.statusList = []
        self.statusList.append(lp.Message.ResponseStatus_ACKNOWLEDGEMENT)
        self.statusList.append(lp.Message.ResponseStatus_WORKING_KEEP_ALIVE)
        self.statusList.append(lp.Message.ResponseStatus_WORKING_KEEP_ALIVE)
        self.statusList.append(lp.Message.ResponseStatus_WORKING_KEEP_ALIVE)
        self.statusList.append(lp.Message.ResponseStatus_SUCCESSFUL_COMPLETION)

    def on_message(self, conn, msg):
        try:
            reply = conn.get_message_factory().create_message("RESP.DIR")
            reply.set_subject(self.test.get_subject("RESP.DIR"))

            if msg.has_field("DESTINATION-COMPONENT"):
                reply.add_field( lp.StringField("COMPONENT", msg.get_string_value("DESTINATION-COMPONENT")) )
            else:
                reply.add_field( lp.StringField("COMPONENT", "RESPONDER") )

            if msg.has_field("COMPONENT"):
                reply.add_field( lp.StringField("DESTINATION-COMPONENT", msg.get_string_value("COMPONENT")) )
            else:
                reply.add_field( lp.StringField("DESTINATION-COMPONENT", "FILL") )

            if msg.has_field("REQUEST-ID"):
                reply.add_field(msg.get_field("REQUEST-ID"))

            lp.log_verbose("Received Request:\n" + msg.to_xml())

            for status in self.statusList:
                reply.set_field_value("RESPONSE-STATUS", status)

                lp.log_verbose("Issuing Reply:\n" + reply.to_xml())

                conn.reply(msg, reply)

        except Exception as e:
            lp.log_error(str(e))

    def expected_responses(self):
        return len(self.statusList)


class T043_ReplyCallback(lp.ReplyCallback):
    def __init__(self):
        super().__init__()
        self.repliesReceived = 0

    def on_reply(self, conn, request, reply):
        try:
            lp.log_info("Received Reply with RESPONSE-STATUS: " + reply.get_string_value("RESPONSE-STATUS"))
            self.repliesReceived += 1
        except Exception as e:
            lp.log_error(str(e))

    def on_event(self, conn, status, event):
            lp.log_warning("Received unexpected event: " + status.get())

    def num_replies_received(self):
        return self.repliesReceived


class Test_MultiResponse(Test):

    def test(self):
        config = lp.Config(self.get_config())
        config.add_value("mw-multi-resp", "true")

        cb  = T043_Callback(self)
        rcb = T043_ReplyCallback()

        conn = None

        try:
            conn = lp.Connection(config)

            self.set_standard_fields(conn.get_message_factory())

            conn.connect()
            conn.start_auto_dispatch()

            request = conn.get_message_factory().create_message("REQ.DIR")
            request.set_subject(self.get_subject("REQ.DIR"))
            request.add_field( lp.StringField("DIRECTIVE-STRING", "Do something!") )
            request.add_field( lp.StringField("DESTINATION-COMPONENT", "RESPONDER") )
            request.add_field( lp.U16Field("REQUEST-ID", 0) )

            # subscribe to receive the response message(s) (hack!)
            conn.subscribe(self.get_subject("RESP.DIR.+"))

            # subscribe to process the request message
            conn.subscribe(request.get_subject(), cb)

            lp.log_verbose("Issuing Request:\n" + request.to_xml())

            conn.request(request, 5000, rcb, lp.GMSEC_REQUEST_REPUBLISH_NEVER)

            lp.TimeUtil.millisleep(5000);   # allow for the response message(s) to be received

            self.check("Unexpected response message count", rcb.num_replies_received() == cb.expected_responses())

        except Exception as e:
            self.require(str(e), False)

        if conn != None:
            conn.disconnect()
            conn = None


Test.run('Multi-Response', Test_MultiResponse())
