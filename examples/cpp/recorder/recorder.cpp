/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file recorder.cpp
 *
 * This is a sample application built with the C++
 * GMSEC API.  This application demonstrates several
 * key components within the GMSEC API including loading
 * from an XML file, producing a heartbeat, and publishing.
 *
 * Please use the XML configuration located at ../sampleAppsCommon.xml
 */

#include "../example.h"

#include <gmsec4/util/TimeUtil.h>

#include <iostream>
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::util;


class LogCallback : public Callback
{
public:
	virtual void CALL_TYPE onMessage(Connection& connection, const Message& message)
	{
		GMSEC_INFO << "[LogCallback::onMessage] Received:\n" << message.toXML();
	}
};


class Recorder
{
public:
	Recorder(const char* filename);
	~Recorder();
	bool run();

private:
	std::string cfgFilename;
	ConfigFile  cfgFile;
	Connection* connection;
	SubscriptionInfo** info;
};


Recorder::Recorder(const char* filename)
	: cfgFilename(filename),
	  cfgFile(),
	  connection(0)
{
	Config tmp;
	example::initialize(tmp);
}


Recorder::~Recorder()
{
	if (connection)
	{
		for (size_t i = 0; i < 2; ++i)
		{
			GMSEC_INFO << "Unsubscribing from " << info[i]->getSubject();
			connection->unsubscribe(info[i]);
		}
		delete[] info;
		connection->disconnect();
		Connection::destroy(connection);
	}

	Connection::shutdownAllMiddlewares();
}


bool Recorder::run()
{
	bool success = true;

	try
	{
		//o Load the config file
		cfgFile.load(cfgFilename.c_str());

		//o Look up program configuration information, and get program config values
		Config progConfig = cfgFile.lookupConfig("program-config");

		//o Lookup subscription subjects/topics
		const char* rcvLogSubject = cfgFile.lookupSubscription("RECEIVE-LOG");
		const char* sndLogSubject = cfgFile.lookupSubscription("SEND-LOG");

		if (!rcvLogSubject)
		{
			throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE, "RECEIVE-LOG subscription entry not found");
		}
		if (!sndLogSubject)
		{
			throw Exception(CONFIGFILE_ERROR, OTHER_ERROR_CODE, "SEND-LOG subscription entry not found");
		}

		//o Look up the heartbeat message definition, and attempt to acquire the PUB-RATE
		Message heartbeatMsg = cfgFile.lookupMessage("C2CX-HEARTBEAT-REC");

		GMSEC_I16 loopCountdown = example::get(progConfig, "loop-time", 30);
		GMSEC_I16 updateRate    = example::get(progConfig, "update-rate", 30);
		GMSEC_I16 pubRate       = 30;

		try {
			pubRate = heartbeatMsg.getI16Field("PUB-RATE").getValue();
		}
		catch (...) {
			GMSEC_WARNING << "Unable to find PUB-RATE; will use default publish rate of " << pubRate << " seconds.";
		}

		//o Look up the config file value that contains the configuration for the middleware
		Config connConfig = cfgFile.lookupConfig("connection-config");

		//o Create a connection object off of the configuration */
		connection = Connection::create(connConfig);

		//o Connect to the GMSEC bus
		connection->connect();

		//o output GMSEC API version
		GMSEC_INFO << Connection::getAPIVersion();

		//o Output middleware version
		GMSEC_INFO << "Middleware version = " << connection->getLibraryVersion();

		//o Create subscriptions from subscription templates in the config file using callback
		LogCallback cb;
		info  = new SubscriptionInfo*[2];
		info[0] = connection->subscribe(rcvLogSubject, &cb);
		info[1] = connection->subscribe(sndLogSubject, &cb);

		//o Output some general program information
		GMSEC_INFO << "Publishing for " << loopCountdown << " seconds.";
		GMSEC_INFO << "Publishing C2CX Heartbeat messages every " << pubRate << " seconds.";

		//o Call to create the thread that will automatically retrieve messages off the message queue
		connection->startAutoDispatch();

		//o Display start time
		char theTime[GMSEC_TIME_BUFSIZE] = {0};
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
		GMSEC_INFO << "Start Time: " << theTime;

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
				heartbeatCountdown = pubRate;
			}

			//o Decrement the counters
			heartbeatCountdown -= updateRate;

			//o Sleep
			example::millisleep(updateRate * 1000);
		}

		//o Display end time
		TimeUtil::formatTime(TimeUtil::getCurrentTime(), theTime);
		GMSEC_INFO << "\n\nEnd Time: " << theTime;

		//o Stop the Auto Dispatcher
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

	return (Recorder(argv[1]).run() ? 0 : -1);
}
