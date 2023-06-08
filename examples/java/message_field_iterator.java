/*
 * Copyright 2007-2023 United States Government as represented by the
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

import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.field.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.Scanner;


class FieldIterationCallback extends Callback
{
	public void onMessage(Connection connection, Message msg)
	{
		Log.info("Received a message with subject: " + msg.getSubject());

		Log.info("Field Name (Field Type): Field Value");

		MessageFieldIterator iter = msg.getFieldIterator();

		while (iter.hasNext())
		{
			Field field = iter.next();

			// Extract the Field Name, Type, and Value
			Log.info(field.getName()
			         + " (" + typeToString(field.getType()) + "): "
			         + field.getStringValue());

			// Field objects can also be converted to specific Field types prior
			// to retrieval of the value contained in the Field. This is useful
			// for ensuring that data types do not lose any level of precision.
			//
			// See getFieldValue() (commented out at the bottom of this example
			// program) for an example of how a Field can be converted to a
			// specialized Field object.
		}

		// Acknowledge processing the message
		msg.acknowledge();
	}


	private static String typeToString(Field.Type type)
	{
		switch (type)
		{
		  case BOOL   : return "BOOL";
		  case BINARY : return "BIN";
		  case CHAR   : return "CHAR";
		  case I16    : return "I16";
		  case I32    : return "I32";
		  case I64    : return "I64";
		  case I8     : return "I8";
		  case F32    : return "F32";
		  case F64    : return "F64";
		  case STRING : return "STRING";
		  case U16    : return "U16";
		  case U32    : return "U32";
		  case U64    : return "U64";
		  case U8     : return "U8";
		}

		return "UNKNOWN";
	}
}


public class message_field_iterator
{
	public static void main(String[] args)
	{
		// Set up connection configuration options using values from the command line.
		Config config = new Config(args);

		// Initialize log level for output
		utility.initializeLogLevel(config);

		// Deduce subscription topic for the example program
		String subscription_topic;
		int level = config.getIntegerValue("gmsec-schema-level", Specification.SchemaLevel.LEVEL_0.getValue());

		switch (Specification.SchemaLevel.valueOf(level))
		{
		case LEVEL_0:
			subscription_topic = "C2MS.>";
			break;

		case LEVEL_1:
			subscription_topic = "C2MS-PIPE.>";
			break;

		case LEVEL_2:
		default:
			subscription_topic = "GMSEC.>";
			break;
		}

		try
		{
			// Create connection instance.
			Connection conn = Connection.create(config);

			// Establish connection to the GMSEC Bus.
			conn.connect();

			// Output information regarding the API version and connection
			Log.info(Connection.getAPIVersion());
			Log.info("Middleware version = " + conn.getLibraryVersion());

			// Subscribe to receive messages using a callback
			FieldIterationCallback cb = new FieldIterationCallback();
			conn.subscribe(subscription_topic, cb);
			Log.info("Subscribed to topic: " + subscription_topic);

			// Start the AutoDispatcher to asynchronously receive messages
			conn.startAutoDispatch();

			// Wait for user input to end the program
			Log.info("Listening for Messages indefinitely; press <enter> to exit the program");
			new Scanner(System.in).nextLine();

			// Clean up
			conn.stopAutoDispatch();
			conn.disconnect();

			// Destroy the connection instance
			Connection.destroy(conn);
		}
		catch (GmsecException e)
		{
			Log.error(e.getMessage());
		}
	}


	// This method is provided as an example of how to retrieve type-specific
	// values from individual Field objects.
	/*
	private void getFieldValue(Field field)
	{
		switch (field.getType())
		{
		case BOOL:
			{
				boolean value = ((BooleanField) field).getValue();
				// do something with the value
			}
			break;
	
		case BINARY:
			{
				byte[] value = ((BinaryField) field).getValue();
				// do something with the value
			}
			break;
	
		case CHAR:
			{
				char value = ((CharField) field).getValue();
				// do something with the value
			}
			break;
	
		case I16:
			{
				short value = ((I16Field) field).getValue();
				// do something with the value
			}
			break;
	
		case I32:
			{
				int value = ((I32Field) field).getValue();
				// do something with the value
			}
			break;
	
		case I64:
			{
				long value = ((I64Field) field).getValue();
				// do something with the value
			}
			break;
	
		case I8:
			{
				byte value = ((I8Field) field).getValue();
				// do something with the value
			}
			break;
	
		case F32:
			{
				float value = ((F32Field) field).getValue();
				// do something with the value
			}
			break;
	
		case F64:
			{
				double value = ((F64Field) field).getValue();
				// do something with the value
			}
			break;
	
		case STRING:
			{
				String value = ((StringField) field).getValue();
				// do something with the value
			}
			break;
	
		case U16:
			{
				U16 value = ((U16Field) field).getValue();
				// do something with the value
			}
			break;
	
		case U32:
			{
				U32 value = ((U32Field) field).getValue();
				// do something with the value
			}
			break;
	
		case U64:
			{
				U64 value = ((U64Field) field).getValue();
				// do something with the value
			}
			break;
	
		case U8:
			{
				U8 value = ((U8Field) field).getValue();
				// do something with the value
			}
			break;
		}
	}
	*/
}
