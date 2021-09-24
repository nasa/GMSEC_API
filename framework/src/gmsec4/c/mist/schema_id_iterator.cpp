/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file schema_id_iterator.cpp
 *
 * @brief This file contains function for the management of a SchemaIDIterator object.
 */


#include <gmsec4/c/mist/schema_id_iterator.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

#include <gmsec4/mist/SchemaIDIterator.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_BOOL CALL_TYPE schemaIDIteratorHasNext(const GMSEC_SchemaIDIterator iter, GMSEC_Status status)
{
	const SchemaIDIterator* idIter = reinterpret_cast<const SchemaIDIterator*>(iter);
	GMSEC_BOOL              hasNext = GMSEC_FALSE;
	Status                  result;

	if (idIter)
	{
		hasNext = (idIter->hasNext() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "SchemaIDIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasNext;
}


const char* CALL_TYPE schemaIDIteratorNext(GMSEC_SchemaIDIterator iter, GMSEC_Status status)
{
	SchemaIDIterator* idIter   = reinterpret_cast<SchemaIDIterator*>(iter);
	const char*       schemaID = NULL;
	Status            result;

	if (idIter)
	{
		try
		{
			schemaID = idIter->next();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "SchemaIDIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return schemaID;
}


void CALL_TYPE schemaIDIteratorReset(GMSEC_SchemaIDIterator iter, GMSEC_Status status)
{
	SchemaIDIterator* idIter = reinterpret_cast<SchemaIDIterator*>(iter);
	Status            result;

	if (idIter)
	{
		idIter->reset();
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "SchemaIDIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}
