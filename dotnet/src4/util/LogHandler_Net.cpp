/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <util/LogHandler_Net.h>
#include <util/LogEntry_Net.h>

#include <vcclr.h>


using namespace GMSEC::API::UTIL;
using namespace System::Runtime::InteropServices;



namespace GMSEC
{
namespace API
{
namespace UTIL
{

// delegate for the LogHandler callback
delegate void OnMessageDelegate(const gmsec::api::util::LogEntry& entry);


/// <summary>
/// Native LogHandler that is passed to the GMSEC API.
/// </summary>
class NativeLogHandler : public gmsec::api::util::LogHandler
{
private:
	typedef void (__stdcall *OnMessageDelegateNative)(const gmsec::api::util::LogEntry& entry);


public:
	explicit NativeLogHandler(OnMessageDelegate^ delegate)
		: msgDelegate(delegate)
	{
		nativeMsg = (OnMessageDelegateNative) Marshal::GetFunctionPointerForDelegate(delegate).ToPointer();
	}


	virtual void CALL_TYPE onMessage(const gmsec::api::util::LogEntry& entry)
	{
		nativeMsg(entry);
	}

	// Set the number handles up by one to avoid garbage collection
	gcroot<OnMessageDelegate^> msgDelegate;
	OnMessageDelegateNative    nativeMsg;
};

}
}
}



LogHandler::LogHandler()
{
	m_impl = new NativeLogHandler(gcnew OnMessageDelegate(this, &LogHandler::OnMessageNative));
}


LogHandler::~LogHandler()
{
	this->!LogHandler();
}


void LogHandler::OnMessage(LogEntry^ entry)
{
	// to be overloaded by subclass
}


LogHandler::!LogHandler()
{
	ResetUnmanagedImplementation();
}


void LogHandler::OnMessageNative(const gmsec::api::util::LogEntry& entry)
{
	LogEntry^ entryWrapped = gcnew LogEntry(const_cast<gmsec::api::util::LogEntry*>(&entry));

	OnMessage(entryWrapped);
}


gmsec::api::util::LogHandler* LogHandler::GetUnmanagedImplementation()
{
	return m_impl;
}


void LogHandler::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void LogHandler::SetUnmanagedImplementation(gmsec::api::util::LogHandler* handler)
{
	ResetUnmanagedImplementation();

	m_impl = handler;
}
