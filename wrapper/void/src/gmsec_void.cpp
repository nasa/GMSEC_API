
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file gmsec_void.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#include <gmsec_void.h>

#include <gmsec/internal/Log.h>
#include <gmsec/internal/StatusException.h>

#include <VoidConnection.h>


using namespace gmsec;
using namespace gmsec::util;


GMSEC_VOID_API Status *CreateConnection(Config *cfg, Connection **conn)
{
	static Status result;

	result.ReSet();

	LOG_DEBUG << "gmsec_void:CreateConnection(" << cfg << ", " << conn << ")";

	try
	{
		VoidConnection * tmp = new VoidConnection(cfg);
		*conn = tmp->createExternal();
	}
	catch (gmsec::internal::StatusException &se)
	{
		result = se;
	}

	LOG_DEBUG << "gmsec_void: *conn=" << *conn;

	return &result;
}


GMSEC_VOID_API void DestroyConnection(Connection *conn)
{
	LOG_DEBUG << "gmsec_void:DestroyConnection(" << conn << ')';

	if (conn)
		gmsec::internal::ConnectionBuddy::destroy(conn);
}


//	EOF	gmsec_void.cpp

