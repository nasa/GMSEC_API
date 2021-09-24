
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file status_c .cpp
 *
 *  This file contains the C-wrapper for functions in the Status object.
*/

#include <gmsec/c/status_c.h>
#include <gmsec/Status.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

void CALL_TYPE CreateStatus(GMSEC_STATUS_HANDLE *sts)
{
	*sts = (GMSEC_STATUS_HANDLE)new Status();
}

void CALL_TYPE DestroyStatus(GMSEC_STATUS_HANDLE *sts)
{
	if (NULL != *sts)
	{
		Status *tmp = (Status *)*sts;
		delete tmp;
		*sts = NULL;
	}
}

const char * CALL_TYPE GetStatus(GMSEC_STATUS_HANDLE sts)
{
	if (NULL == sts)
        {
                LOG_WARNING << "status_c::GetStatus : [x,x] Status handle is NULL ";
		return "[x,x] Status handle is NULL";
        }
	return static_cast<Status *>(sts)->Get();
}

const char * CALL_TYPE GetStatusString(GMSEC_STATUS_HANDLE sts)
{
	if (NULL == sts)
        {
                LOG_WARNING << "status_c::GetStatusString : Status handle is NULL : ";
		return "Status handle is NULL";
        }
	return static_cast<Status *>(sts)->GetString();
}

GMSEC_STATUS_CLASS CALL_TYPE GetStatusClass(GMSEC_STATUS_HANDLE sts)
{
	if (NULL == sts)
        {
                LOG_WARNING << "status_c::GetStatusClass : Status handle is NULL : " << GMSEC_STATUS_OTHER_ERROR;
		return GMSEC_STATUS_OTHER_ERROR;
        }
	return static_cast<Status *>(sts)->GetClass();
}

unsigned int CALL_TYPE GetStatusCode(GMSEC_STATUS_HANDLE sts)
{
	if (NULL == sts)
        {
                LOG_WARNING << "status_c::GetStatusCode : Status handle is NULL : " << GMSEC_OTHER_ERROR;
		return GMSEC_OTHER_ERROR;
        }
	return static_cast<Status *>(sts)->GetCode();
}

int CALL_TYPE GetStatusCustomCode(GMSEC_STATUS_HANDLE sts)
{
	if (NULL == sts)
	{
		LOG_WARNING << "status_c::GetStatusCustomCode : Status handle is NULL : " << GMSEC_OTHER_ERROR;
		return GMSEC_OTHER_ERROR;
	}
	return static_cast<Status *>(sts)->GetCustomCode();
}

int CALL_TYPE isStatusError(GMSEC_STATUS_HANDLE sts)
{
	if (NULL == sts)
        {
                LOG_WARNING << "status_c::isStatusError : Status handle is NULL :" << GMSEC_FALSE;
		return GMSEC_FALSE;
        }
	return static_cast<Status *>(sts)->isError();
}

