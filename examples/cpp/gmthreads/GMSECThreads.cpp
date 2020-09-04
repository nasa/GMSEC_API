/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** 
 * @file GMSECThreads.cpp
 * 
 * This file contains a simple example of using separate threads
 * for separate connections.
 * 
 */

#include "PublishThread.hpp"
#include "SubscribeThread.hpp"

#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>

#include <memory>

using namespace gmsec::api;
using namespace gmsec::api::util;


void runSubscriber(StdSharedPtr<SubscribeThread> shared)
{
	shared->run();
}


void runPublisher(StdSharedPtr<PublishThread> shared)
{
	shared->run();
}


int main(int argc, char* argv[])
{
	Config config(argc, argv);

	example::addToConfigFromFile(config);

	if (example::isOptionInvalid(config, argc))
	{
		example::printUsage("gmthreads");
		return -1;
	}

	try
	{
		StdSharedPtr<SubscribeThread> subscribeThreadShared(new SubscribeThread(config));
		std::auto_ptr<StdThread>      gmsub(new StdThread(&runSubscriber, subscribeThreadShared));

		StdSharedPtr<PublishThread>   publishThreadShared(new PublishThread(config));
		std::auto_ptr<StdThread>      gmpub(new StdThread(&runPublisher, publishThreadShared));

		GMSEC_INFO << "Now beginning Threads test";

		gmsub->start();
		gmpub->start();

		gmpub->join();
		gmsub->join();

		GMSEC_INFO << "Now ending Threads test";
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << "Failed: " << e.what();
		return -1;
	}
}
