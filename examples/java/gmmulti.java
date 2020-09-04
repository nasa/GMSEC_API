/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


 
/** 
 * @file gmmulti.java
 *
 * An example Java GMSEC multi-connections
 *
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.*;

import java.util.ArrayList;


class TestConstant 
{
  public static final String SUBJECT1 = "GMSEC.TEST.CONNECT.AND.SEND";
  public static final String SUBJECT2 = "GMSEC.TEST.CONNECT.AND.SEND.2";
}


class PublishFinalCB extends Callback 
{
	public void onMessage(Connection conn, Message message) 
	{
		// Display XML representation of the received message
		Log.info("PublishFinalCB.onMessage received:\n" + message.toXML());
	}
}


class MyCallback extends Callback 
{
	public void onMessage(Connection conn, Message message) 
	{
		// Display XML representation of the received message
		Log.info("MyCallback.onMessage Received:\n" + message.toXML());

		// create message to send
		Message newMessage = new Message(TestConstant.SUBJECT2, Message.MessageKind.PUBLISH);

		try {
			// publish message
			conn.publish(newMessage);

			// Display XML representation of the published message
			Log.info("MyCallback.onMessage Published:\n" + newMessage.toXML());
		}
		catch (GMSEC_Exception e) {
			Log.error("MyCallback.onMessage failed to publish message; reason => " + e.toString());
		}
	}
}


public class gmmulti implements Example
{
	Config     config;
	Connection connection;
	ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();

	gmmulti(Config config) throws ExampleException
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
		Log.error("\nusage: java gmmulti connectionType=<middleware> " +
		          "[ <parameter>=<value> ]\n" +
		          "\n\tNote that the parameter 'connectiontype' is required. "  +
		          "\n\tThe rest of other parameters are optional.\n" +
		          "\n\tserver=<server name> " +
		          "(required if middleware is not bolt/MB locally)" +
		          "\n\tloglevel=<log level>" +
		          "\n\tcfgfile=<config_filepath.xml>" +
		          "\n\nFor more information, see API User's Guide\n\n");
	}


	public boolean run() throws GMSEC_Exception
	{
		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		// Establish a connection
		connection = Util.openConnection(config);

		// Output middleware version
		Log.info("Middleware version = " + connection.getLibraryVersion());

		// Subscribe
		Log.info("Callback's subscribing to " + TestConstant.SUBJECT1);
		info.add(connection.subscribe(TestConstant.SUBJECT1, new MyCallback()));

		Log.info("Callback's subscribing to " + TestConstant.SUBJECT2);
		info.add(connection.subscribe(TestConstant.SUBJECT2, new PublishFinalCB()));

		// start autodispatcher
		connection.startAutoDispatch();

		// create message to send
		Message message = new Message(TestConstant.SUBJECT1, Message.MessageKind.PUBLISH);

		// send message (xN)
		for (short i = 0; i < 2; i++)
		{
			message.addField("COUNT", i);

			// Publish Message
			connection.publish(message);
		}

		// wait
		Util.millisleep(5000);

		// stop autodispatcher
		Log.info("Stop AutoDispatcher");
		connection.stopAutoDispatch();

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

			example = new gmmulti(config);
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
