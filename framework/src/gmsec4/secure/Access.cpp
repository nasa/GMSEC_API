/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Access.cpp
	Holds definition of AbstractAccess interface.
*/


#include <gmsec4/secure/Access.h>

namespace gmsec {
namespace api {
namespace secure {


Access::Access ()
	:
	ptr(new OpenAccess())
{
}


Access::~Access ()
{
	delete ptr;
	ptr = 0;
}


bool Access::isValid () const
{
	return ptr->isValid();
}


const char * Access::getID () const
{
	return ptr->getID();
}


bool Access::isMiddleware () const
{
	return ptr->isMiddleware();
}


Access::Mode Access::getMode () const
{
	return ptr->getMode();
}


Status Access::initialize (const Config &config)
{
	return ptr->initialize(config);
}


Status Access::reset ()
{
	return ptr->reset();
}


Status Access::authenticate ()
{
	return ptr->authenticate();
}


Status Access::canSubscribe (const char *subject)
{
	return ptr->canSubscribe(subject);
}


Status Access::canSend (const char *subject)
{
	return ptr->canSend(subject);
}



/*
 * AbstractAccess implementation
 */
AbstractAccess::~AbstractAccess()
{
}


void AbstractAccess::setExternal (Access &access)
{
	delete access.ptr;
	access.ptr = this;
}



/*
 * OpenAccess implementation
 */

bool OpenAccess::isValid () const
{
	return !status.isError();
}


const char * OpenAccess::getID () const
{
	return "OPEN";
}


bool OpenAccess::isMiddleware () const
{
	return false;
}


Access::Mode OpenAccess::getMode () const
{
	return Access::OPEN;
}


Status OpenAccess::initialize (const Config &)
{
	return status;
}


Status OpenAccess::reset ()
{
	return status;
}


Status OpenAccess::authenticate ()
{
	return status;
}


Status OpenAccess::canSubscribe (const char *)
{
	return status;
}


Status OpenAccess::canSend (const char *)
{
	return status;
}



/*
 * MiddlewareAccess
 */

bool MiddlewareAccess::isValid () const
{
	return !status.isError();
}


const char * MiddlewareAccess::getID () const
{
	return "MIDDLEWARE";
}


bool MiddlewareAccess::isMiddleware () const
{
	return true;
}


Access::Mode MiddlewareAccess::getMode () const
{
	return fMode;
}


Status MiddlewareAccess::initialize (const Config &)
{
	return status;
}


Status MiddlewareAccess::reset ()
{
	return status;
}


Status MiddlewareAccess::authenticate ()
{
	return status;
}


Status MiddlewareAccess::canSubscribe (const char *)
{
	return status;
}


Status MiddlewareAccess::canSend (const char *)
{
	return status;
}


} // namespace secure
} // namespace api
} // namespace gmsec


