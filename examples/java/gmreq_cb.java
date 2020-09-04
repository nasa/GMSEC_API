/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmreq_cb.java
 *
 * An example requestor with calling back using the Java GMSEC API.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.*;
import java.io.*;


class MyReplyCallback extends ReplyCallback 
{
	public void onReply(Connection conn, Message request, Message reply) 
	{
		// Display message
		Log.info("onReply => Received:\n" + reply.toXML());

		gmreq_cb.gReplyCallbackFlag = 1;
		// Do not destroy the message here
	}

	public void onEvent(Connection conn, Status status, Connection.ConnectionEvent event)
	{
		Log.warning("OnError =>\n" + status.get());
	}
};


public class gmreq_cb implements Example
{
	public static int gReplyCallbackFlag = 0;
	static final String DEFAULT_SUBJECT = "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL";

	Config     config;
	Connection connection;

	gmreq_cb(Config config) throws ExampleException
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
		Log.error("\nusage: java gmreq_cb connectionType=<middleware> " +
		          "[ <parameter>=<value> ]\n" +
		          "\n\tNote that the parameter 'connectiontype' is required. "  +
		          "\n\tThe rest of other parameters are optional.\n" +
		          "\n\tserver=<server name> " +
		          "(required if middleware is not bolt/MB locally)" +
		          "\n\tsubject=<subject name>" +
		          "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
		          "\n\tloglevel=<log level>" +
		          "\n\tcfgfile=<config_filepath.xml>" +
		          "\n\nFor more information, see API User's Guide\n\n");
	}


	public boolean run() throws GMSEC_Exception
	{
		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		String subject        = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
		int    msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS", gmsecAPI.WAIT_FOREVER);

		connection = Util.openConnection(config);
 
		// Output middleware version
		Log.info("Middleware version = " + connection.getLibraryVersion());

		// Request - Reply
		Message request = new Message(subject, Message.MessageKind.REQUEST);

		// Add fields
		request.addField("QUESTION", "Does the request/reply functionality still work?");
		request.addField("COMPONENT", "GMREQ");

		// Display request message
		Log.info("Prepared Request Message:\n" + request.toXML());

		Log.info("Connection.request");
		connection.request(request, msg_timeout_ms, new MyReplyCallback(), 0);

		int deltaTime = 0;
		Log.info("Waiting for response...");
		while (gReplyCallbackFlag == 0)
		{
			Util.millisleep(100);
			if (msg_timeout_ms != gmsecAPI.WAIT_FOREVER)
			{
				deltaTime += 100;
				if (deltaTime >= msg_timeout_ms)
				{
					break;
				}
			}
		}

		if (gReplyCallbackFlag == 1)
		{
			Log.info("Response Received!");
		}
		else
		{
			Log.info("No response received");
		}

		return true;
	}


	public boolean cleanup()
	{
		if (connection != null)
		{
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

			example = new gmreq_cb(config);
			example.run();  
		}
		catch (GMSEC_Exception e) {
			Log.error(e.toString());
		}
		catch (ExampleException e) {
			Log.error(e.toString());
		}
		finally {
			if (example != null)
			{
				Util.cleanup(example);
			}
		}
	}
} 
