/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <Queue.h>
#include <gmsec4/util/Mutex.h>

using namespace gmsec::api::util;
namespace gmsec_opendds
{

Queue::Queue()
{
}

Queue::~Queue()
{
	AutoMutex hold(m_queueCondition.getMutex());

	while(!m_queue.empty())
	{
		MessageInfo info = m_queue.front();
		m_queue.pop();
		delete info.msg;
	}
}

void Queue::push(MessageInfo info)
{
	AutoMutex hold(m_queueCondition.getMutex());

	m_queue.push(info);

	m_queueCondition.signal(GOT_MESSAGE);
}

MessageInfo Queue::pop()
{
	AutoMutex hold(m_queueCondition.getMutex());

	MessageInfo info = m_queue.front();

	m_queue.pop();

	return info;
}

bool Queue::isEmpty()
{
	AutoMutex hold(m_queueCondition.getMutex());

	return m_queue.empty();
}

int Queue::wait()
{
	m_queueCondition.getMutex().enterMutex();

	int signal = m_queueCondition.wait();

	m_queueCondition.getMutex().leaveMutex();

	return signal;
}

int Queue::wait(long timeout)
{
	m_queueCondition.getMutex().enterMutex();

	int signal = m_queueCondition.wait(timeout);

	m_queueCondition.getMutex().leaveMutex();

	return signal;
}

}
