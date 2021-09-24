/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <MessageFieldIterator_Net.h>

#include <field/FieldConverter_Net.h>
#include <GMSEC_Exception_Net.h>

// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API;


MessageFieldIterator::MessageFieldIterator(gmsec::api::MessageFieldIterator* iter)
{
	m_impl = iter;
}


MessageFieldIterator::~MessageFieldIterator()
{
	this->!MessageFieldIterator();
}


bool MessageFieldIterator::HasNext()
{
	return m_impl->hasNext();
}


Field^ MessageFieldIterator::Next()
{
	try
	{
		gmsec::api::Field& nativeField = const_cast<gmsec::api::Field&>(m_impl->next());

		return FieldConverter::CreateFieldReference(&nativeField);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void MessageFieldIterator::Reset()
{
	m_impl->reset();
}


MessageFieldIterator::!MessageFieldIterator()
{
	ResetUnmanagedImplementation();
}


void MessageFieldIterator::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}
