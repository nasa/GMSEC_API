/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file product_file_iterator.cpp
 *
 *  @brief Defines the functions which support the iteration over the Product File objects stored in a ProductFileMessage.
 */


#include <gmsec4/c/mist/product_file_iterator.h>

#include <gmsec4/mist/ProductFileIterator.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_BOOL CALL_TYPE productFileIteratorHasNext(GMSEC_ProductFileIterator iter, GMSEC_Status status)
{
	ProductFileIterator* prodIter = reinterpret_cast<ProductFileIterator*>(iter);
	GMSEC_BOOL           hasNext  = GMSEC_FALSE;
	Status               result;

	if (prodIter)
	{
		hasNext = (prodIter->hasNext() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ProductFileIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasNext;
}


const GMSEC_ProductFile CALL_TYPE productFileIteratorNext(GMSEC_ProductFileIterator iter, GMSEC_Status status)
{
	ProductFileIterator* prodIter = reinterpret_cast<ProductFileIterator*>(iter);
	GMSEC_ProductFile    prodFile = NULL;
	Status               result;

	if (prodIter)
	{
		try
		{
			const ProductFile& pFile = prodIter->next();

			prodFile = (GMSEC_ProductFile) &pFile;
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ProductFileIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return prodFile;
}


void CALL_TYPE productFileIteratorReset(GMSEC_ProductFileIterator iter, GMSEC_Status status)
{
	ProductFileIterator* prodIter = reinterpret_cast<ProductFileIterator*>(iter);
	Status               result;

	if (prodIter)
	{
		prodIter->reset();
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ProductFileIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}
