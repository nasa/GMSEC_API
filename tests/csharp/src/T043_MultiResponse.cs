namespace T043
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Collections.Generic;


	class T043_Callback : Callback
	{
		private TestCase m_test;
		private List<Message.ResponseStatus> m_statusList;

		public T043_Callback(TestCase test)
		{
			m_test       = test;
			m_statusList = new List<Message.ResponseStatus>() { Message.ResponseStatus.ACKNOWLEDGEMENT,
			                                                    Message.ResponseStatus.WORKING_KEEP_ALIVE,
			                                                    Message.ResponseStatus.WORKING_KEEP_ALIVE,
			                                                    Message.ResponseStatus.WORKING_KEEP_ALIVE,
			                                                    Message.ResponseStatus.SUCCESSFUL_COMPLETION
			                                                  };
		}

		public override void OnMessage(Connection conn, Message message)
		{
			try
			{
				Message reply = conn.GetMessageFactory().CreateMessage("RESP.DIR");
				reply.SetSubject( m_test.GetSubject("RESP.DIR") );

				reply.AddField("COMPONENT", (message.HasField("DESTINATION-COMPONENT") ? message.GetStringValue("DESTINATION-COMPONENT") : "RESPONDER"));
				reply.AddField("DESTINATION-COMPONENT", (message.HasField("COMPONENT") ? message.GetStringValue("COMPONENT") : "FILL"));

				if (message.HasField("REQUEST-ID"))
				{
					reply.AddField(message.GetField("REQUEST-ID"));
				}

				Log.Verbose("Received Request:\n" + message.ToXML());

				foreach (Message.ResponseStatus status in m_statusList)
				{
					reply.AddField("RESPONSE-STATUS", (short) status);

					Log.Verbose("Issuing Reply:\n" + reply.ToXML());

					conn.Reply(message, reply);
				}
			}
			catch (GmsecException e)
			{
				Log.Error("GmsecException in T043_Callback: " + e.ToString());
			}
		}

		public int ExpectedResponses()
		{
			return m_statusList.Count;
		}
	}


	class T043_ReplyCallback : ReplyCallback
	{
		private int repliesReceived;

		public T043_ReplyCallback()
		{
			repliesReceived = 0;
		}

		public override void OnReply(Connection conn, Message request, Message reply)
		{
			try
			{
				Log.Info("Received Reply with RESPONSE-STATUS: " + reply.GetI16Value("RESPONSE-STATUS"));
				++repliesReceived;
			}
			catch (GmsecException e)
			{
				Log.Error("GmsecException in T043_ReplyCallback: " + e.ToString());
			}
		}

		public override void OnEvent(Connection conn, Status status, Connection.Event connEvent)
		{
			Log.Warning("Received unexpected event: " + status.Get());
		}

		public int NumRepliesReceived()
		{
			return repliesReceived;
		}
	}


	class T043_MultiResponse : TestCase
	{
		static int Main(string[] args)
		{
			T043_MultiResponse test = new T043_MultiResponse();
			test.SetDescription("Multi-Response");
			test.Run(args);
			return 0;
		}

		public override void Run()
		{
			Config config = new Config(GetConfig());  // make a copy
			config.AddValue("mw-multi-resp", "true");

			T043_Callback       cb = new T043_Callback(this);
			T043_ReplyCallback rcb = new T043_ReplyCallback();

			try
			{
				using (Connection conn = new Connection(config))
				{
					conn.Connect();

					SetStandardFields(conn.GetMessageFactory());

					conn.StartAutoDispatch();

					Message request = conn.GetMessageFactory().CreateMessage("REQ.DIR");
					request.SetSubject( GetSubject("REQ.DIR") );
					request.AddField("DIRECTIVE-STRING", "Do something!");
					request.AddField("DESTINATION-COMPONENT", "RESPONDER");
					request.AddField("REQUEST-ID", (ushort) 0);

					// subscribe to receive the response message (hack!)
					conn.Subscribe( GetSubject("RESP.DIR.+") );

					// subscribe to process the request message
					conn.Subscribe(request.GetSubject(), cb);

					Log.Verbose("Issuing Request:\n" + request.ToXML());

					conn.Request(request, 5000, rcb, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);

					TimeUtil.Millisleep(5000);   // allow for the response message(s) to be received

					conn.Disconnect();

					Check("Unexpected response message count", rcb.NumRepliesReceived() == cb.ExpectedResponses());
				}
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
		}
	}
}
