/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalStatus.h
 *
 *  @brief This file contains the class that encapsulates status information.
 *
**/

#include <gmsec4/internal/InternalStatus.h>

#include <gmsec4/Message.h>

#include <sstream>


namespace gmsec
{
namespace api
{
namespace internal
{

InternalStatus::InternalStatus()
	: m_class(NO_ERROR_CLASS),
	  m_code(NO_ERROR_CODE),
	  m_reason(),
	  m_customCode(0),
	  m_getString(),
	  m_reqMessage(0)
{
}


InternalStatus::InternalStatus(StatusClass eclass, StatusCode code, const char* text, GMSEC_I32 custom)
	: m_class(eclass),
	  m_code(code),
	  m_reason(),
	  m_customCode(custom),
	  m_getString(),
	  m_reqMessage(0)
{
	if (text)
	{
		m_reason = text;
	}
}


InternalStatus::InternalStatus(const Exception& e)
	: m_class(e.getErrorClass()),
	  m_code(e.getErrorCode()),
	  m_reason(e.getErrorMessage()),
	  m_customCode(e.getCustomCode()),
	  m_getString(),
	  m_reqMessage(0)
{
}


InternalStatus::InternalStatus(const InternalStatus& other)
	: m_class(other.m_class),
	  m_code(other.m_code),
	  m_reason(other.m_reason),
	  m_customCode(other.m_customCode),
	  m_getString(),
	  m_reqMessage(0)
{
	if (other.m_reqMessage)
	{
		m_reqMessage = new Message(*other.m_reqMessage);
	}
}


InternalStatus::~InternalStatus()
{
	delete m_reqMessage;
}


InternalStatus& InternalStatus::operator=(const InternalStatus& other)
{
	if (this != &other)
	{
		m_class      = other.m_class;
		m_code       = other.m_code;
		m_reason     = other.m_reason;
		m_customCode = other.m_class;
	    m_getString  = other.m_getString;

		if (other.m_reqMessage)
		{
			m_reqMessage = new Message(*other.m_reqMessage);
		}
	}

	return *this;
}


bool InternalStatus::isError() const
{
	return m_class != NO_ERROR_CLASS;
}


const char* InternalStatus::get() const
{
	std::ostringstream oss;
	oss << "[" << m_class << "," << m_code << "," << m_customCode << "] : " << m_reason;

	m_getString = oss.str();

	return m_getString.c_str();
}


void InternalStatus::set(StatusClass eclass, StatusCode code, const char* text, GMSEC_I32 custom)
{
	m_class = eclass;
	m_code = code;
	if (text)
	{
		m_reason = text;
	}
	m_customCode = custom;
	m_getString.clear();
}


StatusClass InternalStatus::getClass() const
{
	return m_class;
}


void InternalStatus::setClass(StatusClass eclass)
{
	m_class = eclass;
}


StatusCode InternalStatus::getCode() const
{
	return m_code;
}


void InternalStatus::setCode(StatusCode code)
{
	m_code = code;
}


const char* InternalStatus::getReason() const
{
	return m_reason.c_str();
}


void InternalStatus::setReason(const char* reason)
{
	if (reason)
	{
		m_reason = reason;
	}
}


GMSEC_I32 InternalStatus::getCustomCode() const
{
	return m_customCode;
}


void InternalStatus::setCustomCode(GMSEC_I32 code)
{
	m_customCode = code;
}


void InternalStatus::reset()
{
	m_class = NO_ERROR_CLASS;
	m_code = NO_ERROR_CODE;
	m_reason.clear();
	m_customCode = 0;
	m_getString.clear();
}


Message* InternalStatus::getRequestMessage()
{
	return m_reqMessage;
}


void InternalStatus::setRequestMessage(const Message& request)
{
	delete m_reqMessage;

	m_reqMessage = new Message(request);
}

} // namespace internal
} // namespace api
} // namespace gmsec
