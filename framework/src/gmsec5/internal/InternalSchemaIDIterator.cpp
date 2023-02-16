/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMessageFieldIterator.h
 */

#include <gmsec5/internal/InternalSchemaIDIterator.h>

#include <gmsec5/GmsecException.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;


InternalSchemaIDIterator::InternalSchemaIDIterator(InternalSpecification& spec)
	: m_spec(spec),
	  m_ready(false),
	  m_nextID()
{
	reset();
}


InternalSchemaIDIterator::~InternalSchemaIDIterator()
{
}


void InternalSchemaIDIterator::reset()
{
	m_spec.resetSchemaIndex();
}


bool InternalSchemaIDIterator::hasNext()
{
	return m_spec.hasNextID();
}


const char* InternalSchemaIDIterator::next()
{
	m_nextID = m_spec.nextID();

	return m_nextID.c_str();
}
