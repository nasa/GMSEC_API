/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



// Managed
#include <Connection_Net.h>
#include <Callback_Net.h>
#include <Config_Net.h>
#include <EventCallback_Net.h>
#include <GMSEC_Exception_Net.h>
#include <Message_Net.h>
#include <ReplyCallback_Net.h>
#include <SubscriptionInfo_Net.h>

#include <GMSEC_Net.h>



using namespace GMSEC::API;
using namespace System;
using namespace System::Runtime::InteropServices;



Connection::~Connection()
{
	this->!Connection();
}


Connection^ Connection::Create(Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::FACTORY_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	try
	{
		gmsec::api::Config*     cfg  = ((Config^) config)->GetUnmanagedImplementation();
		gmsec::api::Connection* conn = gmsec::api::Connection::create(*cfg);

		return gcnew Connection(conn);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::Destroy(Connection^ % conn)
{
	THROW_EXCEPTION_IF_NULLPTR(conn, StatusClass::FACTORY_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	gmsec::api::Connection* nativeConn = conn->GetUnmanagedImplementation();

	gmsec::api::Connection::destroy(nativeConn);

	conn->ResetUnmanagedImplementation();

	conn = nullptr;
}


String^ Connection::GetAPIVersion()
{
	return gcnew String(gmsec::api::Connection::getAPIVersion());
}


void Connection::ShutdownAllMiddlewares()
{
	gmsec::api::Connection::shutdownAllMiddlewares();
}


void Connection::ShutdownMiddleware(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::Connection::shutdownMiddleware(nameStr);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY,
			"Unable to process name");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


void Connection::Connect()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	try
	{
		m_impl->connect();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::Disconnect()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	try
	{
		m_impl->disconnect();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Connection::ConnectionState Connection::GetState()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return static_cast<Connection::ConnectionState>(m_impl->getState());
}


String^ Connection::GetLibraryRootName()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return gcnew String(m_impl->getLibraryRootName());
}


String^ Connection::GetLibraryVersion()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return gcnew String(m_impl->getLibraryVersion());
}


void Connection::RegisterEventCallback(ConnectionEvent event, EventCallback^ ecb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(ecb, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");

	try
	{
		m_impl->registerEventCallback(static_cast<gmsec::api::Connection::ConnectionEvent>(event), ecb->GetUnmanagedImplementation());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


SubscriptionInfo^ Connection::Subscribe(String^ subject)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::SubscriptionInfo* info = m_impl->subscribe(subjectStr);

		return gcnew SubscriptionInfo(info);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY,
			"Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


SubscriptionInfo^ Connection::Subscribe(String^ subject, Callback^ cb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(cb, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");
	THROW_EXCEPTION_IF_NULLPTR(cb->GetUnmanagedImplementation(), StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CALLBACK, "Callback implementation is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Callback* nativeCallback = cb->GetUnmanagedImplementation();

		gmsec::api::SubscriptionInfo* info = m_impl->subscribe(subjectStr, nativeCallback);

		return gcnew SubscriptionInfo(info);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


SubscriptionInfo^ Connection::Subscribe(String^ subject, Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		gmsec::api::SubscriptionInfo* info = m_impl->subscribe(subjectStr, *nativeConfig);

		return gcnew SubscriptionInfo(info);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


SubscriptionInfo^ Connection::Subscribe(String^ subject, Config^ config, Callback^ cb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONFIG, "Config is null");
	THROW_EXCEPTION_IF_NULLPTR(cb, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");
	THROW_EXCEPTION_IF_NULLPTR(cb->GetUnmanagedImplementation(), StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CALLBACK, "Callback implementation is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		gmsec::api::Callback* nativeCallback = cb->GetUnmanagedImplementation();

		gmsec::api::SubscriptionInfo* info = m_impl->subscribe(subjectStr, *nativeConfig, nativeCallback);

		return gcnew SubscriptionInfo(info);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


void Connection::Unsubscribe(SubscriptionInfo^ %info)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(info, StatusClass::CONNECTION_ERROR, StatusCode::UNINITIALIZED_OBJECT, "SubscriptionInfo is null");

	try
	{
		gmsec::api::SubscriptionInfo* nativeInfo = info->GetUnmanagedImplementation();

		m_impl->unsubscribe(nativeInfo);

		info->ResetUnmanagedImplementation();

		info = nullptr;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


bool Connection::StartAutoDispatch()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return m_impl->startAutoDispatch();
}


bool Connection::StopAutoDispatch()
{
	return this->StopAutoDispatch(true);
}


bool Connection::StopAutoDispatch(bool waitForCompletion)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return m_impl->stopAutoDispatch(waitForCompletion);
}


void Connection::Publish(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	gmsec::api::Message* nativeMsg = ((Message^) message)->GetUnmanagedImplementation();

	try
	{
		m_impl->publish(*nativeMsg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::Publish(Message^ message, Config^ mwConfig)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");
	THROW_EXCEPTION_IF_NULLPTR(mwConfig, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	gmsec::api::Message* nativeMsg = ((Message^) message)->GetUnmanagedImplementation();
	gmsec::api::Config*  nativeCfg = ((Config^) mwConfig)->GetUnmanagedImplementation();

	try
	{
		m_impl->publish(*nativeMsg, *nativeCfg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::Request(Message^ request, Int32 timeout, ReplyCallback^ rcb, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(request, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Request message is null");
	THROW_EXCEPTION_IF_NULLPTR(rcb, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CALLBACK, "ReplyCallback is null");

	try
	{
		gmsec::api::Message* nativeRequest = ((Message^) request)->GetUnmanagedImplementation();

		m_impl->request(*nativeRequest, timeout, rcb->GetUnmanagedImplementation(), republish_ms);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Message^ Connection::Request(Message^ request, Int32 timeout, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(request, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Request message is null");

	try
	{
		gmsec::api::Message* nativeRequest = ((Message^) request)->GetUnmanagedImplementation();

		gmsec::api::Message* nativeReply = m_impl->request(*nativeRequest, timeout, republish_ms);

		if (nativeReply != nullptr)
		{
			return gcnew Message(nativeReply, true);
		}

		return nullptr;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::CancelRequest(ReplyCallback^ rcb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(rcb, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CALLBACK, "ReplyCallback is null");

	try
	{
		m_impl->cancelRequest(rcb->GetUnmanagedImplementation());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::Reply(Message^ request, Message^ reply)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(request, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Request message is null");
	THROW_EXCEPTION_IF_NULLPTR(reply, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Reply message is null");

	try
	{
		gmsec::api::Message* nativeRequest = ((Message^) request)->GetUnmanagedImplementation();
		gmsec::api::Message* nativeReply   = ((Message^) reply)->GetUnmanagedImplementation();

		m_impl->reply(*nativeRequest, *nativeReply);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::Dispatch(Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* nativeMsg = ((Message^) message)->GetUnmanagedImplementation();

		m_impl->dispatch(*nativeMsg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Message^ Connection::Receive()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return this->Receive(-1);
}


Message^ Connection::Receive(Int32 timeout)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	try
	{
		gmsec::api::Message* nativeMsg = m_impl->receive(timeout);

		if (nativeMsg != nullptr)
		{
			return gcnew Message(nativeMsg, true);
		}

		return nullptr;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Connection::Release(Message^ %message)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	message->ResetUnmanagedImplementation();
	message = nullptr;
}


void Connection::ExcludeSubject(String^ subject)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl->excludeSubject(subjectStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY,
			"Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


void Connection::RemoveExcludedSubject(String^ subject)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl->removeExcludedSubject(subjectStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY,
			"Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


String^ Connection::GetName()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return gcnew String(m_impl->getName());
}


void Connection::SetName(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		m_impl->setName(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY,
			"Unable to process name");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


String^ Connection::GetID()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return gcnew String(m_impl->getID());
}


String^ Connection::GetMWInfo()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return gcnew String(m_impl->getMWInfo());
}


UInt64 Connection::GetPublishQueueMessageCount()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONNECTION_ERROR, StatusCode::INVALID_CONNECTION, "Connection is null");

	return m_impl->getPublishQueueMessageCount();
}


Connection::!Connection()
{
	ResetUnmanagedImplementation();
}


Connection::Connection(gmsec::api::Connection* conn)
{
	SetUnmanagedImplementation(conn);
}


gmsec::api::Connection* Connection::GetUnmanagedImplementation()
{
	return m_impl;
}


void Connection::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}


void Connection::SetUnmanagedImplementation(gmsec::api::Connection* conn)
{
	m_impl = conn;
}


Connection::Connection()
{
	m_impl = nullptr;
}
