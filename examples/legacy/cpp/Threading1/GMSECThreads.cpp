/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec_cpp.h>
#include "PublishThread.hpp"
#include "SubscribeThread.hpp"
#include "../example.h"

#include <string>

using namespace std;
using namespace gmsec;
using namespace example;


int main(int argc, char* argv[]) {

	cout << "Now beginning Threads test" << endl;

	Config config(argc, argv);

        addToConfigFromFile(config);

	if (isOptionInvalid(config, argc)) {
		printUsage("threading1");
		return -1;
	}

	try {

		PublishThread * gmpub = new PublishThread(config);
		SubscribeThread * gmsub = new SubscribeThread(config);

		gmsub->start();
		gmpub->start();

		gmpub->join();
		gmsub->join();
		cout << "Now ending Threads test" << endl;

	} catch (exception &e) {

		LOG_ERROR << "Failed: " << e.what();
		return -1;

	}

	return 0;
}
