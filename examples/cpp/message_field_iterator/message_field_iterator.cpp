/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4_cpp.h>

#include <sstream>
#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::mist;

const char* DEFAULT_SUBSCRIPTION_SUBJECT = "GMSEC.>";

//o Helper function
void initializeLogging(Config& config);
std::string typeToString(Field::FieldType type);

class FieldIterationCallback: public ConnectionManagerCallback
{
public:
	virtual void CALL_TYPE onMessage(ConnectionManager& connection, const Message& message)
	{
		try
		{
			GMSEC_INFO << "Received a message with subject: " << message.getSubject();

			GMSEC_INFO << "Field Name (Field Type): Field Value";
			MessageFieldIterator& iter = message.getFieldIterator();
			while(iter.hasNext())
			{
				const Field& field = iter.next();

				//o Extract the Field Name, Type, and Value (As
				// a string, to print)
				//
				// Note: 'getter' functions are also defined for
				// Integer, Unsigned Integer, and Double values.
				GMSEC_INFO << field.getName() << " (" << typeToString(field.getType()).c_str() << "): " << field.getStringValue();

				//o Field objects can also be converted to
				// specific Field types prior to retrieval of
				// the value contained in the Field.  This is
				// useful for ensuring that data types do not
				// lose any level of precision, but requires
				// a more intricate implementation.
				//
				// See the getFieldValue() function (commented
				// out at the bottom of this example program)
				// for an example of how field can be done.
			}
		}
		catch(Exception e)
		{
			GMSEC_ERROR << e.what();
		}
	}
};

int main (int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	//o Load the command-line input into a GMSEC Config object
	// A Config object is basically a key-value pair map which
	// is used to pass configuration options into objects such
	// as Connection objects, ConnectionManager objects, Subscribe
	// and Publish calls, Message objects, etc.
	Config config(argc, argv);

	//o If it was not specified in the command-line arguments, set LOGLEVEL
	// to 'INFO' and LOGFILE to 'stdout' to allow the program report output
	// on the terminal/command line
	initializeLogging(config);
	
	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		ConnectionManager connMgr(config);

		GMSEC_INFO << "Opening the connection to the middleware server";
		connMgr.initialize();

		GMSEC_INFO << connMgr.getLibraryVersion();

		GMSEC_INFO << "Subscribing to the topic: " << DEFAULT_SUBSCRIPTION_SUBJECT;
		FieldIterationCallback cb;
		connMgr.subscribe(DEFAULT_SUBSCRIPTION_SUBJECT, &cb);

		//o Start the AutoDispatcher to begin asynchronously processing
		// messages
		connMgr.startAutoDispatch();

		//o Wait for user input to end the program
		std::string enter;
		GMSEC_INFO << "Listening for Messages indefinitely, press <enter> to exit the program";
		std::getline(std::cin, enter);

		//o Clean up
		connMgr.stopAutoDispatch();
		connMgr.cleanup();
	}
	catch (Exception e)
	{
		GMSEC_ERROR << e.what();
	}
}

void initializeLogging(Config& config)
{
	// If it was not specified in the command-line arguments, set
	// LOGLEVEL to 'INFO' and LOGFILE to 'stdout' to allow the 
	// program report output on the terminal/command line
	const char* logLevel  = config.getValue("LOGLEVEL");
	const char* logFile   = config.getValue("LOGFILE");

	if (!logLevel)
	{
		config.addValue("LOGLEVEL", "INFO");
	}
	if (!logFile)
	{
		config.addValue("LOGFILE", "STDERR");
	}
}

std::string typeToString(Field::FieldType type)
{
	std::string ret_string;

	switch (type)
	{
	  case Field::BOOL_TYPE:
		ret_string = "BOOL";
		break;

	  case Field::BIN_TYPE:
		ret_string = "BIN";
		break;

	  case Field::CHAR_TYPE:
		ret_string = "CHAR";
		break;

	  case Field::I8_TYPE:
		ret_string = "I8";
		break;

	  case Field::I16_TYPE:
		ret_string = "I16";
		break;

	  case Field::I32_TYPE:
		ret_string = "I32";
		break;

	  case Field::I64_TYPE:
		ret_string = "I64";
		break;

	  case Field::F32_TYPE:
		ret_string = "F32";
		break;

	  case Field::F64_TYPE:
		ret_string = "F64";
		break;

	  case Field::STRING_TYPE:
		ret_string = "STRING";
		break;

	  case Field::U8_TYPE:
		ret_string = "U8";
		break;

	  case Field::U16_TYPE:
		ret_string = "U16";
		break;

	  case Field::U32_TYPE:
		ret_string = "U32";
		break;

	  case Field::U64_TYPE:
		ret_string = "U64";
		break;

	  default:
		std::ostringstream oss;
		oss << "Unsupported Field TYPE: " << type;
		throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());

	}

	return ret_string;
}

//o This function is provided as an example of how to retrieve type-specific
// values from individual Field objects.
/*
void getFieldValue(const Field& field)
{
	int type = field.getType();

	switch (type)
	{
	  case Field::BOOL_TYPE:
		{
			bool value = dynamic_cast<const BooleanField&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::BIN_TYPE:
		{
			GMSEC_BIN value = dynamic_cast<const BinaryField&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::CHAR_TYPE:
		{
			GMSEC_CHAR value = dynamic_cast<const CharField&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::I8_TYPE:
		{
			GMSEC_I8 value = dynamic_cast<const I8Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::I16_TYPE:
		{
			GMSEC_I16 value = dynamic_cast<const I16Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::I32_TYPE:
		{
			GMSEC_I32 value = dynamic_cast<const I32Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::I64_TYPE:
		{
			GMSEC_I64 value = dynamic_cast<const I64Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::F32_TYPE:
		{
			GMSEC_F32 value = dynamic_cast<const F32Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::F64_TYPE:
		{
			GMSEC_F64 value = dynamic_cast<const F64Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::STRING_TYPE:
		{
			const char* value = dynamic_cast<const StringField&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::U8_TYPE:
		{
			GMSEC_U8 value = dynamic_cast<const U8Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::U16_TYPE:
		{
			GMSEC_U16 value = dynamic_cast<const U16Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::U32_TYPE:
		{
			GMSEC_U32 value = dynamic_cast<const U32Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  case Field::U64_TYPE:
		{
			GMSEC_U64 value = dynamic_cast<const U64Field&>(field).getValue();
			// Do something with the value
		}
		break;

	  default:
		std::ostringstream oss;
		oss << "Unsupported Field TYPE: " << type;
		throw Exception(FIELD_ERROR, UNKNOWN_FIELD_TYPE, oss.str().c_str());

	}
}
*/

