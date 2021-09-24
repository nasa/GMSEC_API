/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManager.cpp
 *
 *  @brief This file contains a class for the services portion of the MIST ConnectionManager.
 *
**/

#include <gmsec/mist/ConnectionManager.h>

#include <gmsec/internal/mist/BaseConnectionManager.h>

#include <gmsec/Status.h>


using namespace gmsec::mist::internal;


namespace gmsec
{

namespace mist
{

ConnectionManager::ConnectionManager(Config &cfg, unsigned int version, bool validate)
	: mgr(new BaseConnectionManager(cfg, version, validate)),
	  own(true)
{
}

ConnectionManager::ConnectionManager(BaseConnectionManager* baseMgr)
	: mgr(baseMgr),
	  own(false)
{
}

ConnectionManager::~ConnectionManager()
{
	if (own)
	{
		delete mgr;
		mgr = 0;
	}
}

Status ConnectionManager::SetStandardFields(Field* standardFields[], size_t fieldCount)
{
	return mgr->SetStandardFields(standardFields, fieldCount);
}

Status ConnectionManager::Initialize()
{
	return mgr->Initialize();
}

Status ConnectionManager::Cleanup()
{
	return mgr->Cleanup();
}

Status ConnectionManager::RegisterErrorCallback(const char *event, ConnectionManagerErrorCallback *cb)
{
	return mgr->RegisterErrorCallback(event, cb);
}

Status ConnectionManager::Subscribe(const char *subject)
{
	return mgr->Subscribe(subject);
}

Status ConnectionManager::Subscribe(const char *subject, ConnectionManagerCallback *cb)
{
	return mgr->Subscribe(subject, cb);
}

Status ConnectionManager::Subscribe(const char *subject, const Config &config)
{
	return mgr->Subscribe(subject, config);
}

Status ConnectionManager::UnSubscribe(const char *subject)
{
	return mgr->UnSubscribe(subject);
}

Status ConnectionManager::UnSubscribe(const char *subject, ConnectionManagerCallback *cb)
{
	return mgr->UnSubscribe(subject, cb);
}

Status ConnectionManager::CreateMessage(const char* subject, GMSEC_MSG_KIND kind, Message *&msg)
{
	return mgr->CreateMessage(subject, kind, msg);
}

Status ConnectionManager::ConvertMessage(Message *in, Message *&out)
{
	return mgr->ConvertMessage(in, out);
}

Status ConnectionManager::ConvertCleanup(Message *in, Message *out)
{
	return mgr->ConvertCleanup(in, out);
}

Status ConnectionManager::CloneMessage(Message *in, Message *&out)
{
	return mgr->CloneMessage(in, out);
}

Status ConnectionManager::DestroyMessage(Message *msg)
{
	return mgr->DestroyMessage(msg);
}

Status ConnectionManager::Publish(Message *msg)
{
	return mgr->Publish(msg);
}

Status ConnectionManager::Publish(Message *msg, const Config &config)
{
	return mgr->Publish(msg, config);
}

Status ConnectionManager::Request(Message *request, GMSEC_I32 timeout, ConnectionManagerReplyCallback *cb, GMSEC_I32 republish_ms)
{
	return mgr->Request(request, timeout, cb, republish_ms);
}

Status ConnectionManager::Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms)
{
	return mgr->Request(request, timeout, reply, republish_ms);
}

Status ConnectionManager::Reply(Message *request, Message *reply)
{
	return mgr->Reply(request, reply);
}

Status ConnectionManager::StartAutoDispatch()
{
	return mgr->StartAutoDispatch();
}

Status ConnectionManager::StopAutoDispatch()
{
	return mgr->StopAutoDispatch();
}

Status ConnectionManager::StopAutoDispatch(bool waitForComplete)
{
	return mgr->StopAutoDispatch(waitForComplete);
}

Status ConnectionManager::GetNextMsg(Message *&msg, GMSEC_I32 timeout)
{
	return mgr->GetNextMsg(msg, timeout);
}

Status ConnectionManager::DispatchMsg(Message *msg)
{
	return mgr->DispatchMsg(msg);
}

Status ConnectionManager::GetLastDispatcherStatus()
{
	return mgr->GetLastDispatcherStatus();
}

Status ConnectionManager::ExcludeSubject(const char* subject)
{
	return mgr->ExcludeSubject(subject);
}

Status ConnectionManager::RemoveExcludedSubject(const char* subject)
{
	return mgr->RemoveExcludedSubject(subject);
}

Status ConnectionManager::GetHeartbeatMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char * subject)
{
	return mgr->GetHeartbeatMessage(standardFields, fieldCount, msg, subject);
}

Status ConnectionManager::StartHeartbeatService(Field * standardFields[], size_t fieldCount, const char * subject)
{
	return mgr->StartHeartbeatService(standardFields, fieldCount, subject);
}

Status ConnectionManager::ChangeComponentStatus(const Field &componentStatus)
{
	return mgr->ChangeComponentStatus(componentStatus);
}

Status ConnectionManager::ChangeComponentInfo(const Field &componentInfo)
{
	return mgr->ChangeComponentInfo(componentInfo);
}

Status ConnectionManager::ChangeCPUUtil(const Field &cpuUtil)
{
	return mgr->ChangeCPUUtil(cpuUtil);
}

Status ConnectionManager::ChangeCPUMemory(const Field &cpuMemory)
{
	return mgr->ChangeCPUMemory(cpuMemory);
}
		
Status ConnectionManager::StopHeartbeatService()
{
	return mgr->StopHeartbeatService();
}

Status ConnectionManager::GetLogMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char * subject)
{
	return mgr->GetLogMessage(standardFields, fieldCount, msg, subject);
}

Status ConnectionManager::SetLoggingDefaults(Field* standardFields[], size_t fieldCount, const char * subject)
{
	return mgr->SetLoggingDefaults(standardFields, fieldCount, subject);
}

Status ConnectionManager::QuickLog(const char* msg, const Field &severity)
{
	return mgr->QuickLog(msg, severity);
}

}

}
