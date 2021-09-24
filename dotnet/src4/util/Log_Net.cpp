/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <util/Log_Net.h>
#include <util/LogHandler_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/util/Log.h>


using namespace GMSEC::API::UTIL;
using namespace System;
using namespace System::Runtime::InteropServices;


void Log::SetReportingLevel(LoggingLevel level)
{
	gmsec::api::util::Log::setReportingLevel(static_cast<LogLevel>(level));
}


LoggingLevel Log::GetReportingLevel()
{
	return static_cast<LoggingLevel>(gmsec::api::util::Log::getReportingLevel());
}


void Log::RegisterHandler(LogHandler^ handler)
{
	if (handler == nullptr)
	{
		gmsec::api::util::Log::registerHandler(reinterpret_cast<gmsec::api::util::LogHandler*>(NULL));
	}
	else
	{
		THROW_EXCEPTION_IF_NULLPTR(handler->GetUnmanagedImplementation(), StatusClass::OTHER_ERROR, StatusCode::INVALID_CALLBACK, "LogHandler implementation is null");

		gmsec::api::util::Log::registerHandler(handler->GetUnmanagedImplementation());
	}
}


void Log::RegisterHandler(LoggingLevel level, LogHandler^ handler)
{
	if (handler == nullptr)
	{
		gmsec::api::util::Log::registerHandler(static_cast<LogLevel>(level), reinterpret_cast<gmsec::api::util::LogHandler*>(NULL));
	}
	else
	{
		THROW_EXCEPTION_IF_NULLPTR(handler->GetUnmanagedImplementation(), StatusClass::OTHER_ERROR, StatusCode::INVALID_CALLBACK, "LogHandler implementation is null");

		gmsec::api::util::Log::registerHandler(static_cast<LogLevel>(level), handler->GetUnmanagedImplementation());
	}
}


String^ Log::LevelToString(LoggingLevel level)
{
	return gcnew String(gmsec::api::util::Log::toString(static_cast<LogLevel>(level)));
}


LoggingLevel Log::LevelFromString(System::String^ level)
{
	char* levelStr = nullptr;

	try
	{
		levelStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(level).ToPointer());

		return static_cast<LoggingLevel>(gmsec::api::util::Log::fromString(levelStr));
	}
	catch (...)
	{
		return LoggingLevel::NONE;
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(levelStr);
	}
}


void Log::Error(String^ message)
{
	char* msgStr = nullptr;

	try
	{
		msgStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(message).ToPointer());

		GMSEC_ERROR << msgStr;
	}
	catch (...)
	{
		// nothing to do?
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(msgStr);
	}
}


void Log::Secure(String^ message)
{
	char* msgStr = nullptr;

	try
	{
		msgStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(message).ToPointer());

		GMSEC_SECURE << msgStr;
	}
	catch (...)
	{
		// nothing to do?
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(msgStr);
	}
}


void Log::Warning(String^ message)
{
	char* msgStr = nullptr;

	try
	{
		msgStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(message).ToPointer());

		GMSEC_WARNING << msgStr;
	}
	catch (...)
	{
		// nothing to do?
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(msgStr);
	}
}


void Log::Info(String^ message)
{
	char* msgStr = nullptr;

	try
	{
		msgStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(message).ToPointer());

		GMSEC_INFO << msgStr;
	}
	catch (...)
	{
		// nothing to do?
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(msgStr);
	}
}


void Log::Verbose(String^ message)
{
	char* msgStr = nullptr;

	try
	{
		msgStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(message).ToPointer());

		GMSEC_VERBOSE << msgStr;
	}
	catch (...)
	{
		// nothing to do?
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(msgStr);
	}
}


void Log::Debug(String^ message)
{
	char* msgStr = nullptr;

	try
	{
		msgStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(message).ToPointer());

		GMSEC_DEBUG << msgStr;
	}
	catch (...)
	{
		// nothing to do?
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(msgStr);
	}
}
