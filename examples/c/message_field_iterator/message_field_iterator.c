/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file message_field_iterator.c
 * 
 * This file contains a simple example demonstrating the use of the
 * MessageFieldIterator class to iterate over the Fields of a Message.
 *
 * This example program is intended to be run prior to executing any example
 * program which publishes a GMSEC message (e.g. The publish example).
 */

#include "../utility.h"

#include <gmsec5_c.h>

#include <stdio.h>
#include <stdlib.h>


const char* typeToString(GMSEC_FieldType type)
{
	switch (type)
	{
	case GMSEC_BOOL_TYPE   : return "BOOL";
	case GMSEC_BINARY_TYPE : return "BIN";
	case GMSEC_CHAR_TYPE   : return "CHAR";
	case GMSEC_I16_TYPE    : return "I16";
	case GMSEC_I32_TYPE    : return "I32";
	case GMSEC_I64_TYPE    : return "I64";
	case GMSEC_I8_TYPE     : return "I8";
	case GMSEC_F32_TYPE    : return "F32";
	case GMSEC_F64_TYPE    : return "F64";
	case GMSEC_STRING_TYPE : return "STRING";
	case GMSEC_U16_TYPE    : return "U16";
	case GMSEC_U32_TYPE    : return "U32";
	case GMSEC_U64_TYPE    : return "U64";
	case GMSEC_U8_TYPE     : return "U8";

	default:
		GMSEC_ERROR("Unknown/bad field type");
		break;
	}

	return "UNKNOWN";
}


void CALL_TYPE fieldIteratorCallback(GMSEC_Connection connection, GMSEC_Message msg)
{
	GMSEC_INFO("Received a message with subject: %s", messageGetSubject(msg, NULL));

	GMSEC_INFO("Field Name (Field Type): Field Value");

	GMSEC_MessageFieldIterator iter = messageGetFieldIterator(msg, NULL);

	while (messageFieldIteratorHasNext(iter, NULL) == GMSEC_TRUE)
	{
		GMSEC_Field field = messageFieldIteratorNext(iter, NULL);

		// Extract the Field Name, Type, and Value
		GMSEC_INFO("%s (%s): %s", fieldGetName(field, NULL),
		                          typeToString(fieldGetType(field, NULL)),
		                          fieldGetStringValue(field, NULL));

		// Field objects can also be converted to specific Field types prior
		// to retrieval of the value contained in the Field. This is useful
		// for ensuring that data types do not lose any level of precision.
		//
		// See the getFieldValue() function (commented out at the bottom of
		// this example program) for an example of how a Field can be converted
		// to a specialized Field object.
	}

	// Acknowledge processing the message
	messageAcknowledge(msg, NULL);
}


void checkStatus(const char* operation, GMSEC_Status status)
{
	if (statusHasError(status) == GMSEC_TRUE)
	{
		GMSEC_ERROR("%s failed; reason: %s", operation, statusGet(status));
		exit(-1);
	}
}


int main(int argc, char** argv)
{
	GMSEC_Status status = statusCreate();

	// Set up connection configuration options using values from the command line.
	GMSEC_Config config = configCreateWithArgs(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Deduce subscription topic for the example program
	const char* subscription_topic;
	GMSEC_I32 level = configGetIntegerValueOrDefault(config, "gmsec-schema-level", (GMSEC_I32) GMSEC_SCHEMA_LEVEL_0, NULL);

	switch (level)
	{
	case GMSEC_SCHEMA_LEVEL_0:
		subscription_topic = "C2MS.>";
		break;

	case GMSEC_SCHEMA_LEVEL_1:
		subscription_topic = "C2MS-PIPE.>";
		break;

	case GMSEC_SCHEMA_LEVEL_2:
	default:
		subscription_topic = "GMSEC.>";
		break;
	}

	// Create connection instance.
	GMSEC_Connection conn = connectionCreate(config, status);
	checkStatus("connectionCreate", status);

	// Output information regarding the API version and connection
	GMSEC_INFO(connectionGetAPIVersion());
	GMSEC_INFO("Middleware version = %s", connectionGetLibraryVersion(conn, NULL));

	// Establish connection to the GMSEC Bus.
	connectionConnect(conn, status);
	checkStatus("connectionConnect", status);

	// Subscribe to receive messages using a callback
	GMSEC_SubscriptionInfo info = connectionSubscribeWithCallback(conn, subscription_topic, &fieldIteratorCallback, status);
	checkStatus("connectionSubscribeWithCallback", status);
	GMSEC_INFO("Subscribed to topic: %s", subscription_topic);

	// Start the AutoDispatcher to asynchronously receive messages
	connectionStartAutoDispatch(conn, status);
	checkStatus("connectionStartAutoDispatch", status);

	// Wait for user input to end the program
	GMSEC_INFO("Listening for Messages indefinitely; press <enter> to exit the program");
	getchar();

	// Clean up
	connectionUnsubscribe(conn, info, NULL);
	connectionDisconnect(conn, NULL);

	connectionDestroy(&conn);
	configDestroy(&config);
	statusDestroy(&status);

	connectionShutdownAllMiddlewares();

	return 0;
}


// This function is provided as an example of how to retrieve type-specific
// values from individual Field objects.
#if 0
void getFieldValue(GMSEC_Field field)
{
	GMSEC_Status status = statusCreate();

	switch (fieldGetType(field, NULL))
	{
	case GMSEC_BINARY_TYPE:
		{
			const GMSEC_U8* value = binaryFieldGetValue(field, status);
			checkStatus("binaryFieldGetValue", status);

			size_t length = binaryFieldGetLength(field, status);
			checkStatus("binaryFieldGetLength", status);

			// do something with the value
		}
		break;

	case GMSEC_BOOL_TYPE:
		{
			GMSEC_BOOL value = booleanFieldGetValue(field, status);
			checkStatus("booleanFieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_CHAR_TYPE:
		{
			GMSEC_CHAR value = charFieldGetValue(field, status);
			checkStatus("charFieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_I16_TYPE:
		{
			GMSEC_I16 value = i16FieldGetValue(field, status);
			checkStatus("i16FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_I32_TYPE:
		{
			GMSEC_I32 value = i32FieldGetValue(field, status);
			checkStatus("i32FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_I64_TYPE:
		{
			GMSEC_I64 value = i64FieldGetValue(field, status);
			checkStatus("i64FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_I8_TYPE:
		{
			GMSEC_I8 value = i8FieldGetValue(field, status);
			checkStatus("i8FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_F32_TYPE:
		{
			GMSEC_F32 value = f32FieldGetValue(field, status);
			checkStatus("f32FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_F64_TYPE:
		{
			GMSEC_F64 value = f64FieldGetValue(field, status);
			checkStatus("f64FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_STRING_TYPE:
		{
			const char* value = stringFieldGetValue(field, status);
			checkStatus("stringFieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_U16_TYPE:
		{
			GMSEC_U16 value = u16FieldGetValue(field, status);
			checkStatus("u16FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_U32_TYPE:
		{
			GMSEC_U32 value = u32FieldGetValue(field, status);
			checkStatus("u32FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_U64_TYPE:
		{
			GMSEC_U64 value = u64FieldGetValue(field, status);
			checkStatus("u64FieldGetValue", status);

			// do something with the value
		}
		break;

	case GMSEC_U8_TYPE:
		{
			GMSEC_U8 value = u8FieldGetValue(field, status);
			checkStatus("u8FieldGetValue", status);

			// do something with the value
		}
		break;

	default:
		GMSEC_ERROR("Unknown/bad field type");
		break;
	}

	statusDestroy(&status);
}
#endif
