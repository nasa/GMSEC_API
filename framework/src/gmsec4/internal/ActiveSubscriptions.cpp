/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ActiveSubscriptions.h
 * @brief This file contains the class that stores details regarding
 * subscriptions that have been set up by an application.
 */

#include <gmsec4/internal/ActiveSubscriptions.h>


using namespace gmsec::api::internal;
using namespace gmsec::api::util;


ActiveSubscriptions::ActiveSubscriptions()
	: m_registered(),
	  m_mutex(),
	  m_topics()
{
}


ActiveSubscriptions::~ActiveSubscriptions()
{
}


void ActiveSubscriptions::addTopic(const char* topic)
{
	AutoTicket hold(m_mutex);

	RegisteredTopics::iterator it = m_registered.find(topic);

	if (it == m_registered.end())
	{
		m_registered[topic] = 1;

		m_topics.insert(topic);
	}
	else
	{
		++m_registered[topic];
	}
}


void ActiveSubscriptions::removeTopic(const char* topic)
{
	AutoTicket hold(m_mutex);

	RegisteredTopics::iterator it = m_registered.find(topic);

	if (it != m_registered.end())
	{
		int count = --m_registered[topic];

		if (count == 0)
		{
			std::set<std::string>::iterator it = m_topics.find(topic);

			if (it != m_topics.end())
			{
				m_topics.erase(it);
			}
		}
	}
}


std::set<std::string> ActiveSubscriptions::getTopics()
{
	AutoTicket hold(m_mutex);

	return m_topics;
}
