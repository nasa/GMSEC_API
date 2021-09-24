/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file PublishThread.cpp
 * 
 * This file contains a simple example publisher using the GMSEC standard C++ API.
 * 
 */

#include <gmsec_cpp.h>
#include "PublishThread.hpp"
#include "../example.h"

#include <string>

using namespace std;
using namespace gmsec;
using namespace example;


/* constructor - create the connection */
PublishThread::PublishThread(Config &c) :
		config(c), connection(0) {

	initialize (config);

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connect", result);

}

PublishThread::~PublishThread() {

	if (connection) {
		result = ConnectionFactory::Destroy(connection);
		checkNoThrow("Destroying Connection", result);
	}

}

void CALL_TYPE PublishThread::run() {

	/* output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* output connection middleware version */
	LOG_INFO << connection->GetLibraryVersion();

	Publish("GMSEC.TEST.PUBLISH");
	Publish("GMSEC.TEST1.PUBLISH");
	Publish("GMSEC.TEST2.PUBLISH");
	Publish("GMSEC.TEST2.PUBLISH.ANOTHER");
	Publish("GMSEC.TEST.QUIT");

	/* Disconnect */
	result = connection->Disconnect();
	check("Disconnecting", result);

	/* Destroy the Connection */
	result = ConnectionFactory::Destroy(connection);
	check("Destroying Connection", result);

	return;

}

int PublishThread::Publish(const char * subject){

	Field fd;
	const char *tmp;
	int i = 123;
	Status localResult;
	Message *message = 0;

	localResult = connection->CreateMessage(subject, GMSEC_MSG_PUBLISH, message);
	check("CreateMessage", localResult);

	/* Add Fields */
	fd.SetName("C");
	fd.SetValue((GMSEC_CHAR) 'c');
	message->AddField(fd);

	fd.SetName("F");
	fd.SetValue(GMSEC_FALSE);
	message->AddField(fd);

	fd.SetName("I");
	fd.SetValue((GMSEC_I32) i);
	message->AddField(fd);

	fd.SetName("K");
	fd.SetValue((GMSEC_U16) i);
	message->AddField(fd);

	fd.SetName("S");
	fd.SetValue("This is a test");
	message->AddField(fd);

	fd.SetName("D");
	fd.SetValue(GMSEC_F64(1 + 1));
	message->AddField(fd);

	fd.SetName("X");
	fd.SetValue((GMSEC_BIN) "JLMNOPQ", 7);
	message->AddField(fd);

	/* Publish Message */
	localResult = connection->Publish(message);
	check("Publish", localResult);

	/* Dump Message */
	localResult = message->ToXML(tmp);
	check("ToXML", localResult);
	LOG_INFO << tmp;

	/* Destroy the Message */
	localResult = connection->DestroyMessage(message);
	check("DestroyingMessage", localResult);

	return 0;
}
