/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

#include <gmsec4/internal/InternalException.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;


Exception::Exception(StatusClass errorClass, StatusCode errorCode, const char* msg)
	: m_internal(new InternalException(errorClass, errorCode, msg))
{
}


Exception::Exception(StatusClass errorClass, StatusCode errorCode, GMSEC_I32 customCode, const char* msg)
	: m_internal(new InternalException(errorClass, errorCode, customCode, msg))
{
}


Exception::Exception(const Status& status)
	: m_internal(new InternalException(status.getClass(), status.getCode(), status.getCustomCode(), status.getReason()))
{
}


Exception::Exception(const Exception& other)
	: m_internal(new InternalException(*other.m_internal))
{
}


Exception::~Exception()
{
	delete m_internal;
}


Exception& Exception::operator=(const Exception& other)
{
	if (this != &other)
	{
		delete m_internal;
		m_internal = new InternalException(*other.m_internal);
	}

	return *this;
}


StatusClass Exception::getErrorClass() const
{
	return m_internal->getErrorClass();
}


StatusCode Exception::getErrorCode() const
{
	return m_internal->getErrorCode();
}


GMSEC_I32 Exception::getCustomCode() const
{
	return m_internal->getCustomCode();
}


const char* Exception::getErrorMessage() const
{
	return m_internal->getErrorMessage();
}

const char* Exception::what() const
{
	return m_internal->what();
}
