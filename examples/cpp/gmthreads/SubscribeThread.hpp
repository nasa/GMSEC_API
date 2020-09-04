/*
 * Copyright 2007-2016 United States Government as represented by the
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

#ifndef SUBSCRIBE_THREAD_HPP
#define SUBSCRIBE_THREAD_HPP

#include "../example.h"


class SubscribeThread
{
public:
	SubscribeThread(gmsec::api::Config& c);

	~SubscribeThread();

	void CALL_TYPE run();

	void CALL_TYPE shutdown();

private:
	// Defined, but not implemented
	SubscribeThread(const SubscribeThread& st);
	SubscribeThread& operator=(const SubscribeThread& st);

	void setup();
	void teardown();

	gmsec::api::Config&     config;
	gmsec::api::Connection* connection;
	gmsec::api::SubscriptionInfo* info;
};

#endif
