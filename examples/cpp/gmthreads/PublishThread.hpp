/*
 * Copyright 2007-2016 United States Government as represented by the
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

#ifndef PUBLISH_THREAD_HPP
#define PUBLISH_THREAD_HPP

#include "../example.h"


class PublishThread
{
public:
	PublishThread(gmsec::api::Config& c);

	~PublishThread();

	void publish(const char* subject);

	void CALL_TYPE run();

	void CALL_TYPE shutdown();

private:
	// Declared, but not implemented
	PublishThread(const PublishThread& pt);
	PublishThread& operator=(const PublishThread& pt);

	void setup();
	void teardown();

	gmsec::api::Config&     config;
	gmsec::api::Connection* connection;
};

#endif
