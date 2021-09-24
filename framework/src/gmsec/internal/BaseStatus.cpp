/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseStatus.cpp
 *  @brief This file contains the base class for GMSEC BaseStatus object.
**/


#include <gmsec/internal/BaseStatus.h>
#include <gmsec/internal/strutil.h>

#include <sstream>


using namespace gmsec;
using namespace gmsec::internal;


BaseStatus::BaseStatus()
    : m_str(0),
      m_outputStr(0),
      m_class(),
      m_code(GMSEC_STATUS_NO_ERROR),
      m_customCode(0)
{
}


BaseStatus::BaseStatus(GMSEC_STATUS_CLASS errClass, unsigned int errCode, const char* errStr)
    : m_str(0),
      m_outputStr(0),
      m_class(errClass),
      m_code(errCode),
      m_customCode(0)
{
    setString(errStr);
}


BaseStatus::BaseStatus(const BaseStatus& other)
    : m_str(0),
      m_outputStr(0),
      m_class(other.m_class),
      m_code(other.m_code),
      m_customCode(other.m_customCode)
{
    setString(other.m_str);
}


BaseStatus::~BaseStatus()
{
    if (m_str)
    {
        gmsec::util::stringDestroy(m_str);
        m_str = 0;
    }
    if (m_outputStr)
    {
        gmsec::util::stringDestroy(m_outputStr);
        m_outputStr = 0;
    }
}


BaseStatus&
BaseStatus::operator=(const BaseStatus& rhs)
{
    if (this != &rhs)
    {
        setString(rhs.m_str);

        if (m_outputStr)
        {
            gmsec::util::stringDestroy(m_outputStr);
        }

        m_outputStr  = 0;
        m_class      = rhs.m_class;
        m_code       = rhs.m_code;
        m_customCode = rhs.m_customCode;
    }

    return *this;
}


void
BaseStatus::setClass(GMSEC_STATUS_CLASS errClass)
{
    m_class = errClass;
}


GMSEC_STATUS_CLASS
BaseStatus::getClass() const
{
    return m_class;
}


void
BaseStatus::setCode(unsigned int errCode)
{
    m_code = errCode;
}


unsigned int
BaseStatus::getCode() const
{
    return m_code;
}


void
BaseStatus::setString(const char* errStr)
{
	// Check to see if m_str and errStr are the same.
	// If they are the same, don't bother going through
	// the trouble of de-allocating the existing m_str
	// and then allocating a new string.

	if (m_str != errStr)
	{
		if (m_str)
		{
			gmsec::util::stringDestroy(m_str);
			m_str = 0;
		}

		if (errStr)
		{
			m_str = gmsec::util::stringNew(errStr);
		}
	}
}


const char*
BaseStatus::getString() const
{
    return m_str ? m_str : "";
}


void
BaseStatus::setCustomCode(GMSEC_I32 code)
{
    m_customCode = code;
}


GMSEC_I32
BaseStatus::getCustomCode() const
{
    return m_customCode;
}


const char*
BaseStatus::get()
{
    std::ostringstream oss;

    if(m_customCode == 0){
    	oss << "[" << m_class << "," << m_code << "]: " << getString() << std::ends;
    }
    else{
    	oss << "[" << m_class << "," << m_code << "," << m_customCode << "]: " << getString() << std::ends;
    }

    if (m_outputStr)
    {
        gmsec::util::stringDestroy(m_outputStr);
    }

    m_outputStr = gmsec::util::stringNew(oss.str().c_str());

    return m_outputStr;
}


bool
BaseStatus::isError() const
{
    return m_class != GMSEC_STATUS_NO_ERROR;
}


void
BaseStatus::set(GMSEC_STATUS_CLASS errClass, unsigned int errCode, const char* errStr)
{
    setString(errStr);

    m_class = errClass;
    m_code  = errCode;
}


void
BaseStatus::reset()
{
    setString(0);

    if (m_outputStr)
    {
        gmsec::util::stringDestroy(m_outputStr);
        m_outputStr = 0;
    }

    m_class      = GMSEC_STATUS_NO_ERROR;
    m_code       = 0;
    m_customCode = 0;
}
