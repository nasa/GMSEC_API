
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

#include <vcclr.h>

#include "ErrorCallback_Net.h"
#include "Connection_Net.h"

using namespace Nasa::Gmsec::Net;
using namespace System::Runtime::InteropServices;

// delegate for the callback
delegate void OnErrorDelegate(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event);

/// <summary>
/// Native callback that is passed to gmsec api. 
/// Bubbles up to managed code when finished.
/// Code gets around the GCHandle in different App Domain error by using delegates
/// Unmanaged code has no App Domain and so when a call is first made, it is when it is assigned an App Domain (usually wrong causing exception)
/// Since delegate contains info about this App Domain, it gets around the issue
/// </summary>
class NativeWrappedErrorCallback : public gmsec::ErrorCallback
{
private:
	typedef void (__stdcall *OnErrorDelegateNative)(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event);
public:
	/// <summary>
	/// Constructor, set up delegates
	/// </summary>
	explicit NativeWrappedErrorCallback(OnErrorDelegate^ delegateError) : errorDelegate(delegateError)
	{
		// set up delegates
		nativeError = (OnErrorDelegateNative)Marshal::GetFunctionPointerForDelegate(delegateError).ToPointer();
	}

	/// <summary>
	/// Pass through to be translated to managed wrappers
	/// </summary>
	virtual void CALL_TYPE OnError(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event)
	{
		nativeError(conn, msg, status, event);
	}

	gcroot<OnErrorDelegate^> errorDelegate;
	OnErrorDelegateNative nativeError;
};

Nasa::Gmsec::Net::ErrorCallback::!ErrorCallback()
{
	// Free the unmanaged implementation pointer.
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::ErrorCallback::ErrorCallback()
{
	// Initialize the unmanaged implementation pointer.
	mImpl = new NativeWrappedErrorCallback(gcnew OnErrorDelegate(this, &Nasa::Gmsec::Net::ErrorCallback::OnErrorNative));
}

Nasa::Gmsec::Net::ErrorCallback::~ErrorCallback()
{
	// Call the finalizer.
	this->!ErrorCallback();
}

gmsec::ErrorCallback * Nasa::Gmsec::Net::ErrorCallback::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}

void Nasa::Gmsec::Net::ErrorCallback::ResetUnmanImpl()
{
	// Reset the unmanaged implementation pointer.
	if (mImpl != nullptr)
	{
		delete mImpl;
		mImpl = nullptr;
	}
}

void Nasa::Gmsec::Net::ErrorCallback::SetUnmanImpl(gmsec::ErrorCallback* errorCallback)
{
	// Clean up
	ResetUnmanImpl();

	// New native impl
	mImpl = errorCallback;
}

void Nasa::Gmsec::Net::ErrorCallback::OnErrorNative(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event)
{
	// Convert the message.
	Nasa::Gmsec::Net::Message^ msgWrapped = gcnew Nasa::Gmsec::Net::Message(msg);
	Nasa::Gmsec::Net::Connection^ connWrapped = gcnew Nasa::Gmsec::Net::Connection(conn);
	Nasa::Gmsec::Net::Status^ statusWrapped = gcnew Nasa::Gmsec::Net::Status(status);
	String^ eventStr = gcnew String(event);

	// Call managed callback
	OnError(connWrapped, msgWrapped, statusWrapped, eventStr);
}

/// <seealso cref="Connection::RegisterErrorCallback" />
void Nasa::Gmsec::Net::ErrorCallback::OnError(Nasa::Gmsec::Net::Connection ^ conn, Nasa::Gmsec::Net::Message ^ msg, Nasa::Gmsec::Net::Status ^ status, String ^ event_str)
{
	// to be overridden
}
