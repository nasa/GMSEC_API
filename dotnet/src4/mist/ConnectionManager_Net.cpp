/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/ConnectionManager_Net.h>

#include <mist/ConnectionManagerCallback_Net.h>
#include <mist/ConnectionManagerEventCallback_Net.h>
#include <mist/ConnectionManagerReplyCallback_Net.h>
#include <mist/ServiceParam_Net.h>
#include <mist/SubscriptionInfo_Net.h>

#include <field/Field_Net.h>

#include <Config_Net.h>
#include <Message_Net.h>
#include <Status_Net.h>

#include <GMSEC_Net.h>


// C++ API Native
#include <gmsec4/util/DataList.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


ConnectionManager::ConnectionManager(Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	try
	{
		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::ConnectionManager(*nativeConfig, true);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ConnectionManager::ConnectionManager(Config^ config, bool validate)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	try
	{
		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::ConnectionManager(*nativeConfig, validate);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ConnectionManager::ConnectionManager(Config^ config, bool validate, unsigned int version)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	try
	{
		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::ConnectionManager(*nativeConfig, validate, version);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ConnectionManager::~ConnectionManager()
{
	this->!ConnectionManager();
}


void ConnectionManager::Initialize()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

	try
	{
		m_impl->initialize();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::Cleanup()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

	try
	{
		m_impl->cleanup();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


String^ ConnectionManager::GetLibraryVersion()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

	return gcnew String(m_impl->getLibraryVersion());
}


void ConnectionManager::SetStandardFields(List<Field^>^ standardFields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(standardFields, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "List of Standard Fields is null");

	try
	{
		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		for (int i = 0; i < standardFields->Count; ++i)
		{
			Field^ field = standardFields[i];

			if (field != nullptr)
			{
				nativeFields.push_back(field->GetChild());
			}
		}

		m_impl->setStandardFields(nativeFields);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::AddStandardFields(Message^ toMsg)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(toMsg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	gmsec::api::Message* nativeToMsg = toMsg->GetUnmanagedImplementation();

	m_impl->addStandardFields(*nativeToMsg);
}


void ConnectionManager::RegisterEventCallback(Connection::ConnectionEvent event, ConnectionManagerEventCallback^ ecb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(ecb, StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");

	try
	{
		m_impl->registerEventCallback(static_cast<gmsec::api::Connection::ConnectionEvent>(event), ecb->GetUnmanagedImplementation());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


GMSEC::API::MIST::SubscriptionInfo^ ConnectionManager::Subscribe(String^ subject)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::mist::SubscriptionInfo* info = m_impl->subscribe(subjectStr);

		return gcnew GMSEC::API::MIST::SubscriptionInfo(info);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


GMSEC::API::MIST::SubscriptionInfo^ ConnectionManager::Subscribe(String^ subject, ConnectionManagerCallback^ cb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(cb, StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");
	THROW_EXCEPTION_IF_NULLPTR(cb->GetUnmanagedImplementation(), StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Callback implementation is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::mist::ConnectionManagerCallback* nativeCallback = cb->GetUnmanagedImplementation();

		gmsec::api::mist::SubscriptionInfo* info = m_impl->subscribe(subjectStr, nativeCallback);

		return gcnew GMSEC::API::MIST::SubscriptionInfo(info, cb);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


GMSEC::API::MIST::SubscriptionInfo^ ConnectionManager::Subscribe(String^ subject, Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		gmsec::api::mist::SubscriptionInfo* info = m_impl->subscribe(subjectStr, *nativeConfig);

		return gcnew GMSEC::API::MIST::SubscriptionInfo(info);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


GMSEC::API::MIST::SubscriptionInfo^ ConnectionManager::Subscribe(String^ subject, Config^ config, ConnectionManagerCallback^ cb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");
	THROW_EXCEPTION_IF_NULLPTR(cb, StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");
	THROW_EXCEPTION_IF_NULLPTR(cb->GetUnmanagedImplementation(), StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Callback implementation is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		gmsec::api::mist::ConnectionManagerCallback* nativeCallback = cb->GetUnmanagedImplementation();

		gmsec::api::mist::SubscriptionInfo* info = m_impl->subscribe(subjectStr, *nativeConfig, nativeCallback);

		return gcnew GMSEC::API::MIST::SubscriptionInfo(info, cb);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


void ConnectionManager::Unsubscribe(GMSEC::API::MIST::SubscriptionInfo^ % info)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(info, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "SubscriptionInfo is null");

	try
	{
		gmsec::api::mist::SubscriptionInfo* nativeInfo = info->GetUnmanagedImplementation();

		m_impl->unsubscribe(nativeInfo);

		info->ResetUnmanagedImplementation();

		info = nullptr;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


bool ConnectionManager::StartAutoDispatch()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

	try
	{
		return m_impl->startAutoDispatch();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


bool ConnectionManager::StopAutoDispatch()
{
	try
	{
		return this->StopAutoDispatch(true);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


bool ConnectionManager::StopAutoDispatch(bool waitForCompletion)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

	try
	{
		return m_impl->stopAutoDispatch(waitForCompletion);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::Publish(Message^ msg)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(msg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	gmsec::api::Message* nativeMsg = ((Message^) msg)->GetUnmanagedImplementation();

	try
	{
		m_impl->publish(*nativeMsg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::Publish(Message^ msg, Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(msg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	gmsec::api::Message* nativeMsg = ((Message^) msg)->GetUnmanagedImplementation();
	gmsec::api::Config*  nativeCfg = ((Config^) config)->GetUnmanagedImplementation();

	try
	{
		m_impl->publish(*nativeMsg, *nativeCfg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::Request(Message^ requestMsg, Int32 timeout, ConnectionManagerReplyCallback^ rcb, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(requestMsg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Request Message is null");
	THROW_EXCEPTION_IF_NULLPTR(rcb, StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Reply Callback is null");

	try
	{
		gmsec::api::Message* nativeRequestMsg = ((Message^) requestMsg)->GetUnmanagedImplementation();

		m_impl->request(*nativeRequestMsg, timeout, rcb->GetUnmanagedImplementation(), republish_ms);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Message^ ConnectionManager::Request(Message^ requestMsg, Int32 timeout, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(requestMsg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Request Message is null");

	try
	{
		gmsec::api::Message* nativeRequestMsg = ((Message^) requestMsg)->GetUnmanagedImplementation();

		gmsec::api::Message* nativeReplyMsg = m_impl->request(*nativeRequestMsg, timeout, republish_ms);

		if (nativeReplyMsg != nullptr)
		{
			return gcnew Message(nativeReplyMsg, true);
		}

		return nullptr;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::CancelRequest(ConnectionManagerReplyCallback^ rcb)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(rcb, StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Reply Callback is null");

	try
	{
		m_impl->cancelRequest(rcb->GetUnmanagedImplementation());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::Reply(Message^ requestMsg, Message^ replyMsg)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(requestMsg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Request Message is null");
	THROW_EXCEPTION_IF_NULLPTR(replyMsg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Reply Message is null");

	try
	{
		gmsec::api::Message* nativeRequestMsg = ((Message^) requestMsg)->GetUnmanagedImplementation();
		gmsec::api::Message* nativeReplyMsg   = ((Message^) replyMsg)->GetUnmanagedImplementation();

		m_impl->reply(*nativeRequestMsg, *nativeReplyMsg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::Dispatch(Message^ msg)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(msg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* nativeMsg = ((Message^) msg)->GetUnmanagedImplementation();

		m_impl->dispatch(*nativeMsg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Message^ ConnectionManager::Receive()
{
	return this->Receive(-1);
}


Message^ ConnectionManager::Receive(Int32 timeout)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

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


void ConnectionManager::Release(Message^ %message)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	message->ResetUnmanagedImplementation();
	message = nullptr;
}


void ConnectionManager::ExcludeSubject(String^ subject)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

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
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


void ConnectionManager::RemoveExcludedSubject(String^ subject)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

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
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


Message^ ConnectionManager::CreateHeartbeatMessage(String^ subject, List<Field^>^ heartbeatFields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (heartbeatFields != nullptr)
		{
			for (int i = 0; i < heartbeatFields->Count; ++i)
			{
				Field^ field = heartbeatFields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		gmsec::api::Message nativeMsg = m_impl->createHeartbeatMessage(subjectStr, nativeFields);

		return gcnew Message(new gmsec::api::Message(nativeMsg), true);
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


void ConnectionManager::StartHeartbeatService(String^ subject, List<Field^>^ heartbeatFields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (heartbeatFields != nullptr)
		{
			for (int i = 0; i < heartbeatFields->Count; ++i)
			{
				Field^ field = heartbeatFields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		m_impl->startHeartbeatService(subjectStr, nativeFields);
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


void ConnectionManager::StopHeartbeatService()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

	try
	{
		m_impl->stopHeartbeatService();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Status^ ConnectionManager::ChangeComponentStatus(Field^ componentStatus)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(componentStatus, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Field is null");

	try
	{
		gmsec::api::Field* nativeField  = ((Field^) componentStatus)->GetChild();
		gmsec::api::Status nativeStatus = m_impl->changeComponentStatus(*nativeField);

		return gcnew Status(new gmsec::api::Status(nativeStatus));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Status^ ConnectionManager::ChangeComponentInfo(Field^ componentInfo)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(componentInfo, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Field is null");

	try
	{
		gmsec::api::Field* nativeField  = ((Field^) componentInfo)->GetChild();
		gmsec::api::Status nativeStatus = m_impl->changeComponentInfo(*nativeField);

		return gcnew Status(new gmsec::api::Status(nativeStatus));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Status^ ConnectionManager::ChangeCPUMemory(Field^ cpuMemory)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(cpuMemory, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Field is null");

	try
	{
		gmsec::api::Field* nativeField  = ((Field^) cpuMemory)->GetChild();
		gmsec::api::Status nativeStatus = m_impl->changeCPUMemory(*nativeField);

		return gcnew Status(new gmsec::api::Status(nativeStatus));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Status^ ConnectionManager::ChangeCPUUtil(Field^ cpuUtil)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(cpuUtil, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Field is null");

	try
	{
		gmsec::api::Field* nativeField  = ((Field^) cpuUtil)->GetChild();
		gmsec::api::Status nativeStatus = m_impl->changeCPUUtil(*nativeField);

		return gcnew Status(new gmsec::api::Status(nativeStatus));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Message^ ConnectionManager::CreateLogMessage(String^ subject, List<Field^>^ logFields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (logFields != nullptr)
		{
			for (int i = 0; i < logFields->Count; ++i)
			{
				Field^ field = logFields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		gmsec::api::Message nativeMsg = m_impl->createLogMessage(subjectStr, nativeFields);

		return gcnew Message(new gmsec::api::Message(nativeMsg), true);
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


void ConnectionManager::SetLoggingDefaults(String^ subject, List<Field^>^ logFields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (logFields != nullptr)
		{
			for (int i = 0; i < logFields->Count; ++i)
			{
				Field^ field = logFields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		m_impl->setLoggingDefaults(subjectStr, nativeFields);
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


void ConnectionManager::PublishLog(String^ logMessage, Field^ severity)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(logMessage, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Log message string is null");
	THROW_EXCEPTION_IF_NULLPTR(severity, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Field is null");

	char* logMessageStr = nullptr;

	try
	{
		logMessageStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(logMessage).ToPointer());

		gmsec::api::Field* nativeField = ((Field^) severity)->GetChild();

		m_impl->publishLog(logMessageStr, *nativeField);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process log message string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(logMessageStr);
	}
}


void ConnectionManager::RequestDirective(String^ subject, Field^ directiveString, List<Field^>^ fields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(directiveString, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Directive String Field is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Field* nativeDirectiveStr = ((Field^) directiveString)->GetChild();

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		m_impl->requestDirective(subjectStr, *nativeDirectiveStr, nativeFields);
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


void ConnectionManager::RequestDirective(String^ subject, Field^ directiveString, List<Field^>^ fields,
                                         Int32 timeout, ConnectionManagerReplyCallback^ rcb, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(directiveString, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Directive String Field is null");
	THROW_EXCEPTION_IF_NULLPTR(rcb, StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Field* nativeDirectiveStr = ((Field^) directiveString)->GetChild();

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		m_impl->requestDirective(subjectStr, *nativeDirectiveStr, nativeFields, timeout, rcb->GetUnmanagedImplementation(), republish_ms);
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


Message^ ConnectionManager::RequestDirective(String^ subject, Field^ directiveString, List<Field^>^ fields, Int32 timeout, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(directiveString, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "Directive String Field is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Field* nativeDirectiveStr = ((Field^) directiveString)->GetChild();

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		gmsec::api::Message* nativeReply = m_impl->requestDirective(subjectStr, *nativeDirectiveStr, nativeFields, timeout, republish_ms);

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
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process subject");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}


void ConnectionManager::AcknowledgeDirectiveRequest(String^ subject, Message^ request, ResponseStatus status, List<Field^>^ fields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(request, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Request Message is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Message* nativeRequest = ((Message^) request)->GetUnmanagedImplementation();

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		m_impl->acknowledgeDirectiveRequest(subjectStr, *nativeRequest, static_cast<gmsec::api::mist::ResponseStatus::Response>(status), nativeFields);
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


void ConnectionManager::PublishResourceMessage(String^ subject, long sampleInterval, long averageInterval)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl->publishResourceMessage(subjectStr, (size_t) sampleInterval, (size_t) averageInterval);
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


Message^ ConnectionManager::CreateResourceMessage(String^ subject, long sampleInterval, long averageInterval)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Message nativeMsg = m_impl->createResourceMessage(subjectStr, (size_t) sampleInterval, (size_t) averageInterval);

		return gcnew Message(new gmsec::api::Message(nativeMsg), true);
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


void ConnectionManager::StartResourceMessageService(String^ subject, long interval, long sampleInterval, long averageInterval)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		m_impl->startResourceMessageService(subjectStr, (size_t) interval, (size_t) sampleInterval, (size_t) averageInterval);
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


void ConnectionManager::StopResourceMessageService()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");

	try
	{
		m_impl->stopResourceMessageService();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConnectionManager::RequestSimpleService(String^ subject, String^ opName, Field^ opNumber,
                                             List<Field^>^ fields, List<ServiceParam^>^ params)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(opName, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "OpName is null");
	THROW_EXCEPTION_IF_NULLPTR(opNumber, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "OpNumber is null");

	char* subjectStr = nullptr;
	char* opNameStr  = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		opNameStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(opName).ToPointer());

		gmsec::api::Field* nativeOpNumber = ((Field^) opNumber)->GetChild();

		gmsec::api::util::DataList<gmsec::api::Field*>              nativeFields;
		gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*> nativeParams;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		if (params != nullptr)
		{
			for (int i = 0; i < params->Count; ++i)
			{
				ServiceParam^ param = params[i];

				if (param!= nullptr)
				{
					nativeParams.push_back(param->GetUnmanagedImplementation());
				}
			}
		}

		m_impl->requestSimpleService(subjectStr, opNameStr, *nativeOpNumber, nativeFields, nativeParams);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process string arguments");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(opNameStr);
	}

}


void ConnectionManager::RequestSimpleService(String^ subject, String^ opName, Field^ opNumber,
                                             List<Field^>^ fields, List<ServiceParam^>^ params, Int32 timeout,
                                             ConnectionManagerReplyCallback^ rcb, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(opName, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "OpName is null");
	THROW_EXCEPTION_IF_NULLPTR(opNumber, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "OpNumber is null");
	THROW_EXCEPTION_IF_NULLPTR(rcb, StatusClass::MIST_ERROR, StatusCode::INVALID_CALLBACK, "Callback is null");

	char* subjectStr = nullptr;
	char* opNameStr  = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		opNameStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(opName).ToPointer());

		gmsec::api::Field* nativeOpNumber = ((Field^) opNumber)->GetChild();

		gmsec::api::util::DataList<gmsec::api::Field*>              nativeFields;
		gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*> nativeParams;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		if (params != nullptr)
		{
			for (int i = 0; i < params->Count; ++i)
			{
				ServiceParam^ param = params[i];

				if (param!= nullptr)
				{
					nativeParams.push_back(param->GetUnmanagedImplementation());
				}
			}
		}

		m_impl->requestSimpleService(subjectStr, opNameStr, *nativeOpNumber, nativeFields, nativeParams, timeout, rcb->GetUnmanagedImplementation(), republish_ms);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process string arguments");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(opNameStr);
	}
}


Message^ ConnectionManager::RequestSimpleService(String^ subject, String^ opName, Field^ opNumber,
                                                 List<Field^>^ fields, List<ServiceParam^>^ params, Int32 timeout, Int32 republish_ms)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(opName, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "OpName is null");
	THROW_EXCEPTION_IF_NULLPTR(opNumber, StatusClass::MIST_ERROR, StatusCode::INVALID_FIELD, "OpNumber is null");

	char* subjectStr = nullptr;
	char* opNameStr  = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		opNameStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(opName).ToPointer());

		gmsec::api::Field* nativeOpNumber = ((Field^) opNumber)->GetChild();

		gmsec::api::util::DataList<gmsec::api::Field*>              nativeFields;
		gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*> nativeParams;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		if (params != nullptr)
		{
			for (int i = 0; i < params->Count; ++i)
			{
				ServiceParam^ param = params[i];

				if (param!= nullptr)
				{
					nativeParams.push_back(param->GetUnmanagedImplementation());
				}
			}
		}

		gmsec::api::Message* nativeReply = m_impl->requestSimpleService(subjectStr, opNameStr, *nativeOpNumber, nativeFields, nativeParams, timeout, republish_ms);

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
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONNECTION_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process string arguments");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(opNameStr);
	}
}


void ConnectionManager::AcknowledgeSimpleService(String^ subject, Message^ request, ResponseStatus status, List<Field^>^ fields)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConnectionManager is null");
	THROW_EXCEPTION_IF_NULLPTR(subject, StatusClass::MIST_ERROR, StatusCode::INVALID_SUBJECT_NAME, "Subject is null");
	THROW_EXCEPTION_IF_NULLPTR(request, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Request Message is null");

	char* subjectStr = nullptr;

	try
	{
		subjectStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());

		gmsec::api::Message* nativeRequestMsg = ((Message^) request)->GetUnmanagedImplementation();

		gmsec::api::util::DataList<gmsec::api::Field*> nativeFields;

		if (fields != nullptr)
		{
			for (int i = 0; i < fields->Count; ++i)
			{
				Field^ field = fields[i];

				if (field != nullptr)
				{
					nativeFields.push_back(field->GetChild());
				}
			}
		}

		m_impl->acknowledgeSimpleService(subjectStr, *nativeRequestMsg, static_cast<gmsec::api::mist::ResponseStatus::Response>(status), nativeFields);
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


ConnectionManager::!ConnectionManager()
{
	ResetUnmanagedImplementation();
}


ConnectionManager::ConnectionManager(gmsec::api::mist::ConnectionManager* connMgr, bool owned)
{
	SetUnmanagedImplementation(connMgr, owned);
}


gmsec::api::mist::ConnectionManager* ConnectionManager::GetUnmanagedImplementation()
{
	return m_impl;
}


void ConnectionManager::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ConnectionManager::SetUnmanagedImplementation(gmsec::api::mist::ConnectionManager* connMgr, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = connMgr;
	m_owned = owned;
}
