#!/usr/bin/env node


/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * specification_addendum.js
 *
 * This file contains an example demonstrating the extensibility of the GMSEC
 * API Message validation engine through use of additional XML template files
 * represending an additional layer of Message definitions to be validated.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class specification_addendum
{
	run()
	{
		var conn = null;

		try
		{
			// Set up connection configuration options provided by the user
			const config = utility.createConfig(process.argv);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Enable message valiation; this setting is 'false' by default
			config.addValue("gmsec-msg-content-validate-send", "true");

			// Tell the API that there is an additional layer of message schema to
			// validate (The 'EXAMPLE' message definitions). By default, this value
			// is set to 0 (only 'C2MS' specification validation).
			//
			// Note: These levels for validation are determined by the "LEVEL-X"
			// attributes defined in the DIRECTORY.xml file contained in the XML
			// templates directory. In the case of this example, Level-0 implies
			// C2MS, and Level-1 is our example extension.
			//
			// Note: The GMSEC team envisions using message specifications in a
			// layered hierarchical fashion. For example, the "C2MS" message
			// specification would be 'layer 0', followed by an organization-level
			// message specification at 'layer 1' which builds upon the message
			// specification outlined in C2MS. This would be followed by a mission
			// level message specification at 'layer 2' (e.g. GMSEC) and so on.
			config.addValue("gmsec-schema-level", "1");

			// Tell the API where to find all of the schema files.
			//
			// Note: This example only demonstrates a simple usage case of this
			// functionality. When using this functionality, if the intent is
			// to use any of the GMSEC message definitions, then ALL of the XML
			// template files must be contained in the same directory.
			// e.g. GMSEC_API/templates/2019.00 (or the location defined in
			// GMSEC-SCHEMA-PATH)
			config.addValue("gmsec-schema-path", "templates");

			// Since this example relies on the 2019.00 version of the templates,
			// we indicate such within the configuration object.
			config.addValue("gmsec-specification-version", "201900");

			// Create a Connection instance
			conn = gmsec.Connection.create(config);

			// Establish connection to the GMSEC Bus
			conn.connect();

			// Output information regarding the API version and connection
			gmsec.Log.info(gmsec.Connection.getAPIVersion());
			gmsec.Log.info("Middleware version = " + conn.getLibraryVersion());

			// Set up standard fields within the MessageFactory associated
			// with the connection object
			utility.setStandardFields(conn.getMessageFactory(), this.constructor.name);

			// Create a complete Log message with the required fields as defined in our
			// addendum message specification
			const logMsg = conn.getMessageFactory().createMessage("LOG");
			logMsg.addField(new gmsec.U16Field("NUM-OF-EVENTS", 2));
			logMsg.addField(new gmsec.StringField("EVENT.1.DESC", this.addTimeToString("AOS occurred at: ")));
			logMsg.addField(new gmsec.StringField("EVENT.2.DESC", this.addTimeToString("Telemetry downlink began at: ")));

			// Publish our valid Log message
			conn.publish(logMsg);

			gmsec.Log.info("Published custom LOG message:\n" + logMsg.toXML());


			// Create an incomplete Log message without the required fields
			const badLogMsg = conn.getMessageFactory().createMessage("LOG");

			try {
				// Attempt to publish the message
				conn.publish(badLogMsg);
				gmsec.Log.warning("Surpringly was able to publish an invalid LOG message:\n" + badLogMsg.toXML());
			}
			catch (e) {
				gmsec.Log.info("This validation error is expected for the incomplete LOG message:\n" + e.message);
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
		finally
		{
			// Destroy the connection
			gmsec.Connection.destroy(conn);
		}
	}

	addTimeToString(preface)
	{
		return preface + gmsec.TimeUtil.formatTime(gmsec.TimeUtil.getCurrentTime());
	}
}


// Instantiate the specification_addendum example, and run it.
const example = new specification_addendum();
example.run();
