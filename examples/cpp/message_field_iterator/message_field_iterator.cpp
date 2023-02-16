/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file message_field_iterator.cpp
 * 
 * This file contains a simple example demonstrating the use of the
 * MessageFieldIterator class to iterate over the Fields of a Message.
 *
 * This example program is intended to be run prior to executing any example
 * program which publishes a GMSEC message (e.g. The publish example).
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

#include <memory>
#include <string>
#include <iostream>

using namespace gmsec::api5;


class FieldIterationCallback : public Callback
{
public:
	virtual void CALL_TYPE onMessage(Connection& connection, const Message& msg)
	{
		try
		{
			GMSEC_INFO << "Received a message with subject: " << msg.getSubject();

			GMSEC_INFO << "Field Name (Field Type): Field Value";

			MessageFieldIterator& iter = msg.getFieldIterator();

			while (iter.hasNext())
			{
				const Field& field = iter.next();

				// Extract the Field Name, Type, and Value
				GMSEC_INFO << field.getName()
				           << " (" << typeToString(field.getType()).c_str() << "): "
				           << field.getStringValue();

				// Field objects can also be converted to specific Field types prior
				// to retrieval of the value contained in the Field. This is useful
				// for ensuring that data types do not lose any level of precision.
				//
				// See the getFieldValue() function (commented out at the bottom of
				// this example program) for an example of how a Field can be converted
				// to a specialized Field object.
			}

			// Acknowledge processing the message
			msg.acknowledge();
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << e.what();
		}
	}


private:
	static std::string typeToString(Field::Type type)
	{
		switch (type)
		{
		case Field::Type::BOOL   : return "BOOL";
		case Field::Type::BINARY : return "BIN";
		case Field::Type::CHAR   : return "CHAR";
		case Field::Type::I16    : return "I16";
		case Field::Type::I32    : return "I32";
		case Field::Type::I64    : return "I64";
		case Field::Type::I8     : return "I8";
		case Field::Type::F32    : return "F32";
		case Field::Type::F64    : return "F64";
		case Field::Type::STRING : return "STRING";
		case Field::Type::U16    : return "U16";
		case Field::Type::U32    : return "U32";
		case Field::Type::U64    : return "U64";
		case Field::Type::U8     : return "U8";
		}

		return "UNKNOWN";
	}
};


int main(int argc, char** argv)
{
	// Set up connection configuration options using values from the command line.
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	// Deduce subscription topic for the example program
	const char* subscription_topic;
	int level = config.getIntegerValue("gmsec-schema-level", static_cast<GMSEC_I32>(Specification::SchemaLevel::LEVEL_0));

	switch ( static_cast<Specification::SchemaLevel>(level) )
	{
	case Specification::SchemaLevel::LEVEL_0:
		subscription_topic = "C2MS.>";
		break;

	case Specification::SchemaLevel::LEVEL_1:
		subscription_topic = "C2MS-PIPE.>";
		break;

	case Specification::SchemaLevel::LEVEL_2:
	default:
		subscription_topic = "GMSEC.>";
		break;
	}

	std::unique_ptr<Connection> conn;

	try
	{
		// Create connection instance.
		conn.reset( new Connection(config) );

		// Output information regarding the API version and connection
		GMSEC_INFO << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << conn->getLibraryVersion();

		// Establish connection to the GMSEC Bus.
		conn->connect();

		// Subscribe to receive messages using a callback
		FieldIterationCallback cb;
		conn->subscribe(subscription_topic, &cb);
		GMSEC_INFO << "Subscribed to topic: " << subscription_topic;

		// Start the AutoDispatcher to asynchronously receive messages
		conn->startAutoDispatch();

		// Wait for user input to end the program
		GMSEC_INFO << "Listening for Messages indefinitely; press <enter> to exit the program";
		std::string enter;
		std::getline(std::cin, enter);

		// Clean up
		conn->stopAutoDispatch();
		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << e.what();
	}

	conn.release();

	Connection::shutdownAllMiddlewares();
}


// This function is provided as an example of how to retrieve type-specific
// values from individual Field objects.
#if 0
void getFieldValue(const Field& field)
{
	switch (field.getType())
	{
	case Field::BOOL:
		{
			bool value = dynamic_cast<const BooleanField&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::BINARY:
		{
			const GMSEC_U8* value  = dynamic_cast<const BinaryField&>(field).getValue();
			const size_t    length = dynamic_cast<const BinaryField&>(field).getLength();
			// do something with the value
		}
		break;

	case Field::CHAR:
		{
			GMSEC_CHAR value = dynamic_cast<const CharField&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::I16:
		{
			GMSEC_I16 value = dynamic_cast<const I16Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::I32:
		{
			GMSEC_I32 value = dynamic_cast<const I32Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::I64:
		{
			GMSEC_I64 value = dynamic_cast<const I64Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::I8:
		{
			GMSEC_I8 value = dynamic_cast<const I8Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::F32:
		{
			GMSEC_F32 value = dynamic_cast<const F32Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::F64:
		{
			GMSEC_F64 value = dynamic_cast<const F64Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::STRING:
		{
			const char* value = dynamic_cast<const StringField&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::U16:
		{
			GMSEC_U16 value = dynamic_cast<const U16Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::U32:
		{
			GMSEC_U32 value = dynamic_cast<const U32Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::U64:
		{
			GMSEC_U64 value = dynamic_cast<const U64Field&>(field).getValue();
			// do something with the value
		}
		break;

	case Field::U8:
		{
			GMSEC_U8 value = dynamic_cast<const U8Field&>(field).getValue();
			// do something with the value
		}
		break;
	}
}
#endif
