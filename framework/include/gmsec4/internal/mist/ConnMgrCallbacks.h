/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef CONN_MGR_CALLBACKS_H
#define CONN_MGR_CALLBACKS_H

#include <gmsec4/Callback.h>
#include <gmsec4/Connection.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/ReplyCallback.h>


namespace gmsec
{
namespace api
{
	class Message;
	class Status;

namespace mist
{
	class ConnectionManager;
	class ConnectionManagerCallback;
	class ConnectionManagerEventCallback;
	class ConnectionManagerReplyCallback;

namespace internal
{

class GMSEC_API CMCallback : public gmsec::api::Callback
{
public:
	CMCallback(ConnectionManager* manager, ConnectionManagerCallback* cb, bool doValidate);

	inline ConnectionManager* getManager() const { return m_mgr; }
	inline ConnectionManagerCallback* getCallback() const { return m_cb; }
	inline bool doValidate() const { return m_doValidate; }

	virtual void CALL_TYPE onMessage(gmsec::api::Connection& conn, const gmsec::api::Message& msg);

private:
	ConnectionManager* m_mgr;
	ConnectionManagerCallback* m_cb;
	bool m_doValidate;
};


class GMSEC_API CMEventCallback : public gmsec::api::EventCallback
{
public:
	CMEventCallback(ConnectionManager* manager, ConnectionManagerEventCallback* cb);

	inline ConnectionManager* getManager() const { return m_mgr; }
	inline ConnectionManagerEventCallback* getCallback() const { return m_cb; }

	virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, Connection::ConnectionEvent event);

private:
	ConnectionManager* m_mgr;
	ConnectionManagerEventCallback* m_cb;
};


class GMSEC_API CMReplyCallback : public gmsec::api::ReplyCallback
{
public:
	CMReplyCallback(ConnectionManager* manager, ConnectionManagerReplyCallback* cb, bool doValidate);

	inline ConnectionManager* getManager() const { return m_mgr; }
	inline ConnectionManagerReplyCallback* getCallback() const { return m_cb; }
	inline bool doValidate() const { return m_doValidate; }

	virtual void CALL_TYPE onReply(gmsec::api::Connection& conn, const gmsec::api::Message& req, const gmsec::api::Message& rep);
	virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, Connection::ConnectionEvent event);

private:
	ConnectionManager* m_mgr;
	ConnectionManagerReplyCallback* m_cb;
	bool m_doValidate;
};

}  //namespace internal
}  //namespace mist
}  //namespace api
}  //namespace gmsec

#endif
