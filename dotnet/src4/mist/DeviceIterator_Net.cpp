/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <mist/DeviceIterator_Net.h>

#include <mist/Device_Net.h>

#include <GMSEC_Exception_Net.h>

// C++ API native
#include <gmsec4/mist/Device.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API::MIST;


DeviceIterator::DeviceIterator(gmsec::api::mist::DeviceIterator* iter)
{
	m_impl = iter;
}


DeviceIterator::~DeviceIterator()
{
	this->!DeviceIterator();
}


bool DeviceIterator::HasNext()
{
	return m_impl->hasNext();
}


Device^ DeviceIterator::Next()
{
	try
	{
		gmsec::api::mist::Device& nativeDevice = const_cast<gmsec::api::mist::Device&>(m_impl->next());

		return gcnew Device(&nativeDevice, false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void DeviceIterator::Reset()
{
	m_impl->reset();
}


DeviceIterator::!DeviceIterator()
{
	ResetUnmanagedImplementation();
}


void DeviceIterator::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}
