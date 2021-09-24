/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file message_field_iterator.cpp
 *
 * @brief This file contains function for the management of a Msg Field Iterator object.
 */


#include <gmsec4/c/message_field_iterator.h>

#include <gmsec4/Exception.h>
#include <gmsec4/MessageFieldIterator.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;


GMSEC_BOOL CALL_TYPE messageFieldIteratorHasNext(const GMSEC_MessageFieldIterator iter, GMSEC_Status status)
{
	const MessageFieldIterator* msgIter = reinterpret_cast<const MessageFieldIterator*>(iter);
	GMSEC_BOOL                  hasNext = GMSEC_FALSE;
	Status                      result;

	if (msgIter)
	{
		hasNext = (msgIter->hasNext() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "MessageFieldIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasNext;
}


const GMSEC_Field CALL_TYPE messageFieldIteratorNext(GMSEC_MessageFieldIterator iter, GMSEC_Status status)
{
	MessageFieldIterator* msgIter = reinterpret_cast<MessageFieldIterator*>(iter);
	GMSEC_Field           field = NULL;
	Status                result;

	if (msgIter)
	{
		try
		{
			const Field& tmp = msgIter->next();

			field = (void*) &tmp;
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "MessageFieldIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return field;
}


void CALL_TYPE messageFieldIteratorReset(GMSEC_MessageFieldIterator iter, GMSEC_Status status)
{
	MessageFieldIterator* msgIter = reinterpret_cast<MessageFieldIterator*>(iter);
	Status                result;

	if (msgIter)
	{
		msgIter->reset();
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "MessageFieldIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}
