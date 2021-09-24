/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API_INTERNAL_CONNECTION_BUDDY_H
#define GMSEC_API_INTERNAL_CONNECTION_BUDDY_H

#include <gmsec4/Connection.h>
#include <gmsec4/internal/InternalConnection.h>


namespace gmsec
{
namespace api
{
namespace internal
{


class ConnectionBuddy
{
public:
	static InternalConnection& getInternal(const Connection& conn)
	{
		return *conn.m_internal;
	}
};


}  // namespace internal
}  // namespace api
}  // namespace gmsec

#endif
