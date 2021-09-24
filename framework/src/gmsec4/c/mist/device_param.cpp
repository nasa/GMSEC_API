/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file device_param.cpp
 *
 *  @brief This file contains functions for the management of a MIST DeviceParam object.
 */


#include <gmsec4/c/mist/device_param.h>

#include <gmsec4/mist/DeviceParam.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_DeviceParam CALL_TYPE deviceParamCreate(const char* name, const char* timestamp, GMSEC_Field value, GMSEC_Status status)
{
	GMSEC_DeviceParam param = NULL;
	Status            result;

	Field* valField = reinterpret_cast<Field*>(value);

	if (valField)
	{
		try
		{
			param = reinterpret_cast<GMSEC_DeviceParam>(new DeviceParam(name, timestamp, *valField));
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
		*((Status*) status) = result;
	}

	return param;
}


GMSEC_DeviceParam CALL_TYPE deviceParamCreateCopy(const GMSEC_DeviceParam other, GMSEC_Status status)
{
	GMSEC_DeviceParam param = NULL;
	Status            result;

	const DeviceParam* otherParam = reinterpret_cast<const DeviceParam*>(other);

	if (otherParam)
	{
		param = reinterpret_cast<GMSEC_DeviceParam>(new DeviceParam(*otherParam));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceParam handle is NULL");
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return param;
}


void CALL_TYPE deviceParamDestroy(GMSEC_DeviceParam* param)
{
	if (param && *param)
	{
		DeviceParam* tmp = reinterpret_cast<DeviceParam*>(*param);

		delete tmp;

		*param = NULL;
	}
}


const char* CALL_TYPE deviceParamGetName(const GMSEC_DeviceParam param, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	const DeviceParam* tmp = reinterpret_cast<const DeviceParam*>(param);

	if (tmp)
	{
		name = tmp->getName();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceParam handle is NULL");
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return name;
}


const char* CALL_TYPE deviceParamGetTimestamp(const GMSEC_DeviceParam param, GMSEC_Status status)
{
	const char* timestamp = NULL;
	Status      result;

	const DeviceParam* tmp = reinterpret_cast<const DeviceParam*>(param);

	if (tmp)
	{
		timestamp = tmp->getTimestamp();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceParam handle is NULL");
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return timestamp;
}


const GMSEC_Field CALL_TYPE deviceParamGetValue(const GMSEC_DeviceParam param, GMSEC_Status status)
{
	GMSEC_Field value = NULL;
	Status      result;

	const DeviceParam* tmp = reinterpret_cast<const DeviceParam*>(param);

	if (tmp)
	{
		Field& field = const_cast<Field&>(tmp->getValue());

		value = reinterpret_cast<GMSEC_Field>(&field);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "DeviceParam handle is NULL");
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return value;
}
