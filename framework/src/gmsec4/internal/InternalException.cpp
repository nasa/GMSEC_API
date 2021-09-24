/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/InternalException.h>

#include <sstream>

using namespace gmsec::api;
using namespace gmsec::api::internal;


InternalException::InternalException(StatusClass errorClass, StatusCode errorCode, const char* msg)
	: m_errorClass(errorClass),
	  m_errorCode(errorCode),
	  m_customCode(0),
	  m_errorMsg(),
	  m_what()
{
	if (msg != NULL)
	{
		m_errorMsg = msg;
	}
}


InternalException::InternalException(StatusClass errorClass, StatusCode errorCode, GMSEC_I32 customCode, const char* msg)
	: m_errorClass(errorClass),
	  m_errorCode(errorCode),
	  m_customCode(customCode),
	  m_errorMsg(),
	  m_what()
{
	if (msg != NULL)
	{
		m_errorMsg = msg;
	}
}


InternalException::InternalException(const InternalException& other)
	: m_errorClass(other.m_errorClass),
	  m_errorCode(other.m_errorCode),
	  m_customCode(other.m_customCode),
	  m_errorMsg(other.m_errorMsg),
	  m_what(other.m_what)
{
}

    
InternalException::~InternalException()
{
}


InternalException& InternalException::operator=(const InternalException& other)
{
	if (this != &other)
	{
		m_errorClass = other.m_errorClass;
		m_errorCode  = other.m_errorCode;
		m_customCode = other.m_customCode;
		m_errorMsg   = other.m_errorMsg;
		m_what       = other.m_what;
	}

	return *this;
}


StatusClass InternalException::getErrorClass() const
{
	return m_errorClass;
}


StatusCode InternalException::getErrorCode() const
{
	return m_errorCode;
}


GMSEC_I32 InternalException::getCustomCode() const
{
	return m_customCode;
}


const char* InternalException::getErrorMessage() const
{
	return m_errorMsg.c_str();
}

const char* InternalException::what() const
{
	if (m_what.empty())
	{
		std::ostringstream oss;

	    oss << "[" << m_errorClass << "," << m_errorCode << "," << m_customCode << "]: " << m_errorMsg;

		m_what = oss.str();
	}

	return m_what.c_str();
}
