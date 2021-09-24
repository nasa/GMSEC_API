/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file SimpleFieldIterator.cpp
 *
 *  This file contains the implementation of the SimpleFieldIterator class
 *
*/

#include <gmsec/internal/SimpleFieldIterator.h>

#include <gmsec/internal/BaseField.h>
#include <gmsec/internal/MessageFieldMap.h>

#include <gmsec/internal/Log.h>


using gmsec::Status;
using namespace gmsec::internal;
using namespace gmsec::util;


SimpleFieldIterator::SimpleFieldIterator(MessageFieldMap &fields)
	: fFields(fields)
{
	fFields.InitIterator();
}


SimpleFieldIterator::~SimpleFieldIterator()
{
}


bool SimpleFieldIterator::HasNext()
{
	return fFields.HasNext();
}


Status SimpleFieldIterator::Next(gmsec::Field &field)
{
	Status result;

	if (!fFields.HasNext())
	{
		result.Set(GMSEC_STATUS_ITERATOR_ERROR,
				GMSEC_INVALID_NEXT,
				"Next called on iterator at end");
		LOG_WARNING << result.Get();
	}
	else
	{
		BaseField *internal = FieldBuddy::getInternal(&field);
		BaseField *next     = fFields.Next();

		*internal = *next;
	}

	return result;
}
