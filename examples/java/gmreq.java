/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmreq.java
 *
 * A Java demonstration example of GMSEC request/reply functionality. 
 * 
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.*;


public class gmreq implements Example
{
	static final String DEFAULT_SUBJECT = "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL";

	Config     config;
	Connection connection;

	gmreq(Config config) throws ExampleException
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
		Log.error("\nusage: java gmreq connectionType=<middleware> " +
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
		String subject        = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
		int    msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS", gmsecAPI.WAIT_FOREVER);

		connection = Util.openConnection(config);

		// Request - Reply
		Message request = new Message(subject, Message.MessageKind.REQUEST);

		// Add fields
		request.addField("QUESTION", "Does the request/reply functionality still work?");
		request.addField("COMPONENT", "GMREQ");

		// Display the request message
		Log.info("Prepared Request Message:\n" + request.toXML());

		// Send Request Message
		Message reply = connection.request(request, msg_timeout_ms, 0);

		if (reply != null)
		{
			// Display the response message
			Log.info("Received Reply Message:\n" + reply.toXML());
		}
		else
		{
			Log.warning("request timed out");
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


	public static void main(String[] args)
	{
		Example example = null;

		try
		{
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmreq(config);
			example.run();
		}
		catch (GMSEC_Exception e)
		{
			Log.error(e.toString());
		}
		catch (ExampleException e)
		{
			Log.error(e.toString());
		}
		finally
		{
			if (example != null)
			{
				Util.cleanup(example);
			}
		}
	}
}

