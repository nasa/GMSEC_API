/*
 * Copyright 2007-2021 United States Government as represented by the
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


import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.ReplyCallback;
import gov.nasa.gsfc.gmsecapi.Status;

import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;

import gov.nasa.gsfc.gmsecapi.util.Log;


public class gmreq_openresp implements Example, ReplyCallback
{
	private Config        m_config;
	private Connection    m_conn;
	private Message       m_request;
	private StatusChecker m_checker;


	public gmreq_openresp(Config config)
	{
		m_config  = config;
		m_conn    = null;
		m_request = null;
		m_checker = new StatusChecker();
	}


	public boolean run() throws ExampleException
	{
		setup();

		Status status;

		final int timeout = 1000;
		GMSEC_String xml = new GMSEC_String();

		// Using Request(Message* request, GMSEC_I32 timeout, Message* reply)
		//
		Message reply = new Message();

		status = m_conn.Request(m_request, timeout, reply);
		m_checker.calling("Request(req, timeout, rep)").check(status);

		// Display request message.
		m_request.ToXML(xml);
		Log.Info("Issued Request Message:\n" + xml);

		if (!status.IsError() && reply != null)
		{
			// Display reply message.
			reply.ToXML(xml);
			Log.Info("Received Reply Message:\n" + xml);

			m_conn.DestroyMessage(reply);
		}


		// Using Request(Message* request, GMSEC_I32 timeout, ReplyCallback* cb)
		//
		status = m_conn.Request(m_request, timeout, this);
		m_checker.calling("Request(req, timeout, ReplyCallback)").check(status);

		// Display request message.
		m_request.ToXML(xml);
		Log.Info("Issued Request Message:\n" + xml);

		// Allow some time for the ReplyCallback to receive the reply.
		Util.millisleep(timeout);


		// Reply messages are exposed.
		// Using GetNextMsg() to receive replies.
		//
		// Note: Two replies are expected, one for each of the requests performed
		//       above.
		//
		for (int i = 0; i < 2; ++i)
		{
			status = m_conn.GetNextMsg(reply, timeout);
			m_checker.calling("GetNextMsg(rep, timeout)").check(status);

			if (!status.IsError() && reply != null)
			{
				// Display reply message.
				reply.ToXML(xml);
				Log.Info("Received Reply Message " + (i+1) + " from GetNextMsg:\n" + xml);

				m_conn.DestroyMessage(reply);
			}
		}

		return true;
	}


	public void OnReply(Connection conn, Message request, Message reply)
	{
		if (reply != null)
		{
			GMSEC_String xml = new GMSEC_String();
			reply.ToXML(xml);
			Log.Info("Received Reply Message from within OnReply:\n" + xml);
		}
	}


	public void OnError(Connection conn, Message request, Status status)
	{
		if (status.IsError())
		{
			Log.Warning("[OnError] " + status.Get());
		}
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

		m_conn    = new Connection();
		m_request = new Message();

		// Create connection object.
		status = ConnectionFactory.Create(m_config, m_conn);
		m_checker.calling("ConnectionFactory.Create").check(status);

		// Establish a connection.
		status = m_conn.Connect();
		m_checker.calling("Connect").check(status);

		// Subscribe to (anticipated) response/reply subject.
		status = m_conn.Subscribe("GMSEC.SYSTEST.TEST1.RESP.>");
		m_checker.calling("Subscribe").check(status);

		// Create request directive message.
		status = m_conn.CreateMessage("GMSEC.SYSTEST.TEST1.REQ.DIR.MY-REQUEST", GMSEC_MSG_REQUEST, m_request);
		m_checker.calling("CreateMessage").check(status);

		// Add fields required to make request message GMSEC-compliant.
		addRequiredFields(m_request);
	}


	private void addRequiredFields(Message msg)
	{
		if (msg == null) return;

		Field field = new Field();

		field.SetFieldValueF32("HEADER-VERSION", (float) 2014);
		msg.AddField(field);

		field.SetFieldValueSTRING("MESSAGE-TYPE", "REQ");
		msg.AddField(field);

		field.SetFieldValueSTRING("MESSAGE-SUBTYPE", "DIR");
		msg.AddField(field);

		field.SetFieldValueSTRING("MISSION-ID", "Systest");
		msg.AddField(field);

		field.SetFieldValueSTRING("FACILITY", "GMSEC Lab");
		msg.AddField(field);

		field.SetFieldValueSTRING("COMPONENT", "gmreq-openresp");
		msg.AddField(field);
	}


	public boolean cleanup() throws ExampleException
	{
		if (m_request != null && m_request.isValid())
		{
			Status status = m_conn.DestroyMessage(m_request);
			Util.checkNoThrow("DestroyMessage", status);
		}

		if (m_conn != null)
			Util.closeConnection(m_conn);

		return false;
	}


	private static void printUsage()
	{
		Log.Error("\nusage: java gmreq_openresp connectionType=<middleware> "
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

			example = new gmreq_openresp(config);

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
