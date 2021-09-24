
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file Status.cpp
 *
 *  This file contains the Status class.
*/

#include <gmsec/Status.h>
#include <gmsec/internal/BaseStatus.h>

using namespace gmsec;
using namespace gmsec::internal;

Status::Status()
	: ptr(new BaseStatus()),
	  padding_1(0),
	  padding_2(0),
	  padding_3(0),
	  padding_4(0)
{
}

Status::Status(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text)
	: ptr(new BaseStatus(eclass, code, text)),
	  padding_1(0),
	  padding_2(0),
	  padding_3(0),
	  padding_4(0)
{
}

Status::Status(const Status &st)
	: ptr(new BaseStatus(*st.ptr)),
	  padding_1(0),
	  padding_2(0),
	  padding_3(0),
	  padding_4(0)
{
}

Status::~Status()
{
	delete ptr;
	ptr = 0;
}

Status Status::operator=(const Status &st)
{
	if (this != &st)
	{
		delete ptr;

		ptr = new BaseStatus(*st.ptr);
	}

	return *this;
}

void Status::ReSet()
{
	ptr->reset();
}


/* GetString
 * This function will retrieve the string detailed description of this error
*/
const char *Status::GetString() const
{
	return ptr->getString();
}

const char *Status::Get() const
{
	return ptr->get();
}


GMSEC_STATUS_CLASS Status::GetClass() const
{
	return ptr->getClass();
}

/* GetCode
 * This will return the error code number for this status for easy comparison
 */
unsigned int Status::GetCode() const
{
	return ptr->getCode();
}


/* GetCustomCode
 * This will return the custom code number for this status.
 */
GMSEC_I32 Status::GetCustomCode() const
{
	return ptr->getCustomCode();
}


/* isError
 * This will return TRUE if the status is indicating an error, FALSE otherwise
 * This function has been deprecated; remove for API 4.0.
 */
int Status::isError()
{
	return IsError();
}


/* IsError
 * This will return TRUE if the status is indicating an error, FALSE otherwise
 */
int Status::IsError()
{
	return ptr->isError() ? 1 : 0;
}


/* isError
 * This will return TRUE if the status is indicating an error, FALSE otherwise
 * This function has been deprecated; remove for API 4.0.
 */
int Status::isError() const
{
	return IsError();
}


/* IsError
 * This will return TRUE if the status is indicating an error, FALSE otherwise
 */
int Status::IsError() const
{
	return ptr->isError() ? 1 : 0;
}


void Status::SetString(const char *text)
{
	ptr->setString(text);
}

void Status::SetClass(GMSEC_STATUS_CLASS eclass)
{
	ptr->setClass(eclass);
}

void Status::SetCode(unsigned int code)
{
	ptr->setCode(code);
}

void Status::SetCustomCode(GMSEC_I32 code)
{
	ptr->setCustomCode(code);
}

void Status::Set(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text)
{
	ptr->set(eclass, code, text);
}
