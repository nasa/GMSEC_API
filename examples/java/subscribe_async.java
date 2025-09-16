/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscribe_async.java
 *
 * @brief This example program provides a basic demonstration of subscribing
 * to receive a message from the GMSEC Bus using an asynchronous callback.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;


// Define custom callback to receive messages
//
class MyCallback extends Callback
{
	private boolean gotMsg;

	public MyCallback()
	{
		gotMsg = false;
	}

	public void onMessage(Connection conn, Message msg)
	{
		gotMsg = true;

		Log.info("MyCallback.onMessage -- \n" + msg.toXML());

		// Acknowledge processing the message
		msg.acknowledge();
	}

	public boolean gotMessage()
	{
		return gotMsg;
	}
}


// Define custom callback to receive events
//
// Although not required, an event callback can assist with determining
// whether we have a connection to the GMSEC Bus, or in the case of this
// example program, whether the Auto-Dispatcher thread has terminated
// due to some error.
//
class MyEventCallback extends EventCallback
{
	private boolean error;

	public MyEventCallback()
	{
		error = false;
	}

	public void onEvent(Connection conn, Status status, Connection.Event event)
	{
		// Check event type and/or error status of the Status object.

		if (event == Connection.Event.DISPATCHER_ERROR_EVENT)
		{
			// Auto-dispatcher error has occurred; we are done!
			Log.error("MyEventCallback.onEvent -- " + status.getReason());
			error = true;
		}
		else if (status.hasError())
		{
			// Soft-error has occurred
			Log.warning("MyEventCallback.onEvent -- " + status.getReason());
		}
		else
		{
			// Nominal event has occurred
			Log.info("MyEventCallback.onEvent -- " + status.getReason());
		}
	}

	public boolean haveError()
	{
		return error;
	}
}


public class subscribe_async
{
	public static void main(String[] args)
	{
		// Set up connection configuration options from the command line.
		Config config = new Config(args);

		// Validate incoming messages
		config.addValue("gmsec-msg-content-validate-recv", "true");

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Declared custom callback and custom event callback
		MyCallback      mcb = new MyCallback();
		MyEventCallback ecb = new MyEventCallback();

		// Deduce subscription topic for the example program
		String subscription_topic;
		int level = config.getIntegerValue("gmsec-schema-level", Specification.SchemaLevel.LEVEL_0.getValue());

		switch (Specification.SchemaLevel.valueOf(level))
		{
		case LEVEL_0:
			subscription_topic = "C2MS.>";
			break;

		case LEVEL_1:
			subscription_topic = "C2MS-PIPE.>";
			break;

		case LEVEL_2:
		default:
			subscription_topic = "GMSEC.>";
			break;
		}

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Register custom Event Callback
			conn.registerEventCallback(Connection.Event.ALL_EVENTS, ecb);

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Subscribe to receive message via a callback
			conn.subscribe(subscription_topic, mcb);
			Log.info("Subscribed to topic: " + subscription_topic);

			// Start the auto-dispatcher so we can receive messages via the callback
			conn.startAutoDispatch();

			// Normally an application would have something important to do;
			// here we just idle until a message arrives. Should an error
			// occur, we bail.
			Log.info("Waiting to receive a message...");
			while (!mcb.gotMessage() && !ecb.haveError())
			{
				TimeUtil.millisleep(1000);
			}

			// Disconnect from the GMSEC Bus, and terminate subscriptions.
			conn.disconnect();

			// Destroy the connection instance
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus.
			Log.error("GmsecException: " + e.getMessage());
		}
	}
}
