/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <ReplyCallback_Net.h>
#include <Connection_Net.h>
#include <Message_Net.h>

#include <vcclr.h>


using namespace GMSEC::API;
using namespace System::Runtime::InteropServices;


namespace GMSEC
{
namespace API
{

// delegate for the callback
delegate void OnReplyDelegate(gmsec::api::Connection& conn, const gmsec::api::Message& request, const gmsec::api::Message& reply);
delegate void OnEventDelegate(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


/// <summary>
/// Native callback that is passed to the GMSEC API.
/// </summary>
class NativeReplyCallback : public gmsec::api::ReplyCallback
{
private:
    typedef void (__stdcall *OnReplyDelegateNative)(gmsec::api::Connection& conn, const gmsec::api::Message& request, const gmsec::api::Message& reply);
    typedef void (__stdcall *OnEventDelegateNative)(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


public:
    explicit NativeReplyCallback(OnReplyDelegate^ delegateReply, OnEventDelegate^ delegateEvent)
        : replyDelegate(delegateReply),
		  eventDelegate(delegateEvent)
    {
        nativeReply = (OnReplyDelegateNative) Marshal::GetFunctionPointerForDelegate(delegateReply).ToPointer();
        nativeEvent = (OnEventDelegateNative) Marshal::GetFunctionPointerForDelegate(delegateEvent).ToPointer();
    }


    virtual void CALL_TYPE onReply(gmsec::api::Connection& conn, const gmsec::api::Message& request, const gmsec::api::Message& reply)
    {
        nativeReply(conn, request, reply);
    }


    virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event)
    {
        nativeEvent(conn, status, event);
    }


    // Set the number handles up by one to avoid garbage collection
    gcroot<OnReplyDelegate^> replyDelegate;
    gcroot<OnEventDelegate^> eventDelegate;
    OnReplyDelegateNative    nativeReply;
    OnEventDelegateNative    nativeEvent;
};

}
}


ReplyCallback::ReplyCallback()
{
	m_impl = new NativeReplyCallback(
				gcnew OnReplyDelegate(this, &ReplyCallback::OnReplyNative),
				gcnew OnEventDelegate(this, &ReplyCallback::OnEventNative));
}


ReplyCallback::~ReplyCallback()
{
	this->!ReplyCallback();
}


void ReplyCallback::OnReply(Connection^ conn, Message^ request, Message^ reply)
{
	// to be overloaded by subclass
}


ReplyCallback::!ReplyCallback()
{
	ResetUnmanagedImplementation();
}


void ReplyCallback::OnReplyNative(gmsec::api::Connection& conn, const gmsec::api::Message& request, const gmsec::api::Message& reply)
{
	Connection^ connWrapped = gcnew Connection(&conn);
	Message^    reqWrapped  = gcnew Message(const_cast<gmsec::api::Message*>(&request), false);
	Message^    repWrapped  = gcnew Message(const_cast<gmsec::api::Message*>(&reply), false);

	OnReply(connWrapped, reqWrapped, repWrapped);
}


gmsec::api::ReplyCallback* ReplyCallback::GetUnmanagedImplementation()
{
	return m_impl;
}


void ReplyCallback::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ReplyCallback::SetUnmanagedImplementation(gmsec::api::ReplyCallback* callback)
{
	ResetUnmanagedImplementation();

	m_impl = callback;
}
