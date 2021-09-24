/*
 * Copyright 2007-2021 United States Government as represented by the
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
#include <vector>

using namespace gmsec::api;
using namespace gmsec::api::mist;


class gm_msg_config
{
public:
	gm_msg_config(const char* filename);

	~gm_msg_config();

	bool run();

private:
	std::string cfgFilename;
};


gm_msg_config::gm_msg_config(const char* filename)
	: cfgFilename(filename)
{
}


gm_msg_config::~gm_msg_config()
{
}


bool gm_msg_config::run()
{
	bool success = true;

	try
	{
		//o Load and parse configuration file
		ConfigFile cfgFile;
		cfgFile.load(cfgFilename.c_str());

		//o Retrieve config from file
		Config cfg = cfgFile.lookupConfig("Bolt Configuration");

		//o Display details of config file
		GMSEC_INFO << "Config:\n" << cfg.toXML();

		//o Create manifest of message(s) available from ConfigFile
		std::vector<ConfigFile::MessageEntry> availableMessages;

		const ConfigFileIterator& iter = cfgFile.getIterator();

		while (iter.hasNextMessage())
		{
			const ConfigFile::MessageEntry& entry = iter.nextMessage();

			availableMessages.push_back(entry);
		}

		//o Create ConnectionManager (and underlying Specification) from previously loaded config
		ConnectionManager connMgr(cfg);

		const char *separator = "\n========================================================================\n";

		while (true)
		{
			std::string input;
			std::string msg_string;

			std::stringstream display_types;
			display_types << separator << "GMSEC Message to validate:" << separator;

			for (size_t i = 0; i < availableMessages.size(); ++i)
			{
				display_types.width(2);
				display_types << i+1 << ") " << availableMessages[i].getName() << "\n";
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

			if ((msg_type >= 1) && (msg_type <= availableMessages.size()))
			{
				const char*    msgName = availableMessages[ msg_type - 1 ].getName();
				const Message& msg     = availableMessages[ msg_type - 1 ].getMessage();

				//o Display XML representation of the Message
				GMSEC_INFO << separator
				           << " -- Displaying Format for Message Type : "
				           << msgName
				           << separator
				           << msg.toXML();

				//o Validate the message
				try
				{
					connMgr.getSpecification().validateMessage( msg );

					GMSEC_INFO << "Message is valid!";
				}
				catch (const Exception& e)
				{
					GMSEC_ERROR << "Message is not valid!\n" << e.what();
				}
			}
			else
			{
				GMSEC_INFO << "\n*** Entry not found, try again ***\n";
			}
		}
	}
	catch (const Exception& e)
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

	// Enable logging
	Config config(argc, argv);
	config.addValue("loglevel", config.getValue("loglevel", "info"));
	config.addValue("logfile",  config.getValue("logfile", "stderr"));

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
