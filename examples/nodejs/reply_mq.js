#!/usr/bin/env node


/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * reply_mq.js
 *
 * This example program provides a basic demonstration of synchronously
 * receiving a Request Directive message from the GMSEC Bus using an Apache
 * Artemis Message Queue, and then issuing a Response Directive message
 * (should one be requested).
 *
 * This example is intended to be used with the request example program.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class reply_mq
{
	run()
	{
		try
		{
			// Set up connection configuration options provided by the user
			const config = utility.createConfig(process.argv);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Validate messages before being sent to the bus
			config.addValue("gmsec-msg-content-validate-recv", "true");

			// Check user has specified an mw-id of artemis
			if (config.getValue("mw-id", "") != "artemis")
			{
				gmsec.Log.error("This example program only works with Apache Artemis; use mw-id=artemis and run again");
				return;
			}

			// Define subscription topic for receiving request message(s).
			// The topic includes the syntax for a Fully Qualified Queue Name (FQQN).
			var subscription_topic;
			const message_queue = "MY-MSG-QUEUE";
			const level = config.getIntegerValue("gmsec-schema-level", gmsec.Specification.SchemaLevel_LEVEL_0);

			switch (level)
			{
				case gmsec.Specification.SchemaLevel_LEVEL_0:
					subscription_topic = "C2MS.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY" + "::" + message_queue;
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_1:
					subscription_topic = "C2MS-PIPE.*.*.MY-MISSION.*.MY-SAT-ID.REQ.DIR.REPLY" + "::" + message_queue;
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_2:
				default:
					subscription_topic = "GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.REPLY" + "::" + message_queue;
					break;
				}

			// Create connection instance
			const conn = new gmsec.Connection(config);

			// Establish connection to the GMSEC Bus
			conn.connect();

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("Middleware version = " + conn.getLibraryVersion());

			// Set up standard fields within the MessageFactory associated
			// with the connection object
			utility.setStandardFields(conn.getMessageFactory(), this.constructor.name);

			// Subscribe to receive message
			conn.setupSubscription(subscription_topic);
			gmsec.Log.info("Subscribed to topic: " + subscription_topic);

			// Timeout was arbitrarily chosen as Gmsec.WAIT_FOREVER so that
			// we block indefinitely waiting to receive a request message.
			// It can be set to other time values (e.g. 1000 is 1 second) so
			// the application does not block indefinitely waiting for a
			// request message.
			const timeout = gmsec.GMSEC_WAIT_FOREVER;

			gmsec.Log.info("Responder is waiting to receive message(s)...");
			gmsec.Log.info("Consider running a second instance of the example program");
			gmsec.Log.info("Use the 'request' example program to send a message");

			// Block for a Request Directive message to arrive.
			const reqMsg = conn.receive(timeout);

			// Check if we have received a message.
			if (null != reqMsg)
			{
				// Output in XML the received message.
				gmsec.Log.info("Received Request Message:\n" + reqMsg.toXML());

				// Check if a response message should be returned.
				if (reqMsg.hasField("RESPONSE") && reqMsg.getBooleanValue("RESPONSE"))
				{
					// Create a Response Directive message using the MessageFactory.
					const rspMsg = conn.getMessageFactory().createMessage("RESP.DIR");

					// Insert required and optional fields into the Response Directive
					// using values derived from the incoming Request Directive.
					if (reqMsg.hasField("COMPONENT"))
					{
						rspMsg.addField(new gmsec.StringField("DESTINATION-COMPONENT", reqMsg.getStringValue("COMPONENT")));
					}
					if (reqMsg.hasField("REQUEST-ID"))
					{
						rspMsg.addField(reqMsg.getField("REQUEST-ID"));
					}

					// Set the response status for the request.
					rspMsg.setFieldValue("RESPONSE-STATUS", gmsec.Message.ResponseStatus_SUCCESSFUL_COMPLETION);

					// Send the Response Directive message.
					conn.reply(reqMsg, rspMsg);

					// Output in XML the message that has been sent.
					gmsec.Log.info("Sent Response Message:\n" + rspMsg.toXML());
				}

				// Acknowledge processing the request message
				reqMsg.acknowledge();
			}
			else
			{
				// This else-block is reached if a request message is not received.
				// When the timeout period is set to gmsec.GMSEC_WAIT_FOREVER, then
				// this block will never be reached.
				gmsec.Log.warning("Time out; no Request Directive message received");
			}

			// Disconnect from the GMSEC Bus
			conn.disconnect();
		}
		catch (e)
		{
			// If here, the most likely case is the middleware could not be
			// deduced, or a failure occurred when attempting to connect to
			// the GMSEC Bus
			gmsec.Log.error("Exception: " + e.message);
		}
	}
}


// Instantiate the reply_mq example, and run it.
const example = new reply_mq();
example.run();
