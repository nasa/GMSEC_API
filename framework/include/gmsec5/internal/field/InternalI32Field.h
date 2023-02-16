/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_I32FIELD_H
#define GMSEC_API5_INTERNAL_I32FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalI32Field : public InternalField
{
public:
	InternalI32Field(const char* name, GMSEC_I32 value, bool isHeader = false);

	virtual ~InternalI32Field();

	GMSEC_I32 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalI32Field();
	InternalI32Field(const InternalI32Field&);
	InternalI32Field& operator=(const InternalI32Field&);

	GMSEC_I32 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
