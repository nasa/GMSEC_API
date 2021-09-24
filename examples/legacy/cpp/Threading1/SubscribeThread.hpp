/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/**
 * @file SubscribeThread.hpp
 *  
 * This file contains a simple example subscriber using the GMSEC standard C++ API.
 * 
 */

#ifndef SubscribeThread_hpp
#define SubscribeThread_hpp

#include <iostream>
#include <string>
#include "gmsec/util/Thread.h"
#include "../example.h"

using namespace std;
using namespace gmsec;
using namespace example;

class SubscribeThread: public gmsec::util::Thread {

public:

	/* constructor - used to pass arguments in to the class and to
	 set up the subscriptions */
	SubscribeThread(Config &c);
	virtual ~SubscribeThread();

	virtual void CALL_TYPE run();


private:

	/* for now, copying not allowed */
	SubscribeThread(const SubscribeThread& st);
	SubscribeThread& operator=(const SubscribeThread& st);

	Status result;
	Config &config;
	Connection * connection;
};

#endif
