/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file CallbackLookup.cpp
 *
 *  This file contains a class responsible for matching incoming messages
 *  with subscriptions (including patterns).
 */

#include <gmsec4/internal/CallbackLookup.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Callback.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


CallbackLookup::CallbackLookup()
	: m_callbackRegister(new CallbackRegister())
{
}


CallbackLookup::~CallbackLookup()
{
}


void CallbackLookup::addCallback(const char* topic, Callback* cb)
{
	Elements topicElements = StringUtil::split(topic, ".");

	if (topicElements.size() >= 1)
	{
		m_callbackRegister->insert(std::pair<Callback*, Elements>(cb, topicElements));
	}
}


void CallbackLookup::removeCallback(const char* topic)
{
	// Use-Case: A subscription topic may have been registered with more than one callback

	if (topic == NULL || *topic == '\0')
	{
		// Clear all subscriptions

		m_callbackRegister->clear();
	}
	else
	{
		// Remove all subscriptions for the given topic

		bool found = false;

		Elements topicElements = StringUtil::split(topic, ".");

		for (CallbackRegister::iterator it = m_callbackRegister->begin(); it != m_callbackRegister->end(); ++it)
		{
			bool match = false;

			if (it->second.size() == topicElements.size())
			{
				Elements::const_iterator it3 = topicElements.begin();

				for (Elements::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2, ++it3)
				{
					match = (*it2 == *it3);
				}
			}

			if (match)
			{
				found = true;
				m_callbackRegister->erase(it);
			}
		}

		if (!found)
		{
			throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "No callback registered for subject pattern");
		}
	}
}


void CallbackLookup::removeCallback(const char* topic, Callback* cb)
{
	// Use-Case: A callback may be shared by multiple subscription topics

	if (topic == NULL || *topic == '\0')
	{
		// Remove all subscriptions with the given callback

		bool found = false;

		CallbackRegister::iterator it  = m_callbackRegister->lower_bound(cb);
		CallbackRegister::iterator end = m_callbackRegister->upper_bound(cb);

		for (; it != end; ++it)
		{
			if (it->first == cb)
			{
				found = true;
				m_callbackRegister->erase(it);
			}
		}

		if (!found)
		{
			throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "Callback is not registered");
		}
	}
	else
	{
		// Remove the subscription for the given topic/callback

		bool found = false;

		Elements topicElements = StringUtil::split(topic, ".");

		CallbackRegister::iterator it  = m_callbackRegister->lower_bound(cb);
		CallbackRegister::iterator end = m_callbackRegister->upper_bound(cb);

		for (; it != end; ++it)
		{
			bool match = false;

			if (it->second.size() == topicElements.size())
			{
				Elements::const_iterator it3 = topicElements.begin();

				for (Elements::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2, ++it3)
				{
					match = (*it2 == *it3);
				}
			}

			if (match)
			{
				found = true;
				m_callbackRegister->erase(it);
				break;
			}
		}

		if (!found)
		{
			throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "No callback registered for subject pattern");
		}
	}
}


void CallbackLookup::collectCallbacks(const char* msgSubject, CallbackList& callbacks)
{
	Elements msgElements = StringUtil::split(msgSubject, ".");

	for (CallbackRegister::const_iterator it = m_callbackRegister.get()->begin(); it != m_callbackRegister.get()->end(); ++it)
	{
		const Elements& elements = it->second;

		bool match = false;
		bool done  = false;

		Elements::const_iterator it2 = elements.begin();
		Elements::const_iterator it3 = msgElements.begin();

		std::string lastTopicElement;

		//o Phase 1: Check each element of the msg subject with those of the subscription topic
		for (; it2 != elements.end() && it3 != msgElements.end() && !done; ++it2, ++it3)
		{
			if (*it2 == *it3)
			{
				match = true;
			}
			else if (*it2 == "*")
			{
				match = true;
			}
			else if (*it2 == ">")
			{
				match = true;
				done  = true;
			}
			else if (*it2 == "+")
			{
				match = true;
				done  = true;
			}
			else
			{
				match = false;
				done  = true;
			}

			lastTopicElement = *it2;
		}

		GMSEC_VERBOSE << "Phase 1: subject: " << msgSubject << " , match is: " << (match ? "true" : "false");

		//o Phase 2: If we have a preliminary match, check whether all elements of the msg subject and that
		//           of the subscription topic were analysed.
		if (match)
		{
			if ((it2 == elements.end()) && (it3 != msgElements.end()))
			{
				// Finished subscription topic elements, but not msg elements

				switch (lastTopicElement[0])
				{
				case '+':
				case '>':
					match = true;
					break;
				default:
					match = false;
				}
			}
			else if ((it2 != elements.end()) && (it3 == msgElements.end()))
			{
				// Finished msg elements, but not subscription topic elements

				match = (*it2 == "+");
			}
		}

		GMSEC_VERBOSE << "Phase 2: subject: " << msgSubject << " , match is: " << (match ? "true" : "false");

		if (match)
		{
			callbacks.push_back(it->first);
		}
	}
}


void CallbackLookup::dispatchMsg(const Message& msg)
{
	CallbackList tmpCB;
	collectCallbacks(msg.getSubject(), tmpCB);

	for (CallbackItr it = tmpCB.begin(); it != tmpCB.end(); ++it)
	{
		MBCallback* cb = dynamic_cast<MBCallback*>(*it);

		if (cb != NULL)
		{
			cb->onMessage(msg);
		}
	}
}


size_t CallbackLookup::getCallbackCount(const char* msgSubject)
{
	CallbackList tmpCB;

	collectCallbacks(msgSubject, tmpCB);

	return tmpCB.size();
}
