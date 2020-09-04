/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmsub_disp.java
 *
 * A simple example Java GMSEC subscriber 
 *
 */


import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;

import java.util.ArrayList;


class PublishDispatcher extends Callback 
{
	public void onMessage(Connection conn, Message message) 
	{
		Log.info("[PublishDispatcher.onMessage] Received:\n" + message.toXML());

		Log.info("Press the enter key to exit");
	}
}


public class gmsub_disp implements Example 
{
	Config     config;
	Connection connection;
	ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();

	gmsub_disp(Config config) throws ExampleException
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
		System.err.println("\nusage: java gmsub_disp connectionType=<middleware> " +
		                   "[ <parameter>=<value> ]\n" +
		                   "\n\tNote that the parameter 'connectiontype' is required. "  +
		                   "\n\tThe rest of other parameters are optional.\n" +
		                   "\n\tserver=<server name> " +
		                   "(required if middleware is not bolt/MB locally)" +
		                   "\n\tsubject=<subject name>" +
		                   "\n\tsubject.<N>=<sub-subject name>" +
		                   "\n\t\tNote: N must be 1 or greater" +
		                   "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B" +
		                   "\n\tloglevel=<log level>" +
		                   "\n\tcfgfile=<config_filepath.xml>" +
		                   "\n\nFor more information, see API User's Guide\n\n");
	}


	public boolean run()
	{
		boolean result = true;

		// determine what subjects to listen to
		java.util.List<String> subjects = new java.util.ArrayList<String>(10);

		String value = Util.get(config,"SUBJECT");
		if (value != null) {
			subjects.add(value);
		}
		else {
			for (int i = 1; ; ++i) {
				String tmp = Util.get(config, "SUBJECT." + i);
				if (tmp != null)
					subjects.add(tmp);
				else
					break;
			}
		}

		if (subjects.isEmpty()) {
			String subject = Util.get(config, "SUBJECT", "GMSEC.>");
			subjects.add(subject);
		}
		else {
			subjects.add("GMSEC.TERMINATE");
		}

		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		try
		{
			// Establish a connection
			connection = Util.openConnection(config);

			// Output middleware version
			Log.info("Middleware version = " + connection.getLibraryVersion());

			// Subscribe
			for (String subject : subjects)
			{
				Log.info("subscribing to '" + subject + "' with PublishDispatcher");
				info.add(connection.subscribe(subject, new PublishDispatcher()));
			}

			Log.info("Starting Auto-Dispatch");
			connection.startAutoDispatch();

			Log.info("Press the enter key to exit");

			try {
				while (System.in.available() == 0)
				{
					Util.millisleep(100);
				} 
			}
			catch (Exception e) {
			}

			Log.info("Stopping Auto-Dispatch");
			connection.stopAutoDispatch();
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
		try
		{
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			new gmsub_disp(config).run();
		}
		catch (ExampleException e)
		{
			System.err.println(e.toString());
		}
	}
}
