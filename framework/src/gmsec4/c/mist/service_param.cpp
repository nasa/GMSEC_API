/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file service_param.cpp
 *
 *  @brief This file contains functions for the management of a MIST Simple Service Parameter object.
 */


#include <gmsec4/c/mist/service_param.h>

#include <gmsec4/mist/ServiceParam.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_ServiceParam CALL_TYPE serviceParamCreate(const char* name, const GMSEC_Field value, GMSEC_Status status)
{
	GMSEC_ServiceParam param = NULL;
	Status             result;

	const Field* valueField = reinterpret_cast<const Field*>(value);

	if (valueField)
	{
		try
		{
			param = reinterpret_cast<GMSEC_ServiceParam>(new ServiceParam(name, *valueField));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return param;
}


GMSEC_ServiceParam CALL_TYPE serviceParamCreateCopy(const GMSEC_ServiceParam other, GMSEC_Status status)
{
	GMSEC_ServiceParam param = NULL;
	Status             result;

	const ServiceParam* otherParam = reinterpret_cast<const ServiceParam*>(other);

	if (otherParam)
	{
		param = reinterpret_cast<GMSEC_ServiceParam>(new ServiceParam(*otherParam));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ServiceParam handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return param;
}


void CALL_TYPE serviceParamDestroy(GMSEC_ServiceParam* param)
{
	if (param && *param)
	{
		ServiceParam* tmp = reinterpret_cast<ServiceParam*>(*param);

		delete tmp;

		*param = NULL;
	}
}


const char* CALL_TYPE serviceParamGetName(GMSEC_ServiceParam param, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	const ServiceParam* svcParam = reinterpret_cast<const ServiceParam*>(param);

	if (svcParam)
	{
		name = svcParam->getName();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ServiceParam handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return name;
}


const GMSEC_Field CALL_TYPE serviceParamGetValue(GMSEC_ServiceParam param, GMSEC_Status status)
{
	GMSEC_Field value = NULL;
	Status      result;

	const ServiceParam* svcParam = reinterpret_cast<const ServiceParam*>(param);

	if (svcParam)
	{
		Field& tmp = const_cast<Field&>(svcParam->getValue());

		value = reinterpret_cast<GMSEC_Field>(&tmp);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "ServiceParam handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}
