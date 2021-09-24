/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/mist/DeviceIterator.h>

#include <gmsec4/mist/Device.h>

#include <gmsec4/internal/mist/message/InternalDeviceMessage.h>

#include <gmsec4/internal/mist/InternalDeviceIterator.h>


using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


DeviceIterator::DeviceIterator(gmsec::api::mist::message::internal::InternalDeviceMessage& msg)
	: m_iter(new InternalDeviceIterator(msg))
{
}


DeviceIterator::~DeviceIterator()
{
	delete m_iter;
}


bool DeviceIterator::hasNext() const
{
	return m_iter->hasNext();
}


const Device& DeviceIterator::next() const
{
	return m_iter->next();
}


void DeviceIterator::reset()
{
	m_iter->reset();
}
