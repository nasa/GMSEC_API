
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file AbstractFieldIterator.cpp
 *
 *  This file contains the functions for the AbstractFieldIterator class
 *
*/


#include <gmsec/internal/AbstractFieldIterator.h>
#include <gmsec/FieldIterator.h>

#include <gmsec/internal/Log.h>


using namespace gmsec;
using namespace gmsec::util;
using namespace gmsec::internal;


AbstractFieldIterator::AbstractFieldIterator()
{
}


AbstractFieldIterator::~AbstractFieldIterator()
{
}


void AbstractFieldIterator::update(FieldIterator &external)
{
	external.ptr->associate(external, this);
}


void AbstractFieldIterator::associate(FieldIterator &external,
		AbstractFieldIterator * ptr)
{
	if (external.ptr != ptr)
	{
		delete external.ptr;
		external.ptr = ptr;
	}
	else
		LOG_WARNING << "AbstractFieldIterator::associate: reassociating same object";
}


/* NullFieldIterator implementation */
NullFieldIterator::NullFieldIterator()
{
}


NullFieldIterator::~NullFieldIterator()
{
}


bool NullFieldIterator::HasNext()
{
	LOG_WARNING << "HasNext attempted on NullFieldIterator";
	return false;
}


Status NullFieldIterator::Next(gmsec::Field & /*field*/)
{
	Status result(GMSEC_STATUS_ITERATOR_ERROR,
			GMSEC_INVALID_NEXT,
			"Next called on uninitialized field iterator");

	LOG_WARNING << "NullFieldIterator::Next: " << result.Get();

	return result;
}


/* SelectorFieldIterator */
SelectorFieldIterator::SelectorFieldIterator(FieldSelector selector)
	:
	fSelector(selector),
	fChild(NULL),
	fReady(false)
{
	if (!fSelector)
	{
		fSelector = selectAllFields;
		LOG_WARNING << "SelectorFieldIterator given NULL selector- using selectAllFields";
	}
}


SelectorFieldIterator::~SelectorFieldIterator()
{
	delete fChild;
	fChild = NULL;
}


void SelectorFieldIterator::associate(FieldIterator & /*external*/,
		AbstractFieldIterator *internal)
{
	// cassert(external.ptr == this);
	delete fChild;
	fChild = internal;
}


bool SelectorFieldIterator::makeReady()
{
	if (fReady)
		return true;

	while (!fReady && fChild->HasNext())
	{
		Status status = fChild->Next(fField);
		if (status.isError())
		{
			LOG_WARNING << "SelectorFieldIterator::makeReady: Next failed " << status.Get();
			return false;
		}

		fReady = (*fSelector)(fField);
	}

	return fReady;
}


bool SelectorFieldIterator::HasNext()
{
	return makeReady();
}


Status SelectorFieldIterator::Next(Field &field)
{
	Status result;

	if (makeReady())
	{
		field = fField;
		fReady = false;
	}
	else
	{
		result.Set(GMSEC_STATUS_ITERATOR_ERROR,
				GMSEC_INVALID_NEXT,
				"Next called on iterator at end");
		// Not a LOG_WARNING situation.  Nothing alarming about using an
		// iterator to completely step through fields.
		LOG_DEBUG << "SelectorFieldIterator::Next: " << result.Get();
	}

	return result;
}

