
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    
#include <iostream>
#include <string>

#include <vcclr.h>

#include "Connection_Net.h"
#include "Callback_Net.h"

using namespace Nasa::Gmsec::Net;
using namespace std;
using namespace System::Runtime::InteropServices;

// delegate for the callback
delegate void OnMessageDelegate(gmsec::Connection *conn, gmsec::Message *msg);

/// <summary>
/// Native callback that is passed to gmsec api. 
/// Bubbles up to managed code when finished.
/// Code gets around the GCHandle in different App Domain error by using delegates
/// Unmanaged code has no App Domain and so when a call is first made, it is when it is assigned an App Domain (usually wrong causing exception)
/// Since delegate contains info about this App Domain, it gets around the issue
/// </summary>
class NativeWrappedCallback : public gmsec::Callback
{
private:
	typedef void (__stdcall *OnMessageDelegateNative)(gmsec::Connection *conn, gmsec::Message *msg);

public:
	/// <summary>
	/// Constructor, set up delegates
	/// </summary>
	explicit NativeWrappedCallback(OnMessageDelegate^ delegateMessage) : messageDelegate(delegateMessage)
	{
		// set up delegates
		nativeMessage = (OnMessageDelegateNative)Marshal::GetFunctionPointerForDelegate(delegateMessage).ToPointer();
	}

	/// <summary>
	/// Pass through to be translated to managed wrappers
	/// </summary>
	virtual void CALL_TYPE OnMessage(gmsec::Connection *conn, gmsec::Message *msg)
	{
		nativeMessage(conn, msg);
	}

	// Set the number handles up by one to avoid Garbage Collection
	gcroot<OnMessageDelegate^> messageDelegate;
	OnMessageDelegateNative nativeMessage;
};


Nasa::Gmsec::Net::Callback::!Callback()
{
	// Free the unmanaged implementation pointer.
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::Callback::Callback()
{
	// Initialize the unmanaged implementation pointer.
	mImpl = new NativeWrappedCallback(gcnew OnMessageDelegate(this, &Nasa::Gmsec::Net::Callback::OnMessageNative));
}

Nasa::Gmsec::Net::Callback::~Callback()
{
	// Call the finalizer.
	this->!Callback();
}

gmsec::Callback * Nasa::Gmsec::Net::Callback::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}

void Nasa::Gmsec::Net::Callback::ResetUnmanImpl()
{
	// Delete and null impl
	if (mImpl != nullptr)
	{
		delete mImpl;
		mImpl = nullptr;
	}
}


void Nasa::Gmsec::Net::Callback::SetUnmanImpl(gmsec::Callback* callback)
{
	// Clean up
	ResetUnmanImpl();

	// set new native object
	mImpl = callback;
}

void Nasa::Gmsec::Net::Callback::OnMessageNative(gmsec::Connection *conn, gmsec::Message *msg)
{
	// Convert the message.
	Nasa::Gmsec::Net::Message^ msgWrapped = gcnew Nasa::Gmsec::Net::Message(msg);
	Nasa::Gmsec::Net::Connection^ connWrapped = gcnew Nasa::Gmsec::Net::Connection(conn);

	// Call managed callback
	OnMessage(connWrapped, msgWrapped);
}

/// <seealso cref="Connection::Subscribe(const String ^subject, Callback^ cb)" />
/// <seealso cref="Connection::Request" />
/// <seealso cref="Connection::DispatchMsg(Message^  msg)" />
/// <seealso cref="Connection::StartAutoDispatch()" />
/// <seealso cref="Connection::CloneMessage(Message^ in, [Out] Message^% out)" />
void Nasa::Gmsec::Net::Callback::OnMessage(Nasa::Gmsec::Net::Connection ^ conn, Nasa::Gmsec::Net::Message ^ msg)
{
	// to be overriden
}
