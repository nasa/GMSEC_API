
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file FieldIterator.cpp
 *
 *  This file contains the implementation of the FieldIterator class.
 *
*/


#include <gmsec/FieldIterator.h>
#include <gmsec/internal/AbstractFieldIterator.h>


namespace gmsec {


FieldIterator::FieldIterator()
	:
	ptr(new gmsec::internal::NullFieldIterator())
{
}


FieldIterator::FieldIterator(FieldSelector selector)
	:
	ptr(new gmsec::internal::SelectorFieldIterator(selector))
{
}



FieldIterator::~FieldIterator()
{
	delete ptr;
	ptr = NULL;
}


bool FieldIterator::HasNext()
{
	return ptr->HasNext();
}


Status FieldIterator::Next(gmsec::Field &field)
{
	return ptr->Next(field);
}



bool selectAllFields(gmsec::Field & /*field*/)
{
	return true;
}


bool selectHeaderFields(gmsec::Field &field)
{
	return field.isHeader();
}


bool selectNonheaderFields(gmsec::Field &field)
{
	return !field.isHeader();
}


} // namespace gmsec

