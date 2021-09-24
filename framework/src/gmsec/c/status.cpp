
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file status.cpp
 *
 *  This file contains the C-wrapper for functions in the Status object.
*/

#include <gmsec/c/status.h>
#include <gmsec/Status.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

void CALL_TYPE gmsec_CreateStatus(GMSEC_STATUS_OBJECT *sts)
{
	*sts = (GMSEC_STATUS_OBJECT)new Status();
}

void CALL_TYPE gmsec_DestroyStatus(GMSEC_STATUS_OBJECT *sts)
{
	if (NULL != *sts)
	{
		Status *tmp = (Status *)*sts;
		delete tmp;
		*sts = NULL;
	}
}

const char * CALL_TYPE gmsec_GetStatus(GMSEC_STATUS_OBJECT sts)
{
	if (NULL == sts)
	{
		LOG_WARNING << "status::GetStatus : [x,x] Status handle is NULL ";
		return "[x,x] Status handle is NULL";
	}
	Status* status = (Status *) sts;
	return status->Get();
}

const char * CALL_TYPE gmsec_GetStatusString(GMSEC_STATUS_OBJECT sts)
{
	if (NULL == sts)
	{
		LOG_WARNING << "status::GetStatusString : Status handle is NULL : ";
		return "Status handle is NULL";
	}
	Status* status = (Status *) sts;
	return status->GetString();
}

GMSEC_STATUS_CLASS CALL_TYPE gmsec_GetStatusClass(GMSEC_STATUS_OBJECT sts)
{
	if (NULL == sts)
	{
		LOG_WARNING << "status::GetStatusClass : Status handle is NULL : " << GMSEC_STATUS_OTHER_ERROR;
		return GMSEC_STATUS_OTHER_ERROR;
	}
	Status* status = (Status *) sts;
	return status->GetClass();
}

unsigned int CALL_TYPE gmsec_GetStatusCode(GMSEC_STATUS_OBJECT sts)
{
	if (NULL == sts)
	{
		LOG_WARNING << "status::GetStatusCode : Status handle is NULL : " << GMSEC_OTHER_ERROR;
		return GMSEC_OTHER_ERROR;
	}
	Status* status = (Status *) sts;
	return status->GetCode();
}

int CALL_TYPE gmsec_GetStatusCustomCode(GMSEC_STATUS_OBJECT sts)
{
	if (NULL == sts)
	{
		LOG_WARNING << "status::GetStatusCustomCode : Status handle is NULL : " << GMSEC_OTHER_ERROR;
		return GMSEC_OTHER_ERROR;
	}
	Status* status = (Status *) sts;
	return status->GetCustomCode();
}

int CALL_TYPE gmsec_isStatusError(GMSEC_STATUS_OBJECT sts)
{
	if (NULL == sts)
	{
		LOG_WARNING << "status::isStatusError : Status handle is NULL :" << GMSEC_FALSE;
		return GMSEC_FALSE;
	}
	Status* status = (Status *) sts;
	return status->isError();
}

