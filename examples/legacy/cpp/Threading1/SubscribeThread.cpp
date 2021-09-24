/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file SubscribeThread.cpp
 *  
 * This file contains a simple example subscriber using the GMSEC standard C++ API.
 * 
 */

#include <gmsec_cpp.h>
#include "../example.h"
#include "SubscribeThread.hpp"

#include <string>

using namespace std;
using namespace gmsec;
using namespace example;


/* constructor - create the connection and subscribe */
SubscribeThread::SubscribeThread(Config &c) :
		config(c), connection(0) {

	initialize (config);

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connect", result);

	/* Subscribe */
	result = connection->Subscribe("GMSEC.>");
	check("Subscribe", result);

}

SubscribeThread::~SubscribeThread() {

	result = ConnectionFactory::Destroy(connection);
	check("Destroying Connection", result);

}

void CALL_TYPE SubscribeThread::run() {

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* output connection middleware version */
	LOG_INFO << connection->GetLibraryVersion();

	/* Listen */
	Message *message;
	result = connection->GetNextMsg(message,GMSEC_WAIT_FOREVER);
	while (result.GetClass() == GMSEC_STATUS_NO_ERROR) {

		const char *tmp = 0;

		/* Dump the message */
		result = message->ToXML(tmp);
		check("ToXML", result);
		LOG_INFO << tmp;

		// look for the message to quit, for this example
		/* Get Subject */
		const char *subject = 0;
		result = message->GetSubject(subject);
		check("GetSubject", result);

		if (subject) {
			string s(subject);
			if (s == "GMSEC.TEST.QUIT") {
				LOG_INFO << "SubscribeThread:: QUIT message found";
				break;
			}
		}

		result = connection->DestroyMessage(message);
		check("DestroyMessage", result);

		/* read next */
		result = connection->GetNextMsg(message, GMSEC_WAIT_FOREVER);

	}

	LOG_INFO << result.Get();

	/* UnSubscribe */
	result = connection->UnSubscribe("GMSEC.>");
	check("Unsubscribe", result);

	/* Disconnect */
	result = connection->Disconnect();
	check("Disconnect", result);

	/* Destroy the Message */
	result = connection->DestroyMessage(message);
	check("DestroyMessage", result);

	/* Destroy the Connection */
	result = ConnectionFactory::Destroy(connection);
	check("Destroying Connection", result);

	return;
}
