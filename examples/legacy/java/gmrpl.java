 
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






 
/** @file gmrpl.java
 *
 * An example replier using the Java GMSEC API.
 */

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_UShort;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;


public class gmrpl implements Example
{
	static final String DEFAULT_SUBJECT = "GMSEC.>";

	StatusChecker checker = new StatusChecker();
	Config config;
	Connection connection;
	Message message;
	Message reply;


	gmrpl(Config config) throws ExampleException {
		this.config = config;
		Util.initialize(config);

		if (!Util.hasConnectionType(config)) {
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}

	private void printUsage() {
		Log.Error("\nusage: java gmrpl connectionType=<middleware> " +
				"[ <parameter>=<value> ]\n" +
				"\n\tNote that the parameter 'connectiontype' is required. "  +
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " +
				"(required if middleware is not bolt/MB locally)" +
				"\n\tsubject=<subject name>" +
				"\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
				"\n\tprog_timeout_s=<timeout period (seconds)>" +
				"\n\t\tNote: msg_timeout_ms must be specified for prog_timeout_s" +
				"\n\tloglevel=<log level>" +
				"\n\tcfgfile=<config_filepath.xml>" +
				"\n\nFor more information, see API User's Guide\n\n");
	}

	public boolean run() throws ExampleException {
		Status status;

		int msg_timeout_ms = Util.get(config,  "MSG_TIMEOUT_MS", 
				Util.EX_MSG_TIMEOUT_MS);
		int prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S",
				Util.EX_PROG_TIMEOUT_S);
        if(msg_timeout_ms == GMSEC_WAIT_FOREVER && 
									prog_timeout_s != GMSEC_WAIT_FOREVER)
        {
           Log.Error("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
           return false;
        }  	

		connection = Util.openConnection(config);

		String subject = Util.get(config, "SUBJECT", DEFAULT_SUBJECT);
		Log.Info("Subscribing to " + subject);
		checker.calling("Subscribe(" + subject + ")")
			.check(connection.Subscribe(subject));

		Log.Info("Subscribing to GMSEC.TERMINATE");
		checker.calling("Subscribe(" + "GMSEC.TERMINATE" + ")")
			.check(connection.Subscribe("GMSEC.TERMINATE"));

		boolean done = false;
		long prevTime = System.currentTimeMillis();
		long nextTime = 0;
		int elapsedTime = 0;	//ms

		while (!done) {

			if(elapsedTime >= (prog_timeout_s * 1000)
								 && prog_timeout_s != GMSEC_WAIT_FOREVER)
			{
				done = true;
				Log.Info("Program Timeout Reached");
				continue;
			}
			
			message = new Message();

			checker.calling("GetNextMsg(timeout=" + msg_timeout_ms + ")");
			status = connection.GetNextMsg(message, msg_timeout_ms);
			
			nextTime = System.currentTimeMillis();
			elapsedTime += (int) (nextTime - prevTime);
			prevTime = nextTime;
			
			if (status.isError()) {
				if (status.GetCode() == GMSEC_TIMEOUT_OCCURRED)
					Log.Info("timeout occurred");
				else
					Util.check("GetNextMsg", status);
			}
			else {
				GMSEC_String tempSubject = new GMSEC_String();
				checker.calling("GetSubject").check(message.GetSubject(tempSubject));
				if (tempSubject.Get().equals("GMSEC.TERMINATE"))
					done = true;

				GMSEC_String s = new GMSEC_String();
				checker.calling("Message.ToXML").check(message.ToXML(s));

				Log.Info("received\n" + s);

				GMSEC_UShort kind = new GMSEC_UShort();
				checker.calling("Message.GetKind").check(message.GetKind(kind));
				if (GMSEC_MSG_REQUEST == kind.Get()) {

					String component = getStringField(message, "COMPONENT");
					if (component == null)
						throw new ExampleException("request missing COMPONENT");

					String id = getStringField(message, "MSG-ID");
					if (id == null)
						throw new ExampleException("request missing MSG-ID");

					// Set Status Code.  See API Interface Specification for available status codes.
					String replyStatus = "1";
					String replySubject = "GMSEC.MISSION.SAT_ID.RESP." + component + "." + id + "." + replyStatus;

					// Create Reply
					reply = new Message();
					checker.calling("CreateMessage:reply")
						.check(connection.CreateMessage(replySubject, GMSEC_MSG_REPLY, reply));

					// Add fields to reply
					Field field = new Field();
					field.SetFieldValueSTRING("ANSWER", "Sure looks like it!");
					checker.calling("AddField:ANSWER").check(reply.AddField(field));

					// Add the name of the component sending the Reply.
					field.SetFieldValueSTRING("COMPONENT", "GMRPL");
					checker.calling("AddField:COMPONENT").check(reply.AddField(field));

					//..dump msg
					GMSEC_String replyXML = new GMSEC_String();
					checker.calling("reply.ToXML").check(reply.ToXML(replyXML));
					Log.Info("prepared reply =>\n" + replyXML.Get());

					// Send Reply
					checker.calling("Connection.Reply")
							.check(connection.Reply(message, reply));
					Log.Info("reply sent!");

					Message tmp = message;
					message = null;
					checker.calling("DestroyMessage:received").check(connection.DestroyMessage(tmp));

					tmp = reply;
					reply = null;
					checker.calling("DestroyMessage:reply").check(connection.DestroyMessage(tmp));
				}
			}
		}

		return true;
	}
		

	public boolean cleanup() throws ExampleException {
		if (message != null && message.isValid()) {
			Util.checkNoThrow("DestroyMessage", connection.DestroyMessage(message));
		}
		
		if (reply != null && reply.isValid()) {
			Util.checkNoThrow("DestroyMessage", connection.DestroyMessage(reply));
		}
		
		if (connection != null)
			Util.closeConnection(connection);

		return false;
	}

	private String getStringField(Message message, String key) {
		String value = null;
		Field field = new Field();
		Status status = message.GetField(key, field);
		if (Util.checkNoThrow("GetField:" + key, status)) {
			GMSEC_String tmp = new GMSEC_String();
			status = field.GetValueSTRING(tmp);
			if (Util.checkNoThrow("Field.GetStringValue:" + key, status))
				value = tmp.Get();
		}
		return value;
	}

	public static void main(String[] args) {

		Example example = null;

		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmrpl(config);
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


