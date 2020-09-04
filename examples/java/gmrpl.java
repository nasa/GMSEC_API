/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


 
/** @file gmrpl.java
 *
 * An example replier using the Java GMSEC API.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;

import java.util.ArrayList;


public class gmrpl implements Example
{
	static final String DEFAULT_SUBJECT = "GMSEC.>";

	Config     config;
	Connection connection;
	ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();


	gmrpl(String[] args) throws ExampleException
	{
		config = new Config(args);

		Util.initialize(config);

		Util.addToConfigFromFile(config);

		if (!Util.hasConnectionType(config))
		{
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}


	private void printUsage()
	{
		System.err.println("\nusage: java gmrpl connectionType=<middleware> " +
		                   "[ <parameter>=<value> ]\n" +
		                   "\n\tNote that the parameter 'connectiontype' is required. "  +
		                   "\n\tThe rest of other parameters are optional.\n" +
		                   "\n\tserver=<server name> " +
		                   "(required if middleware is not bolt/MB locally)" +
		                   "\n\tsubject=<subject name>" +
		                   "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
		                   "\n\tprog_timeout_s=<timeout period (seconds)>" +
		                   "\n\t\tNote: msg_timeout_ms must be specified for prog_timeout_s" +
		                   "\n\tloglevel=<log level>" +
		                   "\n\tcfgfile=<config_filepath.xml>" +
		                   "\n\nFor more information, see API User's Guide\n\n");
	}


	public boolean run()
	{
		boolean result = true;

		String subject        = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
		int    msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS",  gmsecAPI.WAIT_FOREVER);
		int    prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S",  gmsecAPI.WAIT_FOREVER);

        if (msg_timeout_ms == gmsecAPI.WAIT_FOREVER && prog_timeout_s != gmsecAPI.WAIT_FOREVER)
        {
           Log.error("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
           return false;
        }  	

		try
		{
			connection = Util.openConnection(config);

			Log.info("Subscribing to " + subject);
			info.add(connection.subscribe(subject));

			Log.info("Subscribing to GMSEC.TERMINATE");
			info.add(connection.subscribe("GMSEC.TERMINATE"));

			boolean done        = false;
			long    prevTime    = System.currentTimeMillis();
			long    nextTime    = 0;
			long    elapsedTime = 0;	//ms

			while (!done)
			{
				if (elapsedTime >= (prog_timeout_s * 1000) && prog_timeout_s != gmsecAPI.WAIT_FOREVER)
				{
					done = true;
					Log.info("Program Timeout Reached");
					continue;
				}
			
				Message message = connection.receive(msg_timeout_ms);
			
				nextTime = System.currentTimeMillis();
				elapsedTime += (int) (nextTime - prevTime);
				prevTime = nextTime;

				if (message == null)
				{
					Log.warning("timeout occurred");
					continue;
				}
			
				// Display XML representation of the received message
				Log.info("Received Message:\n" + message.toXML());

				// Check the message subject to determine if we're done
				if (message.getSubject().equals("GMSEC.TERMINATE"))
				{
					done = true;
				}

				if (message.getKind() == Message.MessageKind.REQUEST)
				{
					String component = message.getStringField("COMPONENT").getValue();

					// Set Status Code.  See API Interface Specification for available status codes.
					String replyStatus = "1";
					String replySubject = "GMSEC.MISSION.SAT_ID.RESP." + component + "." + replyStatus;

					// Create Reply
					Message reply = new Message(replySubject, Message.MessageKind.REPLY);

					// Add fields to reply
					reply.addField("ANSWER", "Sure looks like it!");
					reply.addField("COMPONENT", "GMRPL");

					// Send Reply
					connection.reply(message, reply);

					// Display XML representation of the reply message
					Log.info("Send Reply Message:\n" + reply.toXML());
				}
			}
		}
		catch (GMSEC_Exception e)
		{
			Log.error("GMSEC_Exception: " + e.toString());
			result = false;
		}

		try
		{
			cleanup();
		}
		catch (GMSEC_Exception e)
		{
			Log.error("GMSEC_Exception: " + e.toString());
		}

		return result;
	}
		

	public boolean cleanup() throws GMSEC_Exception
	{
		for(int i = info.size()-1; i >= 0; i-- )
		{
			Log.info("Unsubscribing from " + info.get(i).getSubject());
			connection.unsubscribe(info.get(i));
			info.remove(i);
		}

		if (connection != null)
		{
			Util.closeConnection(connection);
		}

		return false;
	}


	public static void main(String[] args)
	{
		try
		{
			new gmrpl(args).run();
		}
		catch (ExampleException e)
		{
			System.err.println(e.toString());
		}
	}
}


