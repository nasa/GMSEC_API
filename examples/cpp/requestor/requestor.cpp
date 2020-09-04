/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file requestor.cpp
 *
 * This is a sample application built with the C++
 * GMSEC API.  This application demonstrates several
 * key components within the GMSEC API including loading
 * from an xml file, producing a heartbeat, and request
 * reply functionality using callbacks. This works in sync
 * with Replier.cpp.
 *  
 */

#include "../example.h"

#include <gmsec4/util/TimeUtil.h>

#include <iostream>	
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


class RequestReplyCallback : public ReplyCallback
{
public:
	RequestReplyCallback(ConfigFile& c)
		: cfgFile(c)
	{
	}


	virtual void CALL_TYPE onReply(Connection& connection, const Message& request, const Message& reply)
	{
		try
		{
			//o Get the status of the response
			GMSEC_I16   response = reply.getI16Field("RESPONSE-STATUS").getValue();
			const char* data     = reply.getStringField("DATA").getValue();

			switch (response)
			{
				case 1:
					GMSEC_INFO << "[RequestReplyCallback::onReply] Acknowledgement";
					break;
				case 2:
					GMSEC_INFO << "[RequestReplyCallback::onReply] Working/keep alive";
					break;
				case 3:
					GMSEC_INFO << "[RequestReplyCallback::onReply] Received Reply (DATA=" << data << ")";
					break;
				case 4 :
					GMSEC_INFO << "[RequestReplyCallback::onReply] Failed completion";
					break;
				case 5 :
					GMSEC_INFO << "[RequestReplyCallback::onReply] Invalid Request";
					break;
				default :
					GMSEC_INFO << "[RequestReplyCallback::onReply] <unknown response status>";
					break;
			}

			//o Lookup from the config file the template log message definition
			Message logMessage = cfgFile.lookupMessage("LOG-REQ");

			//o Get the current time
			char theTime[GMSEC_TIME_BUFSIZE] = {0};
			TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);

			//o Fill fields within the log message
			logMessage.addField("EVENT-TIME", theTime);
			logMessage.addField("MSG-TEXT", data);

			//o Publish the log message
			connection.publish(logMessage);
		}
		catch (Exception& e)
		{
			GMSEC_ERROR << e.what();
		}
	}

	virtual void CALL_TYPE onEvent(Connection& connection, const Status& status, Connection::ConnectionEvent event)
	{
		if (status.isError())
		{
			GMSEC_WARNING << "[RequestReplyCallback:onEvent] status = " << status.get() << ", event = " << event;
		}
	}

private:
	ConfigFile& cfgFile;
};


class Requestor
{
public:
	Requestor(const char* filename);
	~Requestor();
	bool run();

private:
	std::string cfgFilename;
	ConfigFile  cfgFile;
	Connection* connection;
};


Requestor::Requestor(const char* filename)
	: cfgFilename(filename),
	  cfgFile(),
	  connection(0)
{
	Config tmp;
	example::initialize(tmp);
}


Requestor::~Requestor()
{
	if (connection)
	{
		connection->disconnect();
		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool Requestor::run()
{
	bool success = true;

	try
	{
		//o Load the configuration file
		cfgFile.load(cfgFilename.c_str());

		//o Load the configurations for the example program and for the GMSEC middleware
		Config progConfig = cfgFile.lookupConfig("program-config");
		Config connConfig = cfgFile.lookupConfig("connection-config");

		//o Load the heartbeat message definition
		Message heartbeatMsg = cfgFile.lookupMessage("C2CX-HEARTBEAT-REQ");

		//o Load the directive request message definition
		Message directiveReqMsg = cfgFile.lookupMessage("DIRECTIVE-REQUEST");

		//o Get program config values
		GMSEC_I16 updateRate      = progConfig.getIntegerValue("update-rate");
		GMSEC_I16 loopCountdown   = progConfig.getIntegerValue("loop-time");
		GMSEC_I16 directiveHolder = progConfig.getIntegerValue("send-directive-rate");
		GMSEC_I16 heartbeatHolder = heartbeatMsg.getI16Field("PUB-RATE").getValue();

		//o Subscription callback
		RequestReplyCallback cb(cfgFile);

		//o Create a connection object off of the configuration
		connection = Connection::create(connConfig);

		//o Connect to the GMSEC bus
		connection->connect();

		//o Output version information
		GMSEC_INFO << "API version = " << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Output some general program information
		GMSEC_INFO << "Publishing for " << loopCountdown << " seconds.";
		GMSEC_INFO << "Publishing Directive Request Messages every " << directiveHolder << " seconds.";
		GMSEC_INFO << "Publishing Heartbeat Messages every " << heartbeatHolder << " seconds.";

		//o Start time
		char theTime[GMSEC_TIME_BUFSIZE] = {0};
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
		GMSEC_INFO << "Start Time: " << theTime;

		//o Publishing loop
		GMSEC_I16 directiveCountdown = 0;
		GMSEC_I16 heartbeatCountdown = 0;
		GMSEC_I16 requestCount       = 0;
		GMSEC_I16 heartbeatCount     = 0;

		for (; loopCountdown > 0; --loopCountdown)
		{
			/* When the countdown reaches 0, we publish a directive message and reset the counter */
			std::ostringstream oss;

			if (directiveCountdown < 1)
			{
				++requestCount;

				//o Alter the directive request to a unique string
				oss << requestCount;
				directiveReqMsg.addField("DIRECTIVE-KEYWORD", oss.str().c_str());
				oss.str("");

				oss << "DIRECTIVE REQUEST MESSAGE " << requestCount;
				directiveReqMsg.addField("DIRECTIVE-STRING", oss.str().c_str());
				oss.str("");

				//o Publish time
				GMSEC_INFO << "Publishing Directive Request Message (" << requestCount << ")";

				//o Send the request with the reply being send to the callback we created
				connection->request(directiveReqMsg, GMSEC_WAIT_FOREVER, &cb);

				//o Reset the countdown
				directiveCountdown = directiveHolder;
			}

			/* When the countdown reaches 0, we publish a heartbeat message and reset the counter */
			if (heartbeatCountdown < 1)
			{
				++heartbeatCount;

				//o Update the message counter field
				heartbeatMsg.addField("COUNTER", heartbeatCount);

				//o Publish the heartbeat message
				connection->publish(heartbeatMsg);

				//o Output a heartbeat marker and reset the counter
				GMSEC_INFO << "Published Heartbeat";
				heartbeatCountdown = heartbeatHolder;
			}

			/* Decrement the counters */
			heartbeatCountdown -= updateRate;
			directiveCountdown -= updateRate;

			/* Sleep for updateRate seconds */
			example::millisleep(updateRate * 1000);
		}

		//o End time
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
		GMSEC_INFO << "End Time: " << theTime;
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		return -1;
	}

	return (Requestor(argv[1]).run() ? 0 : -1);
}
