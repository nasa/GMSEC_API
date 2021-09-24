/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <EventCallback_Net.h>
#include <Connection_Net.h>
#include <Status_Net.h>

#include <vcclr.h>


using namespace GMSEC::API;
using namespace System;
using namespace System::Runtime::InteropServices;


namespace GMSEC
{
namespace API
{

// delegate for the callback
delegate void OnEventDelegate(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


/// <summary>
/// Native callback that is passed to the GMSEC API.
/// </summary>
class NativeEventCallback : public gmsec::api::EventCallback
{
private:
	typedef void (__stdcall *OnEventDelegateNative)(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event);


public:
    explicit NativeEventCallback(OnEventDelegate^ delegate)
        : eventDelegate(delegate)
    {
        nativeEvent = (OnEventDelegateNative) Marshal::GetFunctionPointerForDelegate(delegate).ToPointer();
    }


    virtual void CALL_TYPE onEvent(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event)
    {
        nativeEvent(conn, status, event);
    }

    // Set the number handles up by one to avoid garbage collection
    gcroot<OnEventDelegate^> eventDelegate;
    OnEventDelegateNative    nativeEvent;
};

}
}


EventCallback::EventCallback()
{
	m_impl = new NativeEventCallback(gcnew OnEventDelegate(this, &EventCallback::OnEventNative));
}


EventCallback::~EventCallback()
{
	this->!EventCallback();
}


void EventCallback::OnEvent(Connection^ conn, Status^ status, Connection::ConnectionEvent event)
{
	// to be overloaded by subclass
}


EventCallback::!EventCallback()
{
	ResetUnmanagedImplementation();
}


void EventCallback::OnEventNative(gmsec::api::Connection& conn, const gmsec::api::Status& status, gmsec::api::Connection::ConnectionEvent event)
{
	Connection^ connWrapped   = gcnew Connection(&conn);
	Status^     statusWrapped = gcnew Status(const_cast<gmsec::api::Status*>(&status));

	OnEvent(connWrapped, statusWrapped, static_cast<Connection::ConnectionEvent>(event));
}


gmsec::api::EventCallback* EventCallback::GetUnmanagedImplementation()
{
	return m_impl;
}


void EventCallback::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void EventCallback::SetUnmanagedImplementation(gmsec::api::EventCallback* callback)
{
	ResetUnmanagedImplementation();

	m_impl = callback;
}
