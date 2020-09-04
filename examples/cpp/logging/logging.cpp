/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include "../example.h"

#include <gmsec4/util/Mutex.h>
#include <gmsec4/util/TimeUtil.h>

#include <iostream>
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


class BaseHandler : public LogHandler
{
private:
	// Using mutex so messages from different threads don't get mixed up
	Mutex       mutex;
	std::string whoAmI;

protected:
	BaseHandler(const char* wai)
		: mutex(),
		  whoAmI(wai)
	{
	}

public:
	virtual void CALL_TYPE onMessage(const LogEntry& entry)
	{
		AutoMutex autoMutex(mutex);

		char tempBuffer[50];
		TimeUtil::formatTime(entry.time, tempBuffer);

		std::cout << "[BaseHandler::onMessage] for: " << whoAmI
		          << " : " << tempBuffer << " [" << Log::toString(entry.level) << "]"
		          << " [" << entry.file << ":" << entry.line << "] "
		          << entry.message
		          << "\n" << std::endl;
	}
};


class ErrorHandler : public BaseHandler
{
public:
	ErrorHandler() : BaseHandler("GMSEC_ERROR") {}
};


class WarningHandler : public BaseHandler
{
public:
	WarningHandler() : BaseHandler("GMSEC_WARNING") {}
};


class InfoHandler: public BaseHandler
{
public:
	InfoHandler() : BaseHandler("GMSEC_INFO") {}
};

class VerboseHandler: public BaseHandler
{
public:
	VerboseHandler() : BaseHandler("GMSEC_VERBOSE") {}
};

class DebugHandler: public BaseHandler
{
public:
	DebugHandler() : BaseHandler("GMSEC_DEBUG") {}
};

class AnyHandler: public BaseHandler
{
public:
	AnyHandler() : BaseHandler("ANY_HANDLER") {}
};


void publishTestMessage(Connection* connection, const char* subject)
{
	int i = 123;

	//o Create message
	Message message(subject, Message::PUBLISH);

	//o Add fields to the message
	message.addField("F", false);
	message.addField("I", (GMSEC_I32) i);
	message.addField("K", (GMSEC_U16) i);
	message.addField("S", "This is a test");
	message.addField("D", (GMSEC_F32) (1 + 1./i));
	message.addField("X", (GMSEC_BIN) "JLMNOPQ", 7);

	//o Publish Message
	GMSEC_INFO << "Sending:\n" << message.toXML();
	connection->publish(message);
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
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

	//o Create and register log handlers
	ErrorHandler   errorHandler;
	WarningHandler warningHandler;
	InfoHandler    infoHandler;
	VerboseHandler verboseHandler;
	DebugHandler   debugHandler;
	AnyHandler     anyHandler;

	Log::registerHandler(logERROR, &errorHandler);
	Log::registerHandler(logWARNING, &warningHandler);
	Log::registerHandler(logINFO, &infoHandler);
	Log::registerHandler(logVERBOSE, &verboseHandler);
	Log::registerHandler(logDEBUG, &debugHandler);

	//o Set logging reporting level
	Log::setReportingLevel(logVERBOSE);
	GMSEC_VERBOSE << "The log reporting level is now set to: " << Log::getReportingLevel();

	//o Output GMSEC API version
	GMSEC_INFO << Connection::getAPIVersion();

	try
	{
		//o Create the Connection
		Connection* connection = Connection::create(config);

		//o output connection middleware version
		GMSEC_INFO << connection->getLibraryVersion();

		//o Connect
		connection->connect();

		//o Publish a message
		publishTestMessage(connection, "GMSEC.TEST.PUBLISH");

		//o Disconnect
		connection->disconnect();

		//o Destroy the Connection
		Connection::destroy(connection);
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		return -1;
	}

	//o Unregister log handlers
	Log::registerHandler((LogHandler*) NULL);

	//o Set log stream to stderr
	config.addValue("LOGFILE", "STDERR");
	GMSEC_INFO << "This message should go to stderr, not stdout.  "
               << "For example, in bash test by running as:\n"
               << "./logging mw-id=bolt 2> testfile.txt\n"
               << "... and then check the contents of testfile.txt";

	//o Reset log stream to stdout
	config.addValue("LOGFILE", "STDOUT");

	GMSEC_ERROR   << "This is an example error message.";
	GMSEC_WARNING << "This is an example warning message.";
	GMSEC_VERBOSE << "This is an example \"verbose\" message.";
	GMSEC_DEBUG   << "This is an example debug message which should not show.";

  	// This last message cannot be shown right now because
  	// Log::setReportingLevel(logVERBOSE), used above, does not
  	// allow DEBUG messages to come out.
	GMSEC_VERBOSE << "This is another example \"verbose\" message.";

	// Set logging reporting level to now allow DEBUG messages to be shown
	Log::setReportingLevel(logDEBUG);
	if (Log::getReportingLevel() == logDEBUG)
	{
		GMSEC_INFO << "Changed reporting level to logDEBUG";
	}
	else
	{
		GMSEC_ERROR << "Failed to change reporting level to logDEBUG";
	}
	GMSEC_DEBUG << "This is an example debug message which should show.";
	// This should be shown successfully, unlike the last debug message.

	GMSEC_DEBUG << "NONE reporting level, numerically, is "    << Log::fromString("NONE");
	GMSEC_DEBUG << "ERROR reporting level, numerically, is "   << Log::fromString("ERROR");
	GMSEC_DEBUG << "SECURE reporting level, numerically, is "  << Log::fromString("SECURE");
	GMSEC_DEBUG << "WARNING reporting level, numerically, is " << Log::fromString("WARNING");
	GMSEC_DEBUG << "INFO reporting level, numerically, is "    << Log::fromString("INFO");
	GMSEC_DEBUG << "VERBOSE reporting level, numerically, is " << Log::fromString("VERBOSE");
	GMSEC_DEBUG << "DEBUG reporting level, numerically, is "   << Log::fromString("DEBUG");

	//o Register general-purpose handler and test
	Log::registerHandler(&anyHandler);

	GMSEC_ERROR   << "NONE reporting level, numerically, is "    << Log::fromString("NONE");
	GMSEC_ERROR   << "ERROR reporting level, numerically, is "   << Log::fromString("ERROR");
	GMSEC_WARNING << "WARNING reporting level, numerically, is " << Log::fromString("WARNING");
	GMSEC_INFO    << "INFO reporting level, numerically, is "    << Log::fromString("INFO");
	GMSEC_VERBOSE << "VERBOSE reporting level, numerically, is " << Log::fromString("VERBOSE");
	GMSEC_DEBUG   << "DEBUG reporting level, numerically, is "   << Log::fromString("DEBUG");

	//o Unregister log handlers
	Log::registerHandler((LogHandler*) NULL);
}
