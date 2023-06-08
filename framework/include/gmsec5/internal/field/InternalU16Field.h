/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_U16FIELD_H
#define GMSEC_API5_INTERNAL_U16FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalU16Field : public InternalField
{
public:
	InternalU16Field(const char* name, GMSEC_U16 value, bool isHeader = false);

	virtual ~InternalU16Field();

	GMSEC_U16 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalU16Field();
	InternalU16Field(const InternalU16Field&);
	InternalU16Field& operator=(const InternalU16Field&);

	GMSEC_U16 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
