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

#include <gmsec_cpp.h>
#include "../example.h"
#include <gmsec/util/sysutil.h>
#include <iostream>
#include <string>

using namespace std;
using namespace gmsec;
using namespace example;


static string msgtypes[] = { "LOG", "ARCHIVE-MESSAGE-RETRIEVAL-REQUEST",
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
		"PRODUCT-REQUEST", "PRODUCT-RESPONSE", "PRODUCT-MSG" };

class gm_msg_config {

public:

	gm_msg_config(ConfigFile &c);
	~gm_msg_config();
	Status Run();

private:

	ConfigFile &cf;
	Connection * connection;

};

gm_msg_config::gm_msg_config(ConfigFile &c) :
		cf(c), connection(0) {

	Config tmp;
	initialize(tmp);

}

gm_msg_config::~gm_msg_config() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gm_msg_config::Run() {

	Status result;

	/* Load and Parse */
	result = cf.Load();
	check("ConfigFileLoad", result);

	/* Retrieve config from file */
	Config c1;
	cf.LookupConfig("GMSEC-MESSAGE-BUS", &c1);

	/* dump to show details */
	const char *xml;
	result = c1.ToXML(xml);
	check("ToXML", result);
	LOG_INFO << xml;

	/* Create connection from loaded Config */
	result = ConnectionFactory::Create(&c1, connection);
	check("Creating Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connecting", result);

	int msgnum = (int) (sizeof(msgtypes) / sizeof(msgtypes[0]));

	bool valid = true;
	const char *separator = "\n========================================================================\n";

	while (true) {

		string input;
		string msg_string;

		/* Create message from config entry */
		Message *message = 0;

		result = connection->CreateMessage(message);
		check("CreateMessage", result);

		stringstream display_types;
		display_types << separator
				<< "GMSEC Message format to displayed/publish:"
				<< separator;

		for (int i = 0; i < msgnum; i++) {
			display_types.width(2);
			display_types << i+1 << ") " << msgtypes[i] << "\n";
		}

		display_types << " x) Exit\n\n" << "Enter value: ";
		LOG_INFO << display_types.str();

		if (!valid) {
			LOG_INFO << "\nEntry not found, try again\n";
		}

		cin >> input;

		int msg_type = 0;

		if (input == "x" || input == "X")
			break;

		{
			Config tmp;
			tmp.AddValue("X", input.c_str());
			msg_type = get(tmp, "X", -1);
		}

		if ((msg_type >= 1) && (msg_type <= msgnum)) {

			valid = true;

			msg_string = msgtypes[msg_type - 1];

			result = cf.LookupMessage(msg_string.c_str(), message);
			check("LookupMessage", result);

			result = message->ToXML(xml);
			check("ToXML", result);

			/* Dump Message */
			LOG_INFO
				<< separator
				<< " -- Displaying Format for Message Type : "
				<< msg_string
				<< separator
				<< xml;

			/* Publish Message */
			result = connection->Publish(message);
			check("Publish", result);

			/* Destroy the Message */
			result = connection->DestroyMessage(message);
			check("DestroyMessage", result);

		}
		else
			valid = false;
	}

	return result;

}

int main(int argc, char* argv[]) {

	if (argc <= 1) {

		cout << "usage: gm_msg_config <filename>" << endl;
		return -1;

	}

	ConfigFile cf(argv[1]);

	try {

		gm_msg_config state(cf);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}
