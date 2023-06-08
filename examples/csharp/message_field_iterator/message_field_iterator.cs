/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file message_field_iterator.cs
 * 
 * This file contains a simple example demonstrating the use of the
 * MessageFieldIterator class to iterate over the Fields of a Message.
 *
 * This example program is intended to be run prior to executing any example
 * program which publishes a GMSEC message (e.g. The publish example).
 */

using GMSEC.API5;
using System;
using System.Text;


class FieldIterationCallback : Callback
{
	public override void OnMessage(Connection connection, Message msg)
	{
		try
		{
			Log.Info("Received a message with subject: " + msg.GetSubject());

			Log.Info("Field Name (Field Type): Field Value");

			MessageFieldIterator iter = msg.GetFieldIterator();

			while (iter.HasNext())
			{
				Field field = iter.Next();

				// Extract the Field Name, Type, and Value
				Log.Info(field.GetName()
				         + " (" + typeToString(field.GetFieldType()) + "): "
				         + field.GetStringValue());

				// Field objects can also be converted to specific Field types prior
				// to retrieval of the value contained in the Field. This is useful
				// for ensuring that data types do not lose any level of precision.
				//
				// See GetFieldValue() (commented out at the bottom of this example
				// program) for an example of how a Field can be converted to a
				// specialized Field object.
			}

			// Acknowledge processing the message
			msg.Acknowledge();
		}
		catch (GmsecException e)
		{
			Log.Error(e.ToString());
		}
	}


	private static string typeToString(Field.Type type)
	{
		switch (type)
		{
		  case Field.Type.BOOL   : return "BOOL";
		  case Field.Type.BINARY : return "BIN";
		  case Field.Type.CHAR   : return "CHAR";
		  case Field.Type.I16    : return "I16";
		  case Field.Type.I32    : return "I32";
		  case Field.Type.I64    : return "I64";
		  case Field.Type.I8     : return "I8";
		  case Field.Type.F32    : return "F32";
		  case Field.Type.F64    : return "F64";
		  case Field.Type.STRING : return "STRING";
		  case Field.Type.U16    : return "U16";
		  case Field.Type.U32    : return "U32";
		  case Field.Type.U64    : return "U64";
		  case Field.Type.U8     : return "U8";
		}

		return "UNKNOWN";
	}
}


public class message_field_iterator
{
	public static int Main(string[] args)
	{
		// Set up connection configuration options using values from the command line.
		Config config = new Config(args);

		// Initialize log level for output
		InitializeLogLevel(config);

		// Deduce subscription topic for the example program
		string subscription_topic;
		int level = config.GetIntegerValue("gmsec-schema-level", (int) Specification.SchemaLevel.LEVEL_0);

		switch ((Specification.SchemaLevel) level)
		{
		case Specification.SchemaLevel.LEVEL_0:
			subscription_topic = "C2MS.>";
			break;

		case Specification.SchemaLevel.LEVEL_1:
			subscription_topic = "C2MS-PIPE.>";
			break;

		case Specification.SchemaLevel.LEVEL_2:
		default:
			subscription_topic = "GMSEC.>";
			break;
		}

		try
		{
			// Create connection instance.
			using (Connection conn = new Connection(config))
			{
				// Output information regarding the API version and connection
				Log.Info(Connection.GetAPIVersion());
				Log.Info("Middleware version = " + conn.GetLibraryVersion());

				// Establish connection to the GMSEC Bus.
				conn.Connect();

				// Subscribe to receive messages using a callback
				FieldIterationCallback cb = new FieldIterationCallback();
				conn.Subscribe(subscription_topic, cb);
				Log.Info("Subscribed to topic: " + subscription_topic);

				// Start the AutoDispatcher to asynchronously receive messages
				conn.StartAutoDispatch();

				// Wait for user input to end the program
				Log.Info("Listening for Messages indefinitely; press <enter> to exit the program");
				Console.ReadLine();

				// Clean up
				conn.StopAutoDispatch();
				conn.Disconnect();
			}
		}
		catch (GmsecException e)
		{
			Log.Error(e.ToString());
			return -1;
		}

		return 0;
	}


	private static void InitializeLogLevel(Config config)
	{
		LogLevel level = Log.FromString( config.GetValue("loglevel", "info") );
		Log.SetReportingLevel(level);
	}


	// This method is provided as an example of how to retrieve type-specific
	// values from individual Field objects.
	/*
	private void GetFieldValue(Field field)
	{
		switch (field.GetFieldType())
		{
		case Field.Type.BOOL:
			{
				bool value = ((BooleanField) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.BINARY:
			{
				byte[] value = ((BinaryField) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.CHAR:
			{
				char value = ((CharField) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.I16:
			{
				short value = ((I16Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.I32:
			{
				int value = ((I32Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.I64:
			{
				long value = ((I64Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.I8:
			{
				sbyte value = ((I8Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.F32:
			{
				float value = ((F32Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.F64:
			{
				double value = ((F64Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.STRING:
			{
				string value = ((StringField) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.U16:
			{
				ushort value = ((U16Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.U32:
			{
				uint value = ((U32Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.U64:
			{
				ulong value = ((U64Field) field).GetValue();
				// do something with the value
			}
			break;
	
		case Field.Type.U8:
			{
				byte value = ((U8Field) field).GetValue();
				// do something with the value
			}
			break;
		}
	}
	*/
}
