/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file status.cpp
 *
 * @brief This file contains functions for the management of status.
 */


#include <gmsec4/c/status.h>

#include <gmsec4/Status.h>

using namespace gmsec::api;


GMSEC_Status CALL_TYPE statusCreate(void)
{
	return reinterpret_cast<GMSEC_Status>(new Status());
}


GMSEC_Status CALL_TYPE statusCreateWithValues(GMSEC_StatusClass clazz, GMSEC_StatusCode code, const char* reason, GMSEC_I32 custom)
{
	int tmp1 = clazz;
	int tmp2 = code;

	StatusClass tmpClazz = static_cast<StatusClass>(tmp1);
	StatusCode  tmpCode  = static_cast<StatusCode>(tmp2);

	return reinterpret_cast<GMSEC_Status>(new Status(tmpClazz, tmpCode, reason, custom));
}


GMSEC_Status CALL_TYPE statusCreateCopy(const GMSEC_Status other)
{
	GMSEC_Status  created = NULL;
	const Status* status  = reinterpret_cast<const Status*>(other);

	if (status)
	{
		created = reinterpret_cast<GMSEC_Status>(new Status(*status));
	}

	return created;
}


void CALL_TYPE statusDestroy(GMSEC_Status* status)
{
	if (status && *status)
	{
		Status* s = reinterpret_cast<Status*>(*status);

		delete s;

		*status = NULL;
	}
}


GMSEC_BOOL CALL_TYPE statusIsError(const GMSEC_Status status)
{
	GMSEC_BOOL    isError = GMSEC_FALSE;
	const Status* s       = reinterpret_cast<const Status*>(status);

	if (s)
	{
		isError = (s->isError() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	return isError;
}


const char* CALL_TYPE statusGet(const GMSEC_Status status)
{
	const char*   get = NULL;
	const Status* s   = reinterpret_cast<const Status*>(status);

	if (s)
	{
		get = s->get();
	}

	return get;
}


void CALL_TYPE statusSet(GMSEC_Status status, GMSEC_StatusClass clazz, GMSEC_StatusCode code, const char* reason, GMSEC_I32 custom)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		int tmp1 = clazz;
		int tmp2 = code;

		StatusClass tmpClazz = static_cast<StatusClass>(tmp1);
		StatusCode  tmpCode  = static_cast<StatusCode>(tmp2);

		s->set(tmpClazz, tmpCode, reason, custom);
	}
}


GMSEC_StatusClass CALL_TYPE statusGetClass(const GMSEC_Status status)
{
	GMSEC_StatusClass clazz = GMSEC_API_NO_ERROR;
	const Status*     s     = reinterpret_cast<const Status*>(status);

	if (s)
	{
		int tmp = static_cast<int>(s->getClass());   // intermediate cast required by solaris
		clazz = static_cast<GMSEC_StatusClass>(tmp);
	}

	return clazz;
}


void CALL_TYPE statusSetClass(const GMSEC_Status status, GMSEC_StatusClass clazz)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		int tmp = clazz;

		StatusClass tmpClazz = static_cast<StatusClass>(tmp);

		s->setClass(tmpClazz);
	}
}


GMSEC_StatusCode CALL_TYPE statusGetCode(const GMSEC_Status status)
{
	GMSEC_StatusCode code = GMSEC_API_NO_ERROR_CODE;
	const Status*    s    = reinterpret_cast<const Status*>(status);

	if (s)
	{
		int tmp = static_cast<int>(s->getCode());   // intermediate cast required by solaris
		code = static_cast<GMSEC_StatusCode>(tmp);
	}

	return code;
}


void CALL_TYPE statusSetCode(GMSEC_Status status, GMSEC_StatusCode code)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		int tmp = code;

		StatusCode tmpCode = static_cast<StatusCode>(tmp);

		s->setCode(tmpCode);
	}
}


const char* CALL_TYPE statusGetReason(const GMSEC_Status status)
{
	const char*   reason = NULL;
	const Status* s      = reinterpret_cast<const Status*>(status);

	if (s)
	{
		reason = s->getReason();
	}

	return reason;
}


void CALL_TYPE statusSetReason(GMSEC_Status status, const char* reason)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		s->setReason(reason);
	}
}


GMSEC_I32 CALL_TYPE statusGetCustomCode(const GMSEC_Status status)
{
	GMSEC_I32     custom = 0;
	const Status* s      = reinterpret_cast<const Status*>(status);

	if (s)
	{
		custom = s->getCustomCode();
	}

	return custom;
}


void CALL_TYPE statusSetCustomCode(GMSEC_Status status, GMSEC_I32 custom)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		s->setCustomCode(custom);
	}
}


void CALL_TYPE statusReset(GMSEC_Status status)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		s->reset();
	}
}
