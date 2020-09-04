/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmsub.java
 *
 * An example Java GMSEC publisher.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;

import java.util.ArrayList;


public class gmsub implements Example
{
	private Config     config;
	private Connection connection;
	private Message    message;
	private ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();


	public gmsub(Config config) throws GMSEC_Exception, ExampleException
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
		System.err.println("\nusage: java gmsub connectionType=<middleware> " +
		                   "[ <parameter>=<value> ]\n" +
		                   "\n\tNote that the parameter 'connectiontype' is required. "  +
		                   "\n\tThe rest of other parameters are optional.\n" +
		                   "\n\tserver=<server name> " + 
		                   "(required if middleware is not bolt/MB locally)" +
		                   "\n\tsubject=<subject name>" +
		                   "\n\titerations=<iterations>" +
		                   "\n\tsubject.<N>=<sub-subject name>" +
		                   "\n\t\tNote: N must be 1 or greater" +
		                   "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B" +
		                   "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
		                   "\n\tprog_timeout_s=<timeout period (seconds)>" +
		                   "\n\t\tNote: msg_timeout_ms must be specified for prog_timeout_s" +
		                   "\n\tloglevel=<log level>" +
		                   "\n\tcfgfile=<config_filepath.xml>" +
		                   "\n\nFor more information, see API User's Guide\n\n");
}


public boolean run() throws GMSEC_Exception
{
		// determine what subjects to listen to
		java.util.List<String> subjects = new java.util.ArrayList<String>(10);

		String value = Util.get(config,"SUBJECT");

		if (value != null)
		{
			subjects.add(value);
		}
		else
		{
			for (int i = 1; ; ++i)
			{
				String tmp = Util.get(config, "SUBJECT." + i);
				if (tmp != null)
				{
					subjects.add(tmp);
				}
				else
				{
					break;
				}
			}
		}

		if (subjects.isEmpty())
		{
			String subject = Util.get(config, "SUBJECT", "GMSEC.>");
			subjects.add(subject);
		}
		else
		{
			subjects.add("GMSEC.TERMINATE");
		}

		int iterations = Util.get(config, "ITERATIONS", 0);
		if (iterations > 0)
		{
			Log.info("waiting for up to " + iterations + " messages");
		}

		int msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS", gmsecAPI.WAIT_FOREVER);
		int prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S", gmsecAPI.WAIT_FOREVER);

        if (msg_timeout_ms == gmsecAPI.WAIT_FOREVER && prog_timeout_s != gmsecAPI.WAIT_FOREVER)
        {
           Log.error("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
           return false;
        }                          

		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());
		
		// Establish a connection
		connection = Util.openConnection(config);

		// Output middleware version
		Log.info("Middleware version = " + connection.getLibraryVersion());

		// Subscribe
		for (String subject : subjects)
		{
			Log.info("subscribing to " + subject);
			info.add(connection.subscribe(subject));
		}

		// Wait for messages
		int     count = 0;
		boolean done = false;
		long    prevTime = System.currentTimeMillis();
		long    nextTime = 0;
		int     elapsedTime = 0;

		while (!done)
		{
			if (elapsedTime >= (prog_timeout_s * 1000) && prog_timeout_s != gmsecAPI.WAIT_FOREVER)
			{
				done = true;
				Log.info("Program Timeout Reached");
			}

			message = connection.receive(msg_timeout_ms);

			nextTime     = System.currentTimeMillis();
			elapsedTime += (int) (nextTime - prevTime);
			prevTime     = nextTime;

			if (message == null)
			{
				Log.info("timeout occurred");
				continue;
			}
			
			// Received message
			++count;

			if (iterations > 0 && count >= iterations)
			{
				done = true;
			}

			// Display XML representation of the received message
			Log.info("Received:\n" + message.toXML());

			if (message.getSubject().equals("GMSEC.TERMINATE"))
			{
				done = true;
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


	public static void main(String[] args)
	{
		Example example = null;

		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmsub(config);
			example.run();
		}
		catch (IllegalArgumentException e) {
			System.err.println(e.toString());
		}
		catch (GMSEC_Exception e) {
			System.err.println(e.toString());
		}
		catch (ExampleException e) {
			System.err.println(e.toString());
		}
		finally {
			if (example != null)
				Util.cleanup(example);
		}

	}

}

