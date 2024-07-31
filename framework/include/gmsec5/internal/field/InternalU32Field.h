/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_U32FIELD_H
#define GMSEC_API5_INTERNAL_U32FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalU32Field : public InternalField
{
public:
	InternalU32Field(const char* name, GMSEC_U32 value, bool isHeader = false);

	virtual ~InternalU32Field();

	GMSEC_U32 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalU32Field();
	InternalU32Field(const InternalU32Field&);
	InternalU32Field& operator=(const InternalU32Field&);

	GMSEC_U32 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
