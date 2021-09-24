/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_LOG_NET_H
#define GMSEC_API_LOG_NET_H

#include <util/LoggingLevel_Net.h>


namespace GMSEC
{
namespace API
{
namespace UTIL
{

// Forward declaration
ref class LogHandler;


/// <summary>
/// Basic logging class.  This class supports basic logging to any output stream.
/// This class is thread-safe.
/// </summary>


public ref class Log
{
public:
	/// <summary>This method sets the maximum logging level.</summary>
	///
	/// <param name="level">The maximum desired logging level</param>
	static void SetReportingLevel(LoggingLevel level);


	/// <summary>This method returns the maximum logging level that is currently set.</summary>
	///
	/// <returns>The maximum logging level</returns>
	static LoggingLevel GetReportingLevel();


	/// <summary>This method registers a LogHandler that will be called for all logging levels.</summary>
	///
	/// <param name="handler">The LogHandler to register, or null to deregister an existing LogHandler</param>
	static void RegisterHandler(LogHandler^ handler);


	/// <summary>This method registers a LogHandler that will be called for a specific logging level.</summary>
	///
	/// <param name="level">The logging level to associate with the LogHandler</param>
	/// <param name="handler">The LogHandler to register, or null to deregister an existing LogHandler</param>
	static void RegisterHandler(LoggingLevel level, LogHandler^ handler);


	/// <summary>Converts the given logging level to a string.</summary>
	///
	/// <param name="level">The logging level</param>
	///
	/// <returns>A string representation of the logging level.</returns>
	static System::String^ LevelToString(LoggingLevel level);


	/// <summary>Converts the given string to a LoggingLevel.</summary>
	///
	/// <param name="level">The string representation of a logging level</param>
	///
	/// <returns>An enumerated value of the logging level.</returns>
	static LoggingLevel LevelFromString(System::String^ level);


	/// <summary>Logs an error message.</summary>
	///
	/// <param name="message">The message to log</param>
	static void Error(System::String^ message);


	/// <summary>Logs a secure message.</summary>
	///
	/// <param name="message">The message to log</param>
	static void Secure(System::String^ message);


	/// <summary>Logs a warning message.</summary>
	///
	/// <param name="message">The message to log</param>
	static void Warning(System::String^ message);


	/// <summary>Logs an info message.</summary>
	///
	/// <param name="message">The message to log</param>
	static void Info(System::String^ message);


	/// <summary>Logs a verbose message.</summary>
	///
	/// <param name="message">The message to log</param>
	static void Verbose(System::String^ message);


	/// <summary>Logs a debug message.</summary>
	///
	/// <param name="message">The message to log</param>
	static void Debug(System::String^ message);
};


} // end namespace UTIL
} // end namespace API
} // end namespace GMSEC


#endif
