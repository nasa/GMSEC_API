/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <mist/ProductFileIterator_Net.h>

#include <mist/ProductFile_Net.h>

#include <GMSEC_Exception_Net.h>

// C++ API native
#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API::MIST;


ProductFileIterator::ProductFileIterator(gmsec::api::mist::ProductFileIterator* iter)
{
	m_impl = iter;
}


ProductFileIterator::~ProductFileIterator()
{
	this->!ProductFileIterator();
}


bool ProductFileIterator::HasNext()
{
	return m_impl->hasNext();
}


ProductFile^ ProductFileIterator::Next()
{
	try
	{
		gmsec::api::mist::ProductFile& nativeProductFile = const_cast<gmsec::api::mist::ProductFile&>(m_impl->next());

		return gcnew ProductFile(&nativeProductFile, false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ProductFileIterator::Reset()
{
	m_impl->reset();
}


ProductFileIterator::!ProductFileIterator()
{
	ResetUnmanagedImplementation();
}


void ProductFileIterator::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}
