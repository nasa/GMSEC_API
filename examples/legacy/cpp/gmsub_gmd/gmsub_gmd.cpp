/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gmsub_gmd.cpp
 * 
 * This file contains a simple example of SmartSockets GMD subscribing.
 * For more information about SmartSockets GMD please refer to the
 * SmartSockets User's Guide.
 * 
 */

#include "gmsec_cpp.h"
#include "../example.h"

#include <iostream>
#include <string>

using namespace std;
using namespace gmsec;
using namespace example;

inline bool gmsub_gmd_printUsage(const gmsec::Config &config, int nbr_of_args)
{
	size_t conn_type_length = get(config, "connectionType").length();
	size_t mw_id_length = get(config, "mw-id").length();
	size_t server_length = get(config, "server").length();

	if ((nbr_of_args <= 1) || (conn_type_length == 0 && mw_id_length == 0) || server_length == 0) {
		std::cout << "\nusage: gmsub_gmd"
		          << " connectiontype=<middleware> server=<server>"
		          << " [ msg_timeout_ms=<timeout (ms)> ]"
		          << "\n\nFor more infomration, see API SmartSockets User's Guide\n";

		return true;
	}

	return false;
}


class gmsub_gmd {

public:

	gmsub_gmd(Config &c);
	~gmsub_gmd();

	Status Run();

private:

	Config &config;
	Connection * connection;
	Message * message;

};

gmsub_gmd::gmsub_gmd(Config &c) :
		config(c), connection(0), message(0) {

	/* Initialize config */
	initialize(c);

}

gmsub_gmd::~gmsub_gmd() {

	if (message)
		checkNoThrow("Cleaning up Message", connection->DestroyMessage(message));

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmsub_gmd::Run() {

	Status result;

	/* Not needed in production, but it allows you to see what
	 options actually get set in SS (as a sanity check)
	 See GMSEC API Users Guide for other settings. */
	config.AddValue("COMMAND_FEEDBACK", "always");

	/* Add the gmdSubject to the connection configuration. */
	config.AddValue("gmdSubject", "GMSEC.GMD_Subject_subscriber");

	/* Set the Server mode, so it 'remembers' if you leave
	 and come back.  See GMSEC API Users Guide for other settings. */
	config.AddValue("SERVER_DISCONNECT_MODE", "warm");

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();
	
	int msg_timeout_ms = get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS);

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connect", result);

	/* Subscribe */
	LOG_INFO << "Subscribing->GMSEC.TEST.PUBLISH";
	result = connection->Subscribe("GMSEC.TEST.PUBLISH");
	check("Subscribe", result);

	/* Listen */
	Field field;

	bool done = false;
	while(!done)
	{
		result = connection->GetNextMsg(message, msg_timeout_ms);
		if (result.isError()) {
			// do not abort on timeouts
			if (result.GetCode() == GMSEC_TIMEOUT_OCCURRED)
			{
				LOG_INFO << "timeout occurred";
				continue;
			}
			else
			{
				check("GetNextMsg", result);
			}
		}

		GMSEC_U32 seq;
		result = message->GetField("sequence number", field);
		check("GetField", result);

		if (GMSEC_STATUS_NO_ERROR == result.GetClass()) {

			field.GetValue(seq);
			LOG_INFO << "sequence number->" << seq;

		}

		/* You must Destroy the incoming message to Acknowledge
		 the GMD message was recieved. */
		result = connection->DestroyMessage(message);
		check("DestroyMessage", result);

	}

	return result;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (gmsub_gmd_printUsage(config, argc))
		return -1;

	try {

		gmsub_gmd state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}



