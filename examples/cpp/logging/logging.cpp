/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file logging.cpp
 * 
 * This file contains an example outlining the use of the GMSEC API logging
 * framework.
 */

#include <gmsec4_cpp.h>

#include <iostream>
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;

//o Helper functions
void publishTestMessage(ConnectionManager* connMgr, const char* subject);


//o Define a baseline LogHandler
// This will be used by the Log macros; The implementation of onMessage
// determines how messages will be logged to output
class BaseHandler : public LogHandler
{
private:
	// Use a mutex so messages from different threads don't get mixed up
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


// Different Handlers can be implemented for each logging level, if so desired
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


int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "usage: " << argv[0] << " mw-id=<middleware ID>" << std::endl;
		return -1;
	}

	Config config(argc, argv);

	//o Since this example program uses an invalid message, we ensure the
	//  validation check is disabled.
	config.addValue("gmsec-msg-content-validate-all", "false");

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

	//o Print the GMSEC API version number using the GMSEC Logging
	// interface
	// This is useful for determining which version of the API is
	// configured within the environment
	GMSEC_INFO << ConnectionManager::getAPIVersion();

	try
	{
		//o Create the ConnectionManager
		ConnectionManager connMgr(config);

		//o Connect
		connMgr.initialize();

		//o Output middleware client library version
		GMSEC_INFO << connMgr.getLibraryVersion();

		//o Publish a message
		publishTestMessage(&connMgr, "GMSEC.TEST.PUBLISH");

		//o Disconnect from the middleware and clean up the Connection
		connMgr.cleanup();
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
	// The DEBUG message below will be shown successfully, unlike the last
	// debug message.
	GMSEC_DEBUG << "This is an example debug message which should show.";

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

	return 0;
}


void publishTestMessage(ConnectionManager* connMgr, const char* subject)
{
	int i = 123;

	//o Create a Message object
	Message message(subject, Message::PUBLISH);

	//o Add fields to the Message
	message.addField("F", false);
	message.addField("I", (GMSEC_I32) i);
	message.addField("K", (GMSEC_U16) i);
	message.addField("S", "This is a test");
	message.addField("D", (GMSEC_F32) (1 + 1./i));
	message.addField("X", (GMSEC_BIN) "JLMNOPQ", 7);

	//o Publish Message 
	connMgr->publish(message);

	//o Output the Message's XML string representation by invoking Log macro
	GMSEC_INFO << "Sent:\n" << message.toXML();
}

