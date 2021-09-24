/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/ConnectionManagerReplyCallback_Net.h>
#include <mist/ConnectionManager_Net.h>
#include <Message_Net.h>

#include <vcclr.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System::Runtime::InteropServices;


namespace GMSEC
{
namespace API
{
namespace MIST
{

// delegate for the callback
delegate void OnReplyDelegate(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& request, const gmsec::api::Message& reply);
delegate void OnEventDelegate(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


/// <summary>
/// Native callback that is passed to the GMSEC API.
/// </summary>
class NativeReplyCallback : public gmsec::api::mist::ConnectionManagerReplyCallback
{
private:
    typedef void (__stdcall *OnReplyDelegateNative)(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& request, const gmsec::api::Message& reply);
    typedef void (__stdcall *OnEventDelegateNative)(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


public:
    explicit NativeReplyCallback(OnReplyDelegate^ delegateReply, OnEventDelegate^ delegateEvent)
        : replyDelegate(delegateReply),
		  eventDelegate(delegateEvent)
    {
        nativeReply = (OnReplyDelegateNative) Marshal::GetFunctionPointerForDelegate(delegateReply).ToPointer();
        nativeEvent = (OnEventDelegateNative) Marshal::GetFunctionPointerForDelegate(delegateEvent).ToPointer();
    }


    virtual void CALL_TYPE onReply(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& request, const gmsec::api::Message& reply)
    {
        nativeReply(connMgr, request, reply);
    }


    virtual void CALL_TYPE onEvent(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event)
    {
        nativeEvent(connMgr, status, event);
    }


    // Set the number handles up by one to avoid garbage collection
    gcroot<OnReplyDelegate^> replyDelegate;
    gcroot<OnEventDelegate^> eventDelegate;
    OnReplyDelegateNative    nativeReply;
    OnEventDelegateNative    nativeEvent;
};

}
}
}


ConnectionManagerReplyCallback::ConnectionManagerReplyCallback()
{
	m_impl = new NativeReplyCallback(
				gcnew OnReplyDelegate(this, &ConnectionManagerReplyCallback::OnReplyNative),
				gcnew OnEventDelegate(this, &ConnectionManagerReplyCallback::OnEventNative));
}


ConnectionManagerReplyCallback::~ConnectionManagerReplyCallback()
{
	this->!ConnectionManagerReplyCallback();
}


void ConnectionManagerReplyCallback::OnReply(ConnectionManager^ connMgr, Message^ request, Message^ reply)
{
	// to be overloaded by subclass
}


ConnectionManagerReplyCallback::!ConnectionManagerReplyCallback()
{
	ResetUnmanagedImplementation();
}


void ConnectionManagerReplyCallback::OnReplyNative(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& request, const gmsec::api::Message& reply)
{
	ConnectionManager^ connMgrWrapped = gcnew ConnectionManager(&connMgr, false);
	Message^           reqWrapped     = gcnew Message(const_cast<gmsec::api::Message*>(&request), false);
	Message^           repWrapped     = gcnew Message(const_cast<gmsec::api::Message*>(&reply), false);

	OnReply(connMgrWrapped, reqWrapped, repWrapped);
}


gmsec::api::mist::ConnectionManagerReplyCallback* ConnectionManagerReplyCallback::GetUnmanagedImplementation()
{
	return m_impl;
}


void ConnectionManagerReplyCallback::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ConnectionManagerReplyCallback::SetUnmanagedImplementation(gmsec::api::mist::ConnectionManagerReplyCallback* callback)
{
	ResetUnmanagedImplementation();

	m_impl = callback;
}
