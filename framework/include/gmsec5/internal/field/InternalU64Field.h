/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_U64FIELD_H
#define GMSEC_API5_INTERNAL_U64FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalU64Field : public InternalField
{
public:
	InternalU64Field(const char* name, GMSEC_U64 value, bool isHeader = false);

	virtual ~InternalU64Field();

	GMSEC_U64 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalU64Field();
	InternalU64Field(const InternalU64Field&);
	InternalU64Field& operator=(const InternalU64Field&);

	GMSEC_U64 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
