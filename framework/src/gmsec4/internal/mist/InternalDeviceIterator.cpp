/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/mist/InternalDeviceIterator.h>

#include <gmsec4/internal/mist/message/InternalDeviceMessage.h>


using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


InternalDeviceIterator::InternalDeviceIterator(InternalDeviceMessage& msg)
	: m_msg(msg),
	  m_ready(false),
	  m_nextDevice(0)
{
}


InternalDeviceIterator::~InternalDeviceIterator()
{
}


bool InternalDeviceIterator::hasNext()
{
	return makeReady();
}


const Device& InternalDeviceIterator::next()
{
	const Device* device = 0;

	if (makeReady())
	{
		device = m_nextDevice;
		m_nextDevice = 0;
	}

	if (!device)
	{
		throw Exception(ITERATOR_ERROR, ITER_INVALID_NEXT, "There are no more Devices");
	}

	return *device;
}


void InternalDeviceIterator::reset()
{
	m_msg.resetDeviceIterator();
}


bool InternalDeviceIterator::makeReady()
{
	bool ready = false;

	if (m_nextDevice)
	{
		ready = true;
	}
	else
	{
		while (m_msg.hasNextDevice() && !ready)
		{
			const Device& device = m_msg.nextDevice();

			m_nextDevice = &device;

			ready = true;
		}
	}

	return ready;
}
