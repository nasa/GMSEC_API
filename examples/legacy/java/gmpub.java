/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gmpub.java
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
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.GMSEC_MSG_PUBLISH;
import gov.nasa.gsfc.gmsecapi.util.*;


public class gmpub implements Example
{

	Config config;
	Connection connection;
	Message message;
	StatusChecker checker = new StatusChecker();


	gmpub(Config config) throws ExampleException {
		this.config = config;
		Util.initialize(config);

		if (!Util.hasConnectionType(config)) {
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}

	private void printUsage() {
		Log.Error("\nusage: java gmpub connectionType=<middleware> " +
				"[ <parameter>=<value> ]\n" +
				"\n\tNote that the parameter 'connectiontype' is required. "  +
				"\n\tThe rest of other parameters are optional.\n" +
				"\n\tserver=<server name> " +
				"(required if middleware is not bolt/MB locally)" +
				"\n\tsubject=<subject name>" +
				"\n\titerations=<iterations>" +
				"\n\tinterval_ms=<interval (milliseconds)>" +
				"\n\tloglevel=<log level>" +
				"\n\tcfgfile=<config_filepath.xml>" +
				"\n\nFor more information, see API User's Guide\n\n");
	}

	public boolean run() throws ExampleException {

		// Output GMSEC API version
		Log.Info(ConnectionFactory.GetAPIVersion());

		String subject = Util.get(config, "SUBJECT", "GMSEC.TEST.PUBLISH");
		int iterations = Util.get(config, "ITERATIONS", 1);
		int interval_ms = Util.get(config, "INTERVAL_MS", 1000);

		connection = Util.openConnection(config);

		// Output middleware version
		Log.Info("Middleware version = " + connection.GetLibraryVersion());

		// Publish messages
		for (int i = 0; i < iterations; ++i) {
			publish(i, subject);
			if (i < iterations - 1)
				Util.millisleep(interval_ms);
		}

		return true;
	}

	void publish(int iteration, String subject) throws ExampleException {

		Status status;
		Field field = new Field();
		int value = iteration + 1;

		message = new Message();

		checker.calling("CreateMessage")
				.check(connection.CreateMessage(subject, GMSEC_MSG_PUBLISH, message));

		// Add Fields
		field.SetFieldValueCHAR("C", (byte) 'c');
		checker.calling("AddField(<GMSEC_CHAR>)")
				.check(message.AddField(field));

		field.SetFieldValueBOOL("T", true);
		checker.calling("AddField(<GMSEC_TRUE>)")
				.check(message.AddField(field));

		field.SetFieldValueBOOL("F", false);
		checker.calling("AddField(<GMSEC_false>)")
				.check(message.AddField(field));

		field.SetFieldValueI16("J", (short)value);
		checker.calling("AddField(<GMSEC_I16>)")
				.check(message.AddField(field));

		field.SetFieldValueI16("K", (short)value);
		checker.calling("AddField(<GMSEC_U16>)")
				.check(message.AddField(field));

		field.SetFieldValueI32("I", value);
		checker.calling("AddField(<GMSEC_I32>)")
				.check(message.AddField(field));

		field.SetFieldValueI32("COUNT", iteration);
		checker.calling("AddField(COUNT)")
				.check(message.AddField(field));

		field.SetFieldValueU32("U", value);
		checker.calling("AddField(<GMSEC_U32>)")
				.check(message.AddField(field));

		field.SetFieldValueSTRING("S", "This is a test");
		checker.calling("AddField(<GMSEC_STR>)")
				.check(message.AddField(field));

		field.SetFieldValueF32("E", (float)(1 + 1. / value));
		checker.calling("AddField(<GMSEC_F32>)")
				.check(message.AddField(field));

		field.SetFieldValueF64("D", 1 + 1. / value);
		checker.calling("AddField(<GMSEC_F64>)")
				.check(message.AddField(field));

		field.SetFieldValueBIN("X", new String("JLMNOPQ").getBytes());
		checker.calling("AddField(<GMSEC_BIN>)")
				.check(message.AddField(field));

		GMSEC_String config_encrypt_value = new GMSEC_String();

		config.GetValue("ENCRYPT", config_encrypt_value);

		if (config_encrypt_value != null)
		{
			if (config_encrypt_value.Get() != null)
			{
				if (config_encrypt_value.Get().equalsIgnoreCase("true"))
				{
	
					field.SetFieldValueBOOL("SEC-ENCRYPT", true);
					checker.calling("AddField(SEC-ENCRYPT)")
					                .check(message.AddField(field));
	
				}
			}
		}

		// Dump Message
		GMSEC_String xml = new GMSEC_String();
		checker.calling("Message.ToXML")
				.check(message.ToXML(xml));
		Log.Info("Sending:\n" + xml.Get());

		// Publish Message
		checker.calling("Publish")
				.check(connection.Publish(message));

		Log.Info("Published!");

		// Destroy the Message
		{
			Message tmp = message;
			message = null;
			checker.calling("DestroyMessage")
					.check(connection.DestroyMessage(tmp));
		}

	}

	public boolean cleanup() throws ExampleException {
		if (message != null) {
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

			example = new gmpub(config);
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

