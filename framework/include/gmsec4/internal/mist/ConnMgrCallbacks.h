/*
 * Copyright 2007-2016 United States Government as represented by the
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
	CMCallback(ConnectionManager* manager, ConnectionManagerCallback* cb);

	ConnectionManager* getManager() const;
	ConnectionManagerCallback* getCallback() const;

	virtual void CALL_TYPE onMessage(gmsec::api::Connection& conn, const gmsec::api::Message& msg);

private:
	ConnectionManager* m_mgr;
	ConnectionManagerCallback* m_cb;
};


class GMSEC_API CMEventCallback : public gmsec::api::EventCallback
{
public:
	CMEventCallback(ConnectionManager* manager, ConnectionManagerEventCallback* cb);

	ConnectionManager* getManager() const;
	ConnectionManagerEventCallback* getCallback() const;

	virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, Connection::ConnectionEvent event);

private:
	ConnectionManager* m_mgr;
	ConnectionManagerEventCallback* m_cb;
};


class GMSEC_API CMReplyCallback : public gmsec::api::ReplyCallback
{
public:
	CMReplyCallback(ConnectionManager* manager, ConnectionManagerReplyCallback* cb);

	ConnectionManager* getManager() const;
	ConnectionManagerReplyCallback* getCallback() const;

	virtual void CALL_TYPE onReply(gmsec::api::Connection& conn, const gmsec::api::Message& req, const gmsec::api::Message& rep);
	virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, Connection::ConnectionEvent event);

private:
	ConnectionManager* m_mgr;
	ConnectionManagerReplyCallback* m_cb;
};

}  //namespace internal
}  //namespace mist
}  //namespace api
}  //namespace gmsec

#endif
