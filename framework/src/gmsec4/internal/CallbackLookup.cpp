/*
 * Copyright 2007-2018 United States Government as represented by the
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

#include <gmsec4/Callback.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Log.h>

#include <iostream>

using namespace gmsec::api;
using namespace gmsec::api::internal;


CallbackLookup::CallbackLookup()
{
}


CallbackLookup::~CallbackLookup()
{
	/* delete iterators */
	callbackLkpItr itr = fCallbackLkps.begin();
	while (itr != fCallbackLkps.end())
	{
		CallbackLookup *lkp = (*itr).second;
		delete lkp;
		++itr;
	}
}


void CallbackLookup::addCallback(const char* subject0, Callback* cb)
{
	if (!cb)
	{
		throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "Attempt to register a NULL Callback");
	}

	/* 1) if subject = "" then we store the cb here */
	if (subject0 == NULL || std::string(subject0).empty())
	{
		/* search for existing cb */
		bool found = false;

		for (callbackItr it = fCallbacks.begin(); it != fCallbacks.end(); ++it)
		{
			Callback* ocb = *it;

			if (ocb == cb)
			{
				found = true;
			}
		}
		if (!found)
		{
			fCallbacks.push_back(cb);
		}
		else
		{
			throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "Callback has already been registered");
		}
	}
	else
	{
		/* 2) get first level item "first.*" to save callback */
		std::string subject = subject0;
		std::string first   = subject;
		std::string last    = "";

		size_t t = subject.find(".");

		if (t != std::string::npos)
		{
			first = subject.substr(0,t);
			last  = subject.substr(t+1);
		}

		/* 3) lookup existing CallbackLookup for first level */
		callbackLkpItr iter = fCallbackLkps.find(first);
		CallbackLookup *lkp = NULL;
		if (iter == fCallbackLkps.end())
		{
			/* not found - create new */
			lkp = new CallbackLookup();
			fCallbackLkps.insert(callbackLkp::value_type(first, lkp));
		}
		else
		{
			lkp = iter->second;
		}
		if (!first.empty())
		{
			lkp->addCallback(last.c_str(), cb);
		}
	}
}


void CallbackLookup::removeCallback(const char* subject0)
{
	/* 1) if subject = "" then we clear cb's here */
	if (subject0 == NULL || std::string(subject0).empty())
	{
		fCallbacks.clear();
	}
	else
	{
		/* 2) get first level item "first.*" to save callback */
		std::string subject = subject0;
		std::string first   = subject;
		std::string last    = "";

		size_t t = subject.find(".");

		if (t != std::string::npos)
		{
			first = subject.substr(0,t);
			last  = subject.substr(t+1);
		}

		/* 3) lookup existing CallbackLookup for first level */
		callbackLkpItrNC iter = fCallbackLkps.find(first);
		if (iter!=fCallbackLkps.end())
		{
			CallbackLookup* lkp = iter->second;

			lkp->removeCallback(last.c_str());

			if (lkp->fCallbacks.size() && !lkp->fCallbackLkps.size())
			{
				fCallbackLkps.erase(iter);
				delete lkp;
			}
		}
		else
		{
			throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "No callback registered for subject pattern");
		}
	}
}


void CallbackLookup::removeCallback(const char* subject0, Callback* cb)
{
	/* 1) if subject = "" then we clear cb's here */
	if (subject0 == NULL || std::string(subject0).empty())
	{
		// remove this particular callback
		bool found = false;
		for (callbackList::iterator it = fCallbacks.begin(); it != fCallbacks.end(); ++it)
		{
			Callback* ocb = *it;

			if (ocb == cb)
			{
				found = true;
				fCallbacks.erase(it);
				break;
			}
		}

		if (!found)
		{
			throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "Callback is not registered");
		}
	}
	else
	{
		/* 2) get first level item "first.*" to save callback */
		std::string subject = subject0;
		std::string first   = subject;
		std::string last    = "";

		size_t t = subject.find(".");

		if (t != std::string::npos)
		{
			first = subject.substr(0,t);
			last  = subject.substr(t+1);
		}

		/* 3) lookup existing CallbackLookup for first level */
		callbackLkpItrNC iter = fCallbackLkps.find(first);
		if (iter != fCallbackLkps.end())
		{
			CallbackLookup* lkp = iter->second;

			lkp->removeCallback(last.c_str(), cb);

			if (!lkp->fCallbacks.size() && !lkp->fCallbackLkps.size())
			{
				fCallbackLkps.erase(iter);
				delete lkp;
			}
		}
		else
		{
			throw Exception(CALLBACK_LOOKUP_ERROR, INVALID_CALLBACK, "No callback registered for subject pattern");
		}
	}
}


void CallbackLookup::collectCallbacks(const char* subject0, callbackList& callbacks)
{
	/* if subject == "" then we dispatch */
	if (subject0 == NULL || std::string(subject0).empty())
	{
		collectCallbacksFinalStep(callbacks, fCallbacks);
	}
	else
	{
		/* 2) get first level item "first.*" to save callback */
		std::string subject = subject0;
		std::string first   = subject;
		std::string last    = "";

		size_t t = subject.find(".");

		if (t != std::string::npos)
		{
			first = subject.substr(0,t);
			last  = subject.substr(t+1);
		}

		/* 3) lookup "*" entries and lookup existing CallbackLookup for first level */
		callbackLkpItr end = fCallbackLkps.end();
		callbackLkpItr iter;
		callbackLkpItr nextIter;

		if ((iter = fCallbackLkps.find("*")) != end || (iter = fCallbackLkps.find(first)) != end)
		{
			/* 4) lookup "+" entries, look ahead to the next level */
			if ((nextIter = iter->second->fCallbackLkps.find("+")) != iter->second->fCallbackLkps.end())
			{
				iter->second->collectCallbacksFinalStep(callbacks, nextIter->second->fCallbacks);
			}
			else
			{
				iter->second->collectCallbacks(last.c_str(), callbacks);	
			}
		}

		/* 5) lookup ">" entries */
		if ((iter = fCallbackLkps.find(">")) != end)
		{
			iter->second->collectCallbacksFinalStep(callbacks, iter->second->fCallbacks);
		}

		/* 6) lookup existing CallbackLookup for first level */
		if ((iter = fCallbackLkps.find(first)) != end)
		{
			iter->second->collectCallbacks(last.c_str(), callbacks);
		}
	}
}


void CallbackLookup::dispatchMsg(const char* subject, const Message& msg)
{
	callbackList tmpCB;

	collectCallbacks(subject, tmpCB);

	for (callbackItr it = tmpCB.begin(); it != tmpCB.end(); ++it)
	{
		MBCallback* cb = dynamic_cast<MBCallback*>(*it);

		if (cb != NULL)
		{
			cb->onMessage(msg);
		}
	}
}


size_t CallbackLookup::getCallbackCount(const char* subject)
{
	callbackList tmpCB;

	collectCallbacks(subject, tmpCB);

	return tmpCB.size();
}


void CallbackLookup::collectCallbacksFinalStep(callbackList& callbacks, callbackList& fCallbacks)
{
	callbackItr itr = fCallbacks.begin();

	while (itr != fCallbacks.end())
	{
		Callback* cb = *itr;

		if (find(callbacks.begin(), callbacks.end(), cb) == callbacks.end())
		{
			callbacks.push_back(cb);
		}

		++itr;
	}
}
