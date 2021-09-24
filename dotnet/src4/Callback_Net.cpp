/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <Callback_Net.h>
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
delegate void OnMessageDelegate(gmsec::api::Connection& conn, const gmsec::api::Message& msg);


/// <summary>
/// Native callback that is passed to the GMSEC API.
/// </summary>
class NativeCallback : public gmsec::api::Callback
{
private:
	typedef void (__stdcall *OnMessageDelegateNative)(gmsec::api::Connection& conn, const gmsec::api::Message& msg);


public:
	explicit NativeCallback(OnMessageDelegate^ delegate)
		: msgDelegate(delegate)
	{
		nativeMsg = (OnMessageDelegateNative) Marshal::GetFunctionPointerForDelegate(delegate).ToPointer();
	}


	virtual void CALL_TYPE onMessage(gmsec::api::Connection& conn, const gmsec::api::Message& msg)
	{
		nativeMsg(conn, msg);
	}

	// Set the number handles up by one to avoid garbage collection
	gcroot<OnMessageDelegate^> msgDelegate;
	OnMessageDelegateNative    nativeMsg;
};

}
}



Callback::Callback()
{
	m_impl = new NativeCallback(gcnew OnMessageDelegate(this, &Callback::OnMessageNative));
}


Callback::~Callback()
{
	this->!Callback();
}


void Callback::OnMessage(Connection^ conn, Message^ msg)
{
	// to be overloaded by subclass
}


Callback::!Callback()
{
	ResetUnmanagedImplementation();
}


void Callback::OnMessageNative(gmsec::api::Connection& conn, const gmsec::api::Message& msg)
{
	Connection^ connWrapped = gcnew Connection(&conn);
	Message^    msgWrapped  = gcnew Message(const_cast<gmsec::api::Message*>(&msg), false);

	OnMessage(connWrapped, msgWrapped);
}


gmsec::api::Callback* Callback::GetUnmanagedImplementation()
{
	return m_impl;
}


void Callback::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void Callback::SetUnmanagedImplementation(gmsec::api::Callback* callback)
{
	ResetUnmanagedImplementation();

	m_impl = callback;
}
