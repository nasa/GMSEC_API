/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file schema_id_iterator.cpp
 *
 * @brief This file contains function for the management of a SchemaIDIterator object.
 */


#include <gmsec5/c/schema_id_iterator.h>

#include <gmsec5/GmsecException.h>
#include <gmsec5/SchemaIDIterator.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


GMSEC_BOOL CALL_TYPE schemaIDIteratorHasNext(GMSEC_SchemaIDIterator iter, GMSEC_Status status)
{
	SchemaIDIterator* idIter = reinterpret_cast<SchemaIDIterator*>(iter);
	GMSEC_BOOL        hasNext = GMSEC_BOOL::GMSEC_FALSE;
	Status            result;

	if (idIter)
	{
		hasNext = (idIter->hasNext() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
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
		catch (const GmsecException& e)
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
