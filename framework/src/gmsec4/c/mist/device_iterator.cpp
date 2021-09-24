/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file device_iterator.cpp
 *
 *  @brief Defines the functions which support the iteration over the Device objects stored in a DeviceMessage.
 */


#include <gmsec4/c/mist/device_iterator.h>

#include <gmsec4/mist/DeviceIterator.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_BOOL CALL_TYPE deviceIteratorHasNext(GMSEC_DeviceIterator iter, GMSEC_Status status)
{
	DeviceIterator* devIter = reinterpret_cast<DeviceIterator*>(iter);
	GMSEC_BOOL      hasNext = GMSEC_FALSE;
	Status          result;

	if (devIter)
	{
		hasNext = (devIter->hasNext() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "DeviceIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasNext;
}


const GMSEC_Device CALL_TYPE deviceIteratorNext(GMSEC_DeviceIterator iter, GMSEC_Status status)
{
	DeviceIterator* devIter = reinterpret_cast<DeviceIterator*>(iter);
	GMSEC_Device    device  = NULL;
	Status          result;

	if (devIter)
	{
		try
		{
			const Device& dev = devIter->next();

			device = (GMSEC_Device) &dev;
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "DeviceIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return device;
}


void CALL_TYPE deviceIteratorReset(GMSEC_DeviceIterator iter, GMSEC_Status status)
{
	DeviceIterator* devIter = reinterpret_cast<DeviceIterator*>(iter);
	Status          result;

	if (devIter)
	{
		devIter->reset();
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "DeviceIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}
