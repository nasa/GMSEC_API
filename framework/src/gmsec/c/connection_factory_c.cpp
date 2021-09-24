
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file connection_factory_c.cpp
 *
 *  This file contains the C-wrapper for functions in the Connection object.
*/

#include <gmsec/c/connection_factory_c.h>
#include <gmsec/ConnectionFactory.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

const char * CALL_TYPE GetAPIVersion()
{
	return ConnectionFactory::GetAPIVersion();
}

/* @deprecated - see CreateConnectionForType */
void CALL_TYPE CreateConnection(GMSEC_CONNECTION_TYPE connectionType, GMSEC_CONFIG_HANDLE config, GMSEC_CONNECTION_HANDLE *conn, GMSEC_STATUS_HANDLE status)
{
	Status result;

	if (NULL == config)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "connection_factory_c::CreateConnection : " << result.Get();
		*conn = NULL;
	}
	else
	{
		Connection *tmpConn = NULL;
		Config *cfg = (Config *)config;
		result = ConnectionFactory::Create(connectionType,cfg,tmpConn);
		*conn = tmpConn;
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void CALL_TYPE CreateConnectionForType(GMSEC_CONNECTION_TYPE connectionType, GMSEC_CONFIG_HANDLE config, GMSEC_CONNECTION_HANDLE *conn, GMSEC_STATUS_HANDLE status)
{
	Status result;

	if (NULL == config)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "connection_factory_c::CreateConnectionForType : " << result.Get();
		*conn = NULL;
	}
	else
	{
		Connection *tmpConn = NULL;
		Config *cfg = (Config *)config;
		result = ConnectionFactory::Create(connectionType,cfg,tmpConn);
		*conn = tmpConn;
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void CALL_TYPE CreateConnectionForConfig(GMSEC_CONFIG_HANDLE config, GMSEC_CONNECTION_HANDLE *conn, GMSEC_STATUS_HANDLE status)
{
	Status result;

	if (NULL == config)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "connection_factory_c::CreateConnectionForConfig : " << result.Get(); 
		*conn = NULL;
	}
	else
	{
		Connection *tmpConn = NULL;
		Config *cfg = (Config *)config;
		result = ConnectionFactory::Create(cfg,tmpConn);
		*conn = tmpConn;
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE DestroyConnection(GMSEC_CONNECTION_HANDLE conn, GMSEC_STATUS_HANDLE status)
{
	Status result;
	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_factory_c::DestroyConnection : " << result.Get();

	}
	else
	{
		result = ConnectionFactory::Destroy((Connection *)conn);
	}
	if (NULL != status)
		*((Status *)status) = result;
}


