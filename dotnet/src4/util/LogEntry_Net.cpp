/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <util/LogEntry_Net.h>


using namespace GMSEC::API::UTIL;
using namespace System;


LogEntry::LogEntry()
{
	/// nothing to do
}


LogEntry::~LogEntry()
{
	this->!LogEntry();
}


LoggingLevel LogEntry::GetLevel()
{
	return static_cast<LoggingLevel>(m_impl->level);
}


TimeSpec^ LogEntry::GetTime()
{
	TimeSpec^ ts = gcnew TimeSpec();

	ts->seconds     = m_impl->time.seconds;
	ts->nanoseconds = m_impl->time.nanoseconds;

	return ts;
}


String^ LogEntry::GetMessage()
{
	 return gcnew String(m_impl->message);
}


LogEntry::!LogEntry()
{
	ResetUnmanagedImplementation();
}


LogEntry::LogEntry(gmsec::api::util::LogEntry* entry)
{
	SetUnmanagedImplementation(entry);
}


gmsec::api::util::LogEntry* LogEntry::GetUnmanagedImplementation()
{
	return m_impl;
}


void LogEntry::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}


void LogEntry::SetUnmanagedImplementation(gmsec::api::util::LogEntry* entry)
{
	ResetUnmanagedImplementation();

	m_impl = entry;
}
