
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

#include "ReplyCallback_Net.h"
#include "Connection_Net.h"

using namespace Nasa::Gmsec::Net;
using namespace System::Runtime::InteropServices;

// delegates for the callback
delegate void OnErrorDelegate(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event);
delegate void OnReplyDelegate(gmsec::Connection *conn, gmsec::Message *request, gmsec::Message *reply);

/// <summary>
/// Native callback that is passed to gmsec api. 
/// Bubbles up to managed code when finished.
/// Code gets around the GCHandle in different App Domain error by using delegates
/// Unmanaged code has no App Domain and so when a call is first made, it is when it is assigned an App Domain (usually wrong causing exception)
/// Since delegate contains info about this App Domain, it gets around the issue
/// </summary>
class NativeWrappedReplyCallback : public gmsec::ReplyCallback
{
public:

private:
	typedef void (__stdcall *OnErrorDelegateNative)(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event);
	typedef void (__stdcall *OnReplyDelegateNative)(gmsec::Connection *conn, gmsec::Message *request, gmsec::Message *reply);

public:
	/// <summary>
	/// Constructor, set up delegates
	/// </summary>
	explicit NativeWrappedReplyCallback(OnErrorDelegate^ delegateError, OnReplyDelegate^ delegateReply) : errorDelegate(delegateError), replyDelegate(delegateReply)
	{
		 // set up delegates
		 nativeError = (OnErrorDelegateNative)Marshal::GetFunctionPointerForDelegate(delegateError).ToPointer();
		 nativeReply = (OnReplyDelegateNative)Marshal::GetFunctionPointerForDelegate(delegateReply).ToPointer();
	}

	/// <summary>
	/// Pass through to be translated to managed wrappers
	/// </summary>
	virtual void CALL_TYPE OnError(gmsec::Connection *conn, gmsec::Message *msg, gmsec::Status *status, const char* event)
	{
		nativeError(conn, msg, status, event);
	}

	/// <summary>
	/// Pass through to be translated to managed wrappers
	/// </summary>
	virtual void CALL_TYPE OnReply(gmsec::Connection *conn, gmsec::Message *request, gmsec::Message *reply)
	{
		nativeReply(conn, request, reply);
	}

	gcroot<OnErrorDelegate^> errorDelegate;
	gcroot<OnReplyDelegate^> replyDelegate;
	OnReplyDelegateNative nativeReply;
	OnErrorDelegateNative nativeError;
};

Nasa::Gmsec::Net::ReplyCallback::!ReplyCallback()
{
	// Free the unmanaged implementation pointer.
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::ReplyCallback::ReplyCallback()
{
	// Initialize the unmanaged implementation pointer.
	mImpl = new NativeWrappedReplyCallback(gcnew OnErrorDelegate(this, &Nasa::Gmsec::Net::ReplyCallback::OnErrorNative), gcnew OnReplyDelegate(this, &Nasa::Gmsec::Net::ReplyCallback::OnReplyNative));
}

Nasa::Gmsec::Net::ReplyCallback::~ReplyCallback()
{
	// Call the finalizer.
	this->!ReplyCallback();
}

gmsec::ReplyCallback * Nasa::Gmsec::Net::ReplyCallback::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}

void Nasa::Gmsec::Net::ReplyCallback::ResetUnmanImpl()
{
	if (mImpl != nullptr)
	{
		// Reset the unmanaged implementation pointer.
		delete mImpl;
		mImpl = nullptr;
	}
}


void Nasa::Gmsec::Net::ReplyCallback::OnReplyNative(gmsec::Connection *conn, gmsec::Message *request, gmsec::Message *reply)
{
	// Convert the message.
	Nasa::Gmsec::Net::Message^ msgWrapped = gcnew Nasa::Gmsec::Net::Message(request);
	Nasa::Gmsec::Net::Message^ replyWrapped = gcnew Nasa::Gmsec::Net::Message(reply);
	Nasa::Gmsec::Net::Connection^ connWrapped = gcnew Nasa::Gmsec::Net::Connection(conn);

	// Call managed callback
	OnReply(connWrapped, msgWrapped, replyWrapped);
}

/// <seealso cref="Connection::Request" />
void Nasa::Gmsec::Net::ReplyCallback::OnReply(Nasa::Gmsec::Net::Connection ^ conn, Nasa::Gmsec::Net::Message ^ request_msg, Nasa::Gmsec::Net::Message ^ reply_msg)
{
	// to be overridden
}
