/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gmconfig.cpp
 * 
 * A C++ example demonstration of loading a configuration from a standard
 * xml file.  The configuration file used for the example code is "gmconfig.xml".
 * 
 */

#include <gmsec_cpp.h>
#include <gmsec/util/sysutil.h>
#include "../example.h"
#include <iostream>
#include <string>

#include <gmsec/ConfigFileIterator.h>


using namespace std;
using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class gmconfig {

public:

	gmconfig(ConfigFile &c);
	~gmconfig();

	Status Run();

private:

	ConfigFile &cf;
	Connection * connection;

};

gmconfig::gmconfig(ConfigFile &c) :
		cf(c), connection(0) {

	Config tmp;
	initialize(tmp);

}

gmconfig::~gmconfig() {

	if (connection) {

		if (connection->IsConnected())
			checkNoThrow("Disconnect", connection->Disconnect());

		checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));

	}

	checkNoThrow("ShutdownAllMiddlewares",
			ConnectionFactory::ShutdownAllMiddlewares());

}

Status gmconfig::Run() {

	Status result;

	/* Load and Parse */
	result = cf.Load();
	check("ConfigLoad", result);

	/* Retrieve 2 configs from file */
	Config c1, c2;
	cf.LookupConfig("config1", &c1);
	cf.LookupConfig("config2", &c2);

	/* Dump Details */
	const char *xml;
	result = c1.ToXML(xml);
	check("ToXML", result);

	LOG_INFO << "Config 1:" << xml;

	result = c2.ToXML(xml);
	check("ToXML", result);

	LOG_INFO << "Config 2:" << xml;

	/* Create the Connection */
	result = ConnectionFactory::Create(&c2, connection);
	check("Create Connection", result);

	/* Connect */
	result = connection->Connect();
	check("Connect", result);

	/* Suscribe by name */
	const char * pattern = NULL;
	result = cf.LookupSubscription("events", pattern);
	check("LookupSuscription", result);

	LOG_INFO << "Subscribing to: " << pattern;

	/* Create ConfigFileIterator & Reset */
	ConfigFileIterator cfgIter;
	cf.Reset(cfgIter);

	LOG_INFO << "Object names: ";

	LOG_INFO << "Subscription names: ";
	while(cfgIter.HasNextSubscriptionName())
	{
		gmsec::util::String gmstr;
		cfgIter.NextSubscriptionName(gmstr);
		std::string str(gmstr.c_str());
		LOG_INFO << str;
	}

	LOG_INFO << "Config names: ";
	while(cfgIter.HasNextConfigName())
	{
		gmsec::util::String gmstr;
		cfgIter.NextConfigName(gmstr);
		std::string str(gmstr.c_str());
		LOG_INFO << str;
	}
	
	LOG_INFO << "Message names: ";
	while(cfgIter.HasNextMessageName())
	{
		gmsec::util::String gmstr;
		cfgIter.NextMessageName(gmstr);
		std::string str(gmstr.c_str());
		LOG_INFO << str;
	}

	LOG_INFO << "Subscriptions from list: ";
	while(cfgIter.HasNextSubscription())
	{
		gmsec::util::String gmstr;
		cfgIter.NextSubscription(gmstr);
		std::string str(gmstr.c_str());
		LOG_INFO << str;
	}

	LOG_INFO << "Configs from list: ";
	while(cfgIter.HasNextConfig())
	{
		const char* xml;
		Config* cfg = new Config();
		cfgIter.NextConfig(cfg);
		cfg->ToXML(xml);
		LOG_INFO << xml;
		delete cfg;
	}

	LOG_INFO << "Messages from list: ";
	while(cfgIter.HasNextMessage())
	{
		const char* xml;
		Message* msg;
		connection->CreateMessage("TEMP",GMSEC_MSG_PUBLISH, msg);
		cfgIter.NextMessage(msg);
		msg->ToXML(xml);
		LOG_INFO << xml;
		connection->DestroyMessage(msg);
	}
	

	result = connection->Subscribe(pattern);
	check("Subscribing", result);

	/* Create message from config entry */
	Message * message = 0;
	result = connection->CreateMessage(message);
	check("CreateMessage", result);

	result = cf.LookupMessage("msg1", message);
	check("LookupMessage", result);

	/* Dump Message */
	result = message->ToXML(xml);
	check("ToXML", result);
	LOG_INFO << "Message 1: " << xml;

	/* Publish Message */
	result = connection->Publish(message);
	check("Publish", result);

	/* Destroy the Message */
	result = connection->DestroyMessage(message);
	check("DestroyMessage", result);

	return result;

}

int main(int argc, char* argv[]) {

	if (argc <= 1) {

		cout << "usage: gmconfig <filename>" << endl;
		return -1;

	}

	ConfigFile cf(argv[1]);

	try {

		gmconfig state(cf);
		state.Run();

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;

}
