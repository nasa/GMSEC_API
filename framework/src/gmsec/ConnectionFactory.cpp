/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file ConnectionFactory.cpp
 */

#include <gmsec_defs.h>

#include <gmsec/ConnectionFactory.h>
#include <gmsec/Connection.h>

#include <gmsec/internal/API4_Adapter.h>
#include <gmsec/internal/InternalConnection.h>

#include <gmsec/util/Log.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Exception.h>

#include <gmsec4/internal/Middleware.h>

#include <memory>


using namespace gmsec;
using namespace gmsec::internal;


Status ConnectionFactory::Create(Config* cfg3, Connection*& conn3)
{
	Status result;

	conn3 = 0;

	try
	{
		gmsec::api::Config cfg4 = API4_Adapter::API3ConfigToAPI4(cfg3);

		gmsec::api::Connection* conn4 = gmsec::api::Connection::create(cfg4);

		InternalConnection* intConn3 = new InternalConnection(conn4);

		conn3 = new Connection(intConn3);

		intConn3->registerParent(conn3);
	}
	catch (gmsec::api::Exception& e)
	{
		result = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return result;
}


Status ConnectionFactory::Create(GMSEC_CONNECTION_TYPE connectionType, Config *cfg, Connection *&conn)
{
	Status result;

	conn = 0;

	result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_INVALID_CONNECTION_TYPE, "This feature is no longer supported");

	LOG_ERROR << "ConnectionFactory::Create : " << result.Get();

	return result;
}


Status ConnectionFactory::Destroy(Connection* conn)
{
	Status result;

	if (conn != NULL)
	{
		delete conn;
	}
	else
	{
		result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_INVALID_CONNECTION, "Connection pointer is NULL or invalid.");

		LOG_ERROR << "ConnectionFactory::Destroy : " << result.Get();
	}

	return result;
}


const char* ConnectionFactory::GetAPIVersion()
{
	return GMSEC_VERSION;
}


Status ConnectionFactory::ShutdownAllMiddlewares()
{
	(void) gmsec::api::internal::Middleware::shutdownAll();

	return Status();
}


Status ConnectionFactory::ShutdownMiddleware(const char* name)
{
	(void) gmsec::api::internal::Middleware::shutdown(name);

	return Status();
}
