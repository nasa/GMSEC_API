/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/SchemaIDIterator_Net.h>

#include <GMSEC_Exception_Net.h>

// C++ API native
#include <gmsec4/Exception.h>


using namespace GMSEC::API::MIST;
using namespace System;


SchemaIDIterator::~SchemaIDIterator()
{
	this->!SchemaIDIterator();
}


bool SchemaIDIterator::HasNext()
{
	return m_impl->hasNext();
}


String^ SchemaIDIterator::Next()
{
	try
	{
		return gcnew String(m_impl->next());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void SchemaIDIterator::Reset()
{
	m_impl->reset();
}


SchemaIDIterator::!SchemaIDIterator()
{
	ResetUnmanagedImplementation();
}


SchemaIDIterator::SchemaIDIterator(gmsec::api::mist::SchemaIDIterator* iter)
{
	m_impl = iter;
}


void SchemaIDIterator::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}
