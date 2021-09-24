/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_LOG_ENTRY_NET_H
#define GMSEC_API_LOG_ENTRY_NET_H

#include <util/LoggingLevel_Net.h>
#include <util/TimeSpec_Net.h>

// C++ API native
#include <gmsec4/util/LogHandler.h>


namespace GMSEC
{
namespace API
{
namespace UTIL
{


/// <summary>
/// Log entry definition
/// </summary>


public ref class LogEntry
{
public:
	/// <summary>Constructor</summmary>
	LogEntry();


	/// <summary>Destructor</summmary>
	~LogEntry();


	/// <summary>Provides the level of the LogEntry</summary>
	LoggingLevel GetLevel();


	/// <summary>Provides the time of the LogEntry was registered</summary>
	TimeSpec^ GetTime();


	/// <summary>Provides the log message</summary>
	System::String^ GetMessage();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!LogEntry();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="entry">unmanaged implementation to initialize with</param>
	LogEntry(gmsec::api::util::LogEntry* entry);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::util::LogEntry* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::util::LogEntry* entry);


private:
	gmsec::api::util::LogEntry* m_impl;
};


} // end namespace UTIL
} // end namespace API
} // end namespace GMSEC


#endif
