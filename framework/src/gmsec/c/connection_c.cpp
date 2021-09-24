
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file connection_c.cpp
 *
 *  This file contains the C-wrapper for functions in the Connection object.
 *
*/

#include <gmsec/c/connection_c.h>

#include <gmsec/Connection.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;
using std::string;


void Connect(GMSEC_CONNECTION_HANDLE conn, GMSEC_STATUS_HANDLE status)
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

void Disconnect(GMSEC_CONNECTION_HANDLE conn, GMSEC_STATUS_HANDLE status)
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

void GetLibraryVersion(GMSEC_CONNECTION_HANDLE conn, char **libver, GMSEC_STATUS_HANDLE status)
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

void GetLibraryRootName(GMSEC_CONNECTION_HANDLE conn, char **libname, GMSEC_STATUS_HANDLE status)
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

void RegisterErrorCallback(GMSEC_CONNECTION_HANDLE conn, char *event, GMSEC_ERROR_CALLBACK *cb, GMSEC_STATUS_HANDLE status)
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
		result = tmpConn->RegisterErrorCallback(event,cb);
	}

	if (NULL != status)
		*((Status *)status) = result;

}

void Subscribe(GMSEC_CONNECTION_HANDLE conn, char *subject, GMSEC_STATUS_HANDLE status)
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

void SubscribeWCallback(GMSEC_CONNECTION_HANDLE conn, char *subject, GMSEC_CALLBACK *cb, GMSEC_STATUS_HANDLE status)
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
		result = tmpConn->Subscribe(subject,cb);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void UnSubscribeWCallback(GMSEC_CONNECTION_HANDLE conn, char *subject, GMSEC_CALLBACK *cb, GMSEC_STATUS_HANDLE status)
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

void UnSubscribe(GMSEC_CONNECTION_HANDLE conn, char *subject, GMSEC_STATUS_HANDLE status)
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

void CreateMessageDflt(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE *msg, GMSEC_STATUS_HANDLE status)
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
		*msg = tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void CreateMessage(GMSEC_CONNECTION_HANDLE conn, char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_HANDLE *msg, GMSEC_STATUS_HANDLE status)
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
		*msg = tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void CreateMessageWCFG(GMSEC_CONNECTION_HANDLE conn, char *subject, GMSEC_MSG_KIND msgKind, GMSEC_MESSAGE_HANDLE *msg, GMSEC_CONFIG_HANDLE config, GMSEC_STATUS_HANDLE status)
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
		*msg = tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void CloneMessage(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE in, GMSEC_MESSAGE_HANDLE *out, GMSEC_STATUS_HANDLE status)
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
		*out = tmpMsg;
	}

	if (NULL != status)
		*((Status *)status) = result;
}


void DestroyMessage(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status)
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

void Publish(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status)
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

void RequestWCallback(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE request, GMSEC_I32 timeout, GMSEC_CALLBACK *cb, GMSEC_STATUS_HANDLE status)
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
		result = tmpConn->Request((Message *)request,timeout,cb);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void RequestWReplyCallback(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE request, GMSEC_I32 timeout, GMSEC_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er, GMSEC_STATUS_HANDLE status)
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
		result = tmpConn->Request((Message *)request,timeout,cb,er);
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void Request(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE request, GMSEC_I32 timeout, GMSEC_MESSAGE_HANDLE *reply, GMSEC_STATUS_HANDLE status)
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
		*reply = tmpMsg;
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void Reply(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE request,GMSEC_MESSAGE_HANDLE reply, GMSEC_STATUS_HANDLE status)
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

void StartAutoDispatch(GMSEC_CONNECTION_HANDLE conn, GMSEC_STATUS_HANDLE status)
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

void StopAutoDispatch(GMSEC_CONNECTION_HANDLE conn, GMSEC_STATUS_HANDLE status)
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

void GetNextMsg(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE *msg, GMSEC_I32 timeout, GMSEC_STATUS_HANDLE status)
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
		*msg = tmpMsg;
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void DispatchMsg(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status)
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

int IsConnected(GMSEC_CONNECTION_HANDLE conn, GMSEC_STATUS_HANDLE status)
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

void GetLastDispatcherStatus(GMSEC_CONNECTION_HANDLE conn, GMSEC_STATUS_HANDLE status)
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



