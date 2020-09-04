/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmrpl_cb.java
 *
 * An example replier with callback using the Java GMSEC API.
 */


import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.*;

import java.util.ArrayList;


class MyReqReplyCallback extends Callback
{
	public void onMessage(Connection conn, Message msg) 
	{
		if (msg.getKind() != Message.MessageKind.REQUEST)
		{
			Log.warning("Received an unexpected message type");
			return;
		}

		try {
			// Display message
			Log.info("onMessage => Received:\n" + msg.toXML());

			// Construct Reply subject.
			String component = msg.getStringField("COMPONENT").getValue();

			// Set Status Code.  
			// See API Interface Specification for available status codes.
			String statusCode = "1";

			StringBuilder subject = new StringBuilder();
			subject.append("GMSEC.MISSION.SAT_ID.RESP.").append(component).append(".").append(statusCode);

			// Create Reply
			Message reply = new Message(subject.toString(), Message.MessageKind.REPLY);

			// Add Fields to replay
			reply.addField("ANSWER",    "Sure looks like it!");
			reply.addField("COMPONENT", "GMRPL");

			// Display reply message
			Log.info("Prepared Reply Message:\n" + reply.toXML());

			// Issue Reply
			conn.reply(msg, reply);

			// Do not destroy the message here
		}
		catch (GMSEC_Exception e) {
			Log.error("onMessage => " + e.toString());
		}
	}
}


public class gmrpl_cb implements Example
{
	static final String DEFAULT_SUBJECT = "GMSEC.>";

	Config     config;
	Connection connection;
	ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();

	gmrpl_cb(Config config) throws ExampleException
	{
		this.config = config;
		Util.initialize(config);

		if (!Util.hasConnectionType(config))
		{
			printUsage();
			throw new ExampleException("Invalid option");
		}
	} 


	private void printUsage()
	{
		Log.error("\nusage: java gmrpl_cb connectionType=<middleware> " +
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


	public boolean run() throws GMSEC_Exception
	{
		String subject        = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
		int    msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS", gmsecAPI.WAIT_FOREVER);
		int    prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S", gmsecAPI.WAIT_FOREVER);

		if (msg_timeout_ms == gmsecAPI.WAIT_FOREVER && prog_timeout_s != gmsecAPI.WAIT_FOREVER)
		{
			Log.error("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
			return false;
		}

		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		connection = Util.openConnection(config);

		// Output middleware version
		Log.info("Middleware version = " + connection.getLibraryVersion());

		// Subscribe
		Log.info("Subscribing to " + subject);
		info.add(connection.subscribe(subject, new MyReqReplyCallback()));

		Log.info("Subscribing to GMSEC.TERMINATE");
		info.add(connection.subscribe("GMSEC.TERMINATE", new MyReqReplyCallback()));


		// Listen
    
		boolean done = false;
		long    prevTime = System.currentTimeMillis();
		long    nextTime = 0;
		int    elapsedTime = 0;	//ms

		while (!done) 
		{
			if (elapsedTime >= (prog_timeout_s * 1000) && prog_timeout_s != gmsecAPI.WAIT_FOREVER)
			{
				done = true;
				Log.info("Program Timeout Reached");
				continue;
			}
    
			Message msg = connection.receive(msg_timeout_ms);

			nextTime     = System.currentTimeMillis();
			elapsedTime += (int) (nextTime - prevTime);
			prevTime     = nextTime;
  
			if (msg == null)
			{
				// This will never print if the parameter is not provided (default 
				// msg_timeout_ms causes GetNextMsg to wait forever)
				Log.info("timeout occurred");
      		}
			else
			{
				done = msg.getSubject().equals("GMSEC.TERMINATE");

				connection.dispatch(msg);
			}
		}

		return true;
	}


	public boolean cleanup() throws GMSEC_Exception
	{
		if (connection != null)
		{
			for(int i = info.size()-1; i >= 0; i-- )
			{
				Log.info("Unsubscribing from " + info.get(i).getSubject());
				connection.unsubscribe(info.get(i));
				info.remove(i);
			}
			Util.closeConnection(connection);
		}

		return true;
	}


	public static void main(String args[]) 
	{
		Example example = null;

		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example =  new gmrpl_cb(config);
			example.run();
		}
		catch (GMSEC_Exception e) {
			Log.error(e.toString());
		}
		catch (ExampleException e) {
			Log.error(e.toString());
		}
		finally {
			if (example != null )
				Util.cleanup(example);
		}
	}
}
