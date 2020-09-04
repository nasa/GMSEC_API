#ifndef GMSEC_API_INTERNAL_ADAPTER_H
#define GMSEC_API_INTERNAL_ADAPTER_H

#include <gmsec_defs.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Errors.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/wdllexp.h>

#include <gmsec/Callback.h>
#include <gmsec/ErrorCallback.h>
#include <gmsec/ReplyCallback.h>

#include <gmsec/util/Mutex.h>

#include <string>
#include <map>


namespace gmsec
{
class Config;
class Connection;
class Field;
class Message;
class Status;

namespace api
{
// Forward declaration(s)
class Callback;
class Connection;
class Config;
class EventCallback;
class Field;
class Message;
class ReplyCallback;


namespace internal
{

class GMSEC_API GMSECadapter
{
public:
	static gmsec::Config* CALL_TYPE createLegacyConfig(const Config& newConfig);

	static gmsec::Field* CALL_TYPE createLegacyField(const Field& newField);

	static gmsec::Message* CALL_TYPE createLegacyMessage(gmsec::Connection* conn, const Message& newMsg);

	static void CALL_TYPE destroyLegacyMessage(gmsec::Connection* conn, gmsec::Message* legacyMsg);

	static Status CALL_TYPE createNewStatus(const gmsec::Status& legacyStatus);

	static Field* CALL_TYPE createNewField(gmsec::Field& legacyField);

	static Message* CALL_TYPE createNewMessage(gmsec::Message* legacyMsg);

	static const char* toLegacyEvent(Connection::ConnectionEvent event);

	static Connection::ConnectionEvent toNewEvent(const char* event);

private:
	static gmsec::api::StatusClass CALL_TYPE translateLegacyStatusClass(GMSEC_U32 legacyClass);

	static gmsec::api::StatusCode CALL_TYPE translateLegacyStatusCode(GMSEC_U32 legacyCode);
};


//
// Begin Legacy Callback Definitions
//

class GMSEC_API LegacyCallback : public gmsec::Callback
{
public:
	LegacyCallback(gmsec::api::Connection* conn, gmsec::api::Callback* cb);

	virtual void CALL_TYPE OnMessage(gmsec::Connection* conn, gmsec::Message* msg);

private:
	gmsec::api::Connection* m_conn;
	gmsec::api::Callback*   m_cb;
};


class GMSEC_API LegacyErrorCallback : public gmsec::ErrorCallback
{
public:
	LegacyErrorCallback(gmsec::api::Connection* conn, gmsec::api::EventCallback* cb);

	virtual void CALL_TYPE OnError(gmsec::Connection* conn, gmsec::Message* msg, gmsec::Status* status, const char* event);

private:
	gmsec::api::Connection*    m_conn;
	gmsec::api::EventCallback* m_cb;
};


class GMSEC_API LegacyReplyCallback : public gmsec::ReplyCallback
{
public:
	LegacyReplyCallback(gmsec::api::Connection* conn, gmsec::api::ReplyCallback* cb);

	virtual void CALL_TYPE OnReply(gmsec::Connection* conn, gmsec::Message* request, gmsec::Message* reply);
	virtual void CALL_TYPE OnError(gmsec::Connection* conn, gmsec::Message* request, gmsec::Status* status, const char* event);

private:
	gmsec::api::Connection*    m_conn;
	gmsec::api::ReplyCallback* m_cb;
};


//
// Begin Callback Table Mapping
//

class GMSEC_API CallbackTable
{
public:
	static CallbackTable& CALL_TYPE self();

	void CALL_TYPE addCallback(Connection* owner, const char* subject, Callback* cb, LegacyCallback* lcb);
	void CALL_TYPE addEventCallback(Connection* owner, EventCallback* cb, LegacyErrorCallback* lcb);
	void CALL_TYPE addReplyCallback(Connection* owner, ReplyCallback* cb, LegacyReplyCallback* lcb);

	bool callbackRegistered(Connection* owner, const char* subject, Callback* cb);

	LegacyCallback*      CALL_TYPE removeCallback(Connection* owner, const char* subject, Callback* cb);
	LegacyErrorCallback* CALL_TYPE removeEventCallback(Connection* owner, EventCallback* cb);
	LegacyReplyCallback* CALL_TYPE removeReplyCallback(Connection* owner, ReplyCallback* cb);

	void CALL_TYPE removeAllCallbacks(Connection* owner);

private:
	CallbackTable();

	// Defined, but not implemented.
	CallbackTable(const CallbackTable&);
	CallbackTable& operator=(const CallbackTable&);

	struct OwnerInfo
	{
		OwnerInfo(Connection* o) : owner(o) {}
		Connection* owner;
	};
	struct CallbackInfo : public OwnerInfo
	{
		CallbackInfo(Connection* o, const char* s, Callback* c) : OwnerInfo(o), subject(s), cb(c) {}

		std::string subject;
		Callback*   cb;
	};
	struct EventCallbackInfo : public OwnerInfo
	{
		EventCallbackInfo(Connection* o, EventCallback* c) : OwnerInfo(o), cb(c) {}

		EventCallback* cb;
	};
	struct ReplyCallbackInfo : public OwnerInfo
	{
		ReplyCallbackInfo(Connection* o, ReplyCallback* c) : OwnerInfo(o), cb(c) {}

		ReplyCallback* cb;
	};

	typedef std::map<LegacyCallback*, CallbackInfo*>           Callbacks;
	typedef std::map<LegacyErrorCallback*, EventCallbackInfo*> EventCallbacks;
	typedef std::map<LegacyReplyCallback*, ReplyCallbackInfo*> ReplyCallbacks;

	Callbacks          m_callbacks;
	EventCallbacks     m_eventCallbacks;
	ReplyCallbacks     m_replyCallbacks;
	gmsec::util::Mutex m_mutex; // STL maps are not thread-safe; will use mutex to protect access.
};

} // namespace internal
} // namespace api
} // namespace gmsec

#endif
