/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


 
/**
 * @file Requestor.java
 *
 * A simple examples of various GMSEC API functionality.
 *
 * Contained within this example:
 *
 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Establish a connection
 * - Using the XML file to store and set various other program configurations
 * - Creating a heartbeat message from an XML definition
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 * Creating a directive request message from an XML definition
 * Publishing the directive request message using the Request call
 * Using a callback class object to receieve a response from the request call
 * - Creating a log message from an XML definition
 * - Programatically altering a field in the log message
 * - Publishing the log message using the Publish call
 * Cleanup
 * - Message memory cleanup
 * - Connection cleanup including Disconnecting from the network
 *
 * Any item with a * next to it is specific to this file in the overall example.
 */   

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;

import java.text.SimpleDateFormat;
import java.util.Date;


// Callback Class for Request-Reply
class RequestorReplyCallback extends ReplyCallback
{
	// Holds the config file used to create new messages
	private ConfigFile configFile;


	public RequestorReplyCallback(ConfigFile configFile)
	{
		this.configFile = configFile;
	}
 

	public void onReply(Connection conn, Message request, Message reply)
	{
		String now = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());

		try
		{
			// Get the status of the response (successful, failed, ect) and act accordingly
			short  status = reply.getI16Field("RESPONSE-STATUS").getValue();
			String data   = "";

			switch (status)
			{
			case 1 :
				Log.info("Acknowledgement");
				break;
			case 2 :
				Log.info("Working/keep alive");
				break;
			case 3 :
				// Obtain the DATA field which is included within the reply message
				data = reply.getStringField("DATA").getValue();
				Log.info("Received Reply (" + data + ")");
				break;
			case 4 :
				Log.info("Failed completion");
				break;
			case 5 :
				Log.info("Invalid Request");
				break;
			default :
				break;
			}

			// Lookup and load the config file log message definition
			Message logMessage = configFile.lookupMessage("LOG-REQ");

    		// Fill the log message time and text
			logMessage.addField("EVENT-TIME", now);
			logMessage.addField("MSG-TEXT", data);

			// Publish the log message
			conn.publish(logMessage);
		}
		catch (GMSEC_Exception e)
		{
			Log.error("GMSEC_Exception: " + e.toString());
		}
	}


	public void onEvent(Connection conn, Status status, Connection.ConnectionEvent event)
	{
		Log.warning("[RequestorReplyCallback.onEvent] : " + status.get());
	}
};


public class Requestor implements Example
{
	Config     config;
	ConfigFile configFile;
	Connection conn;


	Requestor(Config config, ConfigFile configFile)
	{
		this.config     = config;
		this.configFile = configFile;
		this.conn       = null;

		Util.initialize(config);
	}


	public boolean run()
	{
		boolean result = true;

		try
		{
			// Establish a connection
			conn = Util.openConnection(config);

			// output GMSEC API and Middleware info, just for display purposes
			Log.info(Connection.getAPIVersion());
			Log.info(conn.getLibraryVersion());

			// Look up additional program configutation information
			Config progConfig = configFile.lookupConfig("program-config");

			// Get program config values
			short updateRate      = Short.parseShort(Util.get(progConfig, "update-rate"));
			short loopCountdown   = Short.parseShort(Util.get(progConfig, "loop-time"));
			short directiveHolder = Short.parseShort(Util.get(progConfig, "send-directive-rate"));

			// Lookup and load the config file heartbeat message definition
			Message heartbeatMessage = configFile.lookupMessage("C2CX-HEARTBEAT-REQ");

			// Obtain the publish rate field, by name, from the heartbeat message
			short heartbeatHolder = 30;

			try {
				heartbeatHolder = heartbeatMessage.getI16Field("PUB-RATE").getValue();
			}
			catch (GMSEC_Exception e) {
				Log.warning("PUB-RATE not found in C2CX-HEARTBEAT-REQ message; using default rate of 30");
			}

			// Find and load the config file Directive Request Message definition
			Message directiveRequestMessage = configFile.lookupMessage("DIRECTIVE-REQUEST");

			// Main Publishing Loop

			// Output some general program information
			Log.info("Publishing for " + loopCountdown + " seconds.");
			Log.info("Publishing Directive Request Messages every " + directiveHolder + " seconds.");
			Log.info("Publishing Heartbeat Messages every " + heartbeatHolder + " seconds.");

			// Start time
			String startTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
			Log.info("Start Time: " + startTime);

			// Publishing loop
			short directiveCountdown = 0;    
			short heartbeatCountdown = 0;    
			short requestCount = 0;
			short heartbeatCount = 0;

			for (; loopCountdown > 0; --loopCountdown)
			{
				// When the countdown reaches 0, we publish a directive message and
				//  reset the counter
				if (directiveCountdown < 1)
				{
					++requestCount;

					directiveRequestMessage.addField("DIRECTIVE-KEYWORD", Short.toString(requestCount));
					directiveRequestMessage.addField("DIRECTIVE-STRING" , "DIRECTIVE REQUEST MESSAGE " + requestCount);

					Log.info("Publishing Directive Request Message (" + requestCount + ")");

					// Issue the request and attempt to receive the reply using our reply-callback
					conn.request(directiveRequestMessage, gmsecAPI.WAIT_FOREVER, new RequestorReplyCallback(configFile), 0);

					// Reset the countdown
					directiveCountdown = directiveHolder;
				}

				// When the countdown reaches 0, we publish a heartbeat message and
				//  reset the counter
				if (heartbeatCountdown < 1)
				{
					++heartbeatCount;

					// Update the message counter field
					heartbeatMessage.addField("COUNTER", heartbeatCount);

					// Publish the heartbeat message
					conn.publish(heartbeatMessage);

					// Ouput a heartbeat marker and reset the counter
					Log.info("Published heartbeat...");

					heartbeatCountdown = heartbeatHolder;
				}

				// Decrement the counters
				heartbeatCountdown -= updateRate;
				directiveCountdown -= updateRate;

				// Sleep for 1 second
				Util.millisleep(1000);
			}

			// End time
			String endTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
			Log.info("End Time: " + endTime);
		}
		catch (GMSEC_Exception e)
		{
			Log.error("GMSEC_Exception: " + e.toString());
			result = false;
		}

		return result;
	}


	public boolean cleanup()
	{
		if (conn != null)
		{
			Util.closeConnection(conn);
		}

		return true;
	}


	public static void main(String args[])
	{
		// Simple check for proper usage
		if (args.length != 1)
		{
			System.err.println("\nUsage: java Requestor <XML config filename>\n\n");
			System.exit(-1);
		}

		try
		{
			// Create the config file and associate the config filename
			ConfigFile configFile = new ConfigFile();

			configFile.load(args[0]);

			// Look up the config file value that contains the configuration for the middleware
			Config config = configFile.lookupConfig("connection-config");

			new Requestor(config, configFile).run();
		}
		catch (GMSEC_Exception e)
		{
			System.err.println("GMSEC_Exception: " + e.toString());
		}
	}
}
