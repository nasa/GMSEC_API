/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_LOG_HANDLER_NET_H
#define GMSEC_API_LOG_HANDLER_NET_H


// C++ API native
#include <gmsec4/util/LogHandler.h>


namespace GMSEC
{
namespace API
{
namespace UTIL
{

// Forward declarations
ref class LogEntry;


/// <summary>
/// This interface is for receiving log events.
/// A user created class, implemented with this interface, can be passed into 
/// Log.RegisterHandler() to have user code executed when a log event occurs.
/// </summary>
///
/// <seealso cref="LogEntry"/>
/// <seealso cref="TimeUtil"/>


public ref class LogHandler abstract
{
public:
	/// <summary>Default Constructor</summary>
	LogHandler();


	/// <summary>Destructor</summary>
	virtual ~LogHandler();


	/// <summary>This function is called by the API in response to the logging of a message.
	///
    /// <param name="entry">The log entry</param>
	virtual void OnMessage(LogEntry^ entry);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!LogHandler();


	/// <summary>Pass through to callback managed class</summary>
	///
	/// <param name="entry">The log entry</param>
	void OnMessageNative(const gmsec::api::util::LogEntry& entry);


internal:
	/// <summary>Get unmanaged implementation version</summary>
	///
    /// <returns>Unmanaged implementation</returns>
	gmsec::api::util::LogHandler* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::util::LogHandler* handler);


private:
	gmsec::api::util::LogHandler* m_impl;
};


}  // end namespace UTIL
}  // end namespace API
}  // end namespace GMSEC

#endif
