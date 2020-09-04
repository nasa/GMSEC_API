/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file gmreq_openresp.java
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
 * 
 */


import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.*;

import java.util.ArrayList;

public class gmreq_openresp extends ReplyCallback implements Example
{
	private Config     m_config;
	private Connection m_conn;
	private Message    m_request;
	private ArrayList<SubscriptionInfo> info = new ArrayList<SubscriptionInfo>();


	public gmreq_openresp(Config config) throws ExampleException
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

		final int timeout = 1000;

		// Display request message
		Log.info("Issuing Request:\n" + m_request.toXML());

		// Call synchronous request()
		Message reply = m_conn.request(m_request, timeout, 0);

		if (reply != null)
		{
			// Display reply message.
			Log.info("Received Reply:\n" + reply.toXML());
		}
		else
		{
			Log.warning("request timed out");
		}


		// Call asynchronous request()
		m_conn.request(m_request, timeout, this, 0);

		// Allow some time for the ReplyCallback to receive the reply.
		Util.millisleep(timeout);


		// Reply messages are exposed.
		// Using receive() to receive replies.
		//
		// Note: Two replies are expected, one for each of the requests performed
		//       above.
		//
		for (int i = 0; i < 2; ++i)
		{
			reply = m_conn.receive(timeout);

			if (reply != null)
			{
				// Display reply message.
				Log.info("Received Reply Message " + (i+1) + " from receive():\n" + reply.toXML());
			}
		}

		return true;
	}


	public void onReply(Connection conn, Message request, Message reply)
	{
		if (reply != null)
		{
			Log.info("[onReply] Received Reply Message:\n" + reply.toXML());
		}
	}


	public void onEvent(Connection conn, Status status, Connection.ConnectionEvent event)
	{
		if (status.isError())
		{
			Log.warning("[OnEvent] " + status.get() + " [event=" + event + "]");
		}
	}


	private void setup() throws GMSEC_Exception
	{
		// Create connection
		m_conn = Connection.create(m_config);

		// Establish a connection.
		m_conn.connect();

		// Subscribe to (anticipated) response/reply subject.
		info.add(m_conn.subscribe("GMSEC.SYSTEST.TEST1.RESP.>"));

		// Create request directive message.
		m_request = new Message("GMSEC.SYSTEST.TEST1.REQ.DIR.MY-REQUEST", Message.MessageKind.REQUEST);

		// Add fields required to make request message GMSEC-compliant.
		addRequiredFields(m_request);
	}


	private void addRequiredFields(Message msg)
	{
		if (msg == null) return;

		msg.addField("HEADER-VERSION",  (float) 2014);
		msg.addField("MESSAGE-TYPE",    "REQ");
		msg.addField("MESSAGE-SUBTYPE", "DIR");
		msg.addField("MISSION-ID",      "SysTest");
		msg.addField("FACILITY",        "GMSEC Lab");
		msg.addField("COMPONENT",       "GMREQ-OPEN-RESPONSE");
	}


	public boolean cleanup() throws GMSEC_Exception
	{
		if (m_conn != null)
		{
			for(int i = info.size()-1; i >= 0; i--)
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
		Log.error("\nusage: java gmreq_openresp connectionType=<middleware> "
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

			example = new gmreq_openresp(config);

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
