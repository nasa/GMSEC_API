/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_U64FIELD_H
#define GMSEC_API_INTERNAL_U64FIELD_H

#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalU64Field : public InternalField
{
public:
	InternalU64Field(const char* name, GMSEC_U64 value);

	virtual ~InternalU64Field();

	GMSEC_U64 CALL_TYPE getValue() const;

	virtual const char* CALL_TYPE toXML() const;
	virtual const char* CALL_TYPE toJSON() const;

private:
	// defined, but not implemented
	InternalU64Field();
	InternalU64Field(const InternalU64Field&);
	InternalU64Field& operator=(const InternalU64Field&);

	GMSEC_U64 m_value;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif
