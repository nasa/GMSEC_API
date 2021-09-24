/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file gmpub_gmd.cpp
 * 
 * This file contains a simple example of SmartSockets GMD publishing
 * using the C++ GMSEC API.  For more information about SmartSockets 
 * GMD please refer to the SmartSockets User's Guide.
 * 
 */

#include "gmsec_cpp.h"
#include "../example.h"

#include <iostream>
#include <string>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


inline bool gmpub_gmd_printUsage(const gmsec::Config &config, int nbr_of_args)
{
	size_t conn_type_length = get(config, "connectionType").length();
	size_t mw_id_length = get(config, "mw-id").length();
	size_t server_length = get(config, "server").length();

	if ((nbr_of_args <= 1) || (conn_type_length == 0 && mw_id_length == 0) || server_length == 0) {
		std::cout << "\nusage: gmpub_gmd"
		          << " connectiontype=<middleware> server=<server>"
		          << "\n\nFor more infomration, see API SmartSockets User's Guide\n";

		return true;
	}

	return false;
}


class GMDCallback: public ErrorCallback {

public:

	virtual void CALL_TYPE OnError(Connection * connection, Message * message, Status * status, const char * event) {

		Status result;

		LOG_INFO << "\n-- GMD Message Failure Notification Recieved --";

		const char *tmp;
		result = message->GetSubject(tmp);
		if (!checkNoThrow("GetSubject", result))
			return;
		LOG_INFO << "GMD failure: " << tmp << " " << status->Get();

		/* Display recieved GMD Failure Message */
		const char * temp;
		result = message->ToXML(temp);
		if (checkNoThrow("ToXML", result)) {
			if (temp) {
				LOG_INFO << temp;
			}
		}

	}

};

class DispatchCallback: public ErrorCallback {

public:

	virtual void CALL_TYPE OnError(Connection * connection, Message * message, Status * status, const char * event) {

		LOG_INFO << "-- Entered DispatchCallback : OnError() --";
		LOG_INFO << "Dispatch failure: " << status->Get();
		LOG_INFO << "-- Exited DispatchCallback : OnError() --";

	}

};

class gmpub_gmd {

public:

	gmpub_gmd(Config &c);
	~gmpub_gmd();

	Status Run();

private:

	Config &config;
	Connection * connection;
	Message * message;

};

gmpub_gmd::gmpub_gmd(Config &c) :
		config(c), connection(0), message(0) {

	/* Initialize config */
	initialize(c);

}

gmpub_gmd::~gmpub_gmd() {

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

Status gmpub_gmd::Run() {

	Status result;

	/* Not needed in production, but it allows you to see what
	 options actually get set in SS (as a sanity check)
	 See GMSEC API Users Guide for other settings. */
	config.AddValue("COMMAND_FEEDBACK", "always");

	/* Add the gmdSubject to the connection configuration.
	 This subject needs to be static yet unique amongst GMD clients so
	 that the RTServer can associate a connection with each client.
	 In other words, the GMD subject used here can not be used by any
	 other client in the system. */
	config.AddValue("gmdSubject", "GMSEC.GMD_Subject_publisher");

	/* Set the Server mode, so it 'remembers' if you leave
	 and come back.  See GMSEC API Users Guide for other settings. */
	config.AddValue("SERVER_DISCONNECT_MODE", "warm");

	/* Turn on ether Memory OR File based GMD */
	//cfg.AddValue("IPC_GMD_TYPE","default");  // File based.
	config.AddValue("IPC_GMD_TYPE", "memory");

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connect", result);

	GMDCallback gmdcb;
	DispatchCallback dispcb;

	LOG_INFO << "Registering error callback";

	result = connection->RegisterErrorCallback("CONNECTION_DISPATCHER_ERROR",
			&dispcb);
	check("RegisterErrorCallback", result);

	LOG_INFO << "Registering error callback";

	result = connection->RegisterErrorCallback("SS_GMD_ERROR", &gmdcb);
	check("RegisterErrorCallback", result);

	LOG_INFO << "Starting Auto Dispatch";

	result = connection->StartAutoDispatch();
	check("StartAutoDispatch", result);

	Config msg_cfg;

	/* Required to be set on the Message for GMD.  The T_IPC_DELIVERY_ALL setting says to
	 assure delivery to ALL subscribers before sending back an ACK.
	 See GMSEC API Users Guide for other settings. */
	msg_cfg.AddValue("SETDELIVERYMODE", "T_IPC_DELIVERY_ALL");

	/* Set Delivery Timeout to 15 seconds.  The Delivery Timeout determines how long the
	 publisher is willing to wait for all subscribers to acknowledge the messaage.  It
	 also determines the size (in units of seconds) of the resend queue.  In this scenario
	 the resend queue would contain 15 seconds worth messages. */
	msg_cfg.AddValue("SETDELIVERYTIMEOUT", "15.0");

	result = connection->CreateMessage("GMSEC.TEST.PUBLISH", GMSEC_MSG_PUBLISH,
			message, &msg_cfg);
	check("CreateMessage", result);

	GMSEC_U32 j = 1;
	Field fd;
	fd.SetName("sequence number");
	fd.SetValue(j);
	result = message->AddField(fd);
	check("AddField", result);

	/* Publish Message */
	result = connection->Publish(message);
	check("Publish", result);

	LOG_INFO << "Published sequence->" << j;

	while (!result.isError()
			|| (result.GetClass() != GMSEC_STATUS_MIDDLEWARE_ERROR
					&& result.GetCode() != GMSEC_FIELD_TYPE_MISMATCH)) {

		fd.SetValue(++j);
		message->AddField(fd);

		/* Publish Message */
		result = connection->Publish(message);
		check("Publish", result);

		LOG_INFO << "Published sequence->" << j;

		/* Sleep for 1 second */
		millisleep(1000);

	}

	result = connection->DestroyMessage(message);
	check("DestroyMessage", result);

	return result;

}

int main(int argc, char* argv[]) {

	Config config(argc, argv);

	addToConfigFromFile(config);

	if (gmpub_gmd_printUsage(config, argc))
		return -1;

	try {

		gmpub_gmd state(config);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}
