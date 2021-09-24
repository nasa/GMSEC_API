/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file message_field_iterator.java
 *
 * This file contains a simple example demonstrating the use of the
 * MessageFieldIterator class to iterate over the Fields of a Message.
 *
 * This example program is intended to be run prior to executing any example
 * program which publishes a GMSEC message (e.g. The publish example).
 */

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.util.Log;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManager;
import gov.nasa.gsfc.gmsec.api.mist.ConnectionManagerCallback;
import gov.nasa.gsfc.gmsec.api.field.Field;
import java.util.Scanner;


class FieldIterationCallback extends ConnectionManagerCallback
{
	public void onMessage(ConnectionManager connection, Message message)
	{
		try
		{
			Log.info("Received a message with subject: " + message.getSubject());

			Log.info("Field Name (Field Type): Field Value");
			MessageFieldIterator iter = message.getFieldIterator();

			while(iter.hasNext())
			{
				Field field = iter.next();

				//o Extract the Field Name, Type, and Value (As
				// a string, to print)
				//
				// Note: 'getter' functions are also defined for
				// Integer, Unsigned Integer, and Double values.
				Log.info(field.getName() + " (" + typeToString(field.getType()) + "): " + field.getStringValue());

				//o Field objects can also be converted to
				// specific Field types prior to retrieval of
				// the value contained in the Field.  This is
				// useful for ensuring that data types do not
				// lose any level of precision, but requires
				// a more intricate implementation.
			}
		}
		catch(Exception e)
		{
			Log.error(e.getMessage());
		}
	}

	public String typeToString(Field.FieldType type) throws GMSEC_Exception
	{
		String ret_string;

		switch(type)
		{
			case BOOL_TYPE:
			ret_string = "BOOL";
			break;

			case BIN_TYPE:
			ret_string = "BIN";
			break;

			case CHAR_TYPE:
			ret_string = "CHAR";
			break;

			case I8_TYPE:
			ret_string = "I8";
			break;

			case I16_TYPE:
			ret_string = "I16";
			break;

			case I32_TYPE:
			ret_string = "I32";
			break;

			case I64_TYPE:
			ret_string = "I64";
			break;

			case F32_TYPE:
			ret_string = "F32";
			break;

			case F64_TYPE:
			ret_string = "F64";
			break;

			case STRING_TYPE:
			ret_string = "STRING";
			break;

			case U8_TYPE:
			ret_string = "U8";
			break;

			case U16_TYPE:
			ret_string = "U16";
			break;

			case U32_TYPE:
			ret_string = "U32";
			break;

			case U64_TYPE:
			ret_string = "U64";
			break;

			default:
			String oss = "Unsupported Field TYPE: " + type;
			throw new GMSEC_Exception(StatusClassification.FIELD_ERROR, StatusCode.UNKNOWN_FIELD_TYPE, oss);
		}

		return ret_string;
	}
}

public class message_field_iterator
{
	public static final String DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.>";

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.out.println("usage: java message_field_iterator mw-id=<middleware ID>");
			System.exit(-1);
		}

		//o Load the command-line input into a GMSEC Config object
		// A Config object is basically a key-value pair map which
		// is used to pass configuration options into objects such
		// as Connection objects, ConnectionManager objects, Subscribe
		// and Publish calls, Message objects, etc.
		Config config = new Config(args);

		//o If it was not specified in the command-line arguments, set LOGLEVEL
		// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
		// on the terminal/command line
		initializeLogging(config);

		//o Print the GMSEC API version number using the GMSEC Logging
		// interface
		// This is useful for determining which version of the API is
		// configured within the environment
		Log.info(ConnectionManager.getAPIVersion());

		try
		{
			ConnectionManager connMgr = new ConnectionManager(config);

			Log.info("Opening the connection to the middleware server");
			connMgr.initialize();

			Log.info(connMgr.getLibraryVersion());

			Log.info("Subscribing to the topic: " + DEFAULT_SUBSCRIPTION_SUBJECT);
			FieldIterationCallback cb = new FieldIterationCallback();
			connMgr.subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, cb);

			//o Start the AutoDispatcher to begin asynchronously processing
			// messages
			connMgr.startAutoDispatch();

			//o Wait for user input to end the program
			Log.info("Listening for Messages indefinitely, press <enter> to exit the program");
			Scanner scanner = new Scanner(System.in);
			scanner.nextLine();

			connMgr.stopAutoDispatch();
			connMgr.cleanup();
		}
		catch(Exception e)
		{
			Log.error(e.getMessage());
		}
	}

	public static void initializeLogging(Config config)
	{
		// If it was not specified in the command-line arguments, set
		// LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the
		// program report output on the terminal/command line
		try
		{
			String logLevel = config.getValue("LOGLEVEL");
			String logFile = config.getValue("LOGFILE");

			if (logLevel == null)
			{
				config.addValue("LOGLEVEL", "INFO");
			}
			if (logFile == null)
			{
				config.addValue("LOGFILE", "STDOUT");
			}
		}
		catch(Exception e)
		{
	  		Log.error(e.toString());
		}
	}
}
