/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file logging.cpp
 * 
 * This file contains a simple example of using the GMSEC API logging framework
 * 
 */

#include <gmsec_cpp.h>
#include <gmsec/util/timeutil.h>
#include <gmsec/util/Log.h>
#include <gmsec/util/Mutex.h>
#include "../example.h"

#include <iostream>
#include <string>

using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class ErrorHandler: public LogHandler {

private:

	/* Using mutex so messages from different threads don't get mixed up */
	Mutex mutex;

public:

	virtual void CALL_TYPE OnMessage(const LogEntry &entry) {

		AutoMutex autoMutex(mutex);
		std::cout << "OnMessage for LOG_ERROR:" << std::endl;
		char tempBuffer[50];
		formatTime_s(entry.time, tempBuffer);
		std::cout << "  " << tempBuffer << ":" << Log::ToString(entry.level)
		<< ":" << entry.file << ":" << entry.line << ":"
		<< entry.message << endl;

	}

};

class WarningHandler: public LogHandler {

private:

	/* Using mutex so messages from different threads don't get mixed up */
	Mutex mutex;

public:

	virtual void CALL_TYPE OnMessage(const LogEntry &entry) {

		AutoMutex autoMutex(mutex);
		std::cout << "OnMessage for LOG_WARNING:" << std::endl;
		char tempBuffer[50];
		formatTime_s(entry.time, tempBuffer);
		std::cout << "  " << tempBuffer << ":" << Log::ToString(entry.level)
		<< ":" << entry.file << ":" << entry.line << ":" << entry.message
		<< endl;

	}

};

class InfoHandler: public LogHandler {

private:

	/* Using mutex so messages from different threads don't get mixed up */
	Mutex mutex;

public:

	virtual void CALL_TYPE OnMessage(const LogEntry &entry) {

		AutoMutex autoMutex(mutex);
		std::cout << "OnMessage for LOG_INFO:" << std::endl;
		char tempBuffer[50];
		formatTime_s(entry.time, tempBuffer);
		std::cout << "  " << tempBuffer << ":" << Log::ToString(entry.level)
		<< ":" << entry.file << ":" << entry.line << ":" << entry.message
		<< endl;

	}

};

class VerboseHandler: public LogHandler {

private:

	/* Using mutex so messages from different threads don't get mixed up */
	Mutex mutex;

public:

	virtual void CALL_TYPE OnMessage(const LogEntry &entry) {

		AutoMutex autoMutex(mutex);
		std::cout << "OnMessage for LOG_VERBOSE:" << std::endl;
		char tempBuffer[50];
		formatTime_s(entry.time, tempBuffer);
		std::cout << "  " << tempBuffer << ":" << Log::ToString(entry.level)
		<< ":" << entry.file << ":" << entry.line << ":" << entry.message
		<< endl;

	}

};

class DebugHandler: public LogHandler {

private:

	/* Using mutex so messages from different threads don't get mixed up */
	Mutex mutex;

public:

	virtual void CALL_TYPE OnMessage(const LogEntry &entry) {

		AutoMutex autoMutex(mutex);
		std::cout << "OnMessage for LOG_DEBUG:" << std::endl;
		char tempBuffer[50];
		formatTime_s(entry.time, tempBuffer);
		std::cout << "  " << tempBuffer << ":" << Log::ToString(entry.level)
		<< ":" << entry.file << ":" << entry.line << ":" << entry.message
		<< endl;

	}

};

class AnyHandler: public LogHandler {

private:

	/* Using mutex so messages from different threads don't get mixed up */
	Mutex mutex;

public:

	virtual void CALL_TYPE OnMessage(const LogEntry &entry) {

		AutoMutex autoMutex(mutex);
		std::cout << "OnMessage for ANY LOG_*:" << std::endl;
		char tempBuffer[50];
		formatTime_s(entry.time, tempBuffer);
		std::cout << "  " << tempBuffer << ":" << Log::ToString(entry.level)
		<< ":" << entry.file << ":" << entry.line << ":" << entry.message
		<< endl;

	}

};

int PublishTestMessage(Connection * connection, const char * subject) {

	Field fd;
	const char * tmp;
	int i = 123;
	Status result;
	Message * message;

	result = connection->CreateMessage(subject, GMSEC_MSG_PUBLISH, message);
	check("CreateMessage", result);

	/* Add Fields */
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
	fd.SetValue(1 + 1./i);
	message->AddField(fd);

	fd.SetName("X");
	fd.SetValue((GMSEC_BIN) "JLMNOPQ", 7);
	message->AddField(fd);

	/* Publish Message */
	LOG_INFO << "sending";
	result = connection->Publish(message);
	check("Publish", result);

	/* Dump Message */
	result = message->ToXML(tmp);
	check("ToXML", result);
	LOG_INFO << tmp;

	/* Destroy the Message */
	result = connection->DestroyMessage(message);
	check("DestroyMessage", result);

	return 0;
}

int main(int argc, char* argv[]) {

	/* Create and register log handlers */
	ErrorHandler errorHandler;
	Log::RegisterHandler(logERROR, &errorHandler);
	WarningHandler warningHandler;
	Log::RegisterHandler(logWARNING, &warningHandler);
	InfoHandler infoHandler;
	Log::RegisterHandler(logINFO, &infoHandler);
	VerboseHandler verboseHandler;
	Log::RegisterHandler(logVERBOSE, &verboseHandler);
	DebugHandler debugHandler;
	Log::RegisterHandler(logDEBUG, &debugHandler);
	AnyHandler anyHandler;

	/* Set logging reporting level */

	cout << "Setting log level to " << Log::FromString("VERBOSE") << endl;

	Log::SetReportingLevel(logVERBOSE);

	LOG_INFO << "The reporting level is set to "
	         << Log::GetReportingLevel();

	Status result;
	Connection * connection = 0;

	Config config(argc, argv);

        addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		std::cout << "\nusage: logging" << " connectiontype=<middleware> "
			<< "[ <parameter>=<value> ]\n"
			<< "\n\tNote that the parameter 'connectiontype' is required. "
			<< "\n\tThe rest of other parameters are optional.\n"
			<< "\n\tserver=<server name> "
			<< "(required if middleware is not bolt/MB locally)"
			<< "\n\tloglevel=<log level>"
			<< "\n\nFor more infomration, see API User's Guide\n";

		return -1;
	}

	/* Output GMSEC API version */
	LOG_INFO << ConnectionFactory::GetAPIVersion();

	/* Create the Connection */
	result = ConnectionFactory::Create(&config, connection);
	check("Creating Connection", result);

	/* output connection middleware version */
	LOG_INFO << connection->GetLibraryVersion();

	/* Connect */
	result = connection->Connect();
	check("Connecting", result);

	PublishTestMessage(connection, "GMSEC.TEST.PUBLISH");

	/* Disconnect */
	result = connection->Disconnect();
	check("Disconnecting", result);

	/* Destroy the Connection */
	result = ConnectionFactory::Destroy(connection);
	check("Destroying Connection", result);

	Log::SetDefaultStream(&std::cerr);
	LOG_INFO << "This message should go to stderr, not stdout.  "
	         << "For example, in bash test by running as:  " << endl
	         << "./logging mw-id=bolt 2> testfile.txt  " << endl
	         << "... and then check the contents of testfile.txt";

	Log::SetDefaultStream(&std::cout);

	LOG_ERROR   << "This is an example error message.";
	LOG_WARNING << "This is an example warning message.";
	LOG_VERBOSE << "This is an example \"verbose\" message.";
	LOG_DEBUG   << "This is an example debug message "
	            << "which should not show.";
	  // This last message cannot be shown right now because
	  // Log::SetReportingLevel(logVERBOSE), used above, does not
	  // allow DEBUG messages to come out.
	LOG_VERBOSE << "This is another example \"verbose\" message.";

	/* Set logging reporting level to now allow DEBUG messages to be 
	   shown */
	Log::SetReportingLevel(logDEBUG);
        if (Log::GetReportingLevel() == logDEBUG)
	{
		LOG_INFO << "Changed reporting level to logDEBUG";
	}
	else
	{
		LOG_ERROR << "Failed to change reporting level to logDEBUG";
	}
	LOG_DEBUG << "This is an example debug message which should show.";
	// This should be shown successfully, unlike the last debug message.

	LOG_DEBUG << "NONE reporting level, numerically, is "
	          << Log::FromString("NONE");
	LOG_DEBUG << "ERROR reporting level, numerically, is "
	          << Log::FromString("ERROR");
	LOG_DEBUG << "SECURE reporting level, numerically, is "
	          << Log::FromString("SECURE");
	LOG_DEBUG << "WARNING reporting level, numerically, is "
	          << Log::FromString("WARNING");
	LOG_DEBUG << "INFO reporting level, numerically, is "
	          << Log::FromString("INFO");
	LOG_DEBUG << "VERBOSE reporting level, numerically, is "
	          << Log::FromString("VERBOSE");
	LOG_DEBUG << "DEBUG reporting level, numerically, is "
	          << Log::FromString("DEBUG");

	/* Register general-purpose handler and test */

	Log::RegisterHandler(&anyHandler);

	LOG_ERROR << "NONE reporting level, numerically, is "
	          << Log::FromString("NONE");
	LOG_ERROR << "ERROR reporting level, numerically, is "
	          << Log::FromString("ERROR");
	LOG_WARNING << "WARNING reporting level, numerically, is "
	            << Log::FromString("WARNING");
	LOG_INFO << "INFO reporting level, numerically, is "
	         << Log::FromString("INFO");
	LOG_VERBOSE << "VERBOSE reporting level, numerically, is "
	            << Log::FromString("VERBOSE");
	LOG_DEBUG << "DEBUG reporting level, numerically, is "
	          << Log::FromString("DEBUG");

	/* Unregister log handlers */
	Log::RegisterHandler((LogHandler*) NULL);

	return 0;
}
