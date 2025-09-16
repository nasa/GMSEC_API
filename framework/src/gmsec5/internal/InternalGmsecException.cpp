/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/InternalGmsecException.h>

#include <sstream>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;


InternalGmsecException::InternalGmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, const char* msg)
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


InternalGmsecException::InternalGmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, GMSEC_I32 customCode, const char* msg)
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


InternalGmsecException::InternalGmsecException(const InternalGmsecException& other)
	: m_errorClass(other.m_errorClass),
	  m_errorCode(other.m_errorCode),
	  m_customCode(other.m_customCode),
	  m_errorMsg(other.m_errorMsg),
	  m_what(other.m_what)
{
}

    
InternalGmsecException::~InternalGmsecException()
{
}


InternalGmsecException& InternalGmsecException::operator=(const InternalGmsecException& other)
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


GMSEC_I32 InternalGmsecException::getErrorClass() const
{
	return m_errorClass;
}


GMSEC_I32 InternalGmsecException::getErrorCode() const
{
	return m_errorCode;
}


GMSEC_I32 InternalGmsecException::getCustomCode() const
{
	return m_customCode;
}


const char* InternalGmsecException::getErrorMessage() const
{
	return m_errorMsg.c_str();
}

const char* InternalGmsecException::what() const
{
	if (m_what.empty())
	{
		std::ostringstream oss;

	    oss << "[" << m_errorClass << "," << m_errorCode << "," << m_customCode << "] : " << m_errorMsg;

		m_what = oss.str();
	}

	return m_what.c_str();
}
