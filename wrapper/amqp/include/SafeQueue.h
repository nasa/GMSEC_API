/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef AMQP_SAFE_QUEUE_H
#define AMQP_SAFE_QUEUE_H

#include <gmsec5/util/Mutex.h>

#include <queue>


namespace gmsec_amqp
{

template <class T>
class SafeQueue
{
public:
	SafeQueue() {}

	void push(T item)
	{
		gmsec::api5::util::AutoMutex lock(m_mutex);
		m_queue.push(item);
	}

	T front()
	{
		gmsec::api5::util::AutoMutex lock(m_mutex);
		return m_queue.front();
	}

	void pop()
	{
		gmsec::api5::util::AutoMutex lock(m_mutex);
		m_queue.pop();
	}

	size_t size()
	{
		gmsec::api5::util::AutoMutex lock(m_mutex);
		return m_queue.size();
	}

	bool empty()
	{
		return size() == 0;
	}

private:
	std::queue<T>            m_queue;
	gmsec::api5::util::Mutex m_mutex;
};

}

#endif
