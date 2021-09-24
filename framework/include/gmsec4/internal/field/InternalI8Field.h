/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_I8FIELD_H
#define GMSEC_API_INTERNAL_I8FIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalI8Field : public InternalField
{
public:
	InternalI8Field(const char* name, GMSEC_I8 value);

	virtual ~InternalI8Field();

	GMSEC_I8 CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	// defined, but not implemented
	InternalI8Field();
	InternalI8Field(const InternalI8Field&);
	InternalI8Field& operator=(const InternalI8Field&);

	GMSEC_I8 m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
