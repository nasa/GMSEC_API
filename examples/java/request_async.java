/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file request_async.java
 *
 * @brief This example program provides a basic demonstration of issuing
 * an asynchronous Request Directive message to the GMSEC Bus, and then
 * handling the receipt of a Response Directive message(s) via a
 * ReplyCallback.
 */

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;
import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.List;


public class request_async
{
	private static class MyReplyCallback extends ReplyCallback
	{
		private boolean responseComplete;

		public MyReplyCallback()
		{
			responseComplete = false;
		}

		public void onReply(Connection conn, Message reqMsg, Message repMsg)
		{
			Log.info("MyReplyCallback onReply:\n" + repMsg.toXML());

			if (repMsg.hasField("RESPONSE-STATUS"))
			{
				try {
					int status = repMsg.getI32Value("RESPONSE-STATUS");

					switch (Message.ResponseStatus.valueOf(status))
					{
					case ACKNOWLEDGEMENT:
					case WORKING_KEEP_ALIVE:
						responseComplete = false;
						break;

					case SUCCESSFUL_COMPLETION:
					case FAILED_COMPLETION:
					case INVALID_REQUEST:
					case FINAL_MESSAGE:
						responseComplete = true;
						break;
					}
				}
				catch (GmsecException e) {
					Log.error("Unexpected error: " + e.getMessage());
				}
			}
			else
			{
				responseComplete = true;
			}

			// Acknowledge processing the response message
			repMsg.acknowledge();
		}

		public void onEvent(Connection conn, Status status, Connection.Event event)
		{
			if (status.hasError())
			{
				Log.error("MyReplyCallback onEvent: " + status.get() + ", event: " + event);
			}
			else
			{
				Log.info("MyReplyCallback onEvent: " + status.get() + ", event: " + event);
			}
		}

		public boolean isResponseComplete()
		{
			return responseComplete;
		}
	}


	public static void main(String[] args)
	{
		// Set up connection configuration options, some from the command line,
		// others hard-coded.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Validate messages before being sent to and after being received from the bus
		config.addValue("gmsec-msg-content-validate-all", "true");

		// IMPORTANT! This config option must be set to receive multiple response messages
		config.addValue("mw-multi-resp", "true");

		// Define instance of MyReplyCallback
		MyReplyCallback rcb = new MyReplyCallback();

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Set up standard fields within the MessageFactory associated
			// with the connection object.
			setStandardFields(conn.getMessageFactory(), "REQUEST-ASYNC");

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Create a Request Directive message using the MessageFactory.
			// Set both required and optional fields within the message.
			Message reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			reqMsg.addField(new StringField("DESTINATION-COMPONENT", "REPLY-MULTI", true));   // to whom is the request intended for
			reqMsg.addField("DIRECTIVE-STRING", "Turn on the lights");                        // operation to perform
			reqMsg.addField("RESPONSE", true);                                                // a response message is requested
			reqMsg.addField("REQUEST-ID", new U16(0));

			// Timeout and republish periods have been arbitrarily chosen to be 5 seconds.
			// The republish period could also be set to GMSEC_REQUEST_REPUBLISH_NEVER to
			// prevent re-sending request message should a timeout occur.
			int timeout   = 5000;
			int republish = 5000;

			// Issue Request Directive.
			// Note, this is a non-blocking call (i.e. an asynchronous operation).
			conn.request(reqMsg, timeout, rcb, republish);

			// Output in XML what we have sent
			Log.info("Sending Request Directive Message:\n" + reqMsg.toXML());
			Log.info("Use reply_multi example program to issue response messages");

			// Normally an application would continue with other operation, however for
			// demonstration purposes, we will loop until it has been verified that a
			// response message has been received.
			while (!rcb.isResponseComplete())
			{
				Log.info("Waiting to receive response message...");

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


	// Convenience function for associating standard (i.e. common) fields
	// with the MessageFactory which will be included automatically within
	// all messages created using the factory.
	private static void setStandardFields(MessageFactory factory, String component)
	{
		List<Field> standardFields = new ArrayList<Field>();

		standardFields.add(new StringField("MISSION-ID", "MY-MISSION", true));
		standardFields.add(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true));
		standardFields.add(new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true));
		standardFields.add(new StringField("CONSTELLATION-ID", "MY-CNST", true));
		standardFields.add(new StringField("FACILITY", "MY-FACILITY", true));
		standardFields.add(new StringField("DOMAIN1", "MY-DOMAIN-1", true));
		standardFields.add(new StringField("DOMAIN2", "MY-DOMAIN-2", true));
		standardFields.add(new StringField("COMPONENT", component, true));

		factory.setStandardFields(standardFields);
	}
}
