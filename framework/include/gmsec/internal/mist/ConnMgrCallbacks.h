/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




#ifndef CONN_MGR_CALLBACKS_H
#define CONN_MGR_CALLBACKS_H

#include <gmsec/Callback.h>
#include <gmsec/ErrorCallback.h>
#include <gmsec/ReplyCallback.h>

namespace gmsec
{
class Connection;
class Message;
class Status;

namespace mist
{
namespace internal
{
	class BaseConnectionManager;


	class GMSEC_API CMCallback : public gmsec::Callback
	{
	public:
		CMCallback(BaseConnectionManager* manager);

		BaseConnectionManager* getManager() const;

		virtual void CALL_TYPE OnMessage(gmsec::Connection* conn, gmsec::Message* msg);

	private:
		BaseConnectionManager* m_manager;
	};


	class GMSEC_API CMErrorCallback : public gmsec::ErrorCallback
	{
	public:
		CMErrorCallback(BaseConnectionManager* manager);

		BaseConnectionManager* getManager() const;

		virtual void CALL_TYPE OnError(gmsec::Connection* conn, gmsec::Message* msg, gmsec::Status* status, const char* event);

	private:
		BaseConnectionManager* m_manager;
	};


	class GMSEC_API CMReplyCallback : public gmsec::ReplyCallback
	{
	public:
		CMReplyCallback(BaseConnectionManager* manager);

		BaseConnectionManager* getManager() const;

		virtual void CALL_TYPE OnReply(gmsec::Connection* conn, gmsec::Message* req, gmsec::Message* rep);
		virtual void CALL_TYPE OnError(gmsec::Connection* conn, gmsec::Message* msg, gmsec::Status* status, const char* event);

	private:
		BaseConnectionManager* m_manager;
	};
}
}
}

#endif
