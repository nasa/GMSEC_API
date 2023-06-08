/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file status.cpp
 *
 * @brief This file contains functions for the management of status.
 */


#include <gmsec5/c/status.h>

#include <gmsec5/Status.h>

using namespace gmsec::api5;


GMSEC_Status CALL_TYPE statusCreate(void)
{
	return reinterpret_cast<GMSEC_Status>(new Status());
}


GMSEC_Status CALL_TYPE statusCreateWithValues(GMSEC_I32 clazz, GMSEC_I32 code, const char* reason, GMSEC_I32 custom)
{
	return reinterpret_cast<GMSEC_Status>(new Status(clazz, code, reason, custom));
}


GMSEC_Status CALL_TYPE statusCreateCopy(GMSEC_Status other)
{
	GMSEC_Status created = NULL;
	Status*      status  = reinterpret_cast<Status*>(other);

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


GMSEC_BOOL CALL_TYPE statusHasError(GMSEC_Status status)
{
	GMSEC_BOOL hasError = GMSEC_BOOL::GMSEC_FALSE;
	Status*    s        = reinterpret_cast<Status*>(status);

	if (s)
	{
		hasError = (s->hasError() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	return hasError;
}


const char* CALL_TYPE statusGet(GMSEC_Status status)
{
	const char* get = NULL;
	Status*     s   = reinterpret_cast<Status*>(status);

	if (s)
	{
		get = s->get();
	}

	return get;
}


void CALL_TYPE statusSet(GMSEC_Status status, GMSEC_I32 clazz, GMSEC_I32 code, const char* reason, GMSEC_I32 custom)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		s->set(clazz, code, reason, custom);
	}
}


GMSEC_I32 CALL_TYPE statusGetClass(GMSEC_Status status)
{
	GMSEC_I32 clazz = GMSEC_NO_ERROR;
	Status*   s     = reinterpret_cast<Status*>(status);

	if (s)
	{
		clazz = s->getClass();
	}

	return clazz;
}


void CALL_TYPE statusSetClass(GMSEC_Status status, GMSEC_I32 clazz)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		s->setClass(clazz);
	}
}


GMSEC_I32 CALL_TYPE statusGetCode(GMSEC_Status status)
{
	GMSEC_I32 code = GMSEC_NO_ERROR_CODE;
	Status*   s    = reinterpret_cast<Status*>(status);

	if (s)
	{
		code = s->getCode();
	}

	return code;
}


void CALL_TYPE statusSetCode(GMSEC_Status status, GMSEC_I32 code)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		s->setCode(code);
	}
}


const char* CALL_TYPE statusGetReason(GMSEC_Status status)
{
	const char* reason = NULL;
	Status*     s      = reinterpret_cast<Status*>(status);

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


GMSEC_I32 CALL_TYPE statusGetCustomCode(GMSEC_Status status)
{
	GMSEC_I32 custom = 0;
	Status*   s      = reinterpret_cast<Status*>(status);

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
