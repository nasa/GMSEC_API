namespace T010
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T010_Connection : TestCase
	{
		static void Main(string[] args)
		{
			T010_Connection test = new T010_Connection();
			test.SetDescription("Connection");
			test.Run(args);
		}


		public override void Run()
		{
			try
			{
				Test_Constructor();
				Test_GetAPIVersion();
				Test_Connect();
				Test_Disconnect();
				Test_GetLibraryName();
				Test_GetLibraryVersion();
				Test_GetMessageFactory();
				Test_RegisterEventCallback();
				Test_Subscribe();
				Test_SubscribeWithCallback();    // also calls on startAutoDispatch/stopAutoDispatch
				Test_Unsubscribe();
				Test_StartAutoDispatch();
				Test_StopAutoDispatch();
				Test_Publish();                  // also calls on subscribe() and receive()
				Test_PublishWithMwConfig();
				Test_RequestWithCallback();      // also calls on reply()
				Test_Request();                  // also calls on reply()
				Test_Reply();
				Test_Dispatch();
				Test_Receive();
				Test_ExcludeSubject();           // also tests removeExcludedSubject()
				Test_RemoveExcludedSubject();
				Test_GetSetName();
				Test_GetID();
				Test_GetMWInfo();
				Test_GetConnectionEndpoint();
				Test_GetPublishQueueMessageCount();
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
		}


		private void Test_Constructor()
		{
			Log.Info("Test constructor");

			// Nominal test
			try
			{
				using (new Connection( GetConfig() ))
				{
					Check("Okay", true);
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}

			// Nominal test w/ MessageFactory
			try
			{
				using (MessageFactory factory = new MessageFactory( GetConfig() ))
				using (Connection conn1 = new Connection( GetConfig(), factory ))
				using (Connection conn2 = new Connection( GetConfig(), factory ))
				{
					Check("Okay", true);
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}

			// Off-Nominal test
			try
			{   // missing mw-id
				new Connection( new Config() );
				Check("Exception expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("mw-id in Config is not specified"));
			}

			try
			{   //bogus middleware
				new Connection( new Config("mw-id=bogus", DataType.KEY_VALUE_DATA) );
				Check("Exception expected", false);
			}
			catch (Exception e)
			{
				Check(e.ToString(), e.ToString().Contains("Unable to load"));
			}
		}


		private void Test_GetAPIVersion()
		{
			Log.Info("Test GetAPIVersion()");

        	string version = "GMSEC API v" + Gmsec.GMSEC_VERSION_NUMBER;
        	Check("Expected to get API version Info", Connection.GetAPIVersion().Contains(version));
		}


		private void Test_Connect()
		{
			Log.Info("Test Connect()");

			// Nominal test
			using (Connection conn1 = new Connection( GetConfig() ))
			{
				try
				{
					conn1.Connect();
					Check("Okay", true);
					conn1.Disconnect();
				}
				catch (GmsecException e)
				{
					Require(e.ToString(), false);
				}
			}

			// Off-nominal test
			Config config2 = new Config( "mw-id=bolt server=gs580s-gc764mw1:1234", DataType.KEY_VALUE_DATA );

			using (Connection conn2 = new Connection(config2))
			{
				try
				{
					conn2.Connect();
					Check("Expected error to indicate unable to connect", false);
				}
				catch (GmsecException e)
				{
					bool result = (e.GetErrorClass() == (int) StatusClass.MIDDLEWARE_ERROR) &&
					              (e.GetErrorCode()  == (int) StatusCode.CUSTOM_ERROR_CODE) &&
					              (e.GetCustomCode() == 7);

					Check(e.ToString(), result);
				}
			}
		}


		private void Test_Disconnect()
		{
			Log.Info("Test Disconnect()");

			try
			{
				using (Connection conn = new Connection( GetConfig() ))
				{
					conn.Connect();
					conn.Disconnect();
					Check("Okay", true);
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}
		}


		private void Test_GetLibraryName()
		{
			Log.Info("Test GetLibraryName()");

			try
			{
				using (Connection conn = new Connection( GetConfig() ))
				{
					Check("Unexpected library name", conn.GetLibraryName().Contains( GetConfig().GetValue("mw-id", "unknown") ));
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}
		}


		private void Test_GetLibraryVersion()
		{
			Log.Info("Test GetLibraryVersion()");

			try
			{
				using (Connection conn = new Connection( GetConfig() ))
				{
					Check("Expected to get a library version", null != conn.GetLibraryVersion());
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}
		}


		private void Test_GetMessageFactory()
		{
			Log.Info("Test GetMessageFactory()");

			try
			{
				using (Connection conn = new Connection( GetConfig() ))
				{
					Check("Expected to get a MessageFactory for the CURRENT specification",
						Gmsec.GMSEC_MSG_SPEC_CURRENT == conn.GetMessageFactory().GetSpecification().GetVersion());
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}
		}


		private void Test_RegisterEventCallback()
		{
			Log.Info("Test RegisterEventCallback()");

			string mw = GetConfig().GetValue("mw-id", "unknown");

			if (mw == "unknown" || mw == "mb" || mw == "generic_jms" || mw == "zeromq413" || mw == "zeromq432")
			{
				// Test is being skipped because of lame middleware wrapper
				// which does not support event notification upon connect
				// and/or disconnect to/from the GMSEC Bus.
				return;
			}

			T010_EventCallback mec = new T010_EventCallback();

			// Nominal case
			try
			{
				using (Connection conn = new Connection( GetConfig() ))
				{
					conn.RegisterEventCallback(Connection.Event.ALL_EVENTS, mec);

					conn.Connect();
					Check("Expected EventCallback to be called", true == mec.WasCalled());
					Check("Expected to be connected", Connection.Event.CONNECTION_SUCCESSFUL_EVENT == mec.GetEvent());

					mec.Reset();

					conn.Disconnect();
					Check("Expected EventCallback to be called", true == mec.WasCalled());
					Check("Expected to be disconnected", Connection.Event.CONNECTION_BROKEN_EVENT == mec.GetEvent());
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), false);
			}

			// Off-nominal case
			try
			{
				using (Connection conn = new Connection( GetConfig() ))
				{
					conn.RegisterEventCallback(Connection.Event.ALL_EVENTS, (EventCallback) null);
				}
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("EventCallback cannot be NULL"));
			}
		}


		private void Test_Subscribe()
		{
			Log.Info("Test Subscribe()");

			// Nominal test
			using (Connection conn = new Connection( GetConfig() ))
			{
				try
				{
					conn.Connect();
					conn.Subscribe( GetSubject("*.BAR") );
					conn.Subscribe( GetSubject("FOO.BAZ") );

					Message msg1 = conn.GetMessageFactory().CreateMessage("LOG");
					Message msg2 = conn.GetMessageFactory().CreateMessage("LOG");
					Message msg3 = conn.GetMessageFactory().CreateMessage("LOG");

					msg1.SetSubject( GetSubject("GIN.BAR") );
					msg2.SetSubject( GetSubject("FOO.BAZ") );
					msg3.SetSubject( GetSubject("FOO.GAZ") );

					conn.Publish(msg1);

					Message msg = conn.Receive(5000);
					Require("Was expecting to receive a message", msg != null);
					Check("Unexpected message subject", msg.GetSubject() == GetSubject("GIN.BAR"));
					Message.Destroy(msg);

					conn.Publish(msg2);

					msg = conn.Receive(5000);
					Require("Was expecting to receive a message", msg != null);
					Check("Unexpected message subject", msg.GetSubject() == GetSubject("FOO.BAZ"));
					Message.Destroy(msg);

					conn.Publish(msg3);

					msg = conn.Receive(5000);
					Check("Was not expecting to receive a message", msg == null);
				}
				catch (GmsecException e)
				{
					Check(e.ToString(), false);
				}
				finally
				{
					conn.Disconnect();
				}
			}

			// Off-nominal tests
			using (Connection conn = new Connection( GetConfig() ))
			{
				try
				{   // duplicate subscription
					conn.Connect();
					conn.Subscribe("GMSEC.>");
					conn.Subscribe("GMSEC.>");
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e)
				{
					Check(e.ToString(), e.ToString().Contains("Duplicate subscription"));
				}
				finally
				{
					conn.Disconnect();
				}
			}

			using (Connection conn = new Connection( GetConfig() ))
			{
				try
				{   // subscribe() before Connect()
					conn.Subscribe("GMSEC.>");
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e)
				{
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn = new Connection( GetConfig() ))
			{
				try
				{   // null subscription subject
					conn.Connect();
					conn.Subscribe(null);
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e)
				{
					Check(e.ToString(), e.ToString().Contains("Subject is NULL or is empty string"));
				}
				finally
				{
					conn.Disconnect();
				}
			}

			using (Connection conn = new Connection( GetConfig() ))
			{
				try
				{   // illegal subscription subject
					conn.Connect();
					conn.Subscribe("GMSEC.MY MISSION.*.*.>");
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e)
				{
					Check(e.ToString(), e.ToString().Contains("Subject is invalid"));
				}
				finally
				{
					conn.Disconnect();
				}
			}
		}


		private void Test_SubscribeWithCallback()
		{
			Log.Info("Test subscribe() with Callback");

			T010_Callback      mcb = new T010_Callback();
			T010_EventCallback ecb = new T010_EventCallback();

			Config config1 = new Config( GetConfig() );  // make copy!
			Config config2 = new Config( GetConfig() );  // make copy!
			config2.AddValue("gmsec-msg-content-validate-recv", "true");

			// Nominal tests
			using (Connection conn1 = new Connection(config1))
			{
				conn1.Connect();
				conn1.RegisterEventCallback(Connection.Event.ALL_EVENTS, ecb);
				conn1.Subscribe( GetSubject(">"), mcb );

				bool autoStarted = conn1.StartAutoDispatch();
				Check("Expected the auto-dispatcher to start", true == autoStarted);

				SetStandardFields(conn1.GetMessageFactory());

				Message msg = conn1.GetMessageFactory().CreateMessage("HB");
				msg.SetSubject( GetSubject("HB") );

				conn1.Publish(msg);

				TimeUtil.Millisleep(5000);

				Check("Expected Callback to be summoned", mcb.WasCalled());
				Check("Expected ALL_EVENTS", ecb.GetEvent() == Connection.Event.ALL_EVENTS);

				mcb.Reset();
				ecb.Reset();
			}

			using (Connection conn2 = new Connection(config2))
			{
				conn2.Connect();
				conn2.RegisterEventCallback(Connection.Event.ALL_EVENTS, ecb);
				conn2.Subscribe( GetSubject(">"), mcb );

				conn2.StartAutoDispatch();

				SetStandardFields(conn2.GetMessageFactory());

				Message msg = conn2.GetMessageFactory().CreateMessage("HB");
				msg.SetSubject( GetSubject("HB") );

				conn2.Publish(msg);
				conn2.Publish(msg);

				TimeUtil.Millisleep(5000);

				Check("Did not expect Callback to be summoned", mcb.WasCalled() == false);
				Check("Expected an INVALID_MESSAGE_EVENT", ecb.GetEvent() == Connection.Event.INVALID_MESSAGE_EVENT);

				conn2.StopAutoDispatch();

				conn2.Disconnect();
			}

			using (Connection conn3 = new Connection(config1))
			{   // subscriptions using same subject and unique callbacks
				T010_Callback mcb2 = new T010_Callback();
				conn3.Connect();
				conn3.Subscribe("GMSEC.>", mcb);
				conn3.Subscribe("GMSEC.>", mcb2);
				conn3.Disconnect();
			}

			// Off-nominal tests

			using (Connection conn4 = new Connection(config1))
			{   // duplicate subscription using the same callback
				conn4.Connect();
				try {
					conn4.Subscribe("GMSEC.>", mcb);
					conn4.Subscribe("GMSEC.>", mcb);
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Duplicate subscription"));
				}
				conn4.Disconnect();
			}

			using (Connection conn5 = new Connection(config1))
			{   // null subscription subject
				conn5.Connect();
				try {
					conn5.Subscribe(null, mcb);
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Subject is NULL or is empty string"));
				}
				conn5.Disconnect();
			}

			using (Connection conn6 = new Connection(config1))
			{   // illegal subscription subject
				conn6.Connect();
				try {
					conn6.Subscribe("GMSEC.MY MISSION.*.*.>", mcb);
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Subject is invalid"));
				}
				conn6.Disconnect();
			}
		}


		private void Test_Unsubscribe()
		{
			Log.Info("Test Unsubscribe()");

			Config config = new Config( GetConfig() );  // make copy!
			config.AddValue("gmsec-msg-content-validate-recv", "true");

			// Nominal tests

			using (Connection conn1 = new Connection(config))
			{
				// Attempt to unsubscribe using NULL object
				conn1.Connect();
				SubscriptionInfo info = conn1.Subscribe( GetSubject("FOO.BAR") );
				conn1.Unsubscribe(info);

				Message hb = conn1.GetMessageFactory().CreateMessage("HB");
				hb.SetSubject( GetSubject("FOO.BAR") );

				conn1.Publish(hb);

				Message msg = conn1.Receive(5000);

				Check("Did not expect to receive a message", msg == null);

				conn1.Disconnect();
			}

			// Off-nominal tests
			using (Connection conn2 = new Connection(config))
			{
				// Attempt to unsubscribe using NULL object
				conn2.Connect();
				SubscriptionInfo info = null;
				try {
					conn2.Unsubscribe(info);
					Check("Expected an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Cannot unsubscribe using NULL SubscriptionInfo object"));
				}
				conn2.Disconnect();
			}

			using (Connection conn3 = new Connection(config))
			using (Connection conn4 = new Connection(config))
			{
				// Attempt to unsubscribe using wrong Connection object
				conn3.Connect();
				conn4.Connect();

				SubscriptionInfo info3 = conn3.Subscribe("GMSEC.FOO.BAR");

				try {
					conn4.Unsubscribe(info3);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("SubscriptionInfo object is not associated with this Connection"));
				}
				conn3.Disconnect();
				conn4.Disconnect();
			}
		}


		private void Test_StartAutoDispatch()
		{
			Log.Info("Test StartAutoDispatch()");

			// startAutoDispatcher already tested within test_subscribe_callback()

			// Off-nominal test
			using (Connection conn = new Connection( GetConfig() ))
			{
				try
				{   // Auto-dispatch cannot start until Connection is initialized
					conn.StartAutoDispatch();
					Check("Expected an exception", false);
				}
				catch (GmsecException e)
				{
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}
		}


		private void Test_StopAutoDispatch()
		{
			Log.Info("Test StopAutoDispatch()");

			// stopAutoDispatcher already tested within test_subscribe_callback()

			// Off-nominal test
			using (Connection conn = new Connection( GetConfig() ))
			{
				try
				{   // Auto-dispatch cannot be stopped until Connection is initialized
					conn.StopAutoDispatch();
					Check("Expected an exception", false);
				}
				catch (GmsecException e)
				{
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}
		}


		private void Test_Publish()
		{
			Log.Info("Test Publish()");

			Config config1 = new Config( GetConfig() );  // make a copy!
			Config config2 = new Config( GetConfig() );  // make a copy!
			config2.AddValue("gmsec-msg-content-validate", "true");

			// Nominal test

			using (Connection conn1 = new Connection(config1))
			{
				conn1.Connect();
				conn1.Subscribe( GetSubject(">") );

				SetStandardFields(conn1.GetMessageFactory());

				Message msg = conn1.GetMessageFactory().CreateMessage("HB");
				msg.SetSubject( GetSubject("HB") );

				conn1.Publish(msg);

				Message rcvd = conn1.Receive(5000);

				if (rcvd != null)
				{
					Check("Unexpected subject", rcvd.GetSubject() == GetSubject("HB"));
					Check("Unexpected message kind", rcvd.GetKind() == Message.Kind.PUBLISH);

					Message.Destroy(rcvd);
				}
				else
				{
					Check("Failed to receive published message", false);
				}

				conn1.Disconnect();
			}

			// Off-nominal tests

			using (Connection conn2 = new Connection(config2))
			{
				// Message validation fails (missing standard fields)
				Message msg = conn2.GetMessageFactory().CreateMessage("HB");
				conn2.Connect();
				try {
					conn2.Publish(msg);
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Message Validation Failed"));
				}
				conn2.Disconnect();
			}

			using (Connection conn3 = new Connection(config1))
			{
				// Connection not initialized
				Message msg = conn3.GetMessageFactory().CreateMessage("HB");
				try {
					conn3.Publish(msg);
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn4 = new Connection(config1))
			{
				// Non-PUBLISH kind message
				Message msg = conn4.GetMessageFactory().CreateMessage("REQ.DIR");
				conn4.Connect();
				try {
					conn4.Publish(msg);
					Check("Was expecting an exception", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Cannot publish message with non-PUBLISH message kind"));
				}
				conn4.Disconnect();
			}
		}


		private void Test_PublishWithMwConfig()
		{
			Log.Info("Test Publish() with MwConfig");

			using (Connection conn = new Connection( GetConfig() ))
			{
				SetStandardFields(conn.GetMessageFactory());

				Message msg = conn.GetMessageFactory().CreateMessage("HB");
				msg.SetSubject( GetSubject("MW-CONFIG") );

				Config mwConfig = new Config();

				conn.Connect();

				conn.Subscribe( GetSubject("MW-CONFIG") );

				conn.Publish( msg, mwConfig );

				Message rcv = conn.Receive( 5000 );

				if (rcv != null)
				{
					Check("Received a message", true);

					Message.Destroy(rcv);
				}
				else
				{
					Check("Failed to receive a message", false);
				}
			}
		}


		private void Test_RequestWithCallback()
		{
			Log.Info("Test asynchronous Request()");

			T010_RequestCallback reqCallback = new T010_RequestCallback(this);
			T010_ReplyCallback   repCallback = new T010_ReplyCallback();

			// Nominal case

			using (Connection conn1 = new Connection( GetConfig() ))
			{
				conn1.Connect();

				SetStandardFields(conn1.GetMessageFactory());

				Message reqMsg = conn1.GetMessageFactory().CreateMessage("REQ.DIR");
				reqMsg.SetSubject( GetSubject("REQ.DIR") );
				reqMsg.AddField("DIRECTIVE-STRING", "Do something!");
				reqMsg.AddField("DESTINATION-COMPONENT", "RESPONDER");

				// subscribe to receive the response message
				conn1.Subscribe( GetSubject("RESP.DIR.+") );

				// subscribe to process the request message
				conn1.Subscribe(reqMsg.GetSubject(), reqCallback);
				conn1.StartAutoDispatch();

				//o Verify response is returned
				reqMsg.AddField("REQUEST-ID", (ushort) 0);
				conn1.Request(reqMsg, 5000, repCallback, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);
				TimeUtil.Millisleep(5500);

				Check("Was expecting a reply", repCallback.GotReply() == true);
				Check("Was not expecting an event", repCallback.GotEvent() == false);

				repCallback.Reset();

				//o Verify NULL is returned on timeout
				reqCallback.ProvideResponse(false);

				reqMsg.AddField("REQUEST-ID", (ushort) 1);
				conn1.Request(reqMsg, 5000, repCallback, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);
				TimeUtil.Millisleep(5500);

				Check("Was not expecting a reply", repCallback.GotReply() == false);
				Check("Was expecting an event", repCallback.GotEvent() == true);

				//o Verify request is delivered multiple times until response delivered
				reqCallback.ProvideResponse(false);

				reqMsg.AddField("REQUEST-ID", (ushort) 2);
				conn1.Request(reqMsg, 5000, repCallback, 5000);
				TimeUtil.Millisleep(5500*2);

				Check("Was expecting a reply", repCallback.GotReply() == true);
				Check("Was expecting an event", repCallback.GotEvent() == true);

				//o Verify request is delivered multiple times until response delivered
				reqCallback.ProvideResponse(false);

				reqMsg.AddField("REQUEST-ID", (ushort) 3);
				conn1.Request(reqMsg, -1, repCallback, 5000);
				TimeUtil.Millisleep(5500*2);

				Check("Was expecting a reply", repCallback.GotReply() == true);
				Check("Was expecting an event", repCallback.GotEvent() == true);
			}

			// Off-nominal tests

			using (Connection conn2 = new Connection( GetConfig() ))
			{
				// Connection not initialized
				Message reqMsg = conn2.GetMessageFactory().CreateMessage("REQ.DIR");
				try {
					conn2.Request(reqMsg, 5000, repCallback, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);
					Check("An exception was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn3 = new Connection( GetConfig() ))
			{
				// Request using non-REQUEST kind message
				conn3.Connect();
				Message badMsg = conn3.GetMessageFactory().CreateMessage("RESP.DIR");
				try {
					conn3.Request(badMsg, 5000, repCallback, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);
					Check("An exception was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Cannot issue request with non-REQUEST kind message"));
				}
				conn3.Disconnect();
			}
		}


		private void Test_Request()
		{
			Log.Info("Test synchronous Request()");

			T010_RequestCallback reqCallback = new T010_RequestCallback(this);

			// Nominal case
			using (Connection conn1 = new Connection( GetConfig() ))
			{
				conn1.Connect();

				SetStandardFields(conn1.GetMessageFactory());

				Message reqMsg = conn1.GetMessageFactory().CreateMessage("REQ.DIR");
				reqMsg.SetSubject( GetSubject("REQ.DIR") );
				reqMsg.AddField("DIRECTIVE-STRING", "Do something!");
				reqMsg.AddField("DESTINATION-COMPONENT", "RESPONDER");

				// subscribe to receive the response message
				conn1.Subscribe( GetSubject("RESP.DIR.+") );

				// subscribe to process the request message
				conn1.Subscribe(reqMsg.GetSubject(), reqCallback);
				conn1.StartAutoDispatch();

				//o Verify response is returned
				reqMsg.AddField("REQUEST-ID", (ushort) 0);
				Message repMsg = conn1.Request(reqMsg, 5000, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);

				if (repMsg == null)
				{
					Check("Expected to receive response message", false);
				}
				else
				{
					Check("Unexpected response message kind", repMsg.GetKind() == Message.Kind.REPLY);
					Check("Unexpected response message subject", repMsg.GetSubject().Contains( GetSubject("RESP.DIR")));
				}

				// Verify null is returned on timeout
				reqCallback.ProvideResponse(false);

				reqMsg.AddField("REQUEST-ID", (ushort) 1);
				repMsg = conn1.Request(reqMsg, 5000, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);

				if (repMsg != null)
				{
					Check("Was not expecting to receive response message", false);
				}

				// Verify request is delivered multiple times until response delivered
				reqCallback.ProvideResponse(false);

				reqMsg.AddField("REQUEST-ID", (ushort) 2);
				repMsg = conn1.Request(reqMsg, 5000, 1000);

				if (repMsg == null)
				{
					Check("Expected to receive response message", false);
				}
			}

			// Off-nominal tests

			using (Connection conn2 = new Connection( GetConfig() ))
			{
				// Connection not initialized
				Message reqMsg = conn2.GetMessageFactory().CreateMessage("REQ.DIR");
				try {
					conn2.Request(reqMsg, 5000, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);
					Check("Connection not initialized", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn3 = new Connection( GetConfig() ))
			{
				// Request using non-REQUEST kind message
				conn3.Connect();
				Message badMsg = conn3.GetMessageFactory().CreateMessage("RESP.DIR");
				try {
					conn3.Request(badMsg, 5000, Gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);
					Check("Was expecting bad message error", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Cannot issue request with non-REQUEST kind message"));
				}
				conn3.Disconnect();
			}
		}


		private void Test_Reply()
		{
			Log.Info("Test Reply()");

			// Nominal tests for Connection.reply() tested earlier

			// Off-nominal tests
			using (Connection conn2 = new Connection( GetConfig() ))
			{
				// Connection not initialized
				Message reqMsg = conn2.GetMessageFactory().CreateMessage("REQ.DIR");
				Message repMsg = conn2.GetMessageFactory().CreateMessage("RESP.DIR");
				try {
					conn2.Reply(reqMsg, repMsg);
					Check("Was expecting an exception since we ain't initialized", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn4 = new Connection( GetConfig() ))
			{
				// Bad reply message
				conn4.Connect();
				Message reqMsg = conn4.GetMessageFactory().CreateMessage("REQ.DIR");
				Message badMsg = conn4.GetMessageFactory().CreateMessage("REQ.DIR");
				try {
					conn4.Reply(reqMsg, badMsg);
					Check("Was expecting an exception since we have a bad reply message", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Cannot issue reply with non-REPLY kind message"));
				}
				conn4.Disconnect();
			}
		}


		private void Test_Dispatch()
		{
			Log.Info("Test Dispatch()");

			T010_Callback mcb1 = new T010_Callback();
			T010_Callback mcb2 = new T010_Callback();
			T010_Callback mcb3 = new T010_Callback();

			using (Connection conn1 = new Connection( GetConfig() ))
			{
				conn1.Connect();

				SetStandardFields(conn1.GetMessageFactory());

				Message msg = conn1.GetMessageFactory().CreateMessage("HB");
				msg.SetSubject( GetSubject("HB") );

				conn1.Subscribe( msg.GetSubject(), mcb1 );
				conn1.Subscribe( msg.GetSubject(), mcb2 );
				conn1.Subscribe( GetSubject("FOO.BAR"),  mcb3 );

				conn1.Publish(msg);

				Message rcvd = conn1.Receive(5000);

				if (rcvd != null)
				{
					conn1.Dispatch(rcvd);

					TimeUtil.Millisleep(1000);

					Check("Expected Callback 1 to receive a message", mcb1.WasCalled());
					Check("Expected Callback 2 to receive a message", mcb2.WasCalled());
					Check("Expected Callback 3 to not receive a message", mcb3.WasCalled() == false);

					Message.Destroy(rcvd);
				}
				else
				{
					Check("Failed to receive message", false);
				}

				conn1.Disconnect();
			}
		}


		private void Test_Receive()
		{
			Log.Info("Test Receive()");

			// Nominal tests
			using (Connection conn1 = new Connection( GetConfig() ))
			{
				SetStandardFields(conn1.GetMessageFactory());

				conn1.Connect();

				//o Timeout occurs (no subscription)
				Message rcvd = conn1.Receive(10);
				Check("Received unexpected message", rcvd == null);

				//o Receive message
				Message msg = conn1.GetMessageFactory().CreateMessage("HB");
				msg.SetSubject( GetSubject("HB") );

				conn1.Subscribe(msg.GetSubject());
				conn1.Publish(msg);

				rcvd = conn1.Receive(5000);
				Check("Did not receive expected message", rcvd != null);

				Message.Destroy(rcvd);

				conn1.Disconnect();
			}

			// Off-nominal tests
			using (Connection conn2 = new Connection( GetConfig() ))
			{
				// Connection not initialized
				try {
					conn2.Receive(10);
					Check("Expected exception since we're not initialized", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn3 = new Connection( GetConfig() ))
			{
				// Auto-dispatcher enabled
				conn3.Connect();
				conn3.StartAutoDispatch();
				try {
					conn3.Receive(10);
					Check("Expected exception since we've enabled the auto-dispatcher", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("This method cannot be called when the Auto-Dispatcher is in use"));
				}
				conn3.Disconnect();
			}
		}


		private void Test_ExcludeSubject()
		{
			Log.Info("Test ExcludeSubject()");

			using (Connection conn1 = new Connection( GetConfig() ))
			{
				conn1.Connect();

				SetStandardFields(conn1.GetMessageFactory());

				Message msg1 = conn1.GetMessageFactory().CreateMessage("HB");
				Message msg2 = conn1.GetMessageFactory().CreateMessage("LOG");

				msg1.SetSubject( GetSubject("HB") );
				msg2.SetSubject( GetSubject("LOG") );

				msg2.AddField("SUBCLASS", "TEST");
				msg2.AddField("OCCURRENCE-TYPE", "TEST");
				msg2.AddField("SEVERITY", (ushort) 1);

				conn1.Subscribe( GetSubject(">") );
				conn1.ExcludeSubject( GetSubject("LOG.+") );

				conn1.Publish(msg1);

				Message rcvd = conn1.Receive(5000);
				Check("Was expecting to receive an HB message", rcvd != null);
				Message.Destroy(rcvd);

				conn1.Publish(msg2);
				rcvd = conn1.Receive(5000);
				Check("Was not expecting to receive a LOG message", rcvd == null);

				conn1.RemoveExcludedSubject( GetSubject("LOG.+") );

				conn1.Publish(msg2);
				rcvd = conn1.Receive(5000);
				Check("Was expecting to receive a LOG message", rcvd != null);
				Message.Destroy(rcvd);

				conn1.Disconnect();
			}

			// Off-nominal tests
			using (Connection conn2 = new Connection( GetConfig() ))
			{
				// Connection not initialized
				try {
					conn2.ExcludeSubject("GMSEC.FOO.BAR");
					Check("Expected exception since we're not initialized", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn3 = new Connection( GetConfig() ))
			{
				// Invalid subject
				conn3.Connect();
				try {
					conn3.ExcludeSubject("GMSEC FOO BAR");
					Check("Expected exception for invalid subject", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Subject is invalid"));
				}
				conn3.Disconnect();
			}
		}


		private void Test_RemoveExcludedSubject()
		{
			Log.Info("Test RemoveExcludedSubject()");

			// Nominal test for RemoveExcludedSubject() are conducted witin Test_ExcludeSubject()

			// Off-nominal tests

			using (Connection conn2 = new Connection( GetConfig() ))
			{
				// Connection not initialized
				try {
					conn2.RemoveExcludedSubject("GMSEC.FOO.BAR");
					Check("Expected exception since we're not initialized", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Connection has not been initialized"));
				}
			}

			using (Connection conn3 = new Connection( GetConfig() ))
			{
				// Invalid subject
				conn3.Connect();
				try {
					conn3.RemoveExcludedSubject("GMSEC FOO BAR");
					Check("Expected exception for invalid subject", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Subject is invalid"));
				}
				conn3.Disconnect();
			}
		}


		private void Test_GetSetName()
		{
			Log.Info("Test GetName() and SetName()");

			using (Connection conn = new Connection( GetConfig() ))
			{
				conn.Connect();
				Check("Expected a connection name", conn.GetName() != null);

				conn.SetName("FOOBAR");

				Check("Expected a connection name of FOOBAR", conn.GetName() == "FOOBAR");
			}
		}


		private void Test_GetID()
		{
			Log.Info("Test GetID()");

			using (Connection conn = new Connection( GetConfig() ))
			{
				conn.Connect();
				Check("Expected a connection ID", conn.GetID() != null);
				conn.Disconnect();
			}
		}


		private void Test_GetMWInfo()
		{
			Log.Info("Test GetMWInfo()");

			using (Connection conn = new Connection( GetConfig() ))
			{
				Check("Expected mwInfo to contain mw-id", conn.GetMWInfo().Contains( GetConfig().GetValue("mw-id") ));
			}
		}


		private void Test_GetConnectionEndpoint()
		{
			Log.Info("Test GetConnectionEndpoint()");

			using (Connection conn = new Connection(GetConfig()))
			{
				conn.Connect();
				Check("Unexpected connection endpoint", conn.GetConnectionEndpoint() != null);
				conn.Disconnect();
			}
		}


		private void Test_GetPublishQueueMessageCount()
		{
			Log.Info("Test GetPublishQueueMessageCount()");

			Config config = new Config( GetConfig() );  // make a copy!
			config.AddValue("gmsec-async-publish", "true");
			config.AddValue("gmsec-async-publish-queue-depth", "20");
			config.AddValue("gmsec-async-publish-teardown-wait", "2000");

			using (Connection conn = new Connection(config))
			{
				conn.Connect();
				SetStandardFields(conn.GetMessageFactory());

				Message msg = conn.GetMessageFactory().CreateMessage("TLMPROC");
				msg.AddField("STREAM-MODE", "1");

				for (int i = 0; i < 10; ++i)
				{
					conn.Publish(msg);
					Check("Unexpected message count < 0", conn.GetPublishQueueMessageCount() >= 0);
				}

				TimeUtil.Millisleep(2000);

				Check("Unexpected message count != 0", conn.GetPublishQueueMessageCount() == 0);

				conn.Disconnect();
			}
		}
	}


	class T010_EventCallback : EventCallback
	{
		private bool m_called;
		private Connection.Event m_event;

		public T010_EventCallback()
		{
			m_called = false;
			m_event  = Connection.Event.ALL_EVENTS;
		}

		public override void OnEvent(Connection conn, Status status, Connection.Event eventus)
		{
			m_called = true;
			m_event  = eventus;
		}

		public bool WasCalled() { return m_called; }

		public Connection.Event GetEvent() { return m_event; }

		public void Reset()
		{
			m_called = false;
			m_event  = Connection.Event.ALL_EVENTS;
		}
	}


	class T010_Callback : Callback
	{
		private bool m_called;

		public T010_Callback() { m_called = false; }

		public override void OnMessage(Connection conn, Message msg) { m_called = true; }

		public bool WasCalled() { return m_called; }

		public void Reset() { m_called = false; }
	}


	class T010_RequestCallback : Callback
	{
		private TestCase m_test;
		private bool     m_provideResponse;

		public T010_RequestCallback(TestCase test)
		{
			m_test = test;
			m_provideResponse = true;
		}

		public override void OnMessage(Connection conn, Message msg)
		{
			try
			{
				if (m_provideResponse)
				{
					Message repMsg = conn.GetMessageFactory().CreateMessage("RESP.DIR");
					repMsg.SetSubject( m_test.GetSubject("RESP.DIR") );

					repMsg.AddField("RESPONSE-STATUS", (short) Message.ResponseStatus.SUCCESSFUL_COMPLETION);

					if (msg.HasField("COMPONENT"))
					{
						repMsg.AddField("DESTINATION-COMPONENT", msg.GetStringValue("COMPONENT"));
					}
					if (msg.HasField("DESTINATION-COMPONENT"))
					{
						repMsg.AddField("COMPONENT", msg.GetStringValue("DESTINATION-COMPONENT"));
					}
					else
					{
						repMsg.AddField("COMPONENT", "RESPONDER");
					}
					if (msg.HasField("REQUEST-ID"))
					{
						repMsg.AddField(msg.GetField("REQUEST-ID"));
					}

					conn.Reply(msg, repMsg);
				}
				else
				{
					m_provideResponse = true;
				}
			}
			catch (GmsecException e)
			{
				Log.Error("Unexpected Exception: " + e.ToString());
			}
		}

		public void ProvideResponse(bool flag) { m_provideResponse = flag; }
	}


	class T010_ReplyCallback : ReplyCallback
	{
		private bool m_gotReply;
		private bool m_gotEvent;

		public T010_ReplyCallback()
		{
			m_gotReply = false;
			m_gotEvent = false;
		}

		public override void OnReply(Connection conn, Message request, Message reply)
		{
			m_gotReply = true;
		}

		public override void OnEvent(Connection conn, Status status, Connection.Event eventus)
		{
			m_gotEvent = (eventus == Connection.Event.REQUEST_TIMEOUT_EVENT);
		}

		public bool GotReply() { return m_gotReply; }
		public bool GotEvent() { return m_gotEvent; }
		public void Reset()    { m_gotReply = false; m_gotEvent = false; }
	}
}
