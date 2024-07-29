/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalStatus.cpp
 *
 *  @brief This file contains the class that encapsulates status information.
 */

#include <gmsec5/internal/InternalStatus.h>
#include <gmsec5/GmsecException.h>

#include <gmsec5_defs.h>

#include <sstream>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


InternalStatus::InternalStatus()
	: m_eclass(0),
	  m_ecode(0),
	  m_reason(),
	  m_custom(0),
	  m_status()
{
}


InternalStatus::InternalStatus(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason, GMSEC_I32 custom)
	: m_eclass(eclass),
	  m_ecode(ecode),
	  m_reason((reason == NULL ? "" : reason)),
	  m_custom(custom),
	  m_status()
{
}


InternalStatus::InternalStatus(const GmsecException& e)
	: m_eclass(e.getErrorClass()),
	  m_ecode(e.getErrorCode()),
	  m_reason(e.getErrorMessage()),
	  m_custom(e.getCustomCode()),
	  m_status()
{
}


InternalStatus::InternalStatus(const InternalStatus& other)
	: m_eclass(other.m_eclass),
	  m_ecode(other.m_ecode),
	  m_reason(other.m_reason),
	  m_custom(other.m_custom),
	  m_status(other.m_status)
{
}


InternalStatus::~InternalStatus()
{
}


InternalStatus& InternalStatus::operator=(const InternalStatus& other)
{
	if (this != &other)
	{
		m_eclass = other.m_eclass;
		m_ecode  = other.m_ecode;
		m_reason = other.m_reason;
		m_custom = other.m_custom;
		m_status = other.m_status;
	}

	return *this;
}


bool InternalStatus::hasError() const
{
	return m_eclass != 0;
}


const char* InternalStatus::get() const
{
	std::ostringstream oss;

	oss << "[" << m_eclass << "," << m_ecode << "," << m_custom << "] : " << m_reason;

	m_status = oss.str();

	return m_status.c_str();
}


void InternalStatus::set(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason, GMSEC_I32 custom)
{
	m_eclass = eclass;
	m_ecode  = ecode;
	m_reason = (reason == NULL ? "" : reason);
	m_custom = custom;
}


GMSEC_I32 InternalStatus::getClass() const
{
	return m_eclass;
}


void InternalStatus::setClass(GMSEC_I32 eclass)
{
	m_eclass = eclass;
}


GMSEC_I32 InternalStatus::getCode() const
{
	return m_ecode;
}


void InternalStatus::setCode(GMSEC_I32 ecode)
{
	m_ecode = ecode;
}


const char* InternalStatus::getReason() const
{
	return m_reason.c_str();
}


void InternalStatus::setReason(const char* reason)
{
	m_reason = (reason == NULL ? "" : reason);
}


GMSEC_I32 InternalStatus::getCustomCode() const
{
	return m_custom;
}


void InternalStatus::setCustomCode(GMSEC_I32 code)
{
	m_custom = code;
}


void InternalStatus::reset()
{
	m_eclass = 0;
	m_ecode  = 0;
	m_reason = "";
	m_custom = 0;
	m_status = "";
}
