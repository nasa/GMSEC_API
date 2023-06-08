/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API_INTERNAL_SPECIFICATION_BUDDY_H
#define GMSEC_API_INTERNAL_SPECIFICATION_BUDDY_H


#include <gmsec5/Specification.h>
#include <gmsec5/internal/InternalSpecification.h>

namespace gmsec
{
namespace api5
{
namespace internal
{

class SpecificationBuddy
{
public:
	static InternalSpecification& getInternal(const Specification& spec)
	{
		return *spec.m_internal;
	}
};


} // namespace internal
} // namespace api5
} // namespace gmsec


#endif
