/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef OPENDDS_MESSAGE_QUEUE_H
#define OPENDDS_MESSAGE_QUEUE_H

#include <queue>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>
#include <gmsec4/util/Condition.h>


namespace gmsec_opendds
{

struct MessageInfo
{
    gmsec::api::Status   result;
    gmsec::api::Message* msg;
};


class Queue
{
public:
	enum
	{
		INVALID = gmsec::api::util::Condition::INVALID,
		TIMEOUT = gmsec::api::util::Condition::TIMEOUT,
		GOT_MESSAGE = gmsec::api::util::Condition::USER
	};

	Queue();

	~Queue();

	void push(MessageInfo msg);

	MessageInfo pop();

	bool isEmpty();

	int wait();

	int wait(long timeout);

private:
	std::queue<MessageInfo>     m_queue;
	gmsec::api::util::Condition m_queueCondition;
};

}

#endif
