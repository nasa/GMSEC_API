/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_I16FIELD_H
#define GMSEC_API5_INTERNAL_I16FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalI16Field : public InternalField
{
public:
	InternalI16Field(const char* name, GMSEC_I16 value, bool isHeader = false);

	virtual ~InternalI16Field();

	GMSEC_I16 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalI16Field();
	InternalI16Field(const InternalI16Field&);
	InternalI16Field& operator=(const InternalI16Field&);

	GMSEC_I16 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
