/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/Middleware.h>

#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/util/Log.h>

#include <map>


using gmsec::api5::Status;
using gmsec::api5::internal::Middleware;

using namespace gmsec::api5::util;


static std::map<std::string, Middleware *> middlewares;


Middleware::Middleware()
{
}


Middleware::~Middleware()
{
}


void Middleware::shutdown()
{
}


void Middleware::addMiddleware(const char* name0, Middleware* middleware)
{
	std::string name((name0 ? name0 : "unknown middleware"));

	Middleware* old = middlewares[name];

	if (old)
	{
		GMSEC_WARNING << "Middleware " << name0 << " has already been registered.";

		delete old;
	}

	middlewares[name] = middleware;
}


void Middleware::shutdownAll()
{
	while (!middlewares.empty())
	{
		shutdown(middlewares.begin()->first.c_str());
	}
}



void Middleware::shutdown(const char* name0)
{
	std::string name((name0 ? name0 : "unknown middleware"));

	std::map<std::string, Middleware*>::iterator i = middlewares.find(name);

	if (i != middlewares.end())
	{
		GMSEC_DEBUG << "Attempting shutdown of middleware " << name.c_str();

		Middleware* middleware = i->second;

		middleware->shutdown();

		middlewares.erase(i);

		delete middleware;
	}
}
