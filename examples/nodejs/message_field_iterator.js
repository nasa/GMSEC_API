#!/usr/bin/env node


/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * message_field_iterator.js
 *
 * This file contains a simple example demonstrating the use of the
 * MessageFieldIterator class to iterate over the Fields of a Message.
 *
 * This example program is intended to be run prior to executing any example
 * program which publishes a GMSEC message (e.g., the publish example).
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class message_field_iterator
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

			// Deduce subscription topic for the example program
			var subscription_topic;
			const level = config.getIntegerValue("gmsec-schema-level", gmsec.Specification.SchemaLevel_LEVEL_0);

			switch (level)
			{
				case gmsec.Specification.SchemaLevel_LEVEL_0:
					subscription_topic = "C2MS.>";
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_1:
					subscription_topic = "C2MS-PIPE.>";
					break;

				case gmsec.Specification.SchemaLevel_LEVEL_2:
				default:
					subscription_topic = "GMSEC.>";
					break;
				}

			// Create connection instance
			const conn = new gmsec.Connection(config);

			// Establish connection to the GMSEC Bus
			conn.connect();

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("Middleware version = " + conn.getLibraryVersion());

			// Subscribe to receive message
			conn.setupSubscription(subscription_topic);
			gmsec.Log.info("Subscribed to topic: " + subscription_topic);

			// Define timeout period, or use Gmsec.WAIT_FOREVER to block
			// indefinitely to receive a message
			const timeout = 5000;   // 5 seconds

			// Attempt to receive a message
			const msg = conn.receive(timeout);

			if (null != msg)
			{
				// Received a message!
				gmsec.Log.info("Received Message with Subject: " + msg.getSubject());

				// Retrieve field(s) from the message
				//
				// Selector choices include:
				//     gmsec.MessageFieldIterator.Selector_ALL_FIELDS
				//     gmsec.MessageFieldIterator.Selector_HEADER_FIELDS
				//     gmsec.MessageFieldIterator.Selector_NON_HEADER_FIELDS
				//
				const iter = msg.getFieldIterator(gmsec.MessageFieldIterator.Selector_ALL_FIELDS);

				while (iter.hasNext()) {
					const field = iter.next();

					gmsec.Log.info(field.getName() + " (" + this.fieldTypeToString(field.getType()) + "): " + field.getStringValue());

					// Field objects can also be converted to specific Field types prior
					// to retrieval of the value contained in the Field. This is useful
					// for ensuring that data types do not lose any level of precision.
					// 
					// See getFieldValue() for an example of how a Field can be converted to a
					// specialized Field object.
					//gmsec.Log.info(field.getName() + " (" + this.fieldTypeToString(field.getType()) + "): " + this.getFieldValue(field).toString());
				}

				// Acknowledge processing the message
				msg.acknowledge();
			}
			else
			{
				// Time out!
				gmsec.Log.warning("Time out; no message was received");
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

	fieldTypeToString(type)
	{
		switch (type)
		{
		case gmsec.Field.Type_BOOL   : return "BOOL";
		case gmsec.Field.Type_BINARY : return "BIN";
		case gmsec.Field.Type_CHAR   : return "CHAR";
		case gmsec.Field.Type_I16    : return "I16";
		case gmsec.Field.Type_I32    : return "I32";
		case gmsec.Field.Type_I64    : return "I64";
		case gmsec.Field.Type_I8     : return "I8";
		case gmsec.Field.Type_F32    : return "F32";
		case gmsec.Field.Type_F64    : return "F64";
		case gmsec.Field.Type_STRING : return "STRING";
		case gmsec.Field.Type_U16    : return "U16";
		case gmsec.Field.Type_U32    : return "U32";
		case gmsec.Field.Type_U64    : return "U64";
		case gmsec.Field.Type_U8     : return "U8";
		}

		return "UNKNOWN";
	}

	/**
	 * This function is provided as an example of how to retrieve type-specific
	 * values from an individual Field object.
	 */
	getFieldValue(field)
	{
		switch (field.getType())
		{
		case gmsec.Field.Type_BOOL   : return gmsec.Field.toBooleanField(field).getValue();
		case gmsec.Field.Type_BINARY : return gmsec.Field.toBinaryField(field).getValue();
		case gmsec.Field.Type_CHAR   : return gmsec.Field.toCharField(field).getValue();
		case gmsec.Field.Type_I16    : return gmsec.Field.toI16Field(field).getValue();
		case gmsec.Field.Type_I32    : return gmsec.Field.toI32Field(field).getValue();
		case gmsec.Field.Type_I64    : return gmsec.Field.toI64Field(field).getValue();
		case gmsec.Field.Type_I8     : return gmsec.Field.toI8Field(field).getValue();
		case gmsec.Field.Type_F32    : return gmsec.Field.toF32Field(field).getValue();
		case gmsec.Field.Type_F64    : return gmsec.Field.toF64Field(field).getValue();
		case gmsec.Field.Type_STRING : return gmsec.Field.toStringField(field).getValue();
		case gmsec.Field.Type_U16    : return gmsec.Field.toU16Field(field).getValue();
		case gmsec.Field.Type_U32    : return gmsec.Field.toU32Field(field).getValue();
		case gmsec.Field.Type_U64    : return gmsec.Field.toU64Field(field).getValue();
		case gmsec.Field.Type_U8     : return gmsec.Field.toU8Field(field).getValue();
		}
	}
}


// Instantiate the message_field_iterator example, and run it.
const example = new message_field_iterator();
example.run();
