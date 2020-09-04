/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmrpl_openresp.java
 * 
 * Another Java example demonstrating the reply/request abilities of 
 * GMSEC.  This example demonstrates the use of the Open-Response
 * option for Request/Reply messages which requires client (requester)
 * to subscribe to anticipated reply message subject in order to
 * receive the reply.
 *
 * Note:  The Open-Response feature is only available with ActiveMQ,
 *        Bolt and WebSphere.  The feature was first introduced
 *        with GMSEC API 3.6.1.
 */


import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.*;

import java.util.ArrayList;


public class gmrpl_openresp implements Example
{
	private Config      m_config;
	private Connection  m_conn;
	private ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();


	public gmrpl_openresp(Config config) throws ExampleException
	{
		m_config = config;

		Util.initialize(m_config);

		if (!Util.hasConnectionType(m_config))
		{
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}


	public boolean run() throws GMSEC_Exception
	{
		setup();

		boolean done = false;

		while (!done)
		{
			Log.info("Waiting for request...");

			// Wait (indefinitely) for a request message
			Message request = m_conn.receive(5000);

			if (request != null)
			{
				// Display request message.
				Log.info("Received Request Message:\n" + request.toXML());

				// Create reply message.
				Message reply = new Message("GMSEC.SYSTEST.TEST1.RESP.MY-REPLY", Message.MessageKind.REPLY);
				// Add fields required to make reply message GMSEC-compliant.
				addRequiredFields(reply);

				// Issue reply message.
				m_conn.reply(request, reply);

				// Display reply message.
				Log.info("Issued Reply Message:\n" + reply.toXML());
			}
			else
			{
				Log.warning("receive() timed out");
				done = true;
			}
		}

		return true;
	}


	private void setup() throws GMSEC_Exception
	{
		// Create connection object.
		m_conn = Connection.create(m_config);

		// Establish a connection.
		m_conn.connect();

		// Subscribe to receive request directive messages.
		info.add(m_conn.subscribe("GMSEC.SYSTEST.TEST1.REQ.DIR.>"));
	}


	private void addRequiredFields(Message msg)
	{
		if (msg == null) return;

		msg.addField("HEADER-VERSION",  (float) 2014);
		msg.addField("MESSAGE-TYPE",    "RESP");
		msg.addField("MESSAGE-SUBTYPE", "DIR");
		msg.addField("MISSION-ID",      "SysTest");
		msg.addField("FACILITY",        "GMSEC Lab");
		msg.addField("COMPONENT",       "GMRPL-OPEN-RESP");
	}


	public boolean cleanup() throws GMSEC_Exception
	{
		if (m_conn!= null)
		{
			for(int i = info.size()-1; i >= 0; i-- )
			{
				Log.info("Unsubscribing from " + info.get(i).getSubject());
				m_conn.unsubscribe(info.get(i));
				info.remove(i);
			}
			Util.closeConnection(m_conn);
		}
		return true;
	}


	private static void printUsage()
	{
		Log.error("\nusage: java gmrpl connectionType=<middleware> "
			+ "[ <parameter>=<value> ]\n"
			+ "\n\tNote that the parameter 'connectiontype' is required. "
			+ "\n\tThe rest of other parameters are optional.\n"
			+ "\n\tserver=<server name> "
			+ "\n\tloglevel=<log level>"
			+ "\n\tcfgfile=<config_filepath.xml>"
			+ "\n\nFor more information, refer to the GMSEC API User's Guide\n\n");
	}

	public static void main(String[] args)
	{
		Example example = null;

		try
		{
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			// Configuration option, introduced with GMSEC API 3.6.1, to enable
			// Open-Response behavior.
			config.addValue("GMSEC-REQ-RESP", "OPEN-RESP");

			example = new gmrpl_openresp(config);

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
				Util.cleanup(example);
		}
	}
}
