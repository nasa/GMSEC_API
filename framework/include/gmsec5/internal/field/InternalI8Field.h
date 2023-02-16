/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_I8FIELD_H
#define GMSEC_API5_INTERNAL_I8FIELD_H

#include <gmsec5/internal/field/InternalField.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class InternalI8Field : public InternalField
{
public:
	InternalI8Field(const char* name, GMSEC_I8 value, bool isHeader = false);

	virtual ~InternalI8Field();

	GMSEC_I8 getValue() const;

	virtual const char* toXML() const;
	virtual const char* toJSON() const;

private:
	// defined, but not implemented
	InternalI8Field();
	InternalI8Field(const InternalI8Field&);
	InternalI8Field& operator=(const InternalI8Field&);

	GMSEC_I8 m_value;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
