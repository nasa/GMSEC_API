/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef AMQP_SUBSCRIPTION_H
#define AMQP_SUBSCRIPTION_H

#include "AMQPConnection.h"

#include <string>

namespace gmsec_amqp
{

struct AMQPSubscription
{
	std::string     fullAddr;
	std::string     username;
	std::string     password;
	AMQPConnection* amqpConn;

	AMQPSubscription(const std::string& addr, const std::string& username, const std::string& password, AMQPConnection* conn)
		: fullAddr(addr),
		  username(username),
		  password(password),
		  amqpConn(conn)
	{}
};

}

#endif
