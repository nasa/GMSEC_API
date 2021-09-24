/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SubscriptionDetails.cpp
 * @brief This file contains the class that stores details regarding
 * subscriptions that have been set up with the middleware.
 *
 */

#include <gmsec4/internal/SubscriptionDetails.h>

#include <gmsec4/Callback.h>

using namespace gmsec::api;
using namespace gmsec::api::internal;


Details::Details(Callback* cb, const Config& cfg)
	: callback(cb),
	  config(cfg)
{
}


SubscriptionDetails::SubscriptionDetails()
	: m_details(),
	  m_iter(m_details.begin())
{
}


SubscriptionDetails::~SubscriptionDetails()
{
	removeDetails(0);
}


void SubscriptionDetails::addDetails(Callback* cb, const Config& cfg)
{
	m_details.push_back(new Details(cb, cfg));
}


void SubscriptionDetails::removeDetails(Callback* cb)
{
	if (!cb)
	{
		for (DetailsList::iterator it = m_details.begin(); it != m_details.end(); ++it)
		{
			delete *it;
		}

		m_details.clear();
	}
	else
	{
		for (DetailsList::iterator it = m_details.begin(); it != m_details.end(); ++it)
		{
			const Details* details = *it;

			if (details->callback == cb)
			{
				delete *it;
				m_details.erase(it);
				break;
			}
		}
	}
}


size_t SubscriptionDetails::numDetailsRegistered() const
{
	return m_details.size();
}


bool SubscriptionDetails::callbackRegistered(Callback* cb) const
{
	bool found = false;

	for (DetailsList::const_iterator it = m_details.begin(); it != m_details.end() && !found; ++it)
	{
		const Details* details = *it;

		found = (details->callback == cb);
	}

	return found;
}


void SubscriptionDetails::resetIter()
{
	m_iter = m_details.begin();
}


bool SubscriptionDetails::hasNext()
{
	return m_iter != m_details.end();
}


const Details* SubscriptionDetails::next()
{
	const Details* details = 0;

	if (hasNext())
	{
		details = *m_iter;
		++m_iter;
	}

	return details;
}
