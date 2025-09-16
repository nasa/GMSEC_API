/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Status.cpp
 *
 *  @brief This file contains the class that encapsulates status information.
 */

#include <gmsec5/Status.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/internal/InternalStatus.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


Status::Status()
	: m_internal(new InternalStatus())
{
}


Status::Status(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason, GMSEC_I32 custom)
	: m_internal(new InternalStatus(eclass, ecode, reason, custom))
{
}


Status::Status(const GmsecException& e)
	: m_internal(new InternalStatus(e))
{
}


Status::Status(const Status& other)
	: m_internal(new InternalStatus(*other.m_internal))
{
}


Status::~Status()
{
	delete m_internal;
}


Status& Status::operator=(const Status& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new InternalStatus(*other.m_internal);
	}

	return *this;
}


bool Status::hasError() const
{
	return m_internal->hasError();
}


const char* Status::get() const
{
	return m_internal->get();
}


void Status::set(GMSEC_I32 eclass, GMSEC_I32 ecode, const char* reason, GMSEC_I32 custom)
{
	m_internal->set(eclass, ecode, reason, custom);
}


GMSEC_I32 Status::getClass() const
{
	return m_internal->getClass();
}


void Status::setClass(GMSEC_I32 eclass)
{
	m_internal->setClass(eclass);
}


GMSEC_I32 Status::getCode() const
{
	return m_internal->getCode();
}


void Status::setCode(GMSEC_I32 ecode)
{
	m_internal->setCode(ecode);
}


const char* Status::getReason() const
{
	return m_internal->getReason();
}


void Status::setReason(const char* reason)
{
	m_internal->setReason(reason);
}


GMSEC_I32 Status::getCustomCode() const
{
	return m_internal->getCustomCode();
}


void Status::setCustomCode(GMSEC_I32 code)
{
	m_internal->setCustomCode(code);
}


void Status::reset()
{
	m_internal->reset();
}
