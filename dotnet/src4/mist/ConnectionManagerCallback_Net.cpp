/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/ConnectionManagerCallback_Net.h>
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
delegate void OnMessageDelegate(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& msg);


/// <summary>
/// Native callback that is passed to the GMSEC API.
/// </summary>
class NativeCallback : public gmsec::api::mist::ConnectionManagerCallback
{
private:
	typedef void (__stdcall *OnMessageDelegateNative)(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& msg);


public:
	explicit NativeCallback(OnMessageDelegate^ delegate)
		: msgDelegate(delegate)
	{
		nativeMsg = (OnMessageDelegateNative) Marshal::GetFunctionPointerForDelegate(delegate).ToPointer();
	}


	virtual void CALL_TYPE onMessage(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& msg)
	{
		nativeMsg(connMgr, msg);
	}

	// Set the number handles up by one to avoid garbage collection
	gcroot<OnMessageDelegate^> msgDelegate;
	OnMessageDelegateNative    nativeMsg;
};

}
}
}


ConnectionManagerCallback::ConnectionManagerCallback()
{
	m_impl = new NativeCallback(gcnew OnMessageDelegate(this, &ConnectionManagerCallback::OnMessageNative));
}


ConnectionManagerCallback::~ConnectionManagerCallback()
{
	this->!ConnectionManagerCallback();
}


void ConnectionManagerCallback::OnMessage(ConnectionManager^ connMgr, Message^ msg)
{
	// to be overloaded by subclass
}


ConnectionManagerCallback::!ConnectionManagerCallback()
{
	ResetUnmanagedImplementation();
}


void ConnectionManagerCallback::OnMessageNative(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Message& msg)
{
	ConnectionManager^ connMgrWrapped = gcnew ConnectionManager(&connMgr, false);
	Message^           msgWrapped     = gcnew Message(const_cast<gmsec::api::Message*>(&msg), false);

	OnMessage(connMgrWrapped, msgWrapped);
}


gmsec::api::mist::ConnectionManagerCallback* ConnectionManagerCallback::GetUnmanagedImplementation()
{
	return m_impl;
}


void ConnectionManagerCallback::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ConnectionManagerCallback::SetUnmanagedImplementation(gmsec::api::mist::ConnectionManagerCallback* callback)
{
	ResetUnmanagedImplementation();

	m_impl = callback;
}
