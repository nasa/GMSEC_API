/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file replier.cpp
 *
 * This is a sample application built with the C++
 * GMSEC API.  This application demonstrates several
 * key components within the GMSEC API including loading
 * from an xml file, producing a heartbeat, and request
 * reply functionality using callbacks.  This works in sync
 * with Requestor.cpp.
 *  
 */

#include "../example.h"

#include <gmsec4/util/TimeUtil.h>

#include <iostream>	
#include <cstring>

using namespace gmsec::api;
using namespace gmsec::api::util;


class RequestReplyCallback : public Callback
{
public:
	RequestReplyCallback(ConfigFile& cfgFile)
		: cfgFile(cfgFile)
	{
	}


	virtual void CALL_TYPE onMessage(Connection& connection, const Message& message)
	{
		try
		{
			// We don't care about anything but request messages
			if (message.getKind() != Message::REQUEST)
				return;

			//o Obtain the DIRECTIVE-KEYWORD field from the message.  It holds the request count
			const char* requestCount = message.getStringField("DIRECTIVE-KEYWORD").getValue();
			GMSEC_INFO << "[RequestReplyCallback::onMessage] Received Request (" << requestCount << ")";

			//o Load the directive response template message definition
			Message replyTemplate = cfgFile.lookupMessage("DIRECTIVE-RESPONSE");

			//o Create the reply message, with the request message's subject
			Message reply(message.getSubject(), Message::REPLY);

			//o Copy fields from the reply message template to our actual reply message
			replyTemplate.copyFields(reply);

			//o Replace the DATA field with the request number we are responding to
			reply.addField("DATA", requestCount);

			//o Replace the TIME-COMPLETE field with the correct time
			char theTime[GMSEC_TIME_BUFSIZE] = {0};
			TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
			reply.addField("TIME-COMPLETED", theTime);

			//o Send the reply
			connection.reply(message, reply);

			GMSEC_INFO << "[RequestReplyCallback::onMessage] Reply Sent (" << requestCount << ")";

			//o Find and load the config file log message definition
			Message logMessage = cfgFile.lookupMessage("LOG-REP");

			//o Fill the fields within the log message
			logMessage.addField("EVENT-TIME", theTime);
			logMessage.addField("MSG-TEXT", requestCount);

			//o Publish the log message
			connection.publish(logMessage);
		}
		catch (Exception& e)
		{
			GMSEC_WARNING << "[RequestReplyCallback::onMessage] " << e.what();
		}
	}

private:
	ConfigFile& cfgFile;
};


class Replier
{
public:
	Replier(const char* filename);
	~Replier();
	bool run();

private:
	std::string cfgFilename;
	ConfigFile  cfgFile;
	Connection* connection;
	SubscriptionInfo* info;
};


Replier::Replier(const char* filename)
	: cfgFilename(filename),
	  cfgFile(),
	  connection(0)
{
	Config tmp;
	example::initialize(tmp);
}


Replier::~Replier()
{
	if (connection)
	{
		GMSEC_INFO << "Unsubscribing from " << info->getSubject();
		connection->unsubscribe(info);
		connection->disconnect();
		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool Replier::run()
{
	bool success = true;

	try
	{
		//o Load the config file
		cfgFile.load(cfgFilename.c_str());

		//o Look up configuration information
		Config progConfig    = cfgFile.lookupConfig("program-config");
		Config connConfig    = cfgFile.lookupConfig("connection-config");
		Message heartbeatMsg = cfgFile.lookupMessage("C2CX-HEARTBEAT-REP");

		//o Get program config values
		GMSEC_I16 updateRate    = example::get(progConfig, "update-rate", 1);
		GMSEC_I16 loopCountdown = example::get(progConfig, "loop-time", 30);
		GMSEC_I16 heartbeatHolder = heartbeatMsg.getI16Field("PUB-RATE").getValue();

		//o Create a connection object off of the configuration
		connection = Connection::create(connConfig);

		//o Connect to the GMSEC bus
		connection->connect();

		//o Output version information
		GMSEC_INFO << "API version = " << Connection::getAPIVersion();
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Create subscriptions from subscription templates in the config file using callback
		RequestReplyCallback cb(cfgFile);
		const char* topic = cfgFile.lookupSubscription("DIRECTIVE-REQUEST");

		info = connection->subscribe(topic, &cb);

		/* Output some general program information */
		GMSEC_INFO << "Publishing for " << loopCountdown << " seconds.";
		GMSEC_INFO << "Publishing Heartbeat Messages every " << heartbeatHolder << " seconds.";

		//o Call to create the thread that will automatically retrieve messages off the message queue
		connection->startAutoDispatch();

		//o Start time
		char theTime[GMSEC_TIME_BUFSIZE] = {0};
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
		GMSEC_INFO << "Start Time: " << theTime;

		/* Publishing loop */
		GMSEC_I16 heartbeatCount     = 0;
		GMSEC_I16 heartbeatCountdown = 0;

		for (; loopCountdown > 0; --loopCountdown)
		{
			//o When the countdown reaches 0, we publish a heartbeat message and reset the counter
			if (heartbeatCountdown < 1)
			{
				++heartbeatCount;

				heartbeatMsg.addField("COUNTER", heartbeatCount);

				//o Publish the heartbeat message
				connection->publish(heartbeatMsg);

				//o Output a heartbeat marker and reset the counter
				GMSEC_INFO << "Published Heartbeat";
				heartbeatCountdown = heartbeatHolder;
			}

			//o Decrement the counters
			heartbeatCountdown -= updateRate;

			//o Sleep
			example::millisleep(updateRate * 1000);
		}

		//o End time
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
		GMSEC_INFO << "End Time: " << theTime;


		connection->stopAutoDispatch();
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

	return (Replier(argv[1]).run() ? 0 : -1);
}
