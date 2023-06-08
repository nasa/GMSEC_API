/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>

#include <gmsec5/internal/InternalGmsecException.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


GmsecException::GmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, const char* msg)
	: m_internal(new InternalGmsecException(errorClass, errorCode, msg))
{
}


GmsecException::GmsecException(GMSEC_I32 errorClass, GMSEC_I32 errorCode, GMSEC_I32 customCode, const char* msg)
	: m_internal(new InternalGmsecException(errorClass, errorCode, customCode, msg))
{
}


GmsecException::GmsecException(const Status& status)
	: m_internal(new InternalGmsecException(status.getClass(), status.getCode(), status.getCustomCode(), status.getReason()))
{
}


GmsecException::GmsecException(const GmsecException& other)
	: m_internal(new InternalGmsecException(*other.m_internal))
{
}


GmsecException::~GmsecException()
{
	delete m_internal;
}


GmsecException& GmsecException::operator=(const GmsecException& other)
{
	if (this != &other)
	{
		delete m_internal;
		m_internal = new InternalGmsecException(*other.m_internal);
	}

	return *this;
}


GMSEC_I32 GmsecException::getErrorClass() const
{
	return m_internal->getErrorClass();
}


GMSEC_I32 GmsecException::getErrorCode() const
{
	return m_internal->getErrorCode();
}


GMSEC_I32 GmsecException::getCustomCode() const
{
	return m_internal->getCustomCode();
}


const char* GmsecException::getErrorMessage() const
{
	return m_internal->getErrorMessage();
}


const char* GmsecException::what() const
{
	return m_internal->what();
}
