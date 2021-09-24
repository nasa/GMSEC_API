
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file connection.cpp
 *
 *  This file contains the C-wrapper for functions in the Connection object.
*/

#include <gmsec/c/connection.h>

#include <gmsec/Connection.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;
using std::string;


void gmsec_Connect(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::Connect : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Connect();
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_Disconnect(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::Disconnect : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Disconnect();
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_GetLibraryVersion(GMSEC_CONNECTION_OBJECT conn, const char **libver, GMSEC_STATUS_OBJECT status)
{
	static string tmp;
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::GetLibraryVersion : " << result.Get();
		*libver = NULL;

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		tmp = tmpConn->GetLibraryVersion();
		*libver = (char *)tmp.c_str();
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_GetLibraryRootName(GMSEC_CONNECTION_OBJECT conn, const char **libname, GMSEC_STATUS_OBJECT status)
{
	static string tmp;
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::GetLibraryRootName : " << result.Get();
		*libname = NULL;

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		tmp = tmpConn->GetLibraryRootName();
		*libname = (char *)tmp.c_str();
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_RegisterErrorCallback(GMSEC_CONNECTION_OBJECT conn, const char *event, GMSEC_C_ERROR_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::RegisterErrorCallback : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->RegisterErrorCallback(event,(GMSEC_ERROR_CALLBACK *)cb);
	}

	if (NULL != status)
		*((Status *)status) = result;

}

void gmsec_Subscribe(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::Subscribe : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Subscribe(subject);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_SubscribeWCallback(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_C_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::SubscribeWCallback : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Subscribe(subject,(GMSEC_CALLBACK *)cb);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_UnSubscribeWCallback(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::UnSubscribeWCallback : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->UnSubscribe(subject,cb);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_UnSubscribe(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::UnSubscribe : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->UnSubscribe(subject);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_CreateMessageDflt(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT *msg, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::CreateMessageDflt : " << result.Get(); 
		*msg = NULL;
	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		Message *tmpMsg = NULL;
		result = tmpConn->CreateMessage(tmpMsg);
		*msg = (GMSEC_MESSAGE_OBJECT)tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_CreateMessage(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_OBJECT *msg, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::CreateMessage : " << result.Get();
		*msg = NULL;
	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		Message *tmpMsg = NULL;
		result = tmpConn->CreateMessage(subject,msgKind,tmpMsg);
		*msg = (GMSEC_MESSAGE_OBJECT)tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_CreateMessageWCFG(GMSEC_CONNECTION_OBJECT conn, const char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_OBJECT *msg, GMSEC_CONFIG_OBJECT config, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::CreateMessageWCFG : " << result.Get();
		*msg = NULL;

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		Config *tmpConfig = (Config *)config;
		Message *tmpMsg = NULL;
		result = tmpConn->CreateMessage(subject,msgKind,tmpMsg,tmpConfig);
		*msg = (GMSEC_MESSAGE_OBJECT)tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_CloneMessage(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT in, GMSEC_MESSAGE_OBJECT *out, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::CloneMessage : " << result.Get();
		*out = NULL;

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		Message *inMsg = (Message *)in;
		Message *tmpMsg = NULL;

		result = tmpConn->CloneMessage(inMsg,tmpMsg);
		*out = (GMSEC_MESSAGE_OBJECT)tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}


void gmsec_DestroyMessage(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::DestroyMessage : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->DestroyMessage((Message *)msg);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_Publish(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::Publish : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Publish((Message *)msg);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_RequestWCallback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_C_CALLBACK *cb, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::RequestWCallback : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Request((Message *)request,timeout,(GMSEC_CALLBACK *)cb);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_RequestWReplyCallback(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_C_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::RequestWReplyCallback : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Request((Message *)request,timeout,(GMSEC_REPLY_CALLBACK *)cb,er);
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void gmsec_Request(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request, GMSEC_I32 timeout, GMSEC_MESSAGE_OBJECT *reply, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::Request : " << result.Get();
		*reply = NULL;

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		Message *tmpMsg = NULL;
		result = tmpConn->Request((Message *)request,timeout,tmpMsg);
		*reply = (GMSEC_MESSAGE_OBJECT)tmpMsg;
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_Reply(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT request,GMSEC_MESSAGE_OBJECT reply, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::Reply : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->Reply((Message *)request,(Message *)reply);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_StartAutoDispatch(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::StartAutoDispatch : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->StartAutoDispatch();
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_StopAutoDispatch(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::StopAutoDispatch : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->StopAutoDispatch();
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_GetNextMsg(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT *msg, GMSEC_I32 timeout, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::GetNextMsg : " << result.Get();
		*msg = NULL;

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		Message *tmpMsg = NULL;
		result = tmpConn->GetNextMsg(tmpMsg,timeout);
		*msg = (GMSEC_MESSAGE_OBJECT)tmpMsg;
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void gmsec_DispatchMsg(GMSEC_CONNECTION_OBJECT conn, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::DispatchMsg : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->DispatchMsg((Message *)msg);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

int gmsec_IsConnected(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
{
	Status result;
	int retVal = 0;

	if (conn == NULL)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is null");
                LOG_WARNING << "connection_c::IsConnected : " << result.Get();
	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		retVal = tmpConn->IsConnected();
	}
	if (status != NULL)
		*((Status *)status) = result;

	return retVal;
}

void gmsec_GetLastDispatcherStatus(GMSEC_CONNECTION_OBJECT conn, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == conn)
	{
		result.Set(GMSEC_STATUS_CONNECTION_ERROR,
		           GMSEC_INVALID_CONNECTION,
		           "Connection handle is NULL");
                LOG_WARNING << "connection_c::GetLastDispatcherStatus : " << result.Get();

	}
	else
	{
		Connection *tmpConn = (Connection *)conn;
		result = tmpConn->GetLastDispatcherStatus();
	}
	if (NULL != status)
		*((Status *)status) = result;
}



