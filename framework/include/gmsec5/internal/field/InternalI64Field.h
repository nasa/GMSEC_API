/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_I64FIELD_H
#define GMSEC_API5_INTERNAL_I64FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalI64Field : public InternalField
{
public:
	InternalI64Field(const char* name, GMSEC_I64 value, bool isHeader = false);

	virtual ~InternalI64Field();

	GMSEC_I64 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalI64Field();
	InternalI64Field(const InternalI64Field&);
	InternalI64Field& operator=(const InternalI64Field&);

	GMSEC_I64 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
