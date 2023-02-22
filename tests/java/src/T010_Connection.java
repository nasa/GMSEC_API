import gov.nasa.gsfc.gmsec.api5.ApiVersion;
import gov.nasa.gsfc.gmsec.api5.Callback;
import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Connection;
import gov.nasa.gsfc.gmsec.api5.EventCallback;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.ReplyCallback;
import gov.nasa.gsfc.gmsec.api5.Specification;
import gov.nasa.gsfc.gmsec.api5.Status;
import gov.nasa.gsfc.gmsec.api5.StatusClassification;
import gov.nasa.gsfc.gmsec.api5.StatusCode;
import gov.nasa.gsfc.gmsec.api5.SubscriptionInfo;
import gov.nasa.gsfc.gmsec.api5.U16;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.StringField;

import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;

import java.util.ArrayList;
import java.util.List;


public class T010_Connection extends TestCase
{
	public static void main(String[] args)
	{
		T010_Connection test = new T010_Connection();
		test.setDescription("Connection");
		test.run(args);
	}


	public void testCase()
		throws Exception
	{
		try
		{
			test_constructor();
			test_get_api_version();
			test_connect();
			test_disconnect();
			test_get_library_name();
			test_get_library_version();
			test_get_message_factory();
			test_register_event_callback();
			test_subscribe();
			test_subscribe_with_callback();    // also calls on startAutoDispatch/stopAutoDispatch
			test_unsubscribe();
			test_start_auto_dispatch();
			test_stop_auto_dispatch();
			test_publish();                    // also calls on subscribe() and receive()
			test_publish_with_mwconfig();
			test_request_with_callback();      // also calls on reply()
			test_request();                    // also calls on reply()
			test_reply();
			test_dispatch();
			test_receive();
			test_release();
			test_exclude_subject();            // also tests removeExcludedSubject()
			test_remove_excluded_subject();
			test_get_set_name();
			test_get_id();
			test_get_mw_info();
			test_get_connection_endpoint();
			test_get_publish_queue_message_count();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_constructor()
		throws Exception
	{
		Log.info("Test constructor");

		// Nominal test
		try
		{
			Connection conn = Connection.create( getConfig() );
			check("Okay", true);
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			require(e.getMessage(), false);
		}

		// Nominal test w/ MessageFactory
		try
		{
			MessageFactory factory = MessageFactory.create( getConfig() );
			Connection     conn1   = Connection.create( getConfig(), factory );
			Connection     conn2   = Connection.create( getConfig(), factory );
			check("Okay", true);
			Connection.destroy(conn1);
			Connection.destroy(conn2);
			MessageFactory.destroy(factory);
		}
		catch (GmsecException e)
		{
			require(e.getMessage(), false);
		}

		// Off-Nominal test
		try
		{   // missing mw-id
			Connection conn = Connection.create( new Config() );
			check("Exception expected", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("mw-id in Config is not specified"));
		}

		try
		{   //bogus middleware
			Connection conn = Connection.create( new Config("mw-id=bogus", Gmsec.DataType.KEY_VALUE_DATA) );
			check("Exception expected", false);
		}
		catch (Exception e)
		{
			check(e.getMessage(), e.getMessage().contains("Unable to load"));
		}
	}


	private void test_get_api_version()
		throws Exception
	{
		Log.info("Test getAPIVersion()");

        String version = "GMSEC API v" + ApiVersion.GMSEC_VERSION_NUMBER;
        check("Expected to get API version info", Connection.getAPIVersion().contains(version));
	}


	private void test_connect()
		throws Exception
	{
		Log.info("Test connect()");

		Connection conn = null;

		// Nominal test
		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();
			check("Okay", true);
			conn.disconnect();
		}
		catch (GmsecException e)
		{
			require(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal test
		try
		{
			Config config = new Config( "mw-id=bolt server=gs580s-gc764mw1:1234", Gmsec.DataType.KEY_VALUE_DATA );

			conn = Connection.create(config);
			conn.connect();
			check("Expected error to indicate unable to connect", false);
		}
		catch (GmsecException e)
		{
			boolean result = (e.getErrorClassification() == StatusClassification.MIDDLEWARE_ERROR) &&
			                 (e.getErrorCode() == StatusCode.CUSTOM_ERROR_CODE) &&
			                 (e.getCustomCode() == 7);

			check(e.getMessage(), result);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_disconnect()
		throws Exception
	{
		Log.info("Test disconnect()");

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.disconnect();
			check("Okay", true);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_get_library_name()
		throws Exception
	{
		Log.info("Test getLibraryName()");

		try
		{
			Connection conn = Connection.create( getConfig() );
			check("Unexpected library name", conn.getLibraryName().contains( getConfig().getValue("mw-id", "unknown") ));
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
	}


	private void test_get_library_version()
		throws Exception
	{
		Log.info("Test getLibraryVersion()");

		try
		{
			Connection conn = Connection.create( getConfig() );
			check("Expected to get a library version", null != conn.getLibraryVersion());
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
	}


	private void test_get_message_factory()
		throws Exception
	{
		Log.info("Test getMessageFactory()");

		try
		{
			Connection conn = Connection.create( getConfig() );
			check("Expected to get a MessageFactory for the CURRENT specification", Gmsec.MSG_SPEC_CURRENT == conn.getMessageFactory().getSpecification().getVersion());
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
	}


	private void test_register_event_callback()
		throws Exception
	{
		Log.info("Test registerEventCallback()");

		String mw = getConfig().getValue("mw-id", "unknown");

		if (mw.equals("unknown") || mw.equals("mb") || mw.equals("generic_jms") || mw.equals("zeromq413") || mw.equals("zeromq432"))
		{
			// Test is being skipped because of lame middleware wrapper
			// which does not support event notification upon connect
			// and/or disconnect to/from the GMSEC Bus.
			return;
		}

		T010_EventCallback mec = new T010_EventCallback();

		Connection conn = null;

		// Nominal case
		try
		{
			conn = Connection.create( getConfig() );
			conn.registerEventCallback(Connection.Event.ALL_EVENTS, mec);

			conn.connect();
			check("Expected EventCallback to be called", true == mec.wasCalled());
			check("Expected to be connected", Connection.Event.CONNECTION_SUCCESSFUL_EVENT == mec.getEvent());

			mec.reset();

			conn.disconnect();
			check("Expected EventCallback to be called", true == mec.wasCalled());
			check("Expected to be disconnected", Connection.Event.CONNECTION_BROKEN_EVENT == mec.getEvent());
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal case
		try
		{
			conn = Connection.create( getConfig() );
			conn.registerEventCallback(Connection.Event.ALL_EVENTS, (EventCallback) null);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("EventCallback is null"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_subscribe()
		throws Exception
	{
		Log.info("Test subscribe()");

		Connection conn = null;

		// Nominal test
		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();

			conn.subscribe( getSubject("*.BAR") );
			conn.subscribe( getSubject("FOO.BAZ") );

			Message msg1 = conn.getMessageFactory().createMessage("LOG");
			Message msg2 = conn.getMessageFactory().createMessage("LOG");
			Message msg3 = conn.getMessageFactory().createMessage("LOG");

			msg1.setSubject( getSubject("GIN.BAR") );
			msg2.setSubject( getSubject("FOO.BAZ") );
			msg3.setSubject( getSubject("FOO.GAZ") );

			conn.publish(msg1);

			Message msg = conn.receive(5000);
			check("Was expecting to receive a message", msg != null);
			check("Unexpected message subject", msg.getSubject().equals( getSubject("GIN.BAR") ));

			conn.publish(msg2);

			msg = conn.receive(5000);
			check("Was expecting to receive a message", msg != null);
			check("Unexpected message subject", msg.getSubject().equals( getSubject("FOO.BAZ") ));

			conn.publish(msg3);

			msg = conn.receive(5000);
			check("Was not expecting to receive a message", msg == null);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // duplicate subscription
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.subscribe("GMSEC.>");
			conn.subscribe("GMSEC.>");
			check("Was expecting an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Duplicate subscription"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // subscribe() before connect()
			conn = Connection.create( getConfig() );
			conn.subscribe("GMSEC.>");
			check("Was expecting an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // null subscription subject
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.subscribe(null);
			check("Was expecting an exception", false);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Subject is null or contains an empty string"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // illegal subscription subject
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.subscribe("GMSEC.MY MISSION.*.*.>");
			check("Was expecting an exception", false);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Subject is invalid"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_subscribe_with_callback()
		throws Exception
	{
		Log.info("Test subscribe() with Callback");

		T010_Callback      mcb = new T010_Callback();
		T010_EventCallback ecb = new T010_EventCallback();

		Connection conn = null;

		// Nominal tests (verify Callback is summoned)
		try
		{
			conn = Connection.create( getConfig() );

			conn.connect();
			conn.registerEventCallback(Connection.Event.ALL_EVENTS, ecb);
			conn.subscribe( getSubject(">"), mcb );

			conn.startAutoDispatch();

			setStandardFields(conn.getMessageFactory());

			Message msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject( getSubject("HB") );

			conn.publish(msg);

			TimeUtil.millisleep(5000);

			check("Expected Callback to be summoned", mcb.wasCalled());
			check("Expected ALL_EVENTS", ecb.getEvent() == Connection.Event.ALL_EVENTS);

			conn.stopAutoDispatch();

			mcb.reset();
			ecb.reset();
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Nominal test (message validation enabled, illegal subject)
		try
		{
			Config config = new Config( getConfig() );  // make copy!
			config.addValue("gmsec-msg-content-validate-recv", "true");

			conn = Connection.create(config);

			conn.connect();
			conn.registerEventCallback(Connection.Event.ALL_EVENTS, ecb);
			conn.subscribe( getSubject(">"), mcb );

			conn.startAutoDispatch();

			setStandardFields(conn.getMessageFactory());

			Message msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject( getSubject("HB") );

			conn.publish(msg);

			TimeUtil.millisleep(5000);

			check("Did not expect Callback to be summoned", mcb.wasCalled() == false);
			check("Expected an INVALID_MESSAGE_EVENT", ecb.getEvent() == Connection.Event.INVALID_MESSAGE_EVENT);

			conn.stopAutoDispatch();
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // subscriptions using same subject and unique callbacks
			T010_Callback mcb2 = new T010_Callback();
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.subscribe("GMSEC.>", mcb);
			conn.subscribe("GMSEC.>", mcb2);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // duplicate subscription using the same callback
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.subscribe("GMSEC.>", mcb);
			conn.subscribe("GMSEC.>", mcb);
			check("Was expecting an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Duplicate subscription"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // null subscription subject
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.subscribe(null, mcb);
			check("Was expecting an exception", false);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Subject is null or contains an empty string"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // illegal subscription subject
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.subscribe("GMSEC.MY MISSION.*.*.>", mcb);
			check("Was expecting an exception", false);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Subject is invalid"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_unsubscribe()
		throws Exception
	{
		Log.info("Test unsubscribe()");

		T010_Callback mcb = new T010_Callback();

		Config config = new Config( getConfig() );  // make copy!
		config.addValue("gmsec-msg-content-validate-recv", "true");

		Connection conn = null;

		// Nominal tests
		try
		{
			conn = Connection.create(config);
			conn.connect();
			SubscriptionInfo info = conn.subscribe( getSubject("FOO.BAR") );
			conn.unsubscribe(info);

			Message hb = conn.getMessageFactory().createMessage("HB");
			hb.setSubject( getSubject("FOO.BAR") );

			conn.publish(hb);

			Message msg = conn.receive(5000);

			check("Did not expect to receive a message", msg == null);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // Attempt to unsubscribe using NULL object
			conn = Connection.create(config);
			conn.connect();
			SubscriptionInfo info = null;
			conn.unsubscribe(info);
		}
		catch (IllegalArgumentException | GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("SubscriptionInfo is null"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		Connection conn2 = null;
		try
		{   // Attempt to unsubscribe using wrong Connection object
			conn  = Connection.create(config);
			conn2 = Connection.create(config);

			conn.connect();
			conn2.connect();

			SubscriptionInfo info1 = conn.subscribe("GMSEC.FOO.BAR");

			conn2.unsubscribe(info1);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("SubscriptionInfo object is not associated with this Connection"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
			if (conn2 != null) Connection.destroy(conn2);
		}
	}


	private void test_start_auto_dispatch()
		throws Exception
	{
		Log.info("Test startAutoDispatch()");

		// startAutoDispatcher already tested within test_subscribe_callback()

		Connection conn = null;

		// Off-nominal test
		try
		{   // Auto-dispatch cannot start until Connection is initialized
			conn = Connection.create( getConfig() );
			conn.startAutoDispatch();
			check("Expected an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_stop_auto_dispatch()
		throws Exception
	{
		Log.info("Test stopAutoDispatch()");

		// stopAutoDispatcher already tested within test_subscribe_callback()

		Connection conn = null;

		// Off-nominal test
		try
		{   // Auto-dispatch cannot be stopped until Connection is initialized
			conn = Connection.create( getConfig() );
			conn.stopAutoDispatch();
			check("Expected an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_publish()
		throws Exception
	{
		Log.info("Test publish()");

		// Nominal test
		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );

			conn.connect();
			conn.subscribe( getSubject(">") );

			setStandardFields(conn.getMessageFactory());

			Message msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject( getSubject("HB") );

			conn.publish(msg);

			Message rcvd = conn.receive(5000);

			if (rcvd != null)
			{
				check("Unexpected subject", rcvd.getSubject().equals( getSubject("HB") ));
				check("Unexpected message kind", rcvd.getKind() == Message.Kind.PUBLISH);
			}
			else
			{
				check("Failed to receive published message", false);
			}
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // Message validation fails (missing standard fields)
			Config config = new Config( getConfig() );  // make a copy!
			config.addValue("gmsec-msg-content-validate", "true");

			conn = Connection.create(config);
			Message msg = conn.getMessageFactory().createMessage("HB");
			conn.connect();
			conn.publish(msg);
			check("Was expecting an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Message Validation Failed"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Connection not initialized
			conn = Connection.create(getConfig());
			Message msg = conn.getMessageFactory().createMessage("HB");
			conn.publish(msg);
			check("Was expecting an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Non-PUBLISH kind message
			conn = Connection.create(getConfig());
			Message msg = conn.getMessageFactory().createMessage("REQ.DIR");
			conn.connect();
			conn.publish(msg);
			check("Was expecting an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Cannot publish message with non-PUBLISH message kind"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_publish_with_mwconfig()
		throws Exception
	{
		Log.info("Test publish() with mwConfig");

		Connection conn = null;

		try
		{
			conn = Connection.create(getConfig());

			setStandardFields(conn.getMessageFactory());

			Message msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject( getSubject("MW-CONFIG") );

			Config mwConfig = new Config();

			conn.connect();

			conn.subscribe( getSubject("MW-CONFIG") );

			conn.publish( msg, mwConfig );

			Message rcv = conn.receive( 5000 );

			if (rcv != null)
			{
				check("Received a message", true);

				Message.destroy(msg);
			}
			else
			{
				check("Failed to receive a message", false);
			}

			conn.disconnect();
		}
		catch (GmsecException e)
		{
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_request_with_callback()
		throws Exception
	{
		Log.info("Test asynchronous request()");

		T010_RequestCallback reqCallback = new T010_RequestCallback(this);
		T010_ReplyCallback   repCallback = new T010_ReplyCallback();

		Connection conn = null;

		// Nominal case
		try
		{
			conn = Connection.create( getConfig() );

			conn.connect();

			setStandardFields(conn.getMessageFactory());

			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			reqMsg.setSubject( getSubject("REQ.DIR") );
			reqMsg.addField("DIRECTIVE-STRING", "Do something!");
			reqMsg.addField("DESTINATION-COMPONENT", "RESPONDER");

			// subscribe to receive the response message
			conn.subscribe( getSubject("RESP.DIR") );

			// subscribe to receive/process the request message
			conn.subscribe(reqMsg.getSubject(), reqCallback);
			conn.startAutoDispatch();

			//o Verify response is returned
			reqMsg.addField("REQUEST-ID", new U16(0));
			conn.request(reqMsg, 5000, repCallback, Gmsec.REQUEST_REPUBLISH_NEVER);
			TimeUtil.millisleep(2000);

			check("Was expecting a reply", repCallback.gotReply() == true);
			check("Was not expecting an event", repCallback.gotEvent() == false);

			repCallback.reset();

			//o Verify NULL is returned on timeout
			reqCallback.provideResponse(false);

			reqMsg.addField("REQUEST-ID", new U16(1));
			conn.request(reqMsg, 5000, repCallback, Gmsec.REQUEST_REPUBLISH_NEVER);
			TimeUtil.millisleep(5500);

			check("Was not expecting a reply", repCallback.gotReply() == false);
			check("Was expecting an event", repCallback.gotEvent() == true);

			//o Verify request is delivered multiple times until response delivered
			reqCallback.provideResponse(false);

			reqMsg.addField("REQUEST-ID", new U16(2));
			conn.request(reqMsg, 5000, repCallback, 5000);
			TimeUtil.millisleep(5500);

			check("Was expecting a reply", repCallback.gotReply() == true);
			check("Was expecting an event", repCallback.gotEvent() == true);

			//o Verify request is delivered multiple times until response delivered
			reqCallback.provideResponse(false);

			reqMsg.addField("REQUEST-ID", new U16(3));
			conn.request(reqMsg, -1, repCallback, 5000);
			TimeUtil.millisleep(5500);

			check("Was expecting a reply", repCallback.gotReply() == true);
			check("Was expecting an event", repCallback.gotEvent() == true);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // Connection not initialized
			conn = Connection.create( getConfig() );
			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			conn.request(reqMsg, 5000, repCallback, Gmsec.REQUEST_REPUBLISH_NEVER);
			check("An exception was expected", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Request using non-REQUEST kind message
			conn = Connection.create( getConfig() );
			conn.connect();
			Message badMsg = conn.getMessageFactory().createMessage("RESP.DIR");
			conn.request(badMsg, 5000, repCallback, Gmsec.REQUEST_REPUBLISH_NEVER);
			check("An exception was expected", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Cannot issue request with non-REQUEST kind message"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_request()
		throws Exception
	{
		Log.info("Test synchronous request()");

		T010_RequestCallback reqCallback = new T010_RequestCallback(this);

		Connection conn = null;

		// Nominal case
		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();

			setStandardFields(conn.getMessageFactory());

			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			reqMsg.setSubject( getSubject("REQ.DIR") );
			reqMsg.addField("DIRECTIVE-STRING", "Do something!");
			reqMsg.addField("DESTINATION-COMPONENT", "RESPONDER");

			// subscribe to receive the response message
			conn.subscribe( getSubject("RESP.DIR") );

			// subscribe to proess the request message
			conn.subscribe(reqMsg.getSubject(), reqCallback);
			conn.startAutoDispatch();

			//o Verify response is returned
			reqMsg.addField("REQUEST-ID", new U16(0));
			Message repMsg = conn.request(reqMsg, 5000, Gmsec.REQUEST_REPUBLISH_NEVER);

			if (repMsg == null)
			{
				check("Expected to receive response message", false);
			}
			else
			{
				check("Unexpected response message kind", repMsg.getKind() == Message.Kind.REPLY);
				check("Unexpected response message subject", repMsg.getSubject().contains( getSubject("RESP.DIR") ));
			}

			// Verify null is returned on timeout
			reqCallback.provideResponse(false);

			reqMsg.addField("REQUEST-ID", new U16(1));
			repMsg = conn.request(reqMsg, 5000, Gmsec.REQUEST_REPUBLISH_NEVER);

			if (repMsg != null)
			{
				check("Was not expecting to receive response message", false);
			}

			// Verify request is delivered multiple times until response delivered
			reqCallback.provideResponse(false);

			reqMsg.addField("REQUEST-ID", new U16(2));
			repMsg = conn.request(reqMsg, 5000, 1000);

			if (repMsg == null)
			{
				check("Expected to receive response message", false);
			}
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // Connection not initialized
			conn = Connection.create( getConfig() );
			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			conn.request(reqMsg, 5000, Gmsec.REQUEST_REPUBLISH_NEVER);
			check("Connection not initialized", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Request using non-REQUEST kind message
			conn = Connection.create( getConfig() );
			conn.connect();
			Message badMsg = conn.getMessageFactory().createMessage("RESP.DIR");
			conn.request(badMsg, 5000, Gmsec.REQUEST_REPUBLISH_NEVER);
			check("Was expecting bad message error", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Cannot issue request with non-REQUEST kind message"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_reply()
		throws Exception
	{
		Log.info("Test reply()");

		// Nominal tests for Connection.reply() tested earlier

		Connection conn = null;

		// Off-nominal tests
		try
		{   // Connection not initialized
			conn = Connection.create( getConfig() );
			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			Message repMsg = conn.getMessageFactory().createMessage("RESP.DIR");
			conn.reply(reqMsg, repMsg);
			check("Was expecting an exception since we ain't initialized", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Bad reply message
			conn = Connection.create( getConfig() );
			conn.connect();
			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			Message badMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			conn.reply(reqMsg, badMsg);
			check("Was expecting an exception since we have a bad reply message", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Cannot issue reply with non-REPLY kind message"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_dispatch()
	{
		Log.info("Test dispatch()");

		T010_Callback mcb1 = new T010_Callback();
		T010_Callback mcb2 = new T010_Callback();
		T010_Callback mcb3 = new T010_Callback();

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();

			setStandardFields(conn.getMessageFactory());

			Message msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject( getSubject("HB") );

			conn.subscribe( msg.getSubject(), mcb1 );
			conn.subscribe( msg.getSubject(), mcb2 );
			conn.subscribe( getSubject("FOO.BAR"),  mcb3 );

			conn.publish(msg);

			Message rcvd = conn.receive(5000);

			if (rcvd != null)
			{
				conn.dispatch(rcvd);

				TimeUtil.millisleep(1000);

				check("Expected Callback 1 to receive a message", mcb1.wasCalled());
				check("Expected Callback 2 to receive a message", mcb2.wasCalled());
				check("Expected Callback 3 to not receive a message", mcb3.wasCalled() == false);
			}
			else
			{
				check("Failed to receive message", false);
			}
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_receive()
		throws Exception
	{
		Log.info("Test receive()");

		Connection conn = null;

		// Nominal tests
		try
		{
			conn = Connection.create( getConfig() );

			conn.connect();

			setStandardFields(conn.getMessageFactory());

			//o Timeout occurs (no subscription)
			Message rcvd = conn.receive(10);
			check("Received unexpected message", rcvd == null);

			//o Receive message
			Message msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject( getSubject("HB") );

			conn.subscribe(msg.getSubject());
			conn.publish(msg);

			rcvd = conn.receive(5000);
			check("Did not receive expected message", rcvd != null);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // Connection not initialized
			conn = Connection.create( getConfig() );
			conn.receive(10);
			check("Expected exception since we're not initialized", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Auto-dispatcher enabled
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.startAutoDispatch();
			conn.receive(10);
			check("Expected exception since we've enabled the auto-dispatcher", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("This method cannot be called when the Auto-Dispatcher is in use"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_release()
		throws Exception
	{
		Log.info("Test release()");

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );

			conn.connect();

			setStandardFields(conn.getMessageFactory());

			//o Publish and Receive message
			Message msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject( getSubject("HB") );

			conn.subscribe(msg.getSubject());
			conn.publish(msg);

			Message rcvd = conn.receive(5000);
			require("Did not receive expected message", rcvd != null);

			//o Release message
			Message.destroy(rcvd);

			//o Verify received message can only be released once
			try {
				Message.destroy(rcvd);
				check("Expected exception when attempting to release a message twice", false);
			}
			catch (NullPointerException e) {
				check("Okay", e.getMessage().contains("Message reference is null"));
			}
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("This method cannot be called when the Auto-Dispatcher is in use"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

	}


	private void test_exclude_subject()
		throws Exception
	{
		Log.info("Test excludeSubject()");

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();

			setStandardFields(conn.getMessageFactory());

			Message msg1 = conn.getMessageFactory().createMessage("HB");
			Message msg2 = conn.getMessageFactory().createMessage("LOG");

			msg1.setSubject( getSubject("HB") );
			msg2.setSubject( getSubject("LOG") );

			msg2.addField("SUBCLASS", "NO-CLASS");
			msg2.addField("OCCURRENCE-TYPE", "FREQUENTLY");
			msg2.addField("SEVERITY", (short) 1);

			conn.subscribe( getSubject(">") );
			conn.excludeSubject( getSubject("LOG") );

			conn.publish(msg1);

			Message rcvd = conn.receive(5000);
			check("Was expecting to receive an HB message", rcvd != null);

			conn.publish(msg2);
			rcvd = conn.receive(5000);
			check("Was not expecting to receive a LOG message", rcvd == null);

			conn.removeExcludedSubject( getSubject("LOG") );

			conn.publish(msg2);
			rcvd = conn.receive(5000);
			check("Was expecting to receive a LOG message", rcvd != null);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		// Off-nominal tests
		try
		{   // Connection not initialized
			conn = Connection.create( getConfig() );
			conn.excludeSubject("GMSEC.FOO.BAR");
			check("Expected exception since we're not initialized", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Invalid subject
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.excludeSubject("GMSEC FOO BAR");
			check("Expected exception for invalid subject", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Subject is invalid"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_remove_excluded_subject()
		throws Exception
	{
		Log.info("Test removeExcludedSubject()");

		// Nominal test for removeExcludedSubject() are conducted witin test_exclude_subject()

		Connection conn = null;

		// Off-nominal tests
		try
		{   // Connection not initialized
			conn = Connection.create( getConfig() );
			conn.removeExcludedSubject("GMSEC.FOO.BAR");
			check("Expected exception since we're not initialized", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Connection has not been initialized"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		try
		{   // Invalid subject
			conn = Connection.create( getConfig() );
			conn.connect();
			conn.removeExcludedSubject("GMSEC FOO BAR");
			check("Expected exception for invalid subject", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Subject is invalid"));
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_get_set_name()
		throws Exception
	{
		Log.info("Test getName() and setName()");

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();
			check("Expected a connection name", conn.getName() != null);

			conn.setName("FOOBAR");

			check("Expected a connection name of FOOBAR", conn.getName().equals("FOOBAR"));
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_get_id()
		throws Exception
	{
		Log.info("Test getID()");

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();
			check("Expected a connection ID", conn.getID() != null);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_get_mw_info()
		throws Exception
	{
		Log.info("Test getMWInfo()");

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );
			check("Expected mwInfo to contain mw-id", conn.getMWInfo().contains( getConfig().getValue("mw-id") ));
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_get_connection_endpoint()
		throws Exception
	{
		Log.info("Test getConnectionEndpoint()");

		Connection conn = null;

		try
		{
			conn = Connection.create( getConfig() );
			conn.connect();
			check("Unexpected connection endpoint", conn.getConnectionEndpoint() != null);
			conn.disconnect();
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}


	private void test_get_publish_queue_message_count()
		throws Exception
	{
		Log.info("Test getPublishQueueMessageCount()");

		Config config = new Config( getConfig() );  // make a copy!
		config.addValue("gmsec-async-publish", "true");
		config.addValue("gmsec-async-publish-queue-depth", "20");
		config.addValue("gmsec-async-publish-teardown-wait", "2000");

		Connection conn = null;

		try
		{
			conn = Connection.create(config);
			conn.connect();
			setStandardFields(conn.getMessageFactory());

			Message msg = conn.getMessageFactory().createMessage("LOG");
			msg.addField("SUBCLASS", "NO-CLASS");
			msg.addField("OCCURRENCE-TYPE", "FREQUENTLY");
			msg.addField("SEVERITY", (short) 1);

			for (int i = 0; i < 10; ++i)
			{
				conn.publish(msg);
				check("Unexpected message count < 0", conn.getPublishQueueMessageCount() >= 0);
			}

			TimeUtil.millisleep(2000);

			check("Unexpected message count != 0", conn.getPublishQueueMessageCount() == 0);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}
}


class T010_EventCallback extends EventCallback
{
	private boolean m_called;
	private Connection.Event m_event;

	public T010_EventCallback()
	{
		m_called = false;
		m_event  = Connection.Event.ALL_EVENTS;
	}

	public void onEvent(Connection conn, Status status, Connection.Event event)
	{
		m_called = true;
		m_event  = event;
	}

	boolean wasCalled() { return m_called; }

	Connection.Event getEvent() { return m_event; }

	void reset()
	{
		m_called = false;
		m_event  = Connection.Event.ALL_EVENTS;
	}
}


class T010_Callback extends Callback
{
	private boolean m_called;

	public T010_Callback() { m_called = false; }

	public void onMessage(Connection conn, Message msg) { m_called = true; }

	public boolean wasCalled() { return m_called; }

	public void reset() { m_called = false; }
}


class T010_RequestCallback extends Callback
{
	private TestCase m_test;
	private boolean  m_provideResponse;

	public T010_RequestCallback(TestCase test)
	{
		m_test = test;
		m_provideResponse = true;
	}

	public void onMessage(Connection conn, Message msg)
	{
		try
		{
			if (m_provideResponse)
			{
				Message repMsg = conn.getMessageFactory().createMessage("RESP.DIR");
				repMsg.setSubject( m_test.getSubject("RESP.DIR") );

				repMsg.addField("RESPONSE-STATUS", (short) Message.ResponseStatus.SUCCESSFUL_COMPLETION.getValue());

				if (msg.hasField("COMPONENT"))
				{
					repMsg.addField("DESTINATION-COMPONENT", msg.getStringValue("COMPONENT"));
				}
				if (msg.hasField("DESTINATION-COMPONENT"))
				{
					repMsg.addField("COMPONENT", msg.getStringValue("DESTINATION-COMPONENT"));
				}
				else
				{
					repMsg.addField("COMPONENT", "RESPONDER");
				}
				if (msg.hasField("REQUEST-ID"))
				{
					repMsg.addField(msg.getField("REQUEST-ID"));
				}

				conn.reply(msg, repMsg);
			}
			else
			{
				m_provideResponse = true;
			}
		}
		catch (GmsecException e)
		{
			Log.error("Unexpected Exception: " + e.getMessage());
		}
	}

	void provideResponse(boolean flag) { m_provideResponse = flag; }
}


class T010_ReplyCallback extends ReplyCallback
{
	private boolean m_gotReply;
	private boolean m_gotEvent;

	public T010_ReplyCallback()
	{
		m_gotReply = false;
		m_gotEvent = false;
	}

	public void onReply(Connection conn, Message request, Message reply)
	{
		m_gotReply = true;
	}

	public void onEvent(Connection conn, Status status, Connection.Event event)
	{
		m_gotEvent = (event == Connection.Event.REQUEST_TIMEOUT_EVENT);
	}

	public boolean gotReply() { return m_gotReply; }
	public boolean gotEvent() { return m_gotEvent; }
	public void    reset()    { m_gotReply = false; m_gotEvent = false; }
}
