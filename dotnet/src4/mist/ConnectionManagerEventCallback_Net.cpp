/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/ConnectionManagerEventCallback_Net.h>
#include <mist/ConnectionManager_Net.h>
#include <Status_Net.h>

#include <vcclr.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


namespace GMSEC
{
namespace API
{
namespace MIST
{

// delegate for the callback
delegate void OnEventDelegate(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


/// <summary>
/// Native callback that is passed to the GMSEC API.
/// </summary>
class NativeEventCallback : public gmsec::api::mist::ConnectionManagerEventCallback
{
private:
	typedef void (__stdcall *OnEventDelegateNative)(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


public:
    explicit NativeEventCallback(OnEventDelegate^ delegate)
        : eventDelegate(delegate)
    {
        nativeEvent = (OnEventDelegateNative) Marshal::GetFunctionPointerForDelegate(delegate).ToPointer();
    }


    virtual void CALL_TYPE onEvent(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event)
    {
        nativeEvent(connMgr, status, event);
    }

    // Set the number handles up by one to avoid garbage collection
    gcroot<OnEventDelegate^> eventDelegate;
    OnEventDelegateNative    nativeEvent;
};

}
}
}


ConnectionManagerEventCallback::ConnectionManagerEventCallback()
{
	m_impl = new NativeEventCallback(gcnew OnEventDelegate(this, &ConnectionManagerEventCallback::OnEventNative));
}


ConnectionManagerEventCallback::~ConnectionManagerEventCallback()
{
	this->!ConnectionManagerEventCallback();
}


void ConnectionManagerEventCallback::OnEvent(ConnectionManager^ connMgr, Status^ status, Connection::ConnectionEvent event)
{
	// to be overloaded by subclass
}


ConnectionManagerEventCallback::!ConnectionManagerEventCallback()
{
	ResetUnmanagedImplementation();
}


void ConnectionManagerEventCallback::OnEventNative(gmsec::api::mist::ConnectionManager& connMgr, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event)
{
	ConnectionManager^ connMgrWrapped = gcnew ConnectionManager(&connMgr, false);
	Status^            statusWrapped  = gcnew Status(const_cast<gmsec::api::Status*>(&status));

	OnEvent(connMgrWrapped, statusWrapped, static_cast<Connection::ConnectionEvent>(event));
}


gmsec::api::mist::ConnectionManagerEventCallback* ConnectionManagerEventCallback::GetUnmanagedImplementation()
{
	return m_impl;
}


void ConnectionManagerEventCallback::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ConnectionManagerEventCallback::SetUnmanagedImplementation(gmsec::api::mist::ConnectionManagerEventCallback* callback)
{
	ResetUnmanagedImplementation();

	m_impl = callback;
}
