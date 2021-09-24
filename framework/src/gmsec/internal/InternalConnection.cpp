/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalConnection.cpp
 *
 *  @brief This file contains the opaque wrapper for all connections. Instructions
 *	are passed through this interface to the "real" connection.
 */

#include <gmsec/internal/InternalConnection.h>

#include <gmsec/internal/InternalMessage.h>

#include <gmsec/internal/API4_Adapter.h>
#include <gmsec/internal/GarbageCollector.h>
#include <gmsec/internal/Log.h>

#include <gmsec/Callback.h>
#include <gmsec/Config.h>
#include <gmsec/Connection.h>
#include <gmsec/ErrorCallback.h>
#include <gmsec/Message.h>
#include <gmsec/ReplyCallback.h>

#include <gmsec4/internal/ConnectionBuddy.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>
#include <gmsec4/SubscriptionInfo.h>

#include <gmsec4/util/StdUniquePtr.h>


using namespace gmsec;
using namespace gmsec::internal;


InternalConnection::InternalConnection(gmsec::api::Connection* p)
	: m_adapter(p),
	  m_parent(0),
	  m_collector(new GarbageCollector())
{
	gmsec::api::internal::ConnectionBuddy().getInternal(*m_adapter).usingAPI3x();
}


InternalConnection::~InternalConnection()
{
	gmsec::api::util::AutoMutex lock(m_subscribeMutex);

	for (Subscriptions::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
	{
		delete it->second;
	}

	if (IsConnected())
	{
		Disconnect();
	}

	gmsec::api::Connection::destroy(m_adapter);

	delete m_collector;
}


void InternalConnection::registerParent(Connection* parent)
{
	m_parent = parent;
}


const char* InternalConnection::GetLibraryRootName()
{
	return m_adapter->getLibraryRootName();
}


const char* InternalConnection::GetLibraryVersion()
{
	return m_adapter->getLibraryVersion();
}


char* InternalConnection::GetMWINFO(char* infoBuffer)
{
	if (infoBuffer == NULL) return NULL;
	gmsec::api::util::StringUtil::stringCopy(infoBuffer, m_adapter->getMWInfo(), Connection::MWINFO_BUFFER);
	return infoBuffer;
}


Status InternalConnection::GetMWINFO(gmsec::util::String& info)
{
	Status status;
	info = m_adapter->getMWInfo();
	return status;
}


Status InternalConnection::Connect()
{
	Status status;

	if (IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has already been established");
		return status;
	}

	try
	{
		m_adapter->connect();
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::Disconnect()
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	try
	{
		m_adapter->disconnect();
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


bool InternalConnection::IsConnected()
{
	return m_adapter->getState() == gmsec::api::Connection::CONNECTED;
}


Status InternalConnection::RegisterErrorCallback(const char* event, ErrorCallback* ecb)
{
	Status status;

	if (event != NULL && ecb != NULL)
	{
		try
		{
			gmsec::api::Connection::ConnectionEvent event4 = API4_Adapter::API3EventToAPI4(event);

			gmsec::api::EventCallback* ecb4 = m_collector->newEventCallback(m_parent, ecb);

			m_adapter->registerEventCallback(event4, ecb4);
		}
		catch (...)
		{
			// Ignore error; user must have passed a custom event string
		}

		m_errorCallbacks[event] = ecb;
	}

	return status;
}


Status InternalConnection::Subscribe(const char* subject)
{
	Config cfg;
	return Subscribe(subject, cfg, (Callback*) NULL);
}


Status InternalConnection::Subscribe(const char* subject, Callback* cb)
{
	Config cfg;
	return Subscribe(subject, cfg, cb);
}


Status InternalConnection::Subscribe(const char* subject, const Config& config, Callback* cb)
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	try
	{
		gmsec::api::SubscriptionInfo* info = NULL;

		gmsec::api::Config config4 = API4_Adapter::API3ConfigToAPI4(&config);

		if (cb == NULL)
		{
			info = m_adapter->subscribe(subject, config4);
		}
		else
		{
			gmsec::api::util::AutoMutex lock(m_subscribeMutex);

			bool newSubscription = true;
			Subscriptions::iterator it = m_subscriptions.find(subject);

			if (it != m_subscriptions.end())
			{
				// Possibly an existing subscription
				SubscriptionDetails* subDetails = it->second;

				newSubscription = (subDetails->hasDetails(subject, cb) == false);
			}

			if (newSubscription)
			{
				gmsec::api::util::StdUniquePtr<gmsec::api::Callback> cb4(new API4_Adapter::API4Callback(m_parent, cb));

				info = m_adapter->subscribe(subject, config4, cb4.get());

				cb4.release();
			}
			else
			{
				status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_SUBJECT_NAME,
					"Duplicate subscription using callback");
				return status;
			}
		}

		if (info != NULL)
		{
			Subscriptions::iterator it = m_subscriptions.find(subject);

			if (it == m_subscriptions.end())
			{
				// new subscription
				SubscriptionDetails* details = new SubscriptionDetails();
				details->addDetails(cb, info);

				m_subscriptions[subject] = details;
			}
			else
			{
				it->second->addDetails(cb, info);
			}
		}
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::UnSubscribe(const char* subject, Callback* cb)
{
	Status status;

	if (subject == NULL || std::string(subject).empty())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_SUBJECT_NAME, "Subject name is NULL");
		return status;
	}

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	gmsec::api::util::AutoMutex lock(m_subscribeMutex);

	Subscriptions::iterator it = m_subscriptions.find(subject);

	if (it == m_subscriptions.end())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_SUBJECT_NAME, "Not subscribed to pattern");
	}
	else
	{
		SubscriptionDetails* subDetails  = it->second;
		std::list<Details*>  infoDetails = subDetails->getDetails(subject, cb);

		for (std::list<Details*>::iterator it2 = infoDetails.begin(); it2 != infoDetails.end(); ++it2)
		{
			Details* details = *it2;

			gmsec::api::Callback* cb4 = details->info->getCallback();

			try
			{
				m_adapter->unsubscribe(details->info);
			}
			catch (...)
			{
				// ignore error
			}

			delete details;

			if (cb4)
			{
				delete cb4;
			}
		}

		if (subDetails->hasMoreDetails(subject) == false)
		{
			delete subDetails;

			m_subscriptions.erase(it);
		}
	}

	return status;
}


Status InternalConnection::CreateMessage(Message*& msg)
{
	Status status;

	msg = new Message();

	return status;
}


Status InternalConnection::CreateMessage(const char* subject, GMSEC_MSG_KIND msgKind, Message*& msg)
{
	Status status;

	msg = 0;

	try
	{
		msg = new Message(subject, msgKind);
	}
	catch (gmsec::api::Exception& e)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, e.getErrorCode(), e.getErrorMessage());
	}

	return status;
}


Status InternalConnection::CreateMessage(const char* subject, GMSEC_MSG_KIND msgKind, Message*& msg, Config* config)
{
	Status status;

	msg = 0;

	try
	{
		msg = new Message(subject, msgKind, config);
	}
	catch (gmsec::api::Exception& e)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, e.getErrorCode(), e.getErrorMessage());
	}


	return status;
}


Status InternalConnection::ConvertMessage(Message* in, Message*& out)
{
	Status status;

	out = 0;

	if (in == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_MSGCONVERT_FAILURE, "Message is NULL");
		return status;
	}

	out = in;

	return status;
}


Status InternalConnection::ConvertCleanup(Message* in, Message* out)
{
	Status status;

	if (in != out)
	{
		delete out;
	}

	return status;
}


Status InternalConnection::CloneMessage(Message* in, Message*& out)
{
	Status status;

	out = NULL;

	if (in == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Message is NULL");
		return status;
	}

	gmsec::api::Message* in4Ref   = API4_Adapter::referenceAPI4Message(in);
	gmsec::api::Message* in4Clone = new gmsec::api::Message(*in4Ref);

	out = new Message(new InternalMessage(in4Clone));

	return status;
}


Status InternalConnection::DestroyMessage(Message* msg)
{
	Status status;

	delete msg;

	return status;
}


Status InternalConnection::Publish(Message* msg, const Config& config)
{
	Status status;

	if (msg == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Message is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	gmsec::api::Message* msg4 = API4_Adapter::referenceAPI4Message(msg);
	gmsec::api::Config   cfg4 = API4_Adapter::API3ConfigToAPI4(&config);

	try
	{
		m_adapter->publish(*msg4, cfg4);
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::Request(Message* request, GMSEC_I32 timeout, Callback* cb, GMSEC_I32 republish_ms)
{
	Status status;

	if (request == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Request message is NULL");
		return status;
	}
	if (cb == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Callback is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	gmsec::api::Message* req4 = API4_Adapter::referenceAPI4Message(request);

	try
	{
		gmsec::api::ReplyCallback* rcb4 = m_collector->newReplyCallback(m_parent, cb);

		m_adapter->request(*req4, timeout, rcb4, republish_ms);
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::Request(Message* request, GMSEC_I32 timeout, ReplyCallback* rcb, GMSEC_I32 republish_ms)
{
	Status status;

	if (request == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Request message is NULL");
		return status;
	}
	if (rcb == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Callback is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	gmsec::api::Message* req4 = API4_Adapter::referenceAPI4Message(request);

	try
	{
		gmsec::api::ReplyCallback* rcb4 = m_collector->newReplyCallback(m_parent, rcb);

		m_adapter->request(*req4, timeout, rcb4, republish_ms);
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::Request(Message* request, GMSEC_I32 timeout, Message*& reply, GMSEC_I32 republish_ms)
{
	Status status;

	reply = NULL;

	if (request == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Request message is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	gmsec::api::Message* req4 = API4_Adapter::referenceAPI4Message(request);

	try
	{
		gmsec::api::Message* rep4 = m_adapter->request(*req4, timeout, republish_ms);

		if (rep4 != NULL)
		{
			reply = new Message(new InternalMessage(rep4));
		}
		else
		{
			status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_TIMEOUT_OCCURRED, "Timeout");
		}
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::Reply(Message* request, Message* reply)
{
	Status status;

	if (request == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Request message is NULL");
		return status;
	}
	if (reply == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Reply message is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	gmsec::api::Message* req4 = API4_Adapter::referenceAPI4Message(request);
	gmsec::api::Message* rep4 = API4_Adapter::referenceAPI4Message(reply);

	try
	{
		m_adapter->reply(*req4, *rep4);
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::StartAutoDispatch()
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	try
	{
		bool running = m_adapter->startAutoDispatch();

		if (!running)
		{
			status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_AUTODISPATCH_FAILURE,
			           "Auto-dispatcher is already running (or failed to run)");
		}
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::StopAutoDispatch(bool waitForComplete)
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	try
	{
		bool stopped = m_adapter->stopAutoDispatch(waitForComplete);

		if (!stopped)
		{
			status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_AUTODISPATCH_FAILURE,
			           "Failed to shutdown Auto-dispatcher");
		}
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::GetNextMsg(Message*& msg, GMSEC_I32 timeout)
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	msg = NULL;

	try
	{
		gmsec::api::Message* msg4 = m_adapter->receive(timeout);

		if (msg4 == NULL)
		{
			status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_TIMEOUT_OCCURRED, "Timeout");
		}
		else
		{
			msg = new Message(new InternalMessage(msg4));
		}
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::DispatchMsg(Message* msg)
{
	Status status;

	if (msg == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Message is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	try
	{
		gmsec::api::Message* msg4 = API4_Adapter::referenceAPI4Message(msg);

		m_adapter->dispatch(*msg4);
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::GetLastDispatcherStatus()
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	return status;
}


Status InternalConnection::ExcludeSubject(const char* subject)
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	try
	{
		m_adapter->excludeSubject(subject);
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


Status InternalConnection::RemoveExcludedSubject(const char* subject)
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	try
	{
		m_adapter->removeExcludedSubject(subject);
	}
	catch (gmsec::api::Exception& e)
	{
		status = API4_Adapter::API4ExceptionToAPI3(e);
	}

	return status;
}


const char* InternalConnection::GetName()
{
	return m_adapter->getName();
}


void InternalConnection::SetName(const char* name)
{
	try
	{
		m_adapter->setName(name);
	}
	catch (gmsec::api::Exception& e)
	{
		LOG_WARNING << e.getErrorMessage();
	}
}


const char* InternalConnection::GetConnectionGUI()
{
	return m_adapter->getID();
}


GMSEC_U64 InternalConnection::GetPublishQueueMessageCount()
{
	return m_adapter->getPublishQueueMessageCount();
}


Status InternalConnection::DispatchError(const char* event, Message* msg, Status* error)
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	if (event && msg && error)
	{
		ErrorCallbackMap::iterator it = m_errorCallbacks.find(event);

		if (it != m_errorCallbacks.end())
		{
			ErrorCallback* ecb = it->second;

			ecb->OnError(m_parent, msg, error, event);
		}
	}

	return status;
}


/*
 ** these are for C API support ONLY
 */
Status InternalConnection::RegisterErrorCallback(const char* event, GMSEC_ERROR_CALLBACK* ecb)
{
	Status status;

	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	if (event != NULL && ecb != NULL)
	{
		ErrorCallback* cxx_ecb = (ErrorCallback*) m_collector->newCAPIErrorCallback(ecb);

		RegisterErrorCallback(event, cxx_ecb);
	}

	return status;
}


Status InternalConnection::Subscribe(const char* subject, GMSEC_CALLBACK* cb)
{
	Status status;

	if (subject == NULL || std::string(subject).empty())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_SUBJECT_NAME, "Subject is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	Callback* cxx_cb = (Callback*) m_collector->newCAPICallback(cb);

	status = Subscribe(subject, cxx_cb);

	return status;
}


Status InternalConnection::UnSubscribe(const char* subject, GMSEC_CALLBACK* cb)
{
	Status status;

	if (subject == NULL || std::string(subject).empty())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_SUBJECT_NAME, "Subject is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	status = UnSubscribe(subject, (Callback*) m_collector->getCAPICallback(cb));

	return status;
}


Status InternalConnection::Request(Message* request, GMSEC_I32 timeout, GMSEC_CALLBACK* cb)
{
	Status status;

	if (request == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Request Message is NULL");
		return status;
	}
	if (cb == NULL || *cb == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CALLBACK, "Callback is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	Callback* cxx_cb = (Callback*) m_collector->newCAPICallback(cb);

	status = Request(request, timeout, cxx_cb);

	return status;
}


Status InternalConnection::Request(Message* request, GMSEC_I32 timeout, GMSEC_REPLY_CALLBACK* rcb, GMSEC_ERROR_CALLBACK* ecb)
{
	Status status;

	if (request == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_MESSAGE, "Request Message is NULL");
		return status;
	}
	if (rcb == NULL || *rcb == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CALLBACK, "Reply Callback is NULL");
		return status;
	}
	if (ecb == NULL || *ecb == NULL)
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CALLBACK, "Error Callback is NULL");
		return status;
	}
	if (!IsConnected())
	{
		status.Set(GMSEC_STATUS_CONNECTION_ERROR, GMSEC_INVALID_CONNECTION, "Connection has not been established");
		return status;
	}

	ReplyCallback* cxx_rcb = (ReplyCallback*) m_collector->newCAPIReplyCallback(rcb, ecb);

	Request(request, timeout, cxx_rcb);

	return status;
}


InternalConnection::SubscriptionDetails::SubscriptionDetails()
	: infoDetails()
{
}


InternalConnection::SubscriptionDetails::~SubscriptionDetails()
{
	for (std::list<Details*>::iterator it = infoDetails.begin(); it != infoDetails.end(); ++it)
	{
		Details* details = *it;

		if (details->info)
		{
			delete details->info->getCallback();
		}

		delete details;
	}
	infoDetails.clear();
}


void InternalConnection::SubscriptionDetails::addDetails(Callback* cb, gmsec::api::SubscriptionInfo* info)
{
	Details* details = new Details();

	details->cb   = cb;
	details->info = info;

	infoDetails.push_back(details);
}


std::list<InternalConnection::Details*>
InternalConnection::SubscriptionDetails::getDetails(const char* subject, Callback* cb)
{
	std::list<Details*> found;

	for (std::list<Details*>::iterator it = infoDetails.begin(); it != infoDetails.end();)
	{
		Details* details = *it;

		if (cb == NULL)
		{
			// Get all details with matching subject
			found.push_back(details);

			it = infoDetails.erase(it);
		}
		else if (details->cb == cb &&
		         gmsec::api::util::StringUtil::stringEquals(subject, details->info->getSubject()))
		{
			found.push_back(details);

			it = infoDetails.erase(it);
		}
		else
		{
			++it;
		}
	}

	return found;
}


bool InternalConnection::SubscriptionDetails::hasMoreDetails(const char* subject)
{
	bool hasMore = false;

	for (std::list<Details*>::iterator it = infoDetails.begin(); it != infoDetails.end(); ++it)
	{
		Details* details = *it;

		if (gmsec::api::util::StringUtil::stringEquals(subject, details->info->getSubject()))
		{
			hasMore = true;
			break;
		}
	}

	return hasMore;
}


bool InternalConnection::SubscriptionDetails::hasDetails(const char* subject, Callback* cb)
{
	bool has = false;

	for (std::list<Details*>::iterator it = infoDetails.begin(); it != infoDetails.end(); ++it)
	{
		Details* details = *it;

		if (details->cb == cb && gmsec::api::util::StringUtil::stringEquals(subject, details->info->getSubject()))
		{
			has = true;
			break;
		}
	}

	return has;
}
