/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file PublishThread.hpp
 * 
 * This file contains a simple example publisher using the GMSEC standard C++ API.
 * 
 */

#ifndef PublishThread_hpp
#define PublishThread_hpp

#include <iostream>
#include <string>
#include "gmsec/util/Thread.h"
#include "../example.h"

using namespace std;
using namespace gmsec;
using namespace example;

class PublishThread: public gmsec::util::Thread {

public:

	/* constructor - used to pass arguments in to the class and to
	 set up the subscriptions */
	PublishThread(Config &c);
	virtual ~PublishThread();

	int Publish(const char *subject);

	virtual void CALL_TYPE run();

private:

	/* copying not allowed */
	PublishThread(const PublishThread& pt); // = delete;
	PublishThread& operator=(const PublishThread& pt); // = delete;

	Status result;
	Config &config;
	Connection * connection;
};

#endif

