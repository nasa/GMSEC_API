/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/Middleware.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/util/Log.h>

#include <map>


using gmsec::api::Status;
using gmsec::api::internal::Middleware;

using namespace gmsec::api::util;


static std::map<std::string, Middleware *> middlewares;


Middleware::Middleware()
{
}


Middleware::~Middleware()
{
}


Status Middleware::shutdown()
{
	Status status;
	return status;
}


Status Middleware::addMiddleware(const char* name0, Middleware* middleware)
{
	Status status;

	std::string name((name0 ? name0 : "unknown middleware"));

	Middleware* old = middlewares[name];

	if (old)
	{
		char buffer[256];
		StringUtil::stringFormat(buffer, sizeof(buffer), "Middleware '%s' already registered.", name0);

		status.set(MIDDLEWARE_ERROR, INVALID_CONNECTION_TYPE, buffer);
		GMSEC_WARNING << "Middleware::addMiddleware: " << status.get();

		delete old;
	}

	middlewares[name] = middleware;

	return status;
}


Status Middleware::shutdownAll()
{
	Status status;

	while (!middlewares.empty())
	{
		Status s = shutdown(middlewares.begin()->first.c_str());

		if (s.isError() && !status.isError())
		{
			status = s;
		}
	}

	return status;
}



Status Middleware::shutdown(const char* name)
{
	Status status;

	std::map<std::string, Middleware*>::iterator i = middlewares.find(name);

	if (i != middlewares.end())
	{
		GMSEC_DEBUG << "Attempting shutdown of middleware " << name;

		Middleware* middleware = i->second;

		status = middleware->shutdown();

		middlewares.erase(i);

		delete middleware;
	}

	return status;
}
