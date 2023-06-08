/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_OPENDDS_MESSAGE_QUEUE_H
#define GMSEC_OPENDDS_MESSAGE_QUEUE_H

#include <queue>

#include <gmsec5/Message.h>
#include <gmsec5/Status.h>
#include <gmsec5/util/Condition.h>


namespace gmsec_opendds
{

struct MessageInfo
{
    gmsec::api5::Status   result;
    gmsec::api5::Message* msg;
};


class Queue
{
public:
	enum
	{
		INVALID = gmsec::api5::util::Condition::INVALID,
		TIMEOUT = gmsec::api5::util::Condition::TIMEOUT,
		GOT_MESSAGE = gmsec::api5::util::Condition::USER
	};

	Queue();

	~Queue();

	void push(MessageInfo msg);

	MessageInfo pop();

	bool isEmpty();

	int wait();

	int wait(long timeout);

private:
	std::queue<MessageInfo>      m_queue;
	gmsec::api5::util::Condition m_queueCondition;
};

}

#endif
