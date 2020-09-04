/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gmpub_wsmq_async.java
 *
 * An example Java GMSEC publisher.
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;


public class gmpub_wsmq_async implements Example
{
	private Config     config;
	private Connection connection;

	// This callback is user-defined and can be used to handle the Status object returned by this event
	class AsyncStatusCheckCallback extends EventCallback
	{
		public void onEvent(Connection conn, Status status, Connection.ConnectionEvent event)
		{
			Log.info(status.getReason());

			if (status.isError())
			{
				Log.error("The first occurrence of a WebSphere MQ Asynchronous Put Response warning or failure returned the WebSphere Reason Code: " + status.getCustomCode());
			}
		}
	}

	public gmpub_wsmq_async(Config config) throws GMSEC_Exception, ExampleException
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
		System.err.println("\nusage: java gmpub_wsmq_async connectionType=<middleware> " +
		          "[ <parameter>=<value> ]\n" +
		          "\n\tNote that the parameter 'connectiontype' is required. "  +
		          "\n\tThe rest of other parameters are optional.\n" +
		          "\n\tserver=<server name> " +
		          "(required if middleware is not bolt/MB locally)" +
		          "\n\tsubject=<subject name>" +
		          "\n\titerations=<iterations>" +
		          "\n\tinterval_ms=<interval (milliseconds)>" +
		          "\n\tloglevel=<log level>" +
		          "\n\tcfgfile=<config_filepath.xml>" +
	                  "\n\tmw-async-publish=<true or false>" +
		          "\n\tmw-async-status-check=<true or false>" +
		          "\n\tmw-async-status-check-message-interval=<number of publish operations before checking asynchronous put status>" +
		          "\n\nFor more information, see API User's Guide\n\n");
	}


	public boolean run() throws GMSEC_Exception
	{
		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		String subject     = Util.get(config, "SUBJECT", "GMSEC.TEST.PUBLISH");
		int    iterations  = Util.get(config, "ITERATIONS", 1);
		int    interval_ms = Util.get(config, "INTERVAL_MS", 1000);

		connection = Util.openConnection(config);

		// Output middleware version
		Log.info("Middleware version = " + connection.getLibraryVersion());

		/// Set up the WebSphere Asynchronous Publish Status Checker Event Callback
		connection.registerEventCallback(Connection.ConnectionEvent.WSMQ_ASYNC_STATUS_CHECK_EVENT, new AsyncStatusCheckCallback());

		// Publish messages
		for (int i = 0; i < iterations; ++i)
		{
			publish(i, subject);

			if (i < iterations - 1)
			{
				Util.millisleep(interval_ms);
			}
		}

		return true;
	}


	void publish(int iteration, String subject) throws GMSEC_Exception
	{
		int value = iteration + 1;

		Message message = new Message(subject, Message.MessageKind.PUBLISH);

		// Add Fields
		String data = "JLMNOPQ";

		message.addField("BIN-FIELD", data.getBytes());

		message.addField("BOOL-FIELD-TRUE", true);
		message.addField("BOOL-FIELD-FALSE", false);

		message.addField("CHAR-FIELD", 'c');

		message.addField("I8-FIELD", (byte) value);
		message.addField("I16-FIELD", (short) value);
		message.addField("I32-FIELD", value);
		message.addField("I64-FIELD", (long) value);

		message.addField("U8-FIELD", new U8(value % (U8.MAX_VALUE + 1)));
		message.addField("U16-FIELD", new U16(value % (U16.MAX_VALUE + 1)));
		message.addField("U32-FIELD", new U32(value % (U32.MAX_VALUE + 1)));
		message.addField("U64-FIELD", new U64(Integer.toString(value)));

		message.addField("STRING-FIELD", "This is a test");

		message.addField("F32-FIELD", (float)(1 + 1. / value));
		message.addField("F64-FIELD", (double)(1 + 1. / value));

		message.addField("COUNT", iteration);


		String encrypt = config.getValue("ENCRYPT");
		if (encrypt != null) {
			message.addField("SEC-ENCRYPT", true);
		}

		// Publish Message
		connection.publish(message);

		// Display XML representation of the message
		Log.info("Published:\n" + message.toXML());
	}


	public boolean cleanup() throws GMSEC_Exception
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

		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmpub_wsmq_async(config);
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

