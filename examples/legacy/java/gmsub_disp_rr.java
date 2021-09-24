/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** 
 * @file gmsub_disp.java
 *
 * A simple example Java GMSEC subscriber using Dispatch and request/reply
 *
 */

import gov.nasa.gsfc.gmsecapi.Callback;
import gov.nasa.gsfc.gmsecapi.ReplyCallback;
import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;

class PublishDispatch implements Callback {
	public void OnMessage(Connection conn, Message message) {
		// Dump Message
		GMSEC_String xml = new GMSEC_String();
		message.ToXML(xml);
		Log.Info("received \n" + xml.Get());
		// Do not destroy the message here
	}
}

class DispReplyCallback implements ReplyCallback {
	public void OnError(Connection conn, Message message, Status status) {
		Log.Warning("[DispReplyCallback.OnError] Status=" + status.Get());
	}

	public void OnReply(Connection conn, Message message, Message reply) {
		// Dump Reply
		GMSEC_String replyMsg = new GMSEC_String("");
		reply.ToXML(replyMsg);
		Log.Info("OnReply:reply =>\n" + replyMsg.Get());
		// Do not destroy the message here
	}
};

public class gmsub_disp_rr implements Example {

	StatusChecker checker;
	Config config;
	Connection connection;
	Connection connectionADisp;
	Message request;

	gmsub_disp_rr(Config config) throws ExampleException {
		checker = new StatusChecker();
		this.config = config;
		Util.initialize(config);

		String name = Util.class.getName();
		System.out.println(name);
		System.out.println(Util.class.getResource("Util.class"));
		
		if (!Util.hasConnectionType(config)) {
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}

	private void printUsage() {
		Log.Error("\nusage: java gmsub_disp_rr connectionType=<middleware> "
				+ "[ <parameter>=<value> ]\n"
				+ "\n\tNote that the parameter 'connectiontype' is required. "
				+ "\n\tThe rest of other parameters are optional.\n"
				+ "\n\tserver=<server name> "
				+ "(required if middleware is not bolt/MB locally)"
				+ "\n\tsubject=<subject name>"
				+ "\n\tsubject.<N>=<sub-subject name>"
				+ "\n\t\tNote: N must be 1 or greater"
				+ "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B"
				+ "\n\tmsg_timeout_ms=<timeout period (milliseconds)>" 
				+ "\n\tprog_timeout_s=<timeout period (seconds)>"  
				+ "\n\tloglevel=<log level>"
				+ "\n\tcfgfile=<config_filepath.xml>"
				+ "\n\nFor more information, see API User's Guide\n\n");
	}

	public boolean run() throws ExampleException {
		Status result = new Status();

		// Output GMSEC API version
		Log.Info(ConnectionFactory.GetAPIVersion());

		// determine what subjects to listen to
		java.util.List<String> subjects = new java.util.ArrayList<String>(10);

		String value = Util.get(config, "SUBJECT");
		if (value != null) {
			subjects.add(value);
		} else {
			for (int i = 1;; ++i) {
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
		} else {
			subjects.add("GMSEC.TERMINATE");
		}
		
		int msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS",
											Util.EX_MSG_TIMEOUT_MS);
		int prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S",
											Util.EX_PROG_TIMEOUT_S);
		
		// Establish a connection
		connection = Util.openConnection(config);

		// Output middleware version
		Log.Info("Middleware version = " + connection.GetLibraryVersion());

		// Subscribe
		for (String subject : subjects) {
			Log.Info("subscribing to '" + subject + "' with PublishDispatch");
			checker.calling(
					"Subscribe(" + subject + ") with new PublishDispatch")
					.check(connection.Subscribe(subject, new PublishDispatch()));
		}

		Log.Info("Starting AutoDispatch");
		checker.calling("StartAutoDispatch").check(
				connection.StartAutoDispatch());

		
	    // Request - Reply
		String subject = Util.get(config, "SUBJECT", "GMSEC.TEST.REQUEST");
		
	    Message request = new Message();
	    checker.calling("CreateMessage")
	      .check(connection.CreateMessage(subject, GMSEC_MSG_REQUEST, request));

	    Field field = new Field();

	    // Add fields
	    checker.calling("SetFieldValuesSTRING")
	      .check(field.SetFieldValueSTRING("QUESTION",
	             "Does the request/reply functionality still work?"));

	    checker.calling("AddField(QUESTION)").check(request.AddField(field));

	    // Add the name of the component making the Request.
	    field.SetFieldValueSTRING("COMPONENT", "GMSUB-DISP-RR");
	    checker.calling("AddField(COMPONENT)").check(request.AddField(field));

	    // Add the Message ID of this Request.  In theory the MSG-ID should be unique,
	    // but here it is not for the sake of simplicity.  See API Interface Specification
	    // for suggetions on creating a MSG-ID.
	    field.SetFieldValueSTRING("MSG-ID", "GMSUB-DISP-RR-MSG");
	    checker.calling("AddField(MSG-ID)").check(request.AddField(field));

	    // Dump Request
	    GMSEC_String requestMsg = new GMSEC_String();
	    checker.calling("Request.ToXML").check(request.ToXML(requestMsg));
	    Log.Info("built request =>\n" + requestMsg.Get());
		
		checker.calling("Connection.Request").check(
				connection.Request(request, msg_timeout_ms, new DispReplyCallback()));
		
		//Wait
		int deltaTime = 0;
		if(prog_timeout_s == GMSEC_WAIT_FOREVER)
		{
			Log.Info("Press the enter key to exit");
			try {
				while(System.in.available() == 0)
				{
					Util.millisleep(100);
				}
			} catch (Exception e) {
			}
		}
		else
		{
//			while(deltaTime < prog_timeout_s)
	//		{
	//			Util.millisleep(1000);
	//			deltaTime += 1;
	//		}
			Util.millisleep(prog_timeout_s * 1000);
			Log.Info("Program Timeout Reached");
		}

		Log.Info("Stopping AutoDispatch");
		checker.calling("StopAutoDispatch")
				.check(connection.StopAutoDispatch());

		return true;
	}

	public boolean cleanup() throws ExampleException {
		if (request != null && request.isValid()) {
			Status status = connection.DestroyMessage(request);
			Util.checkNoThrow("DestroyMessage", status);
		}

		if (connection != null)
			Util.closeConnection(connection);

		return true;
	}

	public static void main(String args[]) {
		Example example = null;
		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmsub_disp_rr(config);
			example.run();
		} catch (ExampleException e) {
			e.printStackTrace();
		} finally {
			if (example != null)
				Util.cleanup(example);
		}
	}
}
