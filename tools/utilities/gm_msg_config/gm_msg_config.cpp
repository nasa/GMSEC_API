/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file gm_msg_config.cpp
 *
 * This file is an interactive application using the GMSEC standard 
 * C++ API.  It allows the user to select, display and publish one of 
 * several standard GMSEC messages.  The configuration file used for 
 * the example code is "PublisherConfig-GMSEC_Message_Spec-2009_May.xml".
 *
 */

#include <gmsec4_cpp.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace gmsec::api;


static std::string msgtypes[] =
{
	"LOG", "ARCHIVE-MESSAGE-RETRIEVAL-REQUEST",
	"ARCHIVE-MESSAGE-RETRIEVAL-RESPONSE", "DIRECTIVE-REQUEST",
	"DIRECTIVE-RESPONSE", "C2CX-CONFIGURATION", "C2CX-CONTROL",
	"C2CX-DEVICE", "C2CX-HEARTBEAT", "C2CX-RESOURCE",
	"TELEMETRY-CCSDS-PACKET", "TELEMETRY-CCSDS-FRAME",
	"REPLAY-TELEMETRY-REQUEST", "REPLAY-TELEMETRY-RESPONSE",
	"MNEMONIC-VALUE-REQUEST", "MNEMONIC-VALUE-RESPONSE",
	"MNEMONIC-VALUE-DATA-MESSAGE", "ARCHIVE-MNEMONIC-VALUE-REQUEST",
	"ARCHIVE-MNEMONIC-VALUE-RESPONSE", "ARCHIVE-MNEMONIC-VALUE-DATA",
	"DATABASE-ATTRIBUTES-REQUEST", "DB-RESPONSE-LIMIT-SET",
	"DB-RESPONSE-TEXT-CONVERSION", "DB-RESPONSE-CAL-CURVE",
	"DB-RESPONSE-SHORT-DESCRIPTION", "DB-RESPONSE-LONG-DESCRIPTION",
	"DB-RESPONSE-LIST-OF-MNEMONICS", "COMMAND-REQUEST", "COMMAND-RESPONSE",
	"PRODUCT-REQUEST", "PRODUCT-RESPONSE", "PRODUCT-MSG"
};

class gm_msg_config
{
public:
	gm_msg_config(const char* filename);

	~gm_msg_config();

	bool run();

private:
	std::string cfgFilename;
	ConfigFile  cfgFile;
	Connection* connection;
};


gm_msg_config::gm_msg_config(const char* filename)
	: cfgFilename(filename),
	  cfgFile(),
	  connection(0)
{
	Config tmp;
	tmp.addValue("loglevel", "info");
	tmp.addValue("logfile", "stdout");
}


gm_msg_config::~gm_msg_config()
{
	if (connection)
	{
		try
		{
			connection->disconnect();
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << e.what();
		}

		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool gm_msg_config::run()
{
	bool success = true;

	try
	{
		//o Load and parse configuration file
		cfgFile.load(cfgFilename.c_str());

		//o Retrieve config from file
		Config c1 = cfgFile.lookupConfig("GMSEC-MESSAGE-BUS");

		//o Display details of config file
		GMSEC_INFO << "Config:\n" << c1.toXML();

		//o Create connection from loaded config
		connection = Connection::create(c1);

		//o Connect to GMSEC bus
		connection->connect();

		size_t msgnum = sizeof(msgtypes) / sizeof(msgtypes[0]);
		const char *separator = "\n========================================================================\n";

		while (true)
		{
			std::string input;
			std::string msg_string;

			std::stringstream display_types;
			display_types << separator << "GMSEC Message format to displayed/publish:" << separator;

			for (size_t i = 0; i < msgnum; ++i)
			{
				display_types.width(2);
				display_types << i+1 << ") " << msgtypes[i] << "\n";
			}

			display_types << " x) Exit\n\n" << "Enter value: ";

			GMSEC_INFO << display_types.str().c_str();

			std::getline(std::cin, input);

			size_t msg_type = 0;

			if (input == "x" || input == "X")
			{
				break;
			}

			{
				Config tmp;
				tmp.addValue("X", input.c_str());
				msg_type = tmp.getIntegerValue("X", -1);
			}

			if ((msg_type >= 1) && (msg_type <= msgnum))
			{
				msg_string = msgtypes[msg_type - 1];

				Message message = cfgFile.lookupMessage(msg_string.c_str());

				//o Dump Message
				GMSEC_INFO << separator
			             << " -- Displaying Format for Message Type : "
			             << msg_string.c_str()
			             << separator
			             << message.toXML();

				//o Attempt to publish message through the GMSEC bus
				if (message.getKind() == Message::PUBLISH)
				{
					connection->publish(message);
				}
				else if (message.getKind() == Message::REQUEST)
				{
					Message* reply = connection->request(message, 3000, -1);

					if (reply)
					{
						GMSEC_INFO << "Received reply:\n" << reply->toXML();
						connection->release(reply);
					}
					else
					{
						GMSEC_WARNING << "Timeout; reply not received";
					}
				}
				else if (message.getKind() == Message::REPLY)
				{
					//TODO: demonstrate example
				}
			}
			else
			{
				GMSEC_INFO << "\n*** Entry not found, try again ***\n";
			}
		}
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "Usage: gm_msg_config <filename>" << std::endl;
		return -1;
	}

	gm_msg_config msg_config(argv[1]);

	if (msg_config.run())
	{
		GMSEC_INFO << "Completed successfully!";
	}
	else
	{
		GMSEC_ERROR << "Failed!";
		return -1;
	}
}
