/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gmsub.java
 *
 * An example Java GMSEC publisher.
 */

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;

public class gmsub implements Example
{

	StatusChecker checker;
	Config config;
	Connection connection;
	Message message;

	gmsub(Config config) throws ExampleException {
		checker = new StatusChecker();
		this.config = config;
		Util.initialize(config);

		if (!Util.hasConnectionType(config)) {
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}

	private void printUsage() {
		Log.Error("\nusage: java gmsub connectionType=<middleware> " +
				"[ <parameter>=<value> ]\n" +
				"\n\tNote that the parameter 'connectiontype' is required. "  +
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " + 
				"(required if middleware is not bolt/MB locally)" +
				"\n\tsubject=<subject name>" +
				"\n\titerations=<iterations>" +
				"\n\tsubject.<N>=<sub-subject name>" +
				"\n\t\tNote: N must be 1 or greater" +
				"\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B" +
				"\n\tmsg_timeout_ms=<timeout period (milliseconds)>" +
				"\n\tprog_timeout_s=<timeout period (seconds)>" +
				"\n\t\tNote: msg_timeout_ms must be specified for prog_timeout_s" +
				"\n\tloglevel=<log level>" +
				"\n\tcfgfile=<config_filepath.xml>" +
				"\n\nFor more information, see API User's Guide\n\n");
	}

	public boolean run() throws ExampleException {

		Status status;

		// determine what subjects to listen to
		java.util.List<String> subjects = new java.util.ArrayList<String>(10);

		String value = Util.get(config,"SUBJECT");
		if (value != null) {
			subjects.add(value);
		}
		else {
			for (int i = 1; ; ++i) {
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
		}
		else {
			subjects.add("GMSEC.TERMINATE");
		}

		int iterations = Util.get(config, "ITERATIONS", 0);
		if (iterations > 0)
			Log.Info("waiting for up to " + iterations + " messages");

		int msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS",
											Util.EX_MSG_TIMEOUT_MS);
		int prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S",
											Util.EX_PROG_TIMEOUT_S);
        if(msg_timeout_ms == GMSEC_WAIT_FOREVER && 
									prog_timeout_s != GMSEC_WAIT_FOREVER)
        {
           Log.Error("Usage: To use prog_timeout_s, msg_timeout_ms must be defined!");
           return false;
        }                          

		// Establish a connection
		connection = Util.openConnection(config);

		// Output GMSEC API version
		Log.Info(ConnectionFactory.GetAPIVersion());
		
		// Output middleware version
		Log.Info("Middleware version = " + connection.GetLibraryVersion());

		// Subscribe
		for (String subject : subjects) {
			Log.Info("subscribing to " + subject);
			checker.calling("Subscribe(" + subject + ")")
				.check(connection.Subscribe(subject));
		}

		// Wait for messages
		int count = 0;
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
			if (status.isError()) {
				if (status.GetCode() == GMSEC_TIMEOUT_OCCURRED)
					Log.Info("timeout occurred");
				else
					checker.check(status);
			}
			
			nextTime = System.currentTimeMillis();
			elapsedTime += (int) (nextTime - prevTime);
			prevTime = nextTime;
			
			
			if (message.isValid()) {
				++count;
				if (iterations > 0 && count >= iterations)
					done = true;

				GMSEC_String xml = new GMSEC_String();
				checker.calling("Message.ToXML").check(message.ToXML(xml));
				Log.Info("received \n" + xml.Get());

				GMSEC_String subject = new GMSEC_String();
				checker.calling("GetSubject").check(message.GetSubject(subject));
				if (subject.Get().equals("GMSEC.TERMINATE"))
					done = true;

				Message tmp = message;
				message = null;
				checker.calling("DestroyMessage")
						.check(connection.DestroyMessage(tmp));
			}
		}

		return true;
	}


	public boolean cleanup() throws ExampleException {
		if (message != null && message.isValid()) {
			Status status = connection.DestroyMessage(message);
			Util.checkNoThrow("DestroyMessage", status);
		}

		if (connection != null)
			Util.closeConnection(connection);

		return true;
	}

	public static void main(String[] args) {

		Example example = null;

		try {
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmsub(config);
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

