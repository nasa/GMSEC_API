/*
 * Copyright 2007-2021 United States Government as represented by the
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

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;

import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;

import gov.nasa.gsfc.gmsecapi.util.Log;


public class gmreq implements Example
{
	static final String DEFAULT_SUBJECT = "GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL";

	StatusChecker checker;
	Config config;
	Connection connection;
	Message request;
	Message reply;

	gmreq(Config config) throws ExampleException {
		checker = new StatusChecker();
		this.config = config;
		Util.initialize(config);

		if (!Util.hasConnectionType(config)) {
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}

	private void printUsage() {
		Log.Error("\nusage: java gmreq connectionType=<middleware> " +
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


	public boolean run() throws ExampleException {
		Status status;

		String subject = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
		int msg_timeout_ms = Util.get(config,  "MSG_TIMEOUT_MS", 
									Util.EX_MSG_TIMEOUT_MS);

		connection = Util.openConnection(config);

		Field field = new Field();

		// Request - Reply
		Message request = new Message();

		checker.calling("CreateMessage")
			.check(connection.CreateMessage(subject, GMSEC_MSG_REQUEST, request));

		// Add fields
		checker.calling("SetFieldValueSTRING")
			.check(field.SetFieldValueSTRING("QUESTION",
				"Does the request/reply functionality still work?"));
		checker.calling("AddField(QUESTION)")
			.check(request.AddField(field));

		// Add the name of the component making the Request.
		field.SetFieldValueSTRING("COMPONENT", "GMREQ");
		checker.calling("AddField(COMPONENT)")
			.check(request.AddField(field));

		// Add the Message ID of this Request. In theory the MSG-ID should be
		// unique, but here it is not for the sake of simplicity. See API Interface
		// Specification for suggestions on creating a MSG-ID.
		field.SetFieldValueSTRING("MSG-ID", "GMREQ-MSG");
		checker.calling("AddField(MSG-ID)")
			.check(request.AddField(field));

		// Dump Reply
		GMSEC_String requestXML = new GMSEC_String();
		checker.calling("request.ToXML").check(request.ToXML(requestXML));
		Log.Info("built request =>\n" + requestXML.Get());
		
		// Send Request Message
		Message reply = new Message();
		checker.calling("Connection.Request")
			.check(connection.Request(request, msg_timeout_ms, reply));

		// Dump Reply
		GMSEC_String replyXML = new GMSEC_String();
		checker.calling("reply.ToXML").check(reply.ToXML(replyXML));

		Log.Info("received reply =>\n" + replyXML.Get());

		return true;
	}


	public boolean cleanup() throws ExampleException {
		if (request != null && request.isValid()) {
			Status status = connection.DestroyMessage(request);
			Util.checkNoThrow("DestroyMessage", status);
		}
		
		if (reply != null && reply.isValid()) {
			Status status = connection.DestroyMessage(reply);
			Util.checkNoThrow("DestroyMessage", status);
		}
		
		if (connection != null)
			Util.closeConnection(connection);

		return false;
	}

	public static void main(String[] args) {

		Example example = null;

		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmreq(config);
			example.run();
		}
		catch (ExampleException e) {
			e.printStackTrace();
		}
		finally {
			if (example != null)
				Util.cleanup(example);
		}

	}

}

