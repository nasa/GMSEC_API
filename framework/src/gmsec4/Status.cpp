/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/Status.h>
#include <gmsec4/internal/InternalStatus.h>


namespace gmsec
{
namespace api
{


Status::Status()
	: m_iStatus(new internal::InternalStatus())
{
}


Status::Status(StatusClass eclass, StatusCode code, const char* text, GMSEC_I32 custom)
	: m_iStatus(new internal::InternalStatus(eclass, code, text, custom))
{
}


Status::Status(const Exception& e)
	: m_iStatus(new internal::InternalStatus(e))
{
}


Status::Status(const Status& other)
	: m_iStatus(new internal::InternalStatus(other.getClass(), other.getCode(), other.getReason(), other.getCustomCode()))
{
}


Status::~Status()
{
	delete m_iStatus;
}


Status& Status::operator=(const Status& other)
{
	if (this != &other)
	{
		delete m_iStatus;

		m_iStatus = new internal::InternalStatus(other);
	}

	return *this;
}


bool Status::isError() const
{
	return m_iStatus->isError();
}


const char* Status::get() const
{
	return m_iStatus->get();
}


void Status::set(StatusClass eclass, StatusCode code, const char* text, GMSEC_I32 custom)
{
	return m_iStatus->set(eclass, code, text, custom);
}


StatusClass Status::getClass() const
{
	return m_iStatus->getClass();
}


void Status::setClass(StatusClass eclass)
{
	return m_iStatus->setClass(eclass);
}


StatusCode Status::getCode() const
{
	return m_iStatus->getCode();
}


void Status::setCode(StatusCode code)
{
	return m_iStatus->setCode(code);
}


const char* Status::getReason() const
{
	return m_iStatus->getReason();
}


void Status::setReason(const char* reason)
{
	return m_iStatus->setReason(reason);
}


GMSEC_I32 Status::getCustomCode() const
{
	return m_iStatus->getCustomCode();
}


void Status::setCustomCode(GMSEC_I32 code)
{
	return m_iStatus->setCustomCode(code);
}


void Status::reset()
{
	return m_iStatus->reset();
}

} // namespace api
} // namespace gmsec
