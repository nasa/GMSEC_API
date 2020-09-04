/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file Replier.java
 *
 *  A simple examples of various GMSEC API functionality.
 *
 * Contained within this example:
 *
 * - Loading an XML configuration file
 * - Populating a Config object using the XML file
 * - Establish a connection
 * - Using the XML file to store and set various other program configurations
 * - Subscribing to specific subjects to receieve messages through a callback
 * - Creating a heartbeat message from an XML definition
 * - Starting the automatic message dispatching thread
 * - Publishing the heartbeat message using the Publish call
 * - Programatically altering a field in the heartbeat message
 * Creating a response message from an XML definition upon receiving a request
 * Programatically altering a field in the response message
 * Using the Reply call to send a reply message
 * - Creating a log message from an XML definition
 * - Programatically altering a field in the log message
 * - Publishing the log message using the Publish call
 * Cleanup
 * - Message memory cleanup
 * - Connection cleanup disconnectiong from the network
 *
 * Any item with a * next to it is specific to this file in the overall example.
 */


import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.field.I16Field;
import gov.nasa.gsfc.gmsec.api.util.Log;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;


class ReplyInnerExample implements Example
{
	ConfigFile configFile;
	Connection connection;
	Message    message;


	ReplyInnerExample(ConfigFile configFile, Connection connection, Message message)
	{
		this.configFile = configFile;
		this.connection = connection;
		this.message    = message;
	}


	public boolean run()
	{
		boolean result = true;
		String  now    = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());

		try
		{
			String dirKey = message.getStringField("DIRECTIVE-KEYWORD").getValue();

			Log.info("Received Request: " + dirKey);

			// Lookup and load the config file directive response message definition
			Message tmpMsg = configFile.lookupMessage("DIRECTIVE-RESPONSE");

			// Replace the DATA and TIME-COMPLETE fields within the temporary response message
			tmpMsg.addField("DATA", dirKey);
			tmpMsg.addField("TIME-COMPLETED", now);

			// Create the actual reply message
			Message replyMessage = new Message(message.getSubject(), Message.MessageKind.REPLY);

			// Copy the fields from the temporary response message to our reply message
			tmpMsg.copyFields(replyMessage);

			// Send the reply
			connection.reply(message, replyMessage);

			Log.info("Reply sent.");

			// Lookup and load the config file log message definition
			Message logMessage = configFile.lookupMessage("LOG-REP");

			// Fill the log message time and text
			logMessage.addField("EVENT-TIME", now);
			logMessage.addField("MSG-TEXT", dirKey);

			// Publish the log message
			connection.publish(logMessage);
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
		return true;
	}
}


// Callback Class for Request-Reply
class RequestReplyCallback extends Callback
{
	private ConfigFile configFile;


	public RequestReplyCallback(ConfigFile cfgFile)
	{
		configFile = cfgFile;
	}


	public void onMessage(Connection conn, Message msg)
	{
		// We don't care about anything but request messages
		if (msg.getKind() != Message.MessageKind.REQUEST)
		{
			return;
		}

		new ReplyInnerExample(configFile, conn, msg).run();
	}
}


public class Replier implements Example
{
	Config     config;
	ConfigFile configFile;
	Connection conn;
	ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();


	Replier(Config config, ConfigFile configFile)
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
			// Establish a connection object off of the configuration
			conn = Util.openConnection(config); 

			// output GMSEC API and Middleware info, just for display purposes
			Log.info(Connection.getAPIVersion());
			Log.info(conn.getLibraryVersion());

			// Look up additional program configutation information
			Config progConfig = configFile.lookupConfig("program-config");

			// Get program config values
			short updateRate    = Short.parseShort(Util.get(progConfig, "update-rate"));
			short loopCountdown = Short.parseShort(Util.get(progConfig, "loop-time"));

			// Create subscriptions from subscription templates in the config file
			String subject = configFile.lookupSubscription("DIRECTIVE-REQUEST");

			// Subscribe
			info.add(conn.subscribe(subject, new RequestReplyCallback(configFile)));

			// Find and load the config file heartbeat message definition
			Message heartbeatMessage = configFile.lookupMessage("C2CX-HEARTBEAT-REP");

			// Obtain the publish rate field, by name, from the heartbeat message
			short heartbeatHolder = 30;

			try {
				heartbeatHolder = heartbeatMessage.getI16Field("PUB-RATE").getValue();
			}
			catch (GMSEC_Exception e) {
				Log.warning("PUB-RATE not found in C2CX-HEARTBEAT-REP message; using default rate of 30");
			}

			// Main Publishing Loop

			// Output some general program information
			Log.info("Publishing for " + loopCountdown + " seconds.");
			Log.info("Publishing Heartbeat Messages every " + heartbeatHolder + " seconds.");

			// Call to create the thread that will automatically retrieve messages 
			// off the message queue
			conn.startAutoDispatch();

			// Start time
			String startTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
			Log.info("Start Time: " + startTime);

			// Publishing loop
			short heartbeatCount = 0;
			short heartbeatCountdown = 0;

			for (;loopCountdown > 0; --loopCountdown)
			{
				// When the countdown reaches 0, we publish a heartbeat message and
				//  reset the counter
				if (heartbeatCountdown < 1)
				{
					++heartbeatCount;

					// Update the message counter
					heartbeatMessage.addField(new I16Field("COUNTER", heartbeatCount));

					// Publish the heartbeat message
					conn.publish(heartbeatMessage);

					// Ouput a heartbeat marker and reset the counter
					Log.info("Published heartbeat...");

					heartbeatCountdown = heartbeatHolder;
				}

				// Decrement the counters
				heartbeatCountdown = (short)(heartbeatCountdown - updateRate);

				// Sleep for 1 second
				Util.millisleep(1000);
			}

 			// End time
			 String endTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
			 Log.info("End Time: " + endTime);
  
			// Stop auto dispatcher
			conn.stopAutoDispatch();
		}
		catch (GMSEC_Exception e)
		{
			Log.error("GMSEC_Exception: " + e.toString());
			result = false;
		}
		finally
		{
			try
			{
				cleanup();
			}
			catch (GMSEC_Exception e)
			{
				Log.error("GMSEC_Exception: " + e.toString());
				result = false;
			}
		}
  
		return result;
 	}


	public boolean cleanup() throws GMSEC_Exception
	{
		if (conn != null)
		{
			for(int i = info.size()-1; i >= 0; i-- )
			{
				Log.info("Unsubscribing from " + info.get(i).getSubject());
				conn.unsubscribe(info.get(i));
				info.remove(i);
			}
			Util.closeConnection(conn);
		}

		return true;
	}


	public static void main(String args[])
	{
		// Simple check for proper usage
		if (args.length != 1)
		{
			System.err.println("\nUsage:  java Replier <XML config filename>\n\n");
			System.exit(-1);
		}

		try
		{
			// Create the config file and associate the config filename
			ConfigFile configFile = new ConfigFile();

			configFile.load(args[0]);

			// Look up the config file value that contains the configuration for the middleware
			Config config = configFile.lookupConfig("connection-config");

			new Replier(config, configFile).run();
		}
		catch (GMSEC_Exception e)
		{
			System.err.println("GMSEC_Exception: " + e.toString());
			System.exit(-1);
		}
	}
}
