/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/UniqueFilter.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api::internal;
using namespace gmsec::api::util;
using namespace std;


static const int DEFAULT_SEPARATOR = '_';
static const int DEFAULT_DEBUG = 0;



UniqueFilter::UniqueFilter()
	: separator(DEFAULT_SEPARATOR),
	  debug(DEFAULT_DEBUG)
{
}


UniqueFilter::~UniqueFilter()
{
}


void UniqueFilter::setSeparator(char c)
{
	separator = c;
}


void UniqueFilter::setDebug(bool flag)
{
	debug = flag;
}


bool UniqueFilter::update(const char* id0)
{
	if (id0 == NULL)
	{
		GMSEC_WARNING << "UniqueFilter.update: id is NULL";
		return true;
	}

	string id = id0;

	size_t premid = id.find_last_of(separator);
	if (premid == string::npos)
	{
		GMSEC_VERBOSE << "UniqueFilter.update: invalid id " << id.c_str();
		return true;
	}

	string connection_id = id.substr(0, premid);
	string message_id = id.substr(premid+1);
	GMSEC_I32 current = 0;
	if (!StringUtil::stringParseI32(message_id.c_str(), current))
	{
		GMSEC_VERBOSE << "UniqueFilter.update: invalid message_id " << message_id.c_str();
		return true;
	}

	bool unique = false;

	map<string, int>::iterator probe = state.find(connection_id);
	if (probe == state.end())
	{
		GMSEC_VERBOSE << "UniqueFilter.update: new connection " << connection_id.c_str();
		unique = true;
	}
	else
	{
		int previous = (*probe).second;
		const char * info = "none";

		if (current < 0 && previous > 0)
		{
			/* counter must have rolled over */
			unique = true;
			info = "roll over";
		}
		else if (current > previous)
		{
			unique = true;
			info = "increase";
		}
		else if (current <= previous)
		{
			info = "obsolete";
		}

		if (debug)
		{
			GMSEC_DEBUG << "UniqueFilter.update: current=" << current
			            << " previous=" << previous
			            << " => unique=" << unique << " info=" << info;
		}
	}

	if (unique)
	{
		state[connection_id] = current;
	}

	return unique;
}

