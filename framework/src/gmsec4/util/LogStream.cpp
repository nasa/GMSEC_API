/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <gmsec4/util/LogStream.h>
#include <gmsec4/internal/util/InternalLogStream.h>


using namespace gmsec::api::util;
using namespace gmsec::api::internal;

LogStream::LogStream()
	: m_internal(new InternalLogStream)
{
}


LogStream::~LogStream()
{
	delete m_internal;
	m_internal = 0;
}


LogStream& LogStream::operator<<(char x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(int x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(unsigned int x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(long x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(unsigned long x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(long long x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(unsigned long long x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(float x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(double x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(const char* x)
{
	*m_internal << x;
	return *this;
}


LogStream& LogStream::operator<<(const void* x)
{
	*m_internal << x;
	return *this;
}


const char* LogStream::getText() const
{
	return m_internal->getText();
}
