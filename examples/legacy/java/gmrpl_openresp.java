/*
 * Copyright 2007-2021 United States Government as represented by the
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
 * 
 */


import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;

import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;

import gov.nasa.gsfc.gmsecapi.util.Log;


public class gmrpl_openresp implements Example
{
	private Config        m_config;
	private Connection    m_conn;
	private StatusChecker m_checker;


	public gmrpl_openresp(Config config)
	{
		m_config  = config;
		m_conn    = null;
		m_checker = new StatusChecker();
	}


	public boolean run() throws ExampleException
	{
		setup();

		Status status;
		Message request = new Message();
		Message reply = new Message();

		boolean done = false;

		while (!done)
		{
			// Wait (indefinitely) for a request message
			status = m_conn.GetNextMsg(request, -1);
			m_checker.calling("GetNextMsg").checkNoThrow(status);

			if (request != null)
			{
				GMSEC_String xml = new GMSEC_String();

				// Display request message.
				request.ToXML(xml);
				Log.Info("Received Request Message:\n" + xml);

				// Create reply message.
				status = m_conn.CreateMessage("GMSEC.SYSTEST.TEST1.RESP.MY-REPLY", GMSEC_MSG_REPLY, reply);
				m_checker.calling("CreateMessage").check(status);

				// Add fields required to make reply message GMSEC-compliant.
				addRequiredFields(reply);

				// Issue reply message.
				status = m_conn.Reply(request, reply);
				m_checker.calling("Reply(req, rep)").check(status);

				// Display reply message.
				reply.ToXML(xml);
				Log.Info("Issued Reply Message:\n" + xml);

				// Destroy the request message.
				m_conn.DestroyMessage(request);

				// Destroy the reply message.
				m_conn.DestroyMessage(reply);
			}
			else
			{
				Log.Warning("Exiting due to GetNextMsg() error.");
				done = true;
			}
		}

		return true;
	}


	private void setup() throws ExampleException
	{
		Status status;

		Util.initialize(m_config);

		if (!Util.hasConnectionType(m_config))
		{
			printUsage();
			throw new ExampleException("Invalid option");
		}

		m_conn = new Connection();

		// Create connection object.
		status = ConnectionFactory.Create(m_config, m_conn);
		m_checker.calling("ConnectionFactory.Create").check(status);

		// Establish a connection.
		status = m_conn.Connect();
		m_checker.calling("Connect").check(status);

		// Subscribe to receive request directive messages.
		status = m_conn.Subscribe("GMSEC.SYSTEST.TEST1.REQ.DIR.>");
		m_checker.calling("Subscribe").check(status);
	}


	private void addRequiredFields(Message msg)
	{
		if (msg == null) return;

		Field field = new Field();

		field.SetFieldValueF32("HEADER-VERSION", (float) 2014);
		msg.AddField(field);

		field.SetFieldValueSTRING("MESSAGE-TYPE", "RESP");
		msg.AddField(field);

		field.SetFieldValueSTRING("MESSAGE-SUBTYPE", "DIR");
		msg.AddField(field);

		field.SetFieldValueSTRING("MISSION-ID", "Systest");
		msg.AddField(field);

		field.SetFieldValueSTRING("FACILITY", "GMSEC Lab");
		msg.AddField(field);

		field.SetFieldValueSTRING("COMPONENT", "gmrpl-openresp");
		msg.AddField(field);
	}


	public boolean cleanup() throws ExampleException
	{
		if (m_conn!= null)
			Util.closeConnection(m_conn);

		return false;
	}


	private static void printUsage()
	{
		Log.Error("\nusage: java gmrpl connectionType=<middleware> "
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
			config.AddValue("GMSEC-REQ-RESP", "OPEN-RESP");

			example = new gmrpl_openresp(config);

			example.run();
		}
		catch (ExampleException e)
		{
			// Nothing to do here
		}
		finally
		{
			if (example != null)
				Util.cleanup(example);
		}
	}
}
