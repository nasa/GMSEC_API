/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Connection.cpp
 * Opaque Connection class.
 */

#include <gmsec/Connection.h>
#include <gmsec/internal/InternalConnection.h>


namespace gmsec
{

using namespace gmsec::internal;


Connection::Connection()
	: ptr(0)
{
}


Connection::Connection(InternalConnection* p)
	: ptr(p)
{
}


Connection::~Connection()
{
	delete ptr;
}


const char *Connection::GetLibraryRootName()
{
	return ptr->GetLibraryRootName();
}


const char *Connection::GetLibraryVersion()
{
	return ptr->GetLibraryVersion();
}


bool Connection::IsConnected()
{
	return ptr->IsConnected();
}


Status Connection::Connect()
{
	return ptr->Connect();
}


Status Connection::Disconnect()
{
	return ptr->Disconnect();
}


Status Connection::ConvertMessage(Message *in, Message *&out)
{
	return ptr->ConvertMessage(in, out);
}


Status Connection::ConvertCleanup(Message *in, Message *out)
{
	return ptr->ConvertCleanup(in, out);
}


Status Connection::CreateMessage(Message *&msg)
{
	return ptr->CreateMessage(msg);
}


Status Connection::CloneMessage(Message *in, Message *&out)
{
	return ptr->CloneMessage(in, out);
}


Status Connection::RegisterErrorCallback(const char *event, ErrorCallback *cb)
{
	return ptr->RegisterErrorCallback(event, cb);
}


Status Connection::RegisterErrorCallback(const char *event, GMSEC_ERROR_CALLBACK *cb)
{
	return ptr->RegisterErrorCallback(event, cb);
}


Status Connection::DispatchError(const char *event, Message *msg, Status *status)
{
	return ptr->DispatchError(event, msg, status);
}


Status Connection::Subscribe(const char *subject)
{
	return ptr->Subscribe(subject);
}


Status Connection::Subscribe(const char *subject, Callback *cb)
{
	return ptr->Subscribe(subject, cb);
}


Status Connection::Subscribe(const char *subject, GMSEC_CALLBACK *cb)
{
	return ptr->Subscribe(subject, cb);
}


Status Connection::Subscribe(const char *subject, const Config &config)
{
	return ptr->Subscribe(subject, config);
}


Status Connection::Subscribe(const char *subject, const Config &config, Callback* cb)
{
	return ptr->Subscribe(subject, config, cb);
}


Status Connection::UnSubscribe(const char *subject, Callback *cb)
{
	return ptr->UnSubscribe(subject, cb);
}


Status Connection::UnSubscribe(const char *subject, GMSEC_CALLBACK *cb)
{
	return ptr->UnSubscribe(subject, cb);
}

Status Connection::UnSubscribe(const char *subject)
{
	return ptr->UnSubscribe(subject);
}

Status Connection::Publish(Message *message)
{
	Config config = Config();
	return ptr->Publish(message, config);
}

Status Connection::Publish(Message *message, const Config &config)
{
	return ptr->Publish(message, config);
}


Status Connection::GetNextMsg(Message *&message, GMSEC_I32 timeout)
{
	return ptr->GetNextMsg(message, timeout);
}


Status Connection::Request(Message *request, GMSEC_I32 timeout, Callback *cb, GMSEC_I32 republish_ms)
{
	return ptr->Request(request, timeout, cb, republish_ms);
}


Status Connection::Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms)
{
	return ptr->Request(request, timeout, reply, republish_ms);
}


Status Connection::Reply(Message *request, Message *reply)
{
	return ptr->Reply(request, reply);
}


Status Connection::Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb, GMSEC_I32 republish_ms)
{
	return ptr->Request(request, timeout, cb, republish_ms);
}


Status Connection::Request(Message *request, GMSEC_I32 timeout, GMSEC_CALLBACK *cb)
{
	return ptr->Request(request, timeout, cb);
}

Status Connection::Request(Message *request, GMSEC_I32 timeout, GMSEC_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er)
{
	return ptr->Request(request, timeout, cb, er);
}

Status Connection::CreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, Message *&msg)
{
	return ptr->CreateMessage(subject, msgKind, msg);
}


Status Connection::CreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, Message *&msg, Config *config)
{
	return ptr->CreateMessage(subject, msgKind, msg, config);
}


Status Connection::DestroyMessage(Message *msg)
{
	return ptr->DestroyMessage(msg);
}


/*
 * The auto dispatcher will start a seperate thread, and then call the
 * connections getNextMessage and DispatchMessage methods to handle
 * automatic deployment of the messages.
 */
Status Connection::StartAutoDispatch()
{
	return ptr->StartAutoDispatch();
}



/*
 * If the dispatcher is currently running, terminate the dispatcher,
 * reclaim the memory and shutdown
 */
Status Connection::StopAutoDispatch()
{
	return ptr->StopAutoDispatch(true);
}


Status Connection::StopAutoDispatch(bool waitForComplete)
{
	return ptr->StopAutoDispatch(waitForComplete);
}



Status Connection::GetLastDispatcherStatus()
{
	return ptr->GetLastDispatcherStatus();
}


Status Connection::DispatchMsg(Message *msg)
{
	return ptr->DispatchMsg(msg);
}


Status Connection::ExcludeSubject(const char *subject)
{
	return ptr->ExcludeSubject(subject);
}


Status Connection::RemoveExcludedSubject(const char *subject)
{
	return ptr->RemoveExcludedSubject(subject);
}


const char * Connection::GetName()
{
	return ptr->GetName();
}


void Connection::SetName(const char *name)
{
	ptr->SetName(name);
}


const char * Connection::GetConnectionGUI()
{
	return ptr->GetConnectionGUI();
}


char * Connection::GetMWINFO(char * infoBuffer)
{
	return ptr->GetMWINFO(infoBuffer);
}

Status Connection::GetMWINFO(util::String &info)
{
	return ptr->GetMWINFO(info);
}

GMSEC_U64 Connection::GetPublishQueueMessageCount(){
	return ptr->GetPublishQueueMessageCount();
}

} // namespace gmsec
