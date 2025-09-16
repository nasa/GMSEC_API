/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_F32FIELD_H
#define GMSEC_API5_INTERNAL_F32FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalF32Field : public InternalField
{
public:
	InternalF32Field(const char* name, GMSEC_F32 value, bool isHeader = false);

	virtual ~InternalF32Field();

	GMSEC_F32 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	InternalF32Field();
	InternalF32Field(const InternalF32Field&);
	InternalF32Field& operator=(const InternalF32Field&);

	GMSEC_F32 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
