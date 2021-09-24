/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gmsub_attn.java
 *
 * An example Java GMSEC subscriber which packetizes data to be sent
 *   to an Attention! generic collector agent.
 */

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Field;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.GMSEC_Bin;
import gov.nasa.gsfc.gmsecapi.GMSEC_Bool;
import gov.nasa.gsfc.gmsecapi.GMSEC_Char;
import gov.nasa.gsfc.gmsecapi.GMSEC_Double;
import gov.nasa.gsfc.gmsecapi.GMSEC_F32;
import gov.nasa.gsfc.gmsecapi.GMSEC_F64;
import gov.nasa.gsfc.gmsecapi.GMSEC_Float;
import gov.nasa.gsfc.gmsecapi.GMSEC_I8;
import gov.nasa.gsfc.gmsecapi.GMSEC_I16;
import gov.nasa.gsfc.gmsecapi.GMSEC_I32;
import gov.nasa.gsfc.gmsecapi.GMSEC_I64;
import gov.nasa.gsfc.gmsecapi.GMSEC_Long;
import gov.nasa.gsfc.gmsecapi.GMSEC_Short;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;
import gov.nasa.gsfc.gmsecapi.GMSEC_U16;
import gov.nasa.gsfc.gmsecapi.GMSEC_U32;
import gov.nasa.gsfc.gmsecapi.GMSEC_ULong;
import gov.nasa.gsfc.gmsecapi.GMSEC_UShort;
import static gov.nasa.gsfc.gmsecapi.gmsecAPI.*;
import gov.nasa.gsfc.gmsecapi.util.*;

import java.util.List;
import java.util.ArrayList;

import java.io.IOException;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.nio.charset.Charset;
import java.net.Socket;
import java.net.UnknownHostException;

public class gmsub_attn implements Example
{
	private final String ATTENTION_DELIMITER_DEFAULT = "***";

	StatusChecker checker;
	Config config;
	Connection connection;
	Message message;
	String delimiterString = "";
	List<String> includeFields;

	gmsub_attn(Config config) throws ExampleException {
		checker = new StatusChecker();
		this.config = config;
		Util.initialize(config);

		if (!Util.hasConnectionType(config))
		{
			printUsage();
			throw new ExampleException("Invalid option");
		}
	}

	private void printUsage() {
		Log.Error("\nusage: java gmsub_attn connectionType=<middleware> " +
				"[ <parameter>=<value> ]\n" +
				"\n\tNote that the parameters 'connectiontype' is required. "  +
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
				"\n\tattn-port=<Attention! server port>" +
				"\n\tattn-server=<Attention! server host>" +
				"\n\tinclude-fields=<comma-separated list of field names," +
				"\n\t\tthe content of which will be sent to the Attention!" +
				"\n\t\tserver>" +
				"\n\tdelimiter-string=<Attention! server string delimiter>" +
				"\n\nFor more information, see API User's Guide\n\n");
	}

	public boolean run() throws ExampleException
	{
		Status status;
		String strHost, strPort;

		// determine what subjects to listen to
		List<String> subjects = new ArrayList<String>(10);

		String value = Util.get(config,"SUBJECT");
		if (value != null)
		{
			subjects.add(value);
		}
		else
		{
			for (int i = 1; ; ++i)
			{
				String tmp = Util.get(config, "SUBJECT." + i);
				if (tmp != null)
				{
					subjects.add(tmp);
				}
				else
				{
					break;
				}
			}
		}

		if (subjects.isEmpty()) {
			String subject = Util.get(config, "SUBJECT", "GMSEC.>");
			subjects.add(subject);
		}
		else {
			subjects.add("GMSEC.TERMINATE");
		}
		
		// Determine the message field names to insert into the delimited string
		// in the case of an overflow situation (>1000 characters)
		includeFields = determineIncludeFields(config);

		delimiterString = determineDelimiterString(config);

		int iterations = Util.get(config, "ITERATIONS", 0);
		if (iterations > 0)
		{
			Log.Info("waiting for up to " + iterations + " messages");
		}

		int msg_timeout_ms = Util.get(config, "MSG_TIMEOUT_MS", Util.EX_MSG_TIMEOUT_MS);
		int prog_timeout_s = Util.get(config, "PROG_TIMEOUT_S", Util.EX_PROG_TIMEOUT_S);
        if(msg_timeout_ms == GMSEC_WAIT_FOREVER && prog_timeout_s != GMSEC_WAIT_FOREVER)
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
			checker.calling("Subscribe(" + subject + ")").check(connection.Subscribe(subject));
		}

		// Wait for messages
		int count = 0;
		boolean done = false;
		long prevTime = System.currentTimeMillis();
		long nextTime = 0;
		int elapsedTime = 0;	//ms
		
		int intPort = 0;

		try
		{
			strPort = Util.get(config, "ATTN-PORT", "4999");
			intPort = Integer.parseInt(strPort);
		}
		catch (NumberFormatException nfe)
		{
			Log.Error("Encountered a NumberFormatException: " + nfe.toString());
			Log.Error("Value input for ATTN-PORT is not a valid sequence of digits.");
			System.exit(1);
		}

		strHost = Util.get(config, "ATTN-SERVER", "127.0.0.1");
		
		int connectionAttempts = 0;

		while (connectionAttempts < 3)
		{
			connectionAttempts++;
			Socket socket;

			try
			{
				Log.Info("Connecting to server: " + strHost + ":" + intPort);
				socket = new Socket(strHost, intPort);

				connectionAttempts = 0;
				
				while (!done) {
				
					if(elapsedTime >= (prog_timeout_s * 1000)
										 && prog_timeout_s != GMSEC_WAIT_FOREVER)
					{
						done = true;
						Log.Info("Program Timeout Reached");
						continue;
					}
				
					message = new Message();
					
					status = connection.GetNextMsg(message, msg_timeout_ms);
					
					if (status.IsError()) {
						if (status.GetCode() == GMSEC_TIMEOUT_OCCURRED)
						{
							Log.Info("timeout occurred");
						}
						else
						{
							checker.checkNoThrow(status);
						}
					}
					
					// Get the subject of the message
					GMSEC_String gmsecSubject = new GMSEC_String();
					checker.calling("GetSubject").checkNoThrow(message.GetSubject(gmsecSubject));
					String subject = gmsecSubject.Get();
					
					nextTime = System.currentTimeMillis();
					elapsedTime += (int) (nextTime - prevTime);
					prevTime = nextTime;
					
					if (message.IsValid())
					{
						count++;
						
						if (iterations > 0 && count >= iterations)
						{
							done = true;
						}

						// Convert the GMSEC message to an XML String
						GMSEC_String gmsecXml = new GMSEC_String();
						status = message.ToXML(gmsecXml);
						String xml = gmsecXml.Get();

						if (!status.IsError())
						{
							Log.Debug("gmsub_attn Received Message:\n" + xml);

							// Create the delimited string that will be sent to the Attention Generic Agent
							String attnArgs = getAttnArgString(message, subject);
							
							byte[] data = attnArgs.getBytes("UTF-8");
							
							try
							{
								DataOutputStream out = new DataOutputStream(socket.getOutputStream());
								DataInputStream in = new DataInputStream(socket.getInputStream());
								
								// Send the byte-encrypted data to the server
								out.write(data, 0, data.length);

								// Wait for an ack (Operation blocks until data is received, no need for other logic)
								in.read(data);
								
								data = null;
							}
							catch (IOException ioe)
							{
								Log.Error("Encountered an IOException: " + ioe.toString());
								break;
							}
						}
						else
						{
							Log.Error("gmsub_attn Unable to retrieve the XML string from the message using Message::ToXML()");
						}

						Message tmp = message;
						message = null;
						checker.calling("DestroyMessage").checkNoThrow(connection.DestroyMessage(tmp));
					}
					else
					{
						Log.Error("gmsub_attn Received an invalid message according to a function call to Message::IsValid()");
					}
				}
			}
			catch (UnknownHostException uhe)
			{
				Log.Error("Encountered an UnknownHostException: " + uhe.toString());
				Log.Error("Unable to connect to the Attention! server, trying again.");
				continue;
			}
			catch (IOException ioe)
			{
				Log.Error("Encountered an IOException: " + ioe.toString());
				System.exit(1);
			}
		}

		return true;
	}

	
	private String getAttnArgString(Message message, String subject) throws ExampleException
	{
		// 5.0;gpm-auto1|<First field>|3***<Rest of the message fields>
		String attnArgs = "5.0;gpm-auto1|";
		int counter = 0;
		boolean firstTime = true;

		for (String fieldName : includeFields)
		{
			String tmpFieldValue = "";
			
			if (fieldName.equalsIgnoreCase("SUBJECT"))
			{
				tmpFieldValue = subject;
			}
			else
			{
				Field tmpField = new Field();
				message.GetField(fieldName, tmpField);
				
				if (tmpField != null)
				{
					GMSEC_UShort gmsecType = new GMSEC_UShort();
					tmpField.GetType(gmsecType);
					int fType = gmsecType.Get();

					switch (fType)
					{
						case GMSEC_TYPE_BOOL:
							GMSEC_Bool booleanValue = new GMSEC_Bool();
							tmpField.GetValueBOOL(booleanValue);
							tmpFieldValue = booleanValue.toString();
							break;
						case GMSEC_TYPE_CHAR:
							GMSEC_Char charValue = new GMSEC_Char();
							tmpField.GetValueCHAR(charValue);
							tmpFieldValue = charValue.toString();
							break;
						case GMSEC_TYPE_F32:
							GMSEC_F32 floatValue = new GMSEC_F32();
							tmpField.GetValueF32(floatValue);
							tmpFieldValue = floatValue.toString();
							break;
						case GMSEC_TYPE_F64:
							GMSEC_F64 doubleValue = new GMSEC_F64();
							tmpField.GetValueF64(doubleValue);
							tmpFieldValue = doubleValue.toString();
							break;
						case GMSEC_TYPE_I16:
							GMSEC_I16 shortValue = new GMSEC_I16();
							tmpField.GetValueI16(shortValue);
							tmpFieldValue = shortValue.toString();
							break;
						case GMSEC_TYPE_I32:
							GMSEC_I32 intValue = new GMSEC_I32();
							tmpField.GetValueI32(intValue);
							tmpFieldValue = intValue.toString();
							break;
						case GMSEC_TYPE_I64:
							GMSEC_I64 longValue = new GMSEC_I64();
							tmpField.GetValueI64(longValue);
							tmpFieldValue = longValue.toString();
							break;
						case GMSEC_TYPE_STRING:
							GMSEC_String stringValue = new GMSEC_String();
							tmpField.GetValueSTRING(stringValue);
							tmpFieldValue = stringValue.Get();
							break;
						case GMSEC_TYPE_BIN:
						case GMSEC_TYPE_I8:
						case GMSEC_TYPE_U16:
						case GMSEC_TYPE_U32:
							tmpFieldValue = "NO " + fieldName;
							break;
						// TODO: Unsigned short, int, long, unset, bin, compound
					}
				}
				else
				{
					tmpFieldValue = "NO " + fieldName;
				}
			}

			if (tmpFieldValue != null && tmpFieldValue.length() != 0)
			{
				attnArgs += tmpFieldValue;
			}
			else
			{
				attnArgs += "NO " + fieldName;
			}

			if (firstTime)
			{
				attnArgs += "|3";
				firstTime = false;
			}

			if (++counter < includeFields.size())
			{
				attnArgs += delimiterString;
			}
		}

		return attnArgs;
	}
	
	
	// Parse out the overflow field names from the comma-delimited string
	public ArrayList<String> determineIncludeFields(Config cfg)
	{
		ArrayList<String> fields = new ArrayList<String>();
		String inclFields = Util.get(cfg, "INCLUDE-FIELDS");
		if (inclFields != null && inclFields.length() > 0)
		{
			String[] inclFieldsSplit = inclFields.split(",");

			Log.Info("\nThe following fields will be concatenated:");
			for (String fieldName : inclFieldsSplit)
			{
				if (fieldName.length() != 0)
				{
					Log.Info(fieldName);
					fields.add(fieldName);
				}
			}
			Log.Info("");
		}

		return fields;
	}

	// Determine the delimiter used in the formatted string
	public String determineDelimiterString(Config cfg)
	{
		String delimiterString = Util.get(cfg, "DELIMITER-STRING");

		if (delimiterString == null)
		{
			delimiterString = ATTENTION_DELIMITER_DEFAULT;
		}

		Log.Info("Delimiter string is set to: " + delimiterString + "\n");
		return delimiterString;
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

		try
		{
			Config config = new Config(args);

			Util.addToConfigFromFile(config);

			example = new gmsub_attn(config);
			example.run();
		}
		catch (ExampleException e)
		{
			e.printStackTrace();
		}
		finally
		{
			if (example != null)
			{
				Util.cleanup(example);
			}
		}

	}

}

