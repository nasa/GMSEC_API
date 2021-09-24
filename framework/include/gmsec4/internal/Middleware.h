/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_MIDDLEWARE_H
#define GMSEC_API_INTERNAL_MIDDLEWARE_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec {
namespace api {
namespace internal {


class GMSEC_API Middleware
{
public:
	Middleware();

	// base class needs virtual destructor
	virtual ~Middleware();

	// The default implementation of this method is trivial.
	virtual void shutdown();

	static void shutdown(const char* name);

	static void shutdownAll();

	static void addMiddleware(const char* name, Middleware* middleware);

private:
	// Declared, but not implemented.
	Middleware(const Middleware&);
	Middleware& operator=(const Middleware&);
};


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
