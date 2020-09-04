/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file gmsub_cfg.java
 *
 * A Java demonstration example of Java subscriber using the configuration 
 * file.
 *
 */

import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;


class gmsub_cfg_Callback extends Callback
{
	public void onMessage(Connection conn, Message msg) 
	{
		Log.info("[gmsub_cfg_cb.onMessage] Received Message:\n" + msg.toXML());

		Log.info("Press the enter key to exit");
	}
}


class gmsub_cfg_EventCallback extends EventCallback
{
	public void onEvent(Connection conn, Status status, Connection.ConnectionEvent event) 
	{
		if (status.isError())
		{
			Log.error("[gmsub_cfg_cb.onEvent] " + status.get() + " [event=" + event + "]");
		}
	}
}


class gmsub_cfg_cb implements Runnable 
{
	Connection conn = null;
	Message msg = null;

	public gmsub_cfg_cb(Connection conn, Message msg)
	{
		this.conn = conn;
		this.msg  = msg;
	}


	public void run()
	{
		// publish a message
		for (int i = 0; i < 10; ++i)
		{
			Log.info("sending...");

			try
			{
      			conn.publish(msg);
			}
			catch (GMSEC_Exception e)
			{
				Log.error("GMSEC_Exception: " + e.toString());
			}

			try
			{
				Thread.sleep(5000);
			} 
			catch (InterruptedException e1)
			{
				Log.info("Sleep interrupted, exiting program");
				break;
			}
		}
	}
}


public class gmsub_cfg implements Example
{
	Config     config;
	ConfigFile configFile;
	Connection connection1 = null;
	Connection connection2 = null;
	ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();


	public gmsub_cfg(Config config, ConfigFile configFile)
	{
		this.config     = config;
		this.configFile = configFile;

		Util.initialize(config);
	}


	public boolean run()
	{
		boolean result = true;

		// Output GMSEC API version
		Log.info(Connection.getAPIVersion());

		try
		{
			// Establish a connection
			Log.info("Creating first connection"); 
			connection1 = Util.openConnection(config);

			Log.info("Creating second connection"); 
			connection2 = Util.openConnection(config);

			// Lookup subscription topic from configuration file
			String subject = configFile.lookupSubscription("SUB1");

			// Lookup message from configuration file
			Message message = configFile.lookupMessage("MSG1");

			// Instantiate callback
			Callback      subCallback = new gmsub_cfg_Callback();
			EventCallback evtCallback = new gmsub_cfg_EventCallback();
  
			// subscribe
			Log.info("Subscribing to first connection's SUB1: " + subject);
			info.add(connection1.subscribe(subject, subCallback));

			Log.info("Subscribing to second connection's SUB1: " + subject);
			info.add(connection2.subscribe(subject, subCallback));

			// Register event callback
			Log.info("Registering event callback for connection dispatcher");
			connection1.registerEventCallback(Connection.ConnectionEvent.DISPATCHER_ERROR_EVENT, evtCallback);

			// Start auto-dispatcher
			connection1.startAutoDispatch();
			connection2.startAutoDispatch();

			// Publish messages
			gmsub_cfg_cb publisher = new gmsub_cfg_cb(connection1, message);
			Thread t = new Thread(publisher);
			t.start();

			Log.info("Press the enter key to exit");

			try {
				// wait for input
				int c = System.in.read();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
			Log.info("shutting down...");
 
			t.interrupt();
    
			// Stop auto-dispatcher
			connection1.stopAutoDispatch();
			connection2.stopAutoDispatch();
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
		if (connection1 != null)
		{
			Log.info("Unsubscribing from " + info.get(0).getSubject());
			connection1.unsubscribe(info.get(0));

			Util.closeConnection(connection1);
		}
		if (connection2 != null)
		{
			Log.info("Unsubscribing from " + info.get(1).getSubject());
			connection2.unsubscribe(info.get(1));

			Util.closeConnection(connection2);
		}
		if (!info.isEmpty())
		{
			for(int i = info.size()-1; i >= 0; i--)
			{
				info.remove(i);
			}
		}

		return true;
	}


	public static void main(String[] args) 
	{
		if (Array.getLength(args) != 1)
		{
			System.err.println("\nUsage: java gmsub_cfg <config_file.xml>");
			System.err.println("\nConsider using gmsub_cfg.xml\n");
			return;
		}

		Example example = null;

		Log.info("loading:" + args[0]);
    
		try
		{
			// Load configuration file
			ConfigFile configFile = new ConfigFile();
			configFile.load(args[0]);
      
			// Look up the configuration
			Config config = configFile.lookupConfig("default");

			new gmsub_cfg(config, configFile).run();
		}
		catch (GMSEC_Exception e)
		{
			System.err.println("GMSEC_Exception: " + e.toString());
		}
	}
}
