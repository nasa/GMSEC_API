/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_F64FIELD_H
#define GMSEC_API5_INTERNAL_F64FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalF64Field : public InternalField
{
public:
	InternalF64Field(const char* name, GMSEC_F64 value, bool isHeader = false);

	virtual ~InternalF64Field();

	GMSEC_F64 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	InternalF64Field();
	InternalF64Field(const InternalF64Field&);
	InternalF64Field& operator=(const InternalF64Field&);

	GMSEC_F64 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
